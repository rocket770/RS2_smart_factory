import json
import math

import matplotlib.pyplot as plt
import rclpy
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, HistoryPolicy, QoSProfile, ReliabilityPolicy
from std_msgs.msg import String

# this util was written by chatgpt to help me debug and visualize mtsp - nick
class MtspLiveVisualizer(Node):
    def __init__(self):
        super().__init__("mtsp_live_visualizer")
        progress_qos = QoSProfile(
            reliability=ReliabilityPolicy.BEST_EFFORT,
            durability=DurabilityPolicy.VOLATILE,
            history=HistoryPolicy.KEEP_LAST,
            depth=10,
        )
        self.subscription = self.create_subscription(
            String,
            "mtsp_best_solution",
            self.on_message,
            progress_qos,
        )

        self.latest = None

        self.robot_colors = [
            "tab:orange",
            "tab:green",
            "tab:red",
            "tab:purple",
            "tab:brown",
            "tab:blue",
            "tab:pink",
            "tab:gray",
            "tab:olive",
            "tab:cyan",
        ]

        plt.ion()
        self.fig, self.ax = plt.subplots(figsize=(10, 8))
        self.fig.canvas.manager.set_window_title("MTSP Live Visualizer")

    def on_message(self, msg: String):
        try:
            self.latest = json.loads(msg.data)
            self.redraw()
        except Exception as e:
            self.get_logger().error(f"Failed to parse message: {e}")

    def redraw(self):
        if self.latest is None:
            return

        data = self.latest
        robot_starts = data["robot_starts"]
        goals = data["goals"]
        routes = data["routes"]
        generation = data["generation"]
        cost = data["cost"]

        self.ax.clear()

        goal_x = [g[0] for g in goals]
        goal_y = [g[1] for g in goals]
        self.ax.scatter(goal_x, goal_y, marker="x", s=100, color="tab:blue", label="Goals")

        for i, (x, y) in enumerate(goals):
            self.ax.text(x + 0.08, y + 0.08, f"G{i}", fontsize=9)

        for robot_idx, start in enumerate(robot_starts):
            color = self.robot_colors[robot_idx % len(self.robot_colors)]
            sx, sy = start

            self.ax.scatter([sx], [sy], marker="o", s=150, color=color, label=f"Robot {robot_idx} start")
            self.ax.text(sx + 0.08, sy + 0.08, f"R{robot_idx}", fontsize=10)

            route = routes[robot_idx]
            if not route:
                continue

            path_x = [sx]
            path_y = [sy]

            for order_idx, goal_idx in enumerate(route):
                gx, gy = goals[goal_idx]
                path_x.append(gx)
                path_y.append(gy)
                self.ax.text(gx + 0.08, gy - 0.16, f"{order_idx}", fontsize=8)

            self.ax.plot(path_x, path_y, linewidth=2, color=color, label=f"Robot {robot_idx} path")

        self.ax.set_title(f"Generation {generation} | Best Cost = {cost:.3f}")
        self.ax.set_xlabel("X")
        self.ax.set_ylabel("Y")
        self.ax.axis("equal")
        self.ax.grid(True)

        handles, labels = self.ax.get_legend_handles_labels()
        unique = {}
        for h, l in zip(handles, labels):
            if l not in unique:
                unique[l] = h
        self.ax.legend(unique.values(), unique.keys(), loc="best")

        self.fig.canvas.draw()
        self.fig.canvas.flush_events()


def main():
    rclpy.init()
    node = MtspLiveVisualizer()

    try:
        while rclpy.ok():
            rclpy.spin_once(node, timeout_sec=0.1)
            plt.pause(0.001)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
