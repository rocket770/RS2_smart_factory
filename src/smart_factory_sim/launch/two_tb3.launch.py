from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
import os

def generate_launch_description():
    # Path to your vendored turtlebot3_gazebo
    vendor_root = os.path.expanduser("~/smart_factory_ws/vendor_src/turtlebot3_gazebo")
    vendor_launch = os.path.join(vendor_root, "launch", "multi_robot.launch.py")

    use_sim_time = LaunchConfiguration("use_sim_time")

    return LaunchDescription([
        DeclareLaunchArgument("use_sim_time", default_value="true"),

        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(vendor_launch),
            launch_arguments={
                # These args are whatever your multi_robot.launch.py declares.
                # If it doesn't declare use_sim_time, remove it.
                "use_sim_time": use_sim_time,
            }.items()
        ),
    ])