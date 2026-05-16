#!/usr/bin/env python3
"""Check that a ROS map YAML and its image file are readable."""

import argparse
import struct
import sys
from pathlib import Path

try:
    import yaml
except ImportError:
    yaml = None


REQUIRED_KEYS = ("image", "resolution", "origin", "negate", "occupied_thresh", "free_thresh")


def read_yaml(path: Path) -> dict:
    if yaml is None:
        raise RuntimeError("PyYAML is not installed; install python3-yaml or run from a sourced workspace machine.")
    with path.open("r", encoding="utf-8") as stream:
        data = yaml.safe_load(stream)
    if not isinstance(data, dict):
        raise RuntimeError("YAML did not contain a map object.")
    return data


def next_pgm_token(data: bytes, index: int) -> tuple[str, int]:
    while index < len(data):
        byte = data[index]
        if byte == ord("#"):
            while index < len(data) and data[index] not in b"\r\n":
                index += 1
        elif chr(byte).isspace():
            index += 1
        else:
            break

    start = index
    while index < len(data) and not chr(data[index]).isspace():
        index += 1
    if start == index:
        raise RuntimeError("Unexpected end of PGM header.")
    return data[start:index].decode("ascii"), index


def inspect_pgm(path: Path) -> tuple[int, int]:
    data = path.read_bytes()
    magic, index = next_pgm_token(data, 0)
    if magic not in ("P2", "P5"):
        raise RuntimeError(f"Unsupported PGM magic {magic!r}; expected P2 or P5.")
    width_text, index = next_pgm_token(data, index)
    height_text, index = next_pgm_token(data, index)
    max_value_text, index = next_pgm_token(data, index)

    width = int(width_text)
    height = int(height_text)
    max_value = int(max_value_text)
    if width <= 0 or height <= 0:
        raise RuntimeError("PGM width and height must be positive.")
    if not 0 < max_value <= 65535:
        raise RuntimeError("PGM max value is outside the expected range.")
    return width, height


def inspect_png(path: Path) -> tuple[int, int]:
    with path.open("rb") as stream:
        header = stream.read(24)
    if len(header) < 24 or header[:8] != b"\x89PNG\r\n\x1a\n" or header[12:16] != b"IHDR":
        raise RuntimeError("PNG header is invalid.")
    width, height = struct.unpack(">II", header[16:24])
    if width <= 0 or height <= 0:
        raise RuntimeError("PNG width and height must be positive.")
    return width, height


def inspect_image(path: Path) -> tuple[int, int]:
    suffix = path.suffix.lower()
    if suffix == ".pgm":
        return inspect_pgm(path)
    if suffix == ".png":
        return inspect_png(path)
    raise RuntimeError(f"Unsupported image extension {suffix!r}; expected .pgm or .png.")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("map_yaml", type=Path, help="Path to a saved ROS map YAML file.")
    args = parser.parse_args()

    issues = []
    yaml_path = args.map_yaml.expanduser()

    if not yaml_path.exists():
        print(f"ERROR: YAML file does not exist: {yaml_path}")
        return 2

    try:
        data = read_yaml(yaml_path)
    except Exception as exc:
        print(f"ERROR: Could not read YAML: {exc}")
        return 2

    for key in REQUIRED_KEYS:
        if key not in data:
            issues.append(f"missing YAML key: {key}")

    image_value = data.get("image")
    image_path = None
    if isinstance(image_value, str) and image_value:
        candidate = Path(image_value).expanduser()
        image_path = candidate if candidate.is_absolute() else yaml_path.parent / candidate
        if not image_path.exists():
            issues.append(f"image file does not exist: {image_path}")
    else:
        issues.append("image key is empty or not a string")

    resolution = data.get("resolution")
    if not isinstance(resolution, (int, float)) or resolution <= 0:
        issues.append("resolution must be a positive number")

    origin = data.get("origin")
    if not isinstance(origin, list) or len(origin) != 3:
        issues.append("origin must be a three-value list [x, y, yaw]")

    free_thresh = data.get("free_thresh")
    occupied_thresh = data.get("occupied_thresh")
    if isinstance(free_thresh, (int, float)) and isinstance(occupied_thresh, (int, float)):
        if not 0.0 <= free_thresh < occupied_thresh <= 1.0:
            issues.append("thresholds should satisfy 0 <= free_thresh < occupied_thresh <= 1")
    else:
        issues.append("free_thresh and occupied_thresh must be numbers")

    dimensions = None
    if image_path and image_path.exists():
        try:
            dimensions = inspect_image(image_path)
        except Exception as exc:
            issues.append(f"image check failed: {exc}")

    if issues:
        print("Map check FAILED")
        for issue in issues:
            print(f"- {issue}")
        return 1

    print("Map check OK")
    print(f"YAML: {yaml_path}")
    print(f"Image: {image_path}")
    if dimensions:
        print(f"Image size: {dimensions[0]} x {dimensions[1]}")
    print(f"Resolution: {resolution} m/cell")
    return 0


if __name__ == "__main__":
    sys.exit(main())
