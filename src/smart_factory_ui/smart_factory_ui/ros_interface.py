import rclpy
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, HistoryPolicy, QoSProfile, ReliabilityPolicy
from geometry_msgs.msg import PoseStamped
from smart_factory_fleet_msgs.srv import AddTask
from nav_msgs.msg import OccupancyGrid


class FleetGuiNode(Node):
    def __init__(self):
        super().__init__("smart_factory_gui")
        self.add_task_client = self.create_client(AddTask, "/add_task")
        # Map data
        self.latest_map = None
        map_qos = QoSProfile(
            reliability=ReliabilityPolicy.BEST_EFFORT,
            durability=DurabilityPolicy.TRANSIENT_LOCAL,
            history=HistoryPolicy.KEEP_LAST,
            depth=1,
        )
        # Subscribe to merged map
        self.map_sub = self.create_subscription(
            OccupancyGrid,
            "/map",
            self._on_map,
            map_qos,
        )
        self.get_logger().info("Waiting for /add_task service...")
        if not self.add_task_client.wait_for_service(timeout_sec=10.0):
            self.get_logger().error(
                "Service /add_task not available! Make sure the fleet manager node is running."
            )

    def _on_map(self, msg):
        self.latest_map = msg

    def send_task(self, pickup_xy, dropoff_xy, frame="map"):
        if not self.add_task_client.service_is_ready():
            self.get_logger().warn("/add_task service not available, cannot send task")
            return None
        req = AddTask.Request()
        req.type = "pickup_dropoff"
        req.priority = 0
        req.pickup_pose = PoseStamped()
        req.pickup_pose.header.frame_id = frame
        req.pickup_pose.header.stamp = self.get_clock().now().to_msg()
        req.pickup_pose.pose.position.x = float(pickup_xy[0])
        req.pickup_pose.pose.position.y = float(pickup_xy[1])
        req.pickup_pose.pose.orientation.w = 1.0
        req.dropoff_pose = PoseStamped()
        req.dropoff_pose.header.frame_id = frame
        req.dropoff_pose.header.stamp = self.get_clock().now().to_msg()
        req.dropoff_pose.pose.position.x = float(dropoff_xy[0])
        req.dropoff_pose.pose.position.y = float(dropoff_xy[1])
        req.dropoff_pose.pose.orientation.w = 1.0
        return self.add_task_client.call_async(req)
