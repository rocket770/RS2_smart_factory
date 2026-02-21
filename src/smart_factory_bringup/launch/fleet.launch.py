from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    # Example zones: one narrow aisle region you want to mutex
    zones = [
        {"name": "aisle1", "xmin": 0.5, "ymin": -0.4, "xmax": 2.0, "ymax": 0.4},
    ]

    task_server = Node(
        package="smart_factory_task_server",
        executable="task_server",
        name="fleet_task_server",
        output="screen",
        parameters=[{
            "robots": ["TB3_1", "TB3_2"],
            "pose_topic": "amcl_pose",
            "map_frame": "map",
            "zones": zones
        }]
    )

    rviz = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        output="screen",
        # optional: -d path_to_config
        arguments=[]
    )

    return LaunchDescription([task_server, rviz])