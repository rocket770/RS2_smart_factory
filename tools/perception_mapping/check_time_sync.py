#!/usr/bin/env python3
"""Print local time-sync state and scan ROS logs for timestamp/TF symptoms."""

import argparse
import shutil
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path
import re


TIME_PATTERNS = {
    "tf_old_data": re.compile(r"\bTF_OLD_DATA\b|old data", re.I),
    "extrapolation": re.compile(r"extrapolat|future|past|earlier than all the data", re.I),
    "message_filter": re.compile(r"message filter|discarding message|dropping message", re.I),
    "clock_jump": re.compile(r"time jump|clock jump|clock jumped|ros time moved", re.I),
    "stale_pose": re.compile(r"became stale|pose stale|stale", re.I),
}


def default_log_root() -> Path:
    latest = Path.home() / ".ros" / "log" / "latest"
    if latest.exists():
        return latest
    return Path.home() / ".ros" / "log"


def print_timedatectl() -> None:
    if shutil.which("timedatectl") is None:
        print("timedatectl: not found")
        return

    try:
        completed = subprocess.run(
            ["timedatectl", "show", "-p", "NTPSynchronized", "-p", "SystemClockSynchronized", "-p", "Timezone"],
            check=False,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=2.0,
        )
    except subprocess.TimeoutExpired:
        print("timedatectl: timed out")
        return

    if completed.returncode != 0:
        print("timedatectl: unavailable")
        return

    for line in completed.stdout.splitlines():
        if line.strip():
            print(line.strip())


def iter_log_files(root: Path, max_files: int):
    files = sorted(
        [path for path in root.rglob("*") if path.is_file() and path.suffix in ("", ".log", ".txt")],
        key=lambda path: path.stat().st_mtime,
        reverse=True,
    )
    yield from files[:max_files]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--log-root", type=Path, default=default_log_root(), help="ROS log directory to scan.")
    parser.add_argument("--max-files", type=int, default=80, help="Maximum newest log files to scan.")
    parser.add_argument("--max-lines", type=int, default=20, help="Maximum matching lines to print.")
    args = parser.parse_args()

    print("Run this on the workstation and each TurtleBot, then compare UTC times.")
    print("UTC=" + datetime.now(timezone.utc).isoformat(timespec="seconds"))
    print_timedatectl()

    root = args.log_root.expanduser()
    if not root.exists():
        print(f"ROS log root not found: {root}")
        return 0

    counts = {name: 0 for name in TIME_PATTERNS}
    examples = []
    scanned = 0

    for path in iter_log_files(root, args.max_files):
        scanned += 1
        try:
            with path.open("r", encoding="utf-8", errors="replace") as stream:
                for line_no, line in enumerate(stream, start=1):
                    for name, pattern in TIME_PATTERNS.items():
                        if pattern.search(line):
                            counts[name] += 1
                            if len(examples) < args.max_lines:
                                examples.append((name, path, line_no, line.strip()[:220]))
                            break
        except OSError as exc:
            if len(examples) < args.max_lines:
                examples.append(("read_error", path, 0, str(exc)))

    print(f"Scanned {scanned} log files under {root}")
    found = {name: count for name, count in counts.items() if count}
    if not found:
        print("No obvious timestamp, TF extrapolation, or stale-pose hints found.")
        return 0

    print("Timestamp-related matches:")
    for name, count in sorted(found.items()):
        print(f"- {name}: {count}")

    print("Examples:")
    for name, path, line_no, text in examples:
        location = f"{path}:{line_no}" if line_no else str(path)
        print(f"- [{name}] {location}: {text}")

    print("If UTC times differ by more than a few seconds across machines, fix time sync and relaunch mapping.")
    return 1


if __name__ == "__main__":
    sys.exit(main())
