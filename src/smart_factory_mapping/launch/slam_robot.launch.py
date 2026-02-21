from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, GroupAction
from launch.substitutions import LaunchConfiguration, TextSubstitution
from launch_ros.actions import Node, PushRosNamespace
from launch_ros.substitutions import FindPackageShare
from launch.substitutions import PathJoinSubstitution
import os

def generate_launch_description():
    ns = LaunchConfiguration("ns")
    use_sim_time = LaunchConfiguration("use_sim_time")

    params = PathJoinSubstitution([FindPackageShare("smart_factory_mapping"), "config", "slam_tb3.yaml"])

    # Fully-qualified TF frame ids inside TF tree:
    odom_frame = [ns, TextSubstitution(text="/odom")]
    base_frame = [ns, TextSubstitution(text="/base_footprint")]

    return LaunchDescription([
        DeclareLaunchArgument("ns", default_value="TB3_1"),
        DeclareLaunchArgument("use_sim_time", default_value="true"),

        GroupAction([
            PushRosNamespace(ns),
            Node(
                package="slam_toolbox",
                executable="async_slam_toolbox_node",
                name="slam_toolbox",
                output="screen",
                parameters=[
                    params,
                    {
                        "use_sim_time": use_sim_time,
                        "map_frame": "map",
                        "odom_frame": odom_frame,
                        "base_frame": base_frame,
                        "scan_topic": "scan",
                        "publish_tf": True,
                        "mode": "mapping",
                    }
                ],
            ),
        ])
    ])