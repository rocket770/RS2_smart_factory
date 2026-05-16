#!/usr/bin/env python3
"""Print ROS_DOMAIN_ID and summarize visible perception/mapping topics."""

import argparse
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path


MAPPING_TOPIC_RE = re.compile(r"^/(tb\d+)/(scan|map|map_metadata|map_updates|odom)$")


def run_topic_list(timeout_sec: float) -> tuple[int, str, str]:
    env = os.environ.copy()
    # ROS 2 CLI tools write log files. Use /tmp defaults so this check still
    # works on read-only home directories or locked-down lab accounts.
    env.setdefault("ROS_LOG_DIR", "/tmp/ros_logs")
    env.setdefault("ROS_HOME", "/tmp/ros_home")
    Path(env["ROS_LOG_DIR"]).mkdir(parents=True, exist_ok=True)
    Path(env["ROS_HOME"]).mkdir(parents=True, exist_ok=True)

    try:
        completed = subprocess.run(
            ["ros2", "topic", "list", "--no-daemon"],
            check=False,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=timeout_sec,
            env=env,
        )
        return completed.returncode, completed.stdout, completed.stderr
    except subprocess.TimeoutExpired:
        return 124, "", f"ros2 topic list timed out after {timeout_sec:.1f}s"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--timeout", type=float, default=5.0, help="Seconds to wait for ros2 topic list.")
    parser.add_argument("--expect", nargs="*", default=[], help="Expected robot namespaces, for example tb1 tb2.")
    args = parser.parse_args()

    domain_id = os.environ.get("ROS_DOMAIN_ID", "<unset>")
    print(f"ROS_DOMAIN_ID={domain_id}")

    if shutil.which("ros2") is None:
        print("ERROR: ros2 was not found. Source /opt/ros/humble/setup.bash and install/setup.bash first.")
        return 2

    code, stdout, stderr = run_topic_list(args.timeout)
    if code != 0:
        print("ERROR: ros2 topic list failed.")
        if stderr.strip():
            print(stderr.strip())
        return 2

    topics = sorted(line.strip() for line in stdout.splitlines() if line.strip())
    by_robot: dict[str, set[str]] = {}
    for topic in topics:
        match = MAPPING_TOPIC_RE.match(topic)
        if match:
            by_robot.setdefault(match.group(1), set()).add(match.group(2))

    print(f"Visible topics: {len(topics)}")
    print(f"Shared /map visible: {'yes' if '/map' in topics else 'no'}")

    if by_robot:
        print("Robot mapping topics:")
        for robot, names in sorted(by_robot.items()):
            print(f"- {robot}: {', '.join(sorted(names))}")
    else:
        print("No /tbN scan/map/odom topics are visible.")

    missing_expected = [robot for robot in args.expect if robot not in by_robot]
    if missing_expected:
        print("Missing expected robots: " + ", ".join(missing_expected))
        print("If those robots are running, check ROS_DOMAIN_ID, sourcing, firewall, and network.")
        return 1

    if not by_robot and "/map" not in topics:
        print("No mapping topics were visible. If robots are running, ROS_DOMAIN_ID mismatch is a likely cause.")
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
