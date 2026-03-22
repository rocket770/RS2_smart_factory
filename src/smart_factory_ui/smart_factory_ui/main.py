import sys
import rclpy
from PyQt5.QtWidgets import QApplication
from PyQt5.QtCore import QTimer

from .ros_interface import FleetGuiNode
from .dashboard import Dashboard


def main():
    rclpy.init()
    ros_node = FleetGuiNode()

    app = QApplication(sys.argv)
    window = Dashboard(ros_node)
    window.show()

    timer = QTimer()

    def update_ros():
        rclpy.spin_once(ros_node, timeout_sec=0.01)
        window.process_futures()

    timer.timeout.connect(update_ros)
    timer.start(50)

    exit_code = app.exec_()

    ros_node.destroy_node()
    rclpy.shutdown()
    sys.exit(exit_code)


if __name__ == "__main__":
    main()