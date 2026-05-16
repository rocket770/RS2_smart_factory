#!/usr/bin/env python3
"""Scan ROS logs for network, DDS discovery, and queue-pressure hints."""

import argparse
import re
import sys
from collections import defaultdict
from pathlib import Path


NETWORK_PATTERNS = {
    "dds_discovery": re.compile(r"\b(dds|discovery|participant|domain|rmw|cyclone|fastrtps|fastdds)\b", re.I),
    "transport": re.compile(r"\b(socket|udp|tcp|multicast|interface|transport|unreachable|dropped|packet)\b", re.I),
    "timeout": re.compile(r"\b(timeout|timed out|deadline|not received|lost)\b", re.I),
    "queue_pressure": re.compile(r"\b(queue is full|message queue|dropping message|dropped message)\b", re.I),
}


def default_log_root() -> Path:
    latest = Path.home() / ".ros" / "log" / "latest"
    if latest.exists():
        return latest
    return Path.home() / ".ros" / "log"


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

    root = args.log_root.expanduser()
    if not root.exists():
        print(f"ERROR: log root does not exist: {root}")
        return 2

    counts = defaultdict(int)
    examples = []
    scanned = 0

    for path in iter_log_files(root, args.max_files):
        scanned += 1
        try:
            with path.open("r", encoding="utf-8", errors="replace") as stream:
                for line_no, line in enumerate(stream, start=1):
                    for name, pattern in NETWORK_PATTERNS.items():
                        if pattern.search(line):
                            counts[name] += 1
                            if len(examples) < args.max_lines:
                                text = line.strip()
                                examples.append((name, path, line_no, text[:220]))
                            break
        except OSError as exc:
            if len(examples) < args.max_lines:
                examples.append(("read_error", path, 0, str(exc)))

    print(f"Scanned {scanned} log files under {root}")
    if not counts:
        print("No obvious network, DDS discovery, timeout, or queue-pressure hints found.")
        return 0

    print("Matches:")
    for name in sorted(counts):
        print(f"- {name}: {counts[name]}")

    print("Examples:")
    for name, path, line_no, text in examples:
        location = f"{path}:{line_no}" if line_no else str(path)
        print(f"- [{name}] {location}: {text}")

    if counts.get("queue_pressure") and not any(counts.get(key) for key in ("dds_discovery", "transport", "timeout")):
        print("Queue-pressure messages alone are often acceptable if /tbN/map and /map keep updating.")

    return 1 if any(counts.get(key) for key in ("dds_discovery", "transport", "timeout")) else 0


if __name__ == "__main__":
    sys.exit(main())
