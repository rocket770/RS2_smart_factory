
import math
import re
from dataclasses import dataclass
from typing import Dict, List, Optional, Set, Tuple

import rclpy
from rclpy.action import ActionClient
from rclpy.duration import Duration
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, HistoryPolicy, QoSProfile, ReliabilityPolicy
from rclpy.time import Time

from geometry_msgs.msg import Point, PoseStamped, Quaternion, TransformStamped
from nav2_msgs.action import NavigateToPose
from nav_msgs.msg import OccupancyGrid
from std_srvs.srv import Trigger
from tf2_msgs.msg import TFMessage
from visualization_msgs.msg import Marker, MarkerArray


FREE = 0
UNKNOWN = -1
OCCUPIED_THRESHOLD = 50


def best_effort_qos(depth: int = 10, transient_local: bool = False) -> QoSProfile:
    durability = (
        DurabilityPolicy.TRANSIENT_LOCAL
        if transient_local
        else DurabilityPolicy.VOLATILE
    )
    return QoSProfile(
        reliability=ReliabilityPolicy.BEST_EFFORT,
        durability=durability,
        history=HistoryPolicy.KEEP_LAST,
        depth=depth,
    )


@dataclass
class Frontier:
    frontier_id: int
    cells: List[Tuple[int, int]]
    centroid_world: Tuple[float, float]
    size: int


@dataclass
class FrontierAssignmentCandidate:
    frontier: Frontier
    goal_xy: Tuple[float, float]
    goal_distance: float
    cluster_distance: float
    score: float


@dataclass
class AssignedGoal:
    frontier_id: int
    goal_xy: Tuple[float, float]
    assigned_at: Time
    accepted: bool = False
    goal_kind: str = 'frontier'  # frontier | home


@dataclass
class BlacklistEntry:
    robot_ns: Optional[str]
    goal_xy: Tuple[float, float]
    until: Time
    reason: str = ''


@dataclass
class RobotHandle:
    namespace: str
    action_client: ActionClient
    current_goal_handle: Optional[object] = None
    result_future: Optional[object] = None
    assigned_goal: Optional[AssignedGoal] = None
    last_pose_xy: Optional[Tuple[float, float]] = None
    last_pose_stamp: Optional[Time] = None
    available: bool = False
    failures: int = 0
    home_pose_xy: Optional[Tuple[float, float]] = None

    def is_idle(self) -> bool:
        return self.available and self.assigned_goal is None and self.current_goal_handle is None

    def is_busy(self) -> bool:
        return self.assigned_goal is not None or self.current_goal_handle is not None


class RobotTfStore:
    """Stores TF messages and computes 2D robot pose across shared or namespaced TF trees."""

    def __init__(self, node: Node, namespace: str, preferred_base_frame: str, fallback_base_frame: str):
        self._node = node
        self.namespace = namespace
        self.preferred_base_frame = preferred_base_frame
        self.fallback_base_frame = fallback_base_frame
        self.transforms: Dict[Tuple[str, str], TransformStamped] = {}
        self.last_update_time: Optional[Time] = None
        tf_qos = best_effort_qos(depth=100)
        tf_static_qos = best_effort_qos(depth=1, transient_local=True)
        namespaced_tf_qos = best_effort_qos(depth=50)
        self.tf_sub = node.create_subscription(
            TFMessage,
            '/tf',
            self._tf_callback,
            tf_qos,
        )
        self.tf_static_sub = node.create_subscription(
            TFMessage,
            '/tf_static',
            self._tf_callback,
            tf_static_qos,
        )
        self.namespaced_tf_sub = node.create_subscription(
            TFMessage,
            f'/{namespace}/tf',
            self._tf_callback,
            namespaced_tf_qos,
        )
        self.namespaced_tf_static_sub = node.create_subscription(
            TFMessage,
            f'/{namespace}/tf_static',
            self._tf_callback,
            tf_static_qos,
        )

    def _tf_callback(self, msg: TFMessage) -> None:
        now = self._node.get_clock().now()
        for t in msg.transforms:
            parent = self._strip(t.header.frame_id)
            child = self._strip(t.child_frame_id)
            self.transforms[(parent, child)] = t
        self.last_update_time = now

    @staticmethod
    def _strip(frame_id: str) -> str:
        return frame_id.strip('/')

    @staticmethod
    def _yaw_from_quaternion(q: Quaternion) -> float:
        siny_cosp = 2.0 * (q.w * q.z + q.x * q.y)
        cosy_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z)
        return math.atan2(siny_cosp, cosy_cosp)

    @staticmethod
    def _compose_2d(a: Tuple[float, float, float], b: Tuple[float, float, float]) -> Tuple[float, float, float]:
        ax, ay, ath = a
        bx, by, bth = b
        c = math.cos(ath)
        s = math.sin(ath)
        x = ax + c * bx - s * by
        y = ay + s * bx + c * by
        th = math.atan2(math.sin(ath + bth), math.cos(ath + bth))
        return x, y, th

    @staticmethod
    def _invert_2d(t: Tuple[float, float, float]) -> Tuple[float, float, float]:
        x, y, th = t
        c = math.cos(th)
        s = math.sin(th)
        inv_x = -(c * x + s * y)
        inv_y = -(-s * x + c * y)
        inv_th = -th
        return inv_x, inv_y, inv_th

    def _transform_to_2d(self, tf: TransformStamped) -> Tuple[float, float, float]:
        tr = tf.transform.translation
        rot = tf.transform.rotation
        return tr.x, tr.y, self._yaw_from_quaternion(rot)

    def lookup_pose_xy(self) -> Optional[Tuple[float, float]]:
        for source_frame, target_frame in self._pose_lookup_pairs():
            pose = self._resolve_2d(source_frame, target_frame)
            if pose is not None:
                return pose[0], pose[1]
        return None

    def _pose_lookup_pairs(self) -> List[Tuple[str, str]]:
        shared_map_frame = 'map'
        local_map_frame = f'{self.namespace}/map'
        target_frames = self._candidate_target_frames()

        pairs: List[Tuple[str, str]] = []
        for source_frame in [shared_map_frame, local_map_frame]:
            for target_frame in target_frames:
                pair = (source_frame, target_frame)
                if pair not in pairs:
                    pairs.append(pair)
        return pairs

    def _candidate_target_frames(self) -> List[str]:
        candidates: List[str] = []
        for frame in [self.preferred_base_frame, self.fallback_base_frame]:
            for candidate in [f'{self.namespace}/{frame}', frame]:
                if candidate not in candidates:
                    candidates.append(candidate)
        return candidates

    def _resolve_2d(self, source: str, target: str) -> Optional[Tuple[float, float, float]]:
        if source == target:
            return (0.0, 0.0, 0.0)

        visited: Set[str] = set()
        queue: List[Tuple[str, Tuple[float, float, float]]] = [(source, (0.0, 0.0, 0.0))]

        while queue:
            frame, pose = queue.pop(0)
            if frame in visited:
                continue
            visited.add(frame)

            for (parent, child), tf in list(self.transforms.items()):
                t2d = self._transform_to_2d(tf)
                if parent == frame and child not in visited:
                    new_pose = self._compose_2d(pose, t2d)
                    if child == target:
                        return new_pose
                    queue.append((child, new_pose))
                elif child == frame and parent not in visited:
                    new_pose = self._compose_2d(pose, self._invert_2d(t2d))
                    if parent == target:
                        return new_pose
                    queue.append((parent, new_pose))
        return None


class MultiRobotExplorer(Node):
    def __init__(self) -> None:
        super().__init__('multi_robot_explorer')

        self.declare_parameter('map_topic', '/map')
        self.declare_parameter('robot_namespace_regex', '^tb[0-9]+$')
        self.declare_parameter('base_frame', 'base_link')
        self.declare_parameter('fallback_base_frame', 'base_footprint')
        self.declare_parameter('start_paused', True)
        self.declare_parameter('planning_period_sec', 1.0)
        self.declare_parameter('discovery_period_sec', 2.0)
        self.declare_parameter('reservation_radius_m', 1.5)
        self.declare_parameter('frontier_min_cluster_size', 4)
        self.declare_parameter('frontier_merge_radius_m', 0.75)
        self.declare_parameter('min_frontier_separation_m', 1.5)
        self.declare_parameter('goal_tolerance_m', 0.75)
        self.declare_parameter('min_goal_distance_m', 1.0)
        self.declare_parameter('min_goal_distance_floor_m', 0.20)
        self.declare_parameter('goal_cooldown_sec', 15.0)
        self.declare_parameter('robot_stale_timeout_sec', 5.0)
        self.declare_parameter('assignment_timeout_sec', 30.0)
        self.declare_parameter('blacklist_timeout_sec', 20.0)
        self.declare_parameter('cancel_goals_on_pause', False)
        self.declare_parameter('score_distance_weight', 1.0)
        self.declare_parameter('score_size_weight', 0.15)
        self.declare_parameter('score_home_bias_weight', 0.25)
        self.declare_parameter('enable_rviz_markers', True)
        self.declare_parameter('marker_topic', '~/markers')
        self.declare_parameter('marker_lifetime_sec', 1.5)

        self.map_topic = self.get_parameter('map_topic').value
        self.robot_ns_regex = re.compile(self.get_parameter('robot_namespace_regex').value)
        self.base_frame = self.get_parameter('base_frame').value
        self.fallback_base_frame = self.get_parameter('fallback_base_frame').value
        self.start_paused = bool(self.get_parameter('start_paused').value)
        self.planning_period_sec = float(self.get_parameter('planning_period_sec').value)
        self.discovery_period_sec = float(self.get_parameter('discovery_period_sec').value)
        self.reservation_radius_m = float(self.get_parameter('reservation_radius_m').value)
        self.frontier_min_cluster_size = int(self.get_parameter('frontier_min_cluster_size').value)
        self.frontier_merge_radius_m = float(self.get_parameter('frontier_merge_radius_m').value)
        self.min_frontier_separation_m = float(self.get_parameter('min_frontier_separation_m').value)
        self.goal_tolerance_m = float(self.get_parameter('goal_tolerance_m').value)
        self.min_goal_distance_m = float(self.get_parameter('min_goal_distance_m').value)
        self.min_goal_distance_floor_m = float(self.get_parameter('min_goal_distance_floor_m').value)
        self.goal_cooldown_sec = float(self.get_parameter('goal_cooldown_sec').value)
        self.robot_stale_timeout_sec = float(self.get_parameter('robot_stale_timeout_sec').value)
        self.assignment_timeout_sec = float(self.get_parameter('assignment_timeout_sec').value)
        self.blacklist_timeout_sec = float(self.get_parameter('blacklist_timeout_sec').value)
        self.cancel_goals_on_pause = bool(self.get_parameter('cancel_goals_on_pause').value)
        self.score_distance_weight = float(self.get_parameter('score_distance_weight').value)
        self.score_size_weight = float(self.get_parameter('score_size_weight').value)
        self.score_home_bias_weight = float(self.get_parameter('score_home_bias_weight').value)
        self.enable_rviz_markers = bool(self.get_parameter('enable_rviz_markers').value)
        self.marker_topic = self.get_parameter('marker_topic').value
        self.marker_lifetime_sec = float(self.get_parameter('marker_lifetime_sec').value)

        self.paused = self.start_paused
        self.map_msg: Optional[OccupancyGrid] = None
        self.robot_tf_stores: Dict[str, RobotTfStore] = {}
        self.robots: Dict[str, RobotHandle] = {}
        self.blacklist: List[BlacklistEntry] = []
        self.next_frontier_id = 1

        self.latest_frontiers: List[Frontier] = []
        self.latest_available_frontiers: List[Frontier] = []

        self.map_sub = self.create_subscription(
            OccupancyGrid,
            self.map_topic,
            self._map_callback,
            best_effort_qos(depth=1, transient_local=True),
        )
        self.marker_pub = (
            self.create_publisher(
                MarkerArray,
                self.marker_topic,
                best_effort_qos(depth=10),
            )
            if self.enable_rviz_markers else None
        )

        self.start_srv = self.create_service(Trigger, '~/start', self._handle_start)
        self.pause_srv = self.create_service(Trigger, '~/pause', self._handle_pause)
        self.stop_srv = self.create_service(Trigger, '~/stop', self._handle_stop)
        self.return_home_srv = self.create_service(Trigger, '~/return_home', self._handle_return_home)
        self.status_srv = self.create_service(Trigger, '~/status', self._handle_status)

        self.discovery_timer = self.create_timer(self.discovery_period_sec, self._discovery_tick)
        self.plan_timer = self.create_timer(self.planning_period_sec, self._planning_tick)

        self.get_logger().info(
            f'MultiRobotExplorer started. map_topic={self.map_topic}, paused={self.paused}, '
            f'robot_namespace_regex={self.robot_ns_regex.pattern}, markers={self.enable_rviz_markers}'
        )

    def _handle_start(self, request, response):
        del request
        self.paused = False
        captured = []
        missing = []
        for ns, robot in sorted(self.robots.items()):
            if robot.last_pose_xy is not None:
                robot.home_pose_xy = robot.last_pose_xy
                captured.append(f"{ns}=({robot.last_pose_xy[0]:.2f},{robot.last_pose_xy[1]:.2f})")
            else:
                missing.append(ns)
        response.success = True
        response.message = 'Explorer running.'
        if captured:
            response.message += ' Captured home poses: ' + ', '.join(captured)
        if missing:
            response.message += ' Missing pose for: ' + ', '.join(missing)
        self.get_logger().info(response.message)
        return response

    def _handle_pause(self, request, response):
        del request
        self.paused = True
        if self.cancel_goals_on_pause:
            self._cancel_all_goals(clear_assignments=False)
            msg = 'Explorer paused. Active goals cancel requested.'
        else:
            msg = 'Explorer paused. No new goals will be assigned.'
        response.success = True
        response.message = msg
        self.get_logger().info(msg)
        return response

    def _handle_stop(self, request, response):
        del request
        self.paused = True
        self._cancel_all_goals(clear_assignments=True)
        response.success = True
        response.message = 'Explorer stopped. Active goals cleared and reservations released.'
        self.get_logger().info(response.message)
        return response

    def _handle_return_home(self, request, response):
        del request
        self.paused = True
        self._cancel_all_goals(clear_assignments=True)
        self._send_idle_robots_home()

        returning = []
        already_home = []
        missing_home = []
        missing_pose = []
        for ns, robot in sorted(self.robots.items()):
            if robot.home_pose_xy is None:
                missing_home.append(ns)
                continue
            if robot.last_pose_xy is None:
                missing_pose.append(ns)
                continue
            if robot.assigned_goal is not None and robot.assigned_goal.goal_kind == 'home':
                returning.append(ns)
                continue
            if self._distance_xy(robot.last_pose_xy, robot.home_pose_xy) <= self.goal_tolerance_m:
                already_home.append(ns)

        parts = ['Explorer paused and existing return-home logic was requested.']
        if returning:
            parts.append('Returning: ' + ', '.join(returning))
        if already_home:
            parts.append('Already home: ' + ', '.join(already_home))
        if missing_home:
            parts.append('Missing home pose: ' + ', '.join(missing_home))
        if missing_pose:
            parts.append('Missing robot pose: ' + ', '.join(missing_pose))

        response.success = bool(returning or already_home) and not missing_home and not missing_pose
        response.message = ' '.join(parts)
        self.get_logger().info(response.message)
        return response

    def _handle_status(self, request, response):
        del request
        robot_parts = []
        for ns, robot in sorted(self.robots.items()):
            pose = robot.last_pose_xy
            pose_txt = 'None' if pose is None else f'({pose[0]:.2f},{pose[1]:.2f})'
            state = 'busy' if robot.is_busy() else 'idle'
            robot_parts.append(f'{ns}: available={robot.available}, state={state}, pose={pose_txt}')
        response.success = True
        response.message = (
            f'paused={self.paused}; map_ready={self.map_msg is not None}; robots={len(self.robots)}; '
            + ' | '.join(robot_parts)
        )
        return response

    def _map_callback(self, msg: OccupancyGrid) -> None:
        self.map_msg = msg

    def _discovery_tick(self) -> None:
        namespaces = self._discover_robot_namespaces()
        for ns in namespaces:
            if ns not in self.robot_tf_stores:
                self.robot_tf_stores[ns] = RobotTfStore(self, ns, self.base_frame, self.fallback_base_frame)
                self.get_logger().info(f'Discovered TF bus for robot namespace: {ns}')
            if ns not in self.robots:
                action_client = ActionClient(self, NavigateToPose, f'/{ns}/navigate_to_pose')
                self.robots[ns] = RobotHandle(namespace=ns, action_client=action_client)
                self.get_logger().info(f'Created NavigateToPose action client for /{ns}/navigate_to_pose')

        now = self.get_clock().now()
        stale_namespaces = []
        for ns, store in self.robot_tf_stores.items():
            robot = self.robots.get(ns)
            if robot is None:
                continue
            pose = store.lookup_pose_xy()
            if pose is not None:
                robot.last_pose_xy = pose
                robot.last_pose_stamp = now
            robot.available = robot.action_client.server_is_ready()
            if robot.last_pose_stamp is not None:
                age = (now - robot.last_pose_stamp).nanoseconds / 1e9
                if age > self.robot_stale_timeout_sec:
                    stale_namespaces.append(ns)
                    self._release_robot_assignment(ns, reason='pose stale')
                    robot.available = False

        for ns in stale_namespaces:
            self.get_logger().warn(f'Robot {ns} became stale; keeping it discovered but temporarily unavailable.')

        self._publish_debug_markers()

    def _planning_tick(self) -> None:
        self._prune_blacklist()
        self._refresh_robot_goal_states()

        if self.paused:
            return
        if self.map_msg is None:
            self.get_logger().debug('No map yet; skipping planning tick.')
            return

        frontiers = self._detect_frontiers(self.map_msg)
        self.latest_frontiers = list(frontiers)
        if not frontiers:
            self.latest_available_frontiers = []
            self.get_logger().debug('No frontiers found on merged map.')
            self._send_idle_robots_home()
            self._publish_debug_markers()
            return

        now = self.get_clock().now()
        for ns, robot in self.robots.items():
            if robot.assigned_goal is not None:
                age = (now - robot.assigned_goal.assigned_at).nanoseconds / 1e9
                if age > self.assignment_timeout_sec:
                    self.get_logger().warn(f'Assignment timed out for {ns}; releasing frontier {robot.assigned_goal.frontier_id}.')
                    self._release_robot_assignment(ns, reason='assignment timeout')

        available_frontiers = self._filter_reserved_blacklisted_frontiers(frontiers)
        self.latest_available_frontiers = list(available_frontiers)
        if not available_frontiers:
            self._send_idle_robots_home()
            self._publish_debug_markers()
            return

        idle_robots = [r for _, r in sorted(self.robots.items()) if r.is_idle() and r.last_pose_xy is not None]
        if not idle_robots:
            self._publish_debug_markers()
            return

        reserved_points = [
            robot.assigned_goal.goal_xy
            for robot in self.robots.values()
            if robot.assigned_goal is not None
        ]

        chosen_candidates = self._choose_frontiers_globally(idle_robots, available_frontiers, reserved_points)
        chosen_frontier_ids = set()
        for robot, candidate in chosen_candidates:
            chosen_frontier_ids.add(candidate.frontier.frontier_id)
            self._dispatch_goal(robot, candidate.frontier, candidate.goal_xy)

        self.latest_available_frontiers = [
            frontier for frontier in available_frontiers
            if frontier.frontier_id not in chosen_frontier_ids
        ]
        self._publish_debug_markers()

    def _publish_debug_markers(self) -> None:
        if self.marker_pub is None:
            return

        now_msg = self.get_clock().now().to_msg()
        lifetime = Duration(seconds=self.marker_lifetime_sec).to_msg()
        markers: List[Marker] = []

        delete_all = Marker()
        delete_all.header.frame_id = 'map'
        delete_all.header.stamp = now_msg
        delete_all.action = Marker.DELETEALL
        markers.append(delete_all)

        marker_id = 0

        def next_id() -> int:
            nonlocal marker_id
            marker_id += 1
            return marker_id

        def make_marker(ns: str, marker_type: int, x: float = 0.0, y: float = 0.0) -> Marker:
            m = Marker()
            m.header.frame_id = 'map'
            m.header.stamp = now_msg
            m.ns = ns
            m.id = next_id()
            m.type = marker_type
            m.action = Marker.ADD
            m.pose.orientation.w = 1.0
            m.pose.position.x = x
            m.pose.position.y = y
            m.pose.position.z = 0.0
            m.lifetime = lifetime
            return m

        for frontier in self.latest_frontiers:
            m = make_marker('frontiers_all', Marker.SPHERE, frontier.centroid_world[0], frontier.centroid_world[1])
            m.scale.x = 0.18
            m.scale.y = 0.18
            m.scale.z = 0.18
            m.color.a = 0.35
            m.color.r = 0.1
            m.color.g = 0.6
            m.color.b = 1.0
            markers.append(m)

        for frontier in self.latest_available_frontiers:
            m = make_marker('frontiers_available', Marker.SPHERE, frontier.centroid_world[0], frontier.centroid_world[1])
            m.scale.x = 0.25
            m.scale.y = 0.25
            m.scale.z = 0.25
            m.color.a = 0.9
            m.color.r = 0.0
            m.color.g = 1.0
            m.color.b = 0.2
            markers.append(m)

            label = make_marker('frontier_labels', Marker.TEXT_VIEW_FACING, frontier.centroid_world[0], frontier.centroid_world[1])
            label.pose.position.z = 0.25
            label.scale.z = 0.22
            label.color.a = 1.0
            label.color.r = 1.0
            label.color.g = 1.0
            label.color.b = 1.0
            label.text = f'F{frontier.frontier_id} s={frontier.size}'
            markers.append(label)

        now = self.get_clock().now()
        for entry in self.blacklist:
            if entry.until <= now:
                continue
            m = make_marker('blacklist', Marker.CYLINDER, entry.goal_xy[0], entry.goal_xy[1])
            m.scale.x = self.reservation_radius_m * 2.0
            m.scale.y = self.reservation_radius_m * 2.0
            m.scale.z = 0.05
            m.color.a = 0.35
            m.color.r = 1.0
            m.color.g = 0.2
            m.color.b = 0.2
            markers.append(m)

        for ns, robot in sorted(self.robots.items()):
            if robot.home_pose_xy is not None:
                home = make_marker('robot_home', Marker.SPHERE, robot.home_pose_xy[0], robot.home_pose_xy[1])
                home.scale.x = 0.20
                home.scale.y = 0.20
                home.scale.z = 0.20
                home.color.a = 0.85
                home.color.r = 0.9
                home.color.g = 0.0
                home.color.b = 0.9
                markers.append(home)

            if robot.last_pose_xy is not None:
                body = make_marker('robot_pose', Marker.ARROW, robot.last_pose_xy[0], robot.last_pose_xy[1])
                body.scale.x = 0.45
                body.scale.y = 0.12
                body.scale.z = 0.12
                body.color.a = 1.0
                if robot.is_busy():
                    body.color.r = 1.0
                    body.color.g = 0.8
                    body.color.b = 0.0
                elif robot.available:
                    body.color.r = 0.1
                    body.color.g = 1.0
                    body.color.b = 1.0
                else:
                    body.color.r = 0.5
                    body.color.g = 0.5
                    body.color.b = 0.5
                markers.append(body)

                text = make_marker('robot_labels', Marker.TEXT_VIEW_FACING, robot.last_pose_xy[0], robot.last_pose_xy[1])
                text.pose.position.z = 0.45
                text.scale.z = 0.24
                text.color.a = 1.0
                text.color.r = 1.0
                text.color.g = 1.0
                text.color.b = 1.0
                if robot.assigned_goal is not None and robot.assigned_goal.goal_kind == 'home':
                    state = 'going_home'
                else:
                    state = 'busy' if robot.is_busy() else ('ready' if robot.available else 'down')
                text.text = f'{ns} {state}'
                markers.append(text)

            if robot.assigned_goal is not None:
                gx, gy = robot.assigned_goal.goal_xy
                goal = make_marker('assigned_goals', Marker.CYLINDER, gx, gy)
                goal.scale.x = 0.35
                goal.scale.y = 0.35
                goal.scale.z = 0.08
                goal.color.a = 0.95
                if robot.assigned_goal.goal_kind == 'home':
                    goal.color.r = 0.9
                    goal.color.g = 0.0
                    goal.color.b = 0.9
                else:
                    goal.color.r = 1.0
                    goal.color.g = 0.5
                    goal.color.b = 0.0
                markers.append(goal)

                ring = make_marker('reservation_radius', Marker.CYLINDER, gx, gy)
                ring.scale.x = self.reservation_radius_m * 2.0
                ring.scale.y = self.reservation_radius_m * 2.0
                ring.scale.z = 0.03
                ring.color.a = 0.2
                if robot.assigned_goal.goal_kind == 'home':
                    ring.color.r = 0.9
                    ring.color.g = 0.0
                    ring.color.b = 0.9
                else:
                    ring.color.r = 1.0
                    ring.color.g = 0.8
                    ring.color.b = 0.0
                markers.append(ring)

                if robot.last_pose_xy is not None:
                    line = make_marker('assignment_lines', Marker.LINE_STRIP)
                    line.scale.x = 0.05
                    line.color.a = 0.95
                    if robot.assigned_goal.goal_kind == 'home':
                        line.color.r = 0.9
                        line.color.g = 0.0
                        line.color.b = 0.9
                    else:
                        line.color.r = 1.0
                        line.color.g = 0.8
                        line.color.b = 0.0
                    p0 = Point()
                    p0.x, p0.y, p0.z = robot.last_pose_xy[0], robot.last_pose_xy[1], 0.05
                    p1 = Point()
                    p1.x, p1.y, p1.z = gx, gy, 0.05
                    line.points = [p0, p1]
                    markers.append(line)

        self.marker_pub.publish(MarkerArray(markers=markers))

    def _discover_robot_namespaces(self) -> List[str]:
        namespaces: Set[str] = set()
        for topic_name, _types in self.get_topic_names_and_types():
            parts = topic_name.strip('/').split('/')
            if len(parts) >= 2:
                ns = parts[0]
                if self.robot_ns_regex.match(ns):
                    namespaces.add(ns)
        return sorted(namespaces)

    def _detect_frontiers(self, map_msg: OccupancyGrid) -> List[Frontier]:
        width = map_msg.info.width
        height = map_msg.info.height
        if width == 0 or height == 0:
            return []

        data = list(map_msg.data)
        frontiers: List[Frontier] = []
        visited_frontier: Set[Tuple[int, int]] = set()

        for y in range(height):
            for x in range(width):
                if (x, y) in visited_frontier:
                    continue
                if not self._is_frontier_cell(data, width, height, x, y):
                    continue
                cells = self._grow_frontier_cluster(data, width, height, x, y, visited_frontier)
                if len(cells) < self.frontier_min_cluster_size:
                    continue
                centroid_world = self._cells_centroid_to_world(map_msg, cells)
                frontiers.append(Frontier(self.next_frontier_id, cells, centroid_world, len(cells)))
                self.next_frontier_id += 1
        return self._merge_nearby_frontiers(map_msg, frontiers)

    def _grow_frontier_cluster(self, data: List[int], width: int, height: int, start_x: int, start_y: int,
                               visited_frontier: Set[Tuple[int, int]]) -> List[Tuple[int, int]]:
        queue = [(start_x, start_y)]
        cluster: List[Tuple[int, int]] = []
        while queue:
            x, y = queue.pop(0)
            if (x, y) in visited_frontier:
                continue
            visited_frontier.add((x, y))
            if not self._is_frontier_cell(data, width, height, x, y):
                continue
            cluster.append((x, y))
            for nx, ny in self._neighbors8(x, y, width, height):
                if (nx, ny) not in visited_frontier:
                    queue.append((nx, ny))
        return cluster

    def _is_frontier_cell(self, data: List[int], width: int, height: int, x: int, y: int) -> bool:
        idx = y * width + x
        if data[idx] != FREE:
            return False
        has_unknown_neighbor = False
        for nx, ny in self._neighbors8(x, y, width, height):
            nval = data[ny * width + nx]
            if nval == UNKNOWN:
                has_unknown_neighbor = True
            if nval >= OCCUPIED_THRESHOLD:
                return False
        return has_unknown_neighbor

    @staticmethod
    def _neighbors8(x: int, y: int, width: int, height: int) -> List[Tuple[int, int]]:
        result = []
        for dy in (-1, 0, 1):
            for dx in (-1, 0, 1):
                if dx == 0 and dy == 0:
                    continue
                nx = x + dx
                ny = y + dy
                if 0 <= nx < width and 0 <= ny < height:
                    result.append((nx, ny))
        return result

    @staticmethod
    def _cells_centroid_to_world(map_msg: OccupancyGrid, cells: List[Tuple[int, int]]) -> Tuple[float, float]:
        resolution = map_msg.info.resolution
        ox = map_msg.info.origin.position.x
        oy = map_msg.info.origin.position.y
        mean_x = sum(c[0] for c in cells) / len(cells)
        mean_y = sum(c[1] for c in cells) / len(cells)
        wx = ox + (mean_x + 0.5) * resolution
        wy = oy + (mean_y + 0.5) * resolution
        return wx, wy

    @staticmethod
    def _cell_to_world(map_msg: OccupancyGrid, cell: Tuple[int, int]) -> Tuple[float, float]:
        resolution = map_msg.info.resolution
        ox = map_msg.info.origin.position.x
        oy = map_msg.info.origin.position.y
        return ox + (cell[0] + 0.5) * resolution, oy + (cell[1] + 0.5) * resolution

    def _merge_nearby_frontiers(self, map_msg: OccupancyGrid, frontiers: List[Frontier]) -> List[Frontier]:
        merged: List[Frontier] = []
        for frontier in sorted(frontiers, key=lambda f: f.size, reverse=True):
            merged_into = None
            for existing in merged:
                if self._distance_xy(frontier.centroid_world, existing.centroid_world) < self.frontier_merge_radius_m:
                    merged_into = existing
                    break
            if merged_into is None:
                merged.append(frontier)
                continue
            merged_into.cells.extend(frontier.cells)
            merged_into.size = len(merged_into.cells)
            merged_into.centroid_world = self._cells_centroid_to_world(map_msg, merged_into.cells)
        return merged

    def _filter_reserved_blacklisted_frontiers(self, frontiers: List[Frontier]) -> List[Frontier]:
        now = self.get_clock().now()
        reserved_points = [robot.assigned_goal.goal_xy for robot in self.robots.values() if robot.assigned_goal is not None]
        filtered: List[Frontier] = []
        for frontier in frontiers:
            if any(self._distance_xy(frontier.centroid_world, rp) < self.reservation_radius_m for rp in reserved_points):
                continue
            blocked = False
            for entry in self.blacklist:
                if entry.until > now and self._distance_xy(frontier.centroid_world, entry.goal_xy) < self.reservation_radius_m:
                    blocked = True
                    break
            if not blocked:
                filtered.append(frontier)
        return filtered

    def _choose_goal_point_for_frontier(self, robot: RobotHandle, frontier: Frontier) -> Optional[Tuple[Tuple[float, float], float]]:
        if robot.last_pose_xy is None or self.map_msg is None:
            return None
        candidates: List[Tuple[float, Tuple[float, float]]] = []
        fallback: List[Tuple[float, Tuple[float, float]]] = []
        min_reject = max(self.min_goal_distance_floor_m, min(self.goal_tolerance_m, self.min_goal_distance_floor_m))
        for cell in frontier.cells:
            goal_xy = self._cell_to_world(self.map_msg, cell)
            dist = self._distance_xy(robot.last_pose_xy, goal_xy)
            if dist <= self.goal_tolerance_m:
                continue
            if dist >= self.min_goal_distance_floor_m:
                candidates.append((dist, goal_xy))
            else:
                fallback.append((dist, goal_xy))
        pool = candidates if candidates else fallback
        if not pool:
            return None
        dist, goal_xy = max(pool, key=lambda item: item[0])
        if dist < min_reject:
            return None
        return goal_xy, dist

    def _build_candidate_for_robot_frontier(self, robot: RobotHandle, frontier: Frontier,
                                            reserved_points: List[Tuple[float, float]]) -> Optional[FrontierAssignmentCandidate]:
        if robot.last_pose_xy is None:
            return None
        if any(self._distance_xy(frontier.centroid_world, rp) < self.min_frontier_separation_m for rp in reserved_points):
            return None
        selected = self._choose_goal_point_for_frontier(robot, frontier)
        if selected is None:
            return None
        goal_xy, goal_distance = selected
        if any(self._distance_xy(goal_xy, rp) < self.min_frontier_separation_m for rp in reserved_points):
            return None

        cluster_distance = self._distance_xy(robot.last_pose_xy, frontier.centroid_world)
        size_bonus = self.score_size_weight * frontier.size
        home_bias_penalty = self._home_bias_penalty(robot, frontier)
        score = self.score_distance_weight * cluster_distance + home_bias_penalty - size_bonus
        return FrontierAssignmentCandidate(frontier, goal_xy, goal_distance, cluster_distance, score)

    def _home_bias_penalty(self, robot: RobotHandle, frontier: Frontier) -> float:
        if robot.home_pose_xy is None or self.score_home_bias_weight <= 0.0:
            return 0.0
        own_home_dist = self._distance_xy(robot.home_pose_xy, frontier.centroid_world)
        other_home_dists = [
            self._distance_xy(other.home_pose_xy, frontier.centroid_world)
            for other in self.robots.values()
            if other.namespace != robot.namespace and other.home_pose_xy is not None
        ]
        if not other_home_dists:
            return 0.0
        nearest_other = min(other_home_dists)
        return max(0.0, own_home_dist - nearest_other) * self.score_home_bias_weight

    def _choose_frontiers_globally(self, idle_robots: List[RobotHandle], frontiers: List[Frontier],
                                   reserved_points: List[Tuple[float, float]]) -> List[Tuple[RobotHandle, FrontierAssignmentCandidate]]:
        pair_candidates: List[Tuple[RobotHandle, FrontierAssignmentCandidate]] = []
        for robot in idle_robots:
            robot_candidates: List[FrontierAssignmentCandidate] = []
            for frontier in frontiers:
                candidate = self._build_candidate_for_robot_frontier(robot, frontier, reserved_points)
                if candidate is not None:
                    robot_candidates.append(candidate)
            if not robot_candidates:
                continue
            far_enough = [c for c in robot_candidates if c.goal_distance >= self.min_goal_distance_m]
            pair_candidates.extend((robot, c) for c in (far_enough if far_enough else robot_candidates))

        chosen: List[Tuple[RobotHandle, FrontierAssignmentCandidate]] = []
        used_robots: Set[str] = set()
        used_frontiers: Set[int] = set()
        dynamic_reserved = list(reserved_points)

        for robot, candidate in sorted(pair_candidates, key=lambda item: (item[1].score, item[1].cluster_distance, -item[1].frontier.size)):
            if robot.namespace in used_robots:
                continue
            if candidate.frontier.frontier_id in used_frontiers:
                continue
            if any(self._distance_xy(candidate.goal_xy, rp) < self.min_frontier_separation_m for rp in dynamic_reserved):
                continue
            if any(self._distance_xy(candidate.frontier.centroid_world, chosen_candidate.frontier.centroid_world) < self.min_frontier_separation_m
                   for _, chosen_candidate in chosen):
                continue
            chosen.append((robot, candidate))
            used_robots.add(robot.namespace)
            used_frontiers.add(candidate.frontier.frontier_id)
            dynamic_reserved.append(candidate.goal_xy)

        return chosen

    def _dispatch_goal(self, robot: RobotHandle, frontier: Frontier, goal_xy: Tuple[float, float]) -> None:
        self._dispatch_pose_goal(robot, goal_xy, frontier.frontier_id, 'frontier')

    def _dispatch_pose_goal(self, robot: RobotHandle, goal_xy: Tuple[float, float], goal_id: int, goal_kind: str) -> None:
        if robot.last_pose_xy is None:
            return
        goal_msg = NavigateToPose.Goal()
        pose = PoseStamped()
        pose.header.stamp = self.get_clock().now().to_msg()
        pose.header.frame_id = 'map'
        pose.pose.position.x = goal_xy[0]
        pose.pose.position.y = goal_xy[1]
        pose.pose.position.z = 0.0
        yaw = math.atan2(goal_xy[1] - robot.last_pose_xy[1], goal_xy[0] - robot.last_pose_xy[0])
        pose.pose.orientation = self._quaternion_from_yaw(yaw)
        goal_msg.pose = pose

        robot.assigned_goal = AssignedGoal(goal_id, goal_xy, self.get_clock().now(), False, goal_kind)
        if goal_kind == 'home':
            self.get_logger().info(f'Returning {robot.namespace} to home pose at ({goal_xy[0]:.2f}, {goal_xy[1]:.2f})')
        else:
            self.get_logger().info(f'Assigning frontier {goal_id} at ({goal_xy[0]:.2f}, {goal_xy[1]:.2f}) to {robot.namespace}')
        send_future = robot.action_client.send_goal_async(goal_msg)
        send_future.add_done_callback(lambda future, ns=robot.namespace: self._goal_response_cb(ns, future))

    def _send_idle_robots_home(self) -> None:
        for _, robot in sorted(self.robots.items()):
            if not robot.is_idle() or robot.last_pose_xy is None or robot.home_pose_xy is None:
                continue
            if self._distance_xy(robot.last_pose_xy, robot.home_pose_xy) <= self.goal_tolerance_m:
                continue
            self._dispatch_pose_goal(robot, robot.home_pose_xy, -1, 'home')

    def _goal_response_cb(self, namespace: str, future) -> None:
        robot = self.robots.get(namespace)
        if robot is None:
            return
        goal_handle = future.result()
        if goal_handle is None or not goal_handle.accepted:
            self.get_logger().warn(f'Goal rejected by {namespace}')
            self._blacklist_goal(namespace, robot.assigned_goal.goal_xy if robot.assigned_goal else None, reason='goal rejected')
            self._release_robot_assignment(namespace, reason='goal rejected')
            return
        robot.current_goal_handle = goal_handle
        if robot.assigned_goal is not None:
            robot.assigned_goal.accepted = True
        robot.result_future = goal_handle.get_result_async()
        robot.result_future.add_done_callback(lambda f, ns=namespace: self._goal_result_cb(ns, f))
        self.get_logger().info(f'Goal accepted by {namespace}')

    def _goal_result_cb(self, namespace: str, future) -> None:
        robot = self.robots.get(namespace)
        if robot is None:
            return
        try:
            result = future.result()
            status = result.status
        except Exception as exc:
            self.get_logger().error(f'Goal result callback failed for {namespace}: {exc}')
            status = None

        frontier_id = robot.assigned_goal.frontier_id if robot.assigned_goal else -1
        goal_kind = robot.assigned_goal.goal_kind if robot.assigned_goal else 'frontier'
        if status == 4:
            if goal_kind == 'home':
                self.get_logger().info(f'{namespace} reached home pose')
            else:
                self.get_logger().info(f'{namespace} reached frontier {frontier_id}')
                self._blacklist_goal(namespace, robot.assigned_goal.goal_xy if robot.assigned_goal else None,
                                     ttl_sec=self.goal_cooldown_sec, reason='goal reached cooldown')
        else:
            if goal_kind == 'home':
                self.get_logger().warn(f'{namespace} failed return-home goal with status={status}')
            else:
                self.get_logger().warn(f'{namespace} failed frontier {frontier_id} with status={status}')
                self._blacklist_goal(namespace, robot.assigned_goal.goal_xy if robot.assigned_goal else None,
                                     reason='goal failed')
        self._clear_robot_goal_state(robot)

    def _refresh_robot_goal_states(self) -> None:
        now = self.get_clock().now()
        for robot in self.robots.values():
            if robot.assigned_goal is None or robot.last_pose_xy is None:
                continue
            if robot.current_goal_handle is None:
                continue
            age = (now - robot.assigned_goal.assigned_at).nanoseconds / 1e9
            if age > self.assignment_timeout_sec and robot.current_goal_handle is not None:
                self.get_logger().warn(f'Cancelling slow goal for {robot.namespace}')
                try:
                    robot.current_goal_handle.cancel_goal_async()
                except Exception as exc:
                    self.get_logger().warn(f'Cancel request failed for {robot.namespace}: {exc}')
                self._release_robot_assignment(robot.namespace, reason='slow goal timeout')

    def _release_robot_assignment(self, namespace: str, reason: str = '') -> None:
        robot = self.robots.get(namespace)
        if robot is None:
            return
        if reason:
            self.get_logger().info(f'Releasing assignment for {namespace}: {reason}')
        if robot.current_goal_handle is not None:
            try:
                robot.current_goal_handle.cancel_goal_async()
            except Exception:
                pass
        self._clear_robot_goal_state(robot)

    def _clear_robot_goal_state(self, robot: RobotHandle) -> None:
        robot.assigned_goal = None
        robot.current_goal_handle = None
        robot.result_future = None

    def _cancel_all_goals(self, clear_assignments: bool) -> None:
        for robot in self.robots.values():
            if robot.current_goal_handle is not None:
                try:
                    robot.current_goal_handle.cancel_goal_async()
                except Exception:
                    pass
            if clear_assignments:
                self._clear_robot_goal_state(robot)

    def _blacklist_goal(self, namespace: Optional[str], goal_xy: Optional[Tuple[float, float]],
                        ttl_sec: Optional[float] = None, reason: str = '') -> None:
        if goal_xy is None:
            return
        ttl = self.blacklist_timeout_sec if ttl_sec is None else ttl_sec
        until = self.get_clock().now() + Duration(seconds=ttl)
        self.blacklist.append(BlacklistEntry(robot_ns=namespace, goal_xy=goal_xy, until=until, reason=reason))

    def _prune_blacklist(self) -> None:
        now = self.get_clock().now()
        self.blacklist = [entry for entry in self.blacklist if entry.until > now]

    @staticmethod
    def _distance_xy(a: Tuple[float, float], b: Tuple[float, float]) -> float:
        return math.hypot(a[0] - b[0], a[1] - b[1])

    @staticmethod
    def _quaternion_from_yaw(yaw: float) -> Quaternion:
        q = Quaternion()
        q.z = math.sin(yaw / 2.0)
        q.w = math.cos(yaw / 2.0)
        return q


def main(args=None) -> None:
    rclpy.init(args=args)
    node = MultiRobotExplorer()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
