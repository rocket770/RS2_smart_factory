import json
from typing import Dict, Optional, Tuple

from geometry_msgs.msg import TransformStamped
from nav_msgs.msg import OccupancyGrid
from nav2_msgs.srv import SaveMap
from rclpy.node import Node
from rclpy.qos import (
    DurabilityPolicy,
    HistoryPolicy,
    QoSProfile,
    ReliabilityPolicy,
    qos_profile_sensor_data,
)
from std_msgs.msg import String
from std_srvs.srv import Trigger
from tf2_msgs.msg import TFMessage

from .mtsp_models import MtspResult


# -----------------------------
# ROS node
# -----------------------------


class MtspPlannerGuiNode(Node):
    def __init__(self):
        super().__init__("mtsp_planner_gui")

        self.latest_map: Optional[OccupancyGrid] = None
        self.latest_result: Optional[MtspResult] = None
        self.robot_positions: Dict[str, Tuple[float, float]] = {}

        self.robot_namespaces = ["tb1", "tb2"]
        self.save_map_client = self.create_client(SaveMap, "/map_saver/save_map")
        self.explorer_start_client = self.create_client(Trigger, "/multi_robot_explorer/start")
        self.explorer_stop_client = self.create_client(Trigger, "/multi_robot_explorer/stop")
        self.explorer_return_home_client = self.create_client(Trigger, "/multi_robot_explorer/return_home")
        self.explorer_status_client = self.create_client(Trigger, "/multi_robot_explorer/status")

        map_qos = QoSProfile(
            reliability=ReliabilityPolicy.RELIABLE,
            durability=DurabilityPolicy.TRANSIENT_LOCAL,
            history=HistoryPolicy.KEEP_LAST,
            depth=1,
        )

        self.create_subscription(OccupancyGrid, "/map", self._on_map, map_qos)
        self.create_subscription(String, "mtsp_best_solution", self._on_result, 10)
        self.create_subscription(TFMessage, "/tf", self._on_tf, qos_profile_sensor_data)
        self.create_subscription(TFMessage, "/tf_static", self._on_tf, qos_profile_sensor_data)

        self.tf_ns_subs = []
        for ns in self.robot_namespaces:
            self.tf_ns_subs.append(
                self.create_subscription(
                    TFMessage,
                    f"/{ns}/tf",
                    self._on_tf,
                    qos_profile_sensor_data,
                )
            )
            self.tf_ns_subs.append(
                self.create_subscription(
                    TFMessage,
                    f"/{ns}/tf_static",
                    self._on_tf,
                    qos_profile_sensor_data,
                )
            )

    def _on_map(self, msg: OccupancyGrid):
        self.latest_map = msg
        self.get_logger().info(
            f"Received map: width={msg.info.width}, "
            f"height={msg.info.height}, "
            f"resolution={msg.info.resolution}"
        )

    def _on_result(self, msg: String):
        try:
            data = json.loads(msg.data)
            self.latest_result = MtspResult(
                robot_starts=[tuple(p) for p in data["robot_starts"]],
                goals=[tuple(p) for p in data["goals"]],
                routes=[list(r) for r in data["routes"]],
                generation=int(data["generation"]),
                cost=float(data["cost"]),
            )
        except Exception as exc:
            self.get_logger().error(f"Failed to parse MTSP result: {exc}")

    def _on_tf(self, msg: TFMessage):
        for t in msg.transforms:
            robot_name = self._robot_name_from_transform(t)
            if robot_name is None:
                continue
            self.robot_positions[robot_name] = (
                float(t.transform.translation.x),
                float(t.transform.translation.y),
            )

    def _robot_name_from_transform(self, t: TransformStamped) -> Optional[str]:
        child = t.child_frame_id.lstrip("/")
        frame = t.header.frame_id.lstrip("/")

        for ns in self.robot_namespaces:
            if child.startswith(f"{ns}/") and child.endswith(("base_link", "base_footprint")):
                return ns
            if frame.startswith(f"{ns}/") and child.endswith(("base_link", "base_footprint")):
                return ns

        if child in ("base_link", "base_footprint"):
            return None

        return None
