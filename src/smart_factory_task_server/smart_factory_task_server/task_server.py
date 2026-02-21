#!/usr/bin/env python3
import math
import uuid
from dataclasses import dataclass
from typing import Dict, List, Optional, Tuple

import rclpy
from rclpy.node import Node
from rclpy.action import ActionClient

from geometry_msgs.msg import PoseStamped, PoseWithCovarianceStamped
from nav2_msgs.action import NavigateToPose

from smart_factory_fleet_msgs.msg import Task, RobotState
from smart_factory_fleet_msgs.srv import AddTask, GetFleetState


def dist2(a: PoseStamped, b: PoseStamped) -> float:
    dx = a.pose.position.x - b.pose.position.x
    dy = a.pose.position.y - b.pose.position.y
    return dx*dx + dy*dy


def pose_xy(p: PoseStamped) -> Tuple[float, float]:
    return (p.pose.position.x, p.pose.position.y)


@dataclass
class Zone:
    name: str
    xmin: float
    ymin: float
    xmax: float
    ymax: float


def in_zone(x: float, y: float, z: Zone) -> bool:
    return (z.xmin <= x <= z.xmax) and (z.ymin <= y <= z.ymax)


class FleetTaskServer(Node):
    """
    Minimal fleet server:
    - Keeps robot states (pose + status) from /robotX/amcl_pose
    - Accepts AddTask service
    - Assigns queued tasks to nearest idle robot
    - Sends Nav2 NavigateToPose goals: pickup then dropoff
    - Optional traffic zones (mutex): if target is in a locked zone, robot waits
    """

    def __init__(self):
        super().__init__("fleet_task_server")

        # --- Params ---
        self.declare_parameter("robots", ["robot1", "robot2"])
        self.declare_parameter("pose_topic", "amcl_pose")  # under namespace
        self.declare_parameter("map_frame", "map")

        # zones as list of dicts: [{name,xmin,ymin,xmax,ymax}, ...]
        self.declare_parameter("zones", [])
        self.declare_parameter("zone_wait_offset", 0.5)  # meters to back off if blocked

        self.robot_names: List[str] = list(self.get_parameter("robots").value)
        self.pose_topic: str = self.get_parameter("pose_topic").value
        self.map_frame: str = self.get_parameter("map_frame").value

        self.zones: List[Zone] = []
        for z in self.get_parameter("zones").value:
            try:
                self.zones.append(Zone(
                    name=str(z["name"]),
                    xmin=float(z["xmin"]), ymin=float(z["ymin"]),
                    xmax=float(z["xmax"]), ymax=float(z["ymax"])
                ))
            except Exception as e:
                self.get_logger().warn(f"Bad zone param entry {z}: {e}")

        self.zone_owner: Dict[str, str] = {}  # zone_name -> robot_name

        # --- State ---
        self.robots: Dict[str, RobotState] = {}
        self.tasks: Dict[str, Task] = {}
        self.robot_nav_clients: Dict[str, ActionClient] = {}

        # Per-robot "execution state"
        self.robot_exec: Dict[str, Dict] = {rn: {"active_task": None, "stage": None, "goal_handle": None} for rn in self.robot_names}

        # --- ROS interfaces ---
        self.add_task_srv = self.create_service(AddTask, "add_task", self.on_add_task)
        self.get_state_srv = self.create_service(GetFleetState, "get_fleet_state", self.on_get_fleet_state)

        # Optional: publish robot states for UI/RViz debugging
        self.robot_state_pub = self.create_publisher(RobotState, "fleet/robot_state", 10)
        self.task_pub = self.create_publisher(Task, "fleet/task", 10)

        # Subscribe to each robot's pose
        for rn in self.robot_names:
            topic = f"/{rn}/{self.pose_topic}"
            self.create_subscription(PoseWithCovarianceStamped, topic, lambda msg, r=rn: self.on_pose(r, msg), 10)

            # Nav2 action client under namespace
            act = f"/{rn}/navigate_to_pose"
            self.robot_nav_clients[rn] = ActionClient(self, NavigateToPose, act)

            # Init robot state
            rs = RobotState()
            rs.robot_name = rn
            rs.status = "idle"
            rs.pose = PoseStamped()
            rs.pose.header.frame_id = self.map_frame
            rs.battery = 1.0
            rs.current_task_id = ""
            self.robots[rn] = rs

        # Main loop timer
        self.timer = self.create_timer(0.2, self.spin_once)

        self.get_logger().info(f"FleetTaskServer up with robots={self.robot_names}, zones={[z.name for z in self.zones]}")

    # ---------------- ROS callbacks ----------------

    def on_pose(self, robot_name: str, msg: PoseWithCovarianceStamped):
        rs = self.robots[robot_name]
        rs.stamp = self.get_clock().now().to_msg()
        rs.pose.header = msg.header
        rs.pose.pose = msg.pose.pose
        self.robots[robot_name] = rs

        # publish for debugging
        self.robot_state_pub.publish(rs)

    def on_add_task(self, req: AddTask.Request, resp: AddTask.Response):
        task_id = uuid.uuid4().hex[:8]
        t = Task()
        t.stamp = self.get_clock().now().to_msg()
        t.task_id = task_id
        t.type = req.type if req.type else "pickup_dropoff"
        t.priority = int(req.priority)
        t.pickup_pose = req.pickup_pose
        t.dropoff_pose = req.dropoff_pose
        t.assigned_robot = ""
        t.status = "queued"

        # enforce frame id
        if not t.pickup_pose.header.frame_id:
            t.pickup_pose.header.frame_id = self.map_frame
        if not t.dropoff_pose.header.frame_id:
            t.dropoff_pose.header.frame_id = self.map_frame

        self.tasks[task_id] = t
        self.task_pub.publish(t)

        resp.accepted = True
        resp.task_id = task_id
        resp.message = "queued"
        self.get_logger().info(f"Added task {task_id}")
        return resp

    def on_get_fleet_state(self, req: GetFleetState.Request, resp: GetFleetState.Response):
        resp.robots = list(self.robots.values())
        resp.tasks = list(self.tasks.values())
        return resp

    # ---------------- Core logic ----------------

    def spin_once(self):
        # 1) assign tasks to idle robots
        self.assign_tasks()

        # 2) progress any active robot executions
        for rn in self.robot_names:
            self.progress_robot(rn)

        # 3) publish tasks periodically (debug)
        for t in self.tasks.values():
            self.task_pub.publish(t)

    def assign_tasks(self):
        queued = [t for t in self.tasks.values() if t.status == "queued"]
        if not queued:
            return

        # prioritize higher priority, then older (stamp)
        queued.sort(key=lambda x: (-x.priority, x.stamp.sec, x.stamp.nanosec))

        for task in queued:
            idle_robots = [r for r in self.robot_names if self.robots[r].status == "idle" and self.robot_exec[r]["active_task"] is None]
            if not idle_robots:
                return

            # nearest robot to pickup
            best_r = None
            best_cost = float("inf")
            for rn in idle_robots:
                rs = self.robots[rn]
                if rs.pose.header.frame_id != task.pickup_pose.header.frame_id:
                    continue
                cost = dist2(rs.pose, task.pickup_pose)
                if cost < best_cost:
                    best_cost = cost
                    best_r = rn

            if best_r is None:
                continue

            # assign
            task.assigned_robot = best_r
            task.status = "assigned"
            self.tasks[task.task_id] = task

            self.robot_exec[best_r]["active_task"] = task.task_id
            self.robot_exec[best_r]["stage"] = "to_pickup"
            self.robots[best_r].status = "busy"
            self.robots[best_r].current_task_id = task.task_id

            self.get_logger().info(f"Assigned task {task.task_id} -> {best_r}")

    def progress_robot(self, robot_name: str):
        exec_state = self.robot_exec[robot_name]
        task_id = exec_state["active_task"]
        if task_id is None:
            return

        task = self.tasks.get(task_id)
        if task is None:
            self.reset_robot(robot_name)
            return

        stage = exec_state["stage"]
        if stage == "to_pickup":
            target = task.pickup_pose
            if self.ensure_zone_access(robot_name, target):
                self.send_or_monitor_nav(robot_name, task, target, next_stage="to_dropoff", inprogress_status="enroute_pickup")
            else:
                self.robots[robot_name].status = "waiting"
                task.status = "assigned"

        elif stage == "to_dropoff":
            target = task.dropoff_pose
            if self.ensure_zone_access(robot_name, target):
                self.send_or_monitor_nav(robot_name, task, target, next_stage="done", inprogress_status="enroute_dropoff")
            else:
                self.robots[robot_name].status = "waiting"
                task.status = "enroute_pickup"  # still after pickup stage; keep simple

        elif stage == "done":
            task.status = "done"
            self.tasks[task_id] = task
            self.release_all_zones(robot_name)
            self.reset_robot(robot_name)

        elif stage == "failed":
            task.status = "failed"
            self.tasks[task_id] = task
            self.release_all_zones(robot_name)
            self.reset_robot(robot_name)

    def reset_robot(self, robot_name: str):
        self.robot_exec[robot_name] = {"active_task": None, "stage": None, "goal_handle": None}
        self.robots[robot_name].status = "idle"
        self.robots[robot_name].current_task_id = ""

    # ---------------- Nav2 action handling ----------------

    def send_or_monitor_nav(self, robot_name: str, task: Task, target: PoseStamped, next_stage: str, inprogress_status: str):
        exec_state = self.robot_exec[robot_name]
        client = self.robot_nav_clients[robot_name]

        # wait for server once (non-blocking-ish by short timeout)
        if not client.wait_for_server(timeout_sec=0.1):
            self.get_logger().warn(f"[{robot_name}] Nav2 action server not ready")
            return

        if exec_state["goal_handle"] is None:
            # send new goal
            goal = NavigateToPose.Goal()
            goal.pose = target

            task.status = inprogress_status
            self.tasks[task.task_id] = task

            send_future = client.send_goal_async(goal)
            send_future.add_done_callback(lambda fut, r=robot_name: self._on_goal_response(r, fut))
            exec_state["goal_handle"] = "PENDING"
            self.robot_exec[robot_name] = exec_state
            return

        # if pending, wait for callback to set real handle
        if exec_state["goal_handle"] == "PENDING":
            return

        gh = exec_state["goal_handle"]
        if gh is None:
            return

        # monitor result
        if not hasattr(exec_state, "result_future") and exec_state.get("result_future") is None:
            # start result future once
            exec_state["result_future"] = gh.get_result_async()
            exec_state["result_future"].add_done_callback(lambda fut, r=robot_name: self._on_goal_result(r, fut))
            self.robot_exec[robot_name] = exec_state

        # stage transitions happen in result callback

    def _on_goal_response(self, robot_name: str, future):
        exec_state = self.robot_exec[robot_name]
        try:
            goal_handle = future.result()
        except Exception as e:
            self.get_logger().error(f"[{robot_name}] Goal response error: {e}")
            exec_state["stage"] = "failed"
            exec_state["goal_handle"] = None
            self.robot_exec[robot_name] = exec_state
            return

        if not goal_handle.accepted:
            self.get_logger().warn(f"[{robot_name}] Goal rejected")
            exec_state["stage"] = "failed"
            exec_state["goal_handle"] = None
            self.robot_exec[robot_name] = exec_state
            return

        exec_state["goal_handle"] = goal_handle
        exec_state["result_future"] = None
        self.robot_exec[robot_name] = exec_state
        self.get_logger().info(f"[{robot_name}] Goal accepted")

    def _on_goal_result(self, robot_name: str, future):
        exec_state = self.robot_exec[robot_name]
        task_id = exec_state["active_task"]
        task = self.tasks.get(task_id)

        try:
            result = future.result().result
            status = future.result().status
        except Exception as e:
            self.get_logger().error(f"[{robot_name}] Result error: {e}")
            exec_state["stage"] = "failed"
            exec_state["goal_handle"] = None
            exec_state["result_future"] = None
            self.robot_exec[robot_name] = exec_state
            return

        # Nav2 status codes: 4 == SUCCEEDED
        if status == 4:
            # advance stage
            if exec_state["stage"] == "to_pickup":
                exec_state["stage"] = "to_dropoff"
            elif exec_state["stage"] == "to_dropoff":
                exec_state["stage"] = "done"
            exec_state["goal_handle"] = None
            exec_state["result_future"] = None
            self.robot_exec[robot_name] = exec_state
            self.get_logger().info(f"[{robot_name}] Goal succeeded -> stage={exec_state['stage']}")
        else:
            self.get_logger().warn(f"[{robot_name}] Goal failed status={status}")
            if task:
                task.status = "failed"
                self.tasks[task.task_id] = task
            exec_state["stage"] = "failed"
            exec_state["goal_handle"] = None
            exec_state["result_future"] = None
            self.robot_exec[robot_name] = exec_state

    # ---------------- Traffic zones (mutex) ----------------

    def ensure_zone_access(self, robot_name: str, target: PoseStamped) -> bool:
        if not self.zones:
            return True

        x, y = pose_xy(target)
        for z in self.zones:
            if in_zone(x, y, z):
                owner = self.zone_owner.get(z.name)
                if owner is None or owner == robot_name:
                    self.zone_owner[z.name] = robot_name
                    return True
                return False
        return True

    def release_all_zones(self, robot_name: str):
        to_release = [zn for zn, owner in self.zone_owner.items() if owner == robot_name]
        for zn in to_release:
            del self.zone_owner[zn]


def main():
    rclpy.init()
    node = FleetTaskServer()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()