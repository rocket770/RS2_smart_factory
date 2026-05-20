import math
import re
from typing import Dict, List, Optional, Tuple

import rclpy
from rclpy.time import Time
from rclpy.node import Node
from rclpy.qos import (
    QoSProfile,
    ReliabilityPolicy,
    DurabilityPolicy,
    HistoryPolicy,
)

from nav_msgs.msg import OccupancyGrid
from tf2_ros import Buffer, TransformException, TransformListener


def world_to_grid_index(world_coord: float, origin_coord: float, resolution: float) -> int:
    return int(round(((world_coord - origin_coord) / resolution) - 0.5))


def resolve_cell(
    old_val: int,
    new_val: int,
    unknown_value: int = -1,
    occupied_threshold: int = 50,
    conflict_policy: str = "prefer_occupied",
) -> int:
    if new_val == unknown_value:
        return old_val

    if old_val == unknown_value:
        return new_val

    old_occ = old_val >= occupied_threshold
    new_occ = new_val >= occupied_threshold

    if old_occ == new_occ:
        if old_occ:
            return max(old_val, new_val)
        return min(old_val, new_val)

    if conflict_policy == "latest":
        return new_val

    if conflict_policy == "prefer_free":
        return min(old_val, new_val)

    if conflict_policy == "prefer_occupied":
        return max(old_val, new_val)

    return min(old_val, new_val)


def transform_point(x: float, y: float, dx: float, dy: float, yaw: float) -> Tuple[float, float]:
    cos_yaw = math.cos(yaw)
    sin_yaw = math.sin(yaw)

    tx = cos_yaw * x - sin_yaw * y + dx
    ty = sin_yaw * x + cos_yaw * y + dy
    return tx, ty


def yaw_from_quaternion(x: float, y: float, z: float, w: float) -> float:
    siny_cosp = 2.0 * ((w * z) + (x * y))
    cosy_cosp = 1.0 - (2.0 * ((y * y) + (z * z)))
    return math.atan2(siny_cosp, cosy_cosp)


def map_origin_yaw(map_msg: OccupancyGrid) -> float:
    orientation = map_msg.info.origin.orientation
    return yaw_from_quaternion(
        float(orientation.x),
        float(orientation.y),
        float(orientation.z),
        float(orientation.w),
    )


def grid_point_to_map_frame(map_msg: OccupancyGrid, x: float, y: float) -> Tuple[float, float]:
    origin = map_msg.info.origin.position
    return transform_point(
        x,
        y,
        float(origin.x),
        float(origin.y),
        map_origin_yaw(map_msg),
    )


def merge_all_maps(
    maps_with_offsets: List[Tuple[str, OccupancyGrid, Tuple[float, float, float]]],
    merged_frame_id: str = "merge_map",
    unknown_value: int = -1,
    occupied_threshold: int = 50,
    overwrite_known_cells: bool = False,
    conflict_policy: str = "prefer_occupied",
    free_space_clear_radius_cells: int = 0,
) -> Optional[OccupancyGrid]:
    if not maps_with_offsets:
        return None

    resolution = min(m.info.resolution for _, m, _ in maps_with_offsets)

    all_world_points = []

    for _, m, (dx, dy, yaw) in maps_with_offsets:
        corners = [
            (0.0, 0.0),
            (m.info.width * m.info.resolution, 0.0),
            (0.0, m.info.height * m.info.resolution),
            (m.info.width * m.info.resolution, m.info.height * m.info.resolution),
        ]

        for cx, cy in corners:
            local_x, local_y = grid_point_to_map_frame(m, cx, cy)
            wx, wy = transform_point(local_x, local_y, dx, dy, yaw)
            all_world_points.append((wx, wy))

    min_x = min(p[0] for p in all_world_points)
    min_y = min(p[1] for p in all_world_points)
    max_x = max(p[0] for p in all_world_points)
    max_y = max(p[1] for p in all_world_points)

    width = int(math.ceil((max_x - min_x) / resolution)) + 1
    height = int(math.ceil((max_y - min_y) / resolution)) + 1

    merged = OccupancyGrid()
    merged.header = maps_with_offsets[0][1].header
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

    for topic_name, m, (dx, dy, yaw) in maps_with_offsets:
        for y in range(m.info.height):
            for x in range(m.info.width):
                src_i = x + y * m.info.width
                val = m.data[src_i]

                # Merge using cell centers so maps with slightly different origins
                # align better and a newly-free cell can clear a stale obstacle.
                local_x, local_y = grid_point_to_map_frame(
                    m,
                    (x + 0.5) * m.info.resolution,
                    (y + 0.5) * m.info.resolution,
                )

                wx, wy = transform_point(local_x, local_y, dx, dy, yaw)

                mx = world_to_grid_index(wx, min_x, resolution)
                my = world_to_grid_index(wy, min_y, resolution)

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
                        conflict_policy=conflict_policy,
                    )

                    if (
                        free_space_clear_radius_cells > 0
                        and val != unknown_value
                        and val < occupied_threshold
                    ):
                        for ny in range(
                            max(0, my - free_space_clear_radius_cells),
                            min(height, my + free_space_clear_radius_cells + 1),
                        ):
                            for nx in range(
                                max(0, mx - free_space_clear_radius_cells),
                                min(width, mx + free_space_clear_radius_cells + 1),
                            ):
                                if (nx - mx) ** 2 + (ny - my) ** 2 > free_space_clear_radius_cells ** 2:
                                    continue
                                neighbor_i = nx + ny * width
                                merged.data[neighbor_i] = resolve_cell(
                                    merged.data[neighbor_i],
                                    val,
                                    unknown_value=unknown_value,
                                    occupied_threshold=occupied_threshold,
                                    conflict_policy=conflict_policy,
                                )

    return merged


class MergeMapNode(Node):
    @staticmethod
    def reliability_policy(value: str, default: ReliabilityPolicy) -> ReliabilityPolicy:
        normalized = str(value).strip().lower()
        if normalized in ("best_effort", "besteffort", "best-effort"):
            return ReliabilityPolicy.BEST_EFFORT
        if normalized == "reliable":
            return ReliabilityPolicy.RELIABLE
        return default

    def __init__(self):
        super().__init__("merge_map")

        self.declare_parameter("map_topic_regex", r"^/tb\d+/map$")
        self.declare_parameter("publish_topic", "/merge_map")
        self.declare_parameter("input_reliability", "best_effort")
        self.declare_parameter("output_reliability", "reliable")
        self.declare_parameter("scan_period_sec", 2.0)
        self.declare_parameter("publish_period_sec", 1.0)
        self.declare_parameter("merged_frame_id", "merge_map")
        self.declare_parameter("use_tf_transforms", True)
        self.declare_parameter("unknown_value", -1)
        self.declare_parameter("occupied_threshold", 50)
        self.declare_parameter("overwrite_known_cells", False)
        self.declare_parameter("conflict_policy", "prefer_occupied")
        self.declare_parameter("free_space_clear_radius_cells", 0)

        # Per-topic manual offsets: [x, y, yaw]
        self.declare_parameter("map_offsets./tb1/map", [0.0, 0.0, 0.0])
        self.declare_parameter("map_offsets./tb2/map", [0.0, 0.0, 0.0])

        self.map_topic_regex = self.get_parameter("map_topic_regex").value
        self.publish_topic = self.get_parameter("publish_topic").value
        self.input_reliability = self.reliability_policy(
            self.get_parameter("input_reliability").value,
            ReliabilityPolicy.BEST_EFFORT,
        )
        self.output_reliability = self.reliability_policy(
            self.get_parameter("output_reliability").value,
            ReliabilityPolicy.RELIABLE,
        )
        self.scan_period_sec = float(self.get_parameter("scan_period_sec").value)
        self.publish_period_sec = max(
            0.1,
            float(self.get_parameter("publish_period_sec").value),
        )
        self.merged_frame_id = self.get_parameter("merged_frame_id").value
        self.use_tf_transforms = bool(self.get_parameter("use_tf_transforms").value)
        self.unknown_value = int(self.get_parameter("unknown_value").value)
        self.occupied_threshold = int(self.get_parameter("occupied_threshold").value)
        self.overwrite_known_cells = bool(
            self.get_parameter("overwrite_known_cells").value
        )
        self.conflict_policy = str(self.get_parameter("conflict_policy").value)
        self.free_space_clear_radius_cells = int(
            self.get_parameter("free_space_clear_radius_cells").value
        )

        self.topic_pattern = re.compile(self.map_topic_regex)

        self.input_map_qos = QoSProfile(
            reliability=self.input_reliability,
            durability=DurabilityPolicy.TRANSIENT_LOCAL,
            history=HistoryPolicy.KEEP_LAST,
            depth=1,
        )
        self.output_map_qos = QoSProfile(
            reliability=self.output_reliability,
            durability=DurabilityPolicy.TRANSIENT_LOCAL,
            history=HistoryPolicy.KEEP_LAST,
            depth=1,
        )

        self.publisher = self.create_publisher(
            OccupancyGrid,
            self.publish_topic,
            self.output_map_qos,
        )
        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)

        self.subscriptions_by_topic: Dict[str, object] = {}
        self.latest_maps: Dict[str, OccupancyGrid] = {}
        self._tf_fallback_warned_topics = set()
        self._pending_publish = False

        self.get_logger().info("Calling initial scan manually")
        self.scan_for_map_topics()

        self.scan_timer = self.create_timer(
            self.scan_period_sec,
            self.scan_for_map_topics,
        )
        self.publish_timer = self.create_timer(
            self.publish_period_sec,
            self.publish_merged_map,
        )

        self.get_logger().info(
            f"merge_map started. Watching topics matching: {self.map_topic_regex}"
        )
        self.get_logger().info(
            f"Publishing merged map on: {self.publish_topic} "
            f"with {self.output_reliability.name.lower()} reliability "
            f"at up to {1.0 / self.publish_period_sec:.2f} Hz"
        )

    def get_topic_offset(self, topic_name: str) -> Tuple[float, float, float]:
        param_name = f"map_offsets.{topic_name}"
        if self.has_parameter(param_name):
            vals = self.get_parameter(param_name).value
            if isinstance(vals, (list, tuple)) and len(vals) == 3:
                return float(vals[0]), float(vals[1]), float(vals[2])

        self.get_logger().warn(
            f"No offset configured for {topic_name}, using [0.0, 0.0, 0.0]"
        )
        return 0.0, 0.0, 0.0

    def get_map_pose(self, topic_name: str, msg: OccupancyGrid) -> Tuple[float, float, float]:
        if not self.use_tf_transforms:
            return self.get_topic_offset(topic_name)

        source_frame = msg.header.frame_id.lstrip("/")
        target_frame = str(self.merged_frame_id).lstrip("/")

        if not source_frame or source_frame == target_frame:
            return 0.0, 0.0, 0.0

        try:
            transform = self.tf_buffer.lookup_transform(
                target_frame,
                source_frame,
                Time(),
            )
            translation = transform.transform.translation
            rotation = transform.transform.rotation
            return (
                float(translation.x),
                float(translation.y),
                yaw_from_quaternion(
                    float(rotation.x),
                    float(rotation.y),
                    float(rotation.z),
                    float(rotation.w),
                ),
            )
        except TransformException as exc:
            if topic_name not in self._tf_fallback_warned_topics:
                self.get_logger().warn(
                    f"Falling back to manual offset for {topic_name}: "
                    f"no TF from {target_frame} to {source_frame} ({exc})"
                )
                self._tf_fallback_warned_topics.add(topic_name)
            return self.get_topic_offset(topic_name)

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
                self.input_map_qos,
            )
            self.subscriptions_by_topic[topic_name] = subscription

        self._pending_publish = True

    def map_callback(self, msg: OccupancyGrid, topic_name: str) -> None:
        self.latest_maps[topic_name] = msg
        self._pending_publish = True

    def publish_merged_map(self) -> None:
        if not self.latest_maps or not self._pending_publish:
            return

        ordered_topics = sorted(self.latest_maps.keys())
        maps_with_offsets = []

        for topic in ordered_topics:
            map_msg = self.latest_maps[topic]
            pose = self.get_map_pose(topic, map_msg)
            maps_with_offsets.append((topic, map_msg, pose))

        merged = merge_all_maps(
            maps_with_offsets,
            merged_frame_id=self.merged_frame_id,
            unknown_value=self.unknown_value,
            occupied_threshold=self.occupied_threshold,
            overwrite_known_cells=self.overwrite_known_cells,
            conflict_policy=self.conflict_policy,
            free_space_clear_radius_cells=self.free_space_clear_radius_cells,
        )

        if merged is None:
            return

        merged.header.stamp = self.get_clock().now().to_msg()
        self.publisher.publish(merged)
        self._pending_publish = False


def main(args=None):
    rclpy.init(args=args)
    node = MergeMapNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
