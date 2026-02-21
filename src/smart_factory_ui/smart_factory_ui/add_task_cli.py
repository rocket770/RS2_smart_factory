#!/usr/bin/env python3
import sys
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PoseStamped
from smart_factory_fleet_msgs.srv import AddTask


class AddTaskCli(Node):
    def __init__(self):
        super().__init__("add_task_cli")
        self.cli = self.create_client(AddTask, "/add_task")
        while not self.cli.wait_for_service(timeout_sec=1.0):
            self.get_logger().info("Waiting for /add_task...")

    def send(self, pickup_xy, dropoff_xy, frame="map", priority=0):
        req = AddTask.Request()
        req.type = "pickup_dropoff"
        req.priority = int(priority)

        req.pickup_pose = PoseStamped()
        req.pickup_pose.header.frame_id = frame
        req.pickup_pose.pose.position.x = float(pickup_xy[0])
        req.pickup_pose.pose.position.y = float(pickup_xy[1])
        req.pickup_pose.pose.orientation.w = 1.0

        req.dropoff_pose = PoseStamped()
        req.dropoff_pose.header.frame_id = frame
        req.dropoff_pose.pose.position.x = float(dropoff_xy[0])
        req.dropoff_pose.pose.position.y = float(dropoff_xy[1])
        req.dropoff_pose.pose.orientation.w = 1.0

        fut = self.cli.call_async(req)
        rclpy.spin_until_future_complete(self, fut)
        resp = fut.result()
        if resp:
            print(f"accepted={resp.accepted} task_id={resp.task_id} msg={resp.message}")
        else:
            print("No response")


def main():
    if len(sys.argv) < 5:
        print("usage: add_task_cli pickup_x pickup_y dropoff_x dropoff_y [priority]")
        return

    px, py, dx, dy = sys.argv[1:5]
    pr = int(sys.argv[5]) if len(sys.argv) > 5 else 0

    rclpy.init()
    node = AddTaskCli()
    node.send((px, py), (dx, dy), priority=pr)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()