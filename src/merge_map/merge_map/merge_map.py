import math
import re
from typing import Dict, List, Optional

import rclpy
from rclpy.node import Node
from rclpy.qos import (
    QoSProfile,
    ReliabilityPolicy,
    DurabilityPolicy,
    HistoryPolicy,
)

from nav_msgs.msg import OccupancyGrid


def resolve_cell(
    old_val: int,
    new_val: int,
    unknown_value: int = -1,
    occupied_threshold: int = 50,
) -> int:

    if new_val == unknown_value:
        return old_val

    if old_val == unknown_value:
        return new_val

    old_occ = old_val >= occupied_threshold
    new_occ = new_val >= occupied_threshold

    if old_occ or new_occ:
        return max(old_val, new_val)

    return min(old_val, new_val)


def merge_all_maps(
    maps: List[OccupancyGrid],
    merged_frame_id: str = "merge_map",
    unknown_value: int = -1,
    occupied_threshold: int = 50,
    overwrite_known_cells: bool = False,
) -> Optional[OccupancyGrid]:
    
    if not maps:
        return None

    min_x = min(m.info.origin.position.x for m in maps)
    min_y = min(m.info.origin.position.y for m in maps)

    max_x = max(
        m.info.origin.position.x + (m.info.width * m.info.resolution)
        for m in maps
    )
    max_y = max(
        m.info.origin.position.y + (m.info.height * m.info.resolution)
        for m in maps
    )

    resolution = min(m.info.resolution for m in maps)

    width = int(math.ceil((max_x - min_x) / resolution))
    height = int(math.ceil((max_y - min_y) / resolution))

    merged = OccupancyGrid()
    merged.header = maps[0].header
    merged.header.frame_id = merged_frame_id

    merged.info.resolution = resolution
    merged.info.width = width
    merged.info.height = height
    merged.info.origin.position.x = min_x
    merged.info.origin.position.y = min_y
    merged.info.origin.position.z = 0.0
    merged.info.origin.orientation.x = 0.0
    merged.info.origin.orientation.y = 0.0
    merged.info.origin.orientation.z = 0.0
    merged.info.origin.orientation.w = 1.0

    merged.data = [unknown_value] * (width * height)

    for m in maps:
        for y in range(m.info.height):
            for x in range(m.info.width):
                src_i = x + y * m.info.width
                val = m.data[src_i]

                wx = m.info.origin.position.x + (x * m.info.resolution)
                wy = m.info.origin.position.y + (y * m.info.resolution)

                mx = int(math.floor((wx - min_x) / resolution))
                my = int(math.floor((wy - min_y) / resolution))

                if not (0 <= mx < width and 0 <= my < height):
                    continue

                dst_i = mx + my * width

                if overwrite_known_cells:
                    if val != unknown_value:
                        merged.data[dst_i] = val
                else:
                    merged.data[dst_i] = resolve_cell(
                        merged.data[dst_i],
                        val,
                        unknown_value=unknown_value,
                        occupied_threshold=occupied_threshold,
                    )

    return merged


class MergeMapNode(Node):
    def __init__(self):
        super().__init__("merge_map")

        self.declare_parameter("map_topic_regex", r"^/tb\d+/map$")
        self.declare_parameter("publish_topic", "/merge_map")
        self.declare_parameter("scan_period_sec", 2.0)
        self.declare_parameter("merged_frame_id", "merge_map")
        self.declare_parameter("unknown_value", -1)
        self.declare_parameter("occupied_threshold", 50)
        self.declare_parameter("overwrite_known_cells", False)

        self.map_topic_regex = self.get_parameter("map_topic_regex").value
        self.publish_topic = self.get_parameter("publish_topic").value
        self.scan_period_sec = float(self.get_parameter("scan_period_sec").value)
        self.merged_frame_id = self.get_parameter("merged_frame_id").value
        self.unknown_value = int(self.get_parameter("unknown_value").value)
        self.occupied_threshold = int(self.get_parameter("occupied_threshold").value)
        self.overwrite_known_cells = bool(
            self.get_parameter("overwrite_known_cells").value
        )

        self.topic_pattern = re.compile(self.map_topic_regex)

        self.map_qos = QoSProfile(
            reliability=ReliabilityPolicy.RELIABLE,
            durability=DurabilityPolicy.TRANSIENT_LOCAL,
            history=HistoryPolicy.KEEP_LAST,
            depth=1,
        )

        self.publisher = self.create_publisher(
            OccupancyGrid,
            self.publish_topic,
            self.map_qos,
        )

        self.subscriptions_by_topic: Dict[str, object] = {}
        self.latest_maps: Dict[str, OccupancyGrid] = {}

        self.scan_timer = self.create_timer(
            self.scan_period_sec,
            self.scan_for_map_topics,
        )

        self.get_logger().info(
            f"merge_map started. Watching topics matching: {self.map_topic_regex}"
        )
        self.get_logger().info(
            f"Publishing merged map on: {self.publish_topic}"
        )

    def scan_for_map_topics(self) -> None:
        topics = self.get_topic_names_and_types()

        matching_topics = set()

        for topic_name, topic_types in topics:
            if "nav_msgs/msg/OccupancyGrid" not in topic_types:
                continue

            if not self.topic_pattern.match(topic_name):
                continue

            matching_topics.add(topic_name)

            if topic_name in self.subscriptions_by_topic:
                continue

            self.get_logger().info(f"Discovered map topic: {topic_name}")

            subscription = self.create_subscription(
                OccupancyGrid,
                topic_name,
                lambda msg, t=topic_name: self.map_callback(msg, t),
                self.map_qos,
            )

            self.subscriptions_by_topic[topic_name] = subscription

        vanished_topics = set(self.latest_maps.keys()) - matching_topics
        for topic_name in vanished_topics:
            self.get_logger().warn(f"Map topic no longer visible: {topic_name}")
            self.latest_maps.pop(topic_name, None)

        self.publish_merged_map()

    def map_callback(self, msg: OccupancyGrid, topic_name: str) -> None:
        self.latest_maps[topic_name] = msg
        self.publish_merged_map()

    def publish_merged_map(self) -> None:
        if not self.latest_maps:
            return

        ordered_topics = sorted(self.latest_maps.keys())
        maps_to_merge = [self.latest_maps[t] for t in ordered_topics]

        merged = merge_all_maps(
            maps_to_merge,
            merged_frame_id=self.merged_frame_id,
            unknown_value=self.unknown_value,
            occupied_threshold=self.occupied_threshold,
            overwrite_known_cells=self.overwrite_known_cells,
        )

        if merged is None:
            return

        merged.header.stamp = self.get_clock().now().to_msg()
        self.publisher.publish(merged)


def main(args=None):
    rclpy.init(args=args)
    node = MergeMapNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()