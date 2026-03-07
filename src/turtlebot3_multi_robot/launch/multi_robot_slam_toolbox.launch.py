#!/usr/bin/env python3

import os
import yaml

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    ld = LaunchDescription()

    package_dir = get_package_share_directory('turtlebot3_multi_robot')
    use_sim_time = LaunchConfiguration('use_sim_time')

    ld.add_action(DeclareLaunchArgument(
        'use_sim_time',
        default_value='true',
        description='Use simulator time'
    ))

    settings_file = os.path.join(package_dir, 'params', 'general_settings.yaml')
    with open(settings_file, 'r') as f:
        settings = yaml.safe_load(f)

    robots = settings['robots']

    # You can also use your own slam toolbox yaml here if you have one.
    slam_params_file = os.path.join(
        get_package_share_directory('slam_toolbox'),
        'config',
        'mapper_params_online_async.yaml'
    )

    for robot in robots:
        namespace = robot['name']

        ld.add_action(Node(
            package='slam_toolbox',
            executable='async_slam_toolbox_node',
            name='slam_toolbox',
            namespace=namespace,
            output='screen',
            parameters=[
                slam_params_file,
                {
                    'use_sim_time': use_sim_time,
                    'odom_frame': 'odom',
                    'base_frame': 'base_footprint',
                    'map_frame': 'map',
                    'scan_topic': 'scan',
                }
            ],
            remappings=[
                ('/scan', 'scan'),
                ('/map', 'map'),
                ('/map_metadata', 'map_metadata'),
                ('/tf', 'tf'),
                ('/tf_static', 'tf_static'),
            ]
        ))

    return ld