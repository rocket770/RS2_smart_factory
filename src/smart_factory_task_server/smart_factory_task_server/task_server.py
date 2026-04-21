import rclpy
from rclpy.node import Node
from rclpy.action import ActionClient
from nav2_msgs.action import NavigateToPose

from smart_factory_fleet_msgs.srv import AddTask
from smart_factory_fleet_msgs.msg import Task, RobotState

import uuid
from datetime import datetime


ROBOTS = ["tb1", "tb2"]


class TaskServerNode(Node):
    def __init__(self):
        super().__init__("smart_factory_task_server")

        # Service server — this is what the GUI calls
        self.srv = self.create_service(AddTask, "/add_task", self.handle_add_task)

        # One Nav2 action client per robot
        self.nav_clients = {}
        for robot in ROBOTS:
            self.nav_clients[robot] = ActionClient(
                self, NavigateToPose, f"/{robot}/navigate_to_pose"
            )

        # Track robot status: "free" or "busy"
        self.robot_status = {robot: "free" for robot in ROBOTS}

        # Task queue for when both robots are busy
        self.task_queue = []

        self.get_logger().info("Task server ready, waiting for tasks...")

    # ------------------------------------------------------------------
    # Service callback — called when GUI sends a task
    # ------------------------------------------------------------------
    def handle_add_task(self, request, response):
        task_id = str(uuid.uuid4())[:8]

        self.get_logger().info(
            f"Received task {task_id}: type={request.type}, priority={request.priority}"
        )

        task = Task()
        task.task_id = task_id
        task.type = request.type
        task.priority = request.priority
        task.pickup_pose = request.pickup_pose
        task.dropoff_pose = request.dropoff_pose
        task.status = "pending"

        # Try to assign immediately, otherwise queue
        robot = self._get_free_robot()
        if robot:
            task.assigned_robot = robot
            task.status = "assigned"
            self.robot_status[robot] = "busy"
            self.get_logger().info(f"Assigning task {task_id} to {robot}")
            self._send_pickup(robot, task)
        else:
            self.get_logger().info(f"All robots busy, queuing task {task_id}")
            self.task_queue.append(task)

        response.accepted = True
        response.task_id = task_id
        response.message = f"Task {task_id} accepted"
        return response

    # ------------------------------------------------------------------
    # Navigation helpers
    # ------------------------------------------------------------------
    def _get_free_robot(self):
        for robot in ROBOTS:
            if self.robot_status[robot] == "free":
                return robot
        return None

    def _send_pickup(self, robot, task):
        self.get_logger().info(f"{robot} navigating to PICKUP for task {task.task_id}")
        goal = NavigateToPose.Goal()
        goal.pose = task.pickup_pose

        client = self.nav_clients[robot]
        client.wait_for_server(timeout_sec=5.0)

        send_goal_future = client.send_goal_async(goal)
        send_goal_future.add_done_callback(
            lambda fut: self._on_pickup_accepted(fut, robot, task)
        )

    def _on_pickup_accepted(self, future, robot, task):
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().warn(f"{robot} rejected pickup goal for task {task.task_id}")
            self._mark_robot_free(robot)
            return

        result_future = goal_handle.get_result_async()
        result_future.add_done_callback(
            lambda fut: self._on_pickup_done(fut, robot, task)
        )

    def _on_pickup_done(self, future, robot, task):
        self.get_logger().info(f"{robot} reached PICKUP, now going to DROPOFF for task {task.task_id}")
        goal = NavigateToPose.Goal()
        goal.pose = task.dropoff_pose

        client = self.nav_clients[robot]
        send_goal_future = client.send_goal_async(goal)
        send_goal_future.add_done_callback(
            lambda fut: self._on_dropoff_accepted(fut, robot, task)
        )

    def _on_dropoff_accepted(self, future, robot, task):
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().warn(f"{robot} rejected dropoff goal for task {task.task_id}")
            self._mark_robot_free(robot)
            return

        result_future = goal_handle.get_result_async()
        result_future.add_done_callback(
            lambda fut: self._on_dropoff_done(fut, robot, task)
        )

    def _on_dropoff_done(self, future, robot, task):
        self.get_logger().info(f"{robot} completed task {task.task_id} ✓")
        self._mark_robot_free(robot)

    def _mark_robot_free(self, robot):
        self.robot_status[robot] = "free"
        self.get_logger().info(f"{robot} is now free")

        # Check if anything is queued
        if self.task_queue:
            next_task = self.task_queue.pop(0)
            next_task.assigned_robot = robot
            next_task.status = "assigned"
            self.robot_status[robot] = "busy"
            self.get_logger().info(f"Assigning queued task {next_task.task_id} to {robot}")
            self._send_pickup(robot, next_task)


def main(args=None):
    rclpy.init(args=args)
    node = TaskServerNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()