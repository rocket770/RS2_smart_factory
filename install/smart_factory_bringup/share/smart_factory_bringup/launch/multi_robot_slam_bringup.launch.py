#!/usr/bin/env python3

import os
import yaml

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from nav2_common.launch import ReplaceString, RewrittenYaml

def generate_launch_description():
    ld = LaunchDescription()

    package_dir = get_package_share_directory('smart_factory_bringup')

    use_sim_time = LaunchConfiguration('use_sim_time')
    enable_rviz = LaunchConfiguration('enable_rviz')
    rviz_config_file = LaunchConfiguration('rviz_config_file')
    params_file = LaunchConfiguration('nav_params_file')
    map_file = LaunchConfiguration('map')

    ld.add_action(DeclareLaunchArgument(
        'use_sim_time',
        default_value='true',
        description='Use simulator time'
    ))

    ld.add_action(DeclareLaunchArgument(
        'enable_rviz',
        default_value='true',
        description='Enable rviz launch'
    ))

    ld.add_action(DeclareLaunchArgument(
        'rviz_config_file',
        default_value=os.path.join(
            package_dir, 'rviz', 'multi_nav2_default_view.rviz'
        ),
        description='Full path to the RVIZ config file to use'
    ))

    ld.add_action(DeclareLaunchArgument(
        'nav_params_file',
        default_value=os.path.join(
            package_dir, 'params', 'nav2_slam_params.yaml'
        ),
        description='Full path to the ROS2 parameters file to use'
    ))

    # Required by bringup_launch.py even in slam mode.
    ld.add_action(DeclareLaunchArgument(
        'map',
        default_value=os.path.join(
            package_dir, 'map', 'map.yaml'
        ),
        description='Dummy map yaml path required by bringup_launch.py'
    ))

    setting_path = os.path.join(package_dir, 'params', 'general_settings.yaml')
    with open(setting_path, 'r') as file:
        settings = yaml.safe_load(file)

    robots = settings["robots"]
    nav_launch_dir = os.path.join(package_dir, 'launch', 'nav2_bringup')

    for robot in robots:
        namespace = robot['name']

        ld.add_action(IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                os.path.join(nav_launch_dir, 'bringup_launch.py')
            ),
            launch_arguments={
                'slam': 'False',
                'namespace': namespace,
                'use_namespace': 'True',
                'map_server': 'False',
                'map': map_file,
                'params_file': params_file,
                'autostart': 'true',
                'use_sim_time': use_sim_time,
                'log_level': 'warn',
            }.items()
        ))

        # ld.add_action(Node(
        #     package='turtlebot3_multi_robot',
        #     executable='odom_to_tf.py',
        #     namespace=namespace,
        #     name='odom_to_tf',
        #     output='screen',
        #     parameters=[{
        #         'odom_topic': f'/{namespace}/odom',
        #         'tf_frame_id': 'odom',
        #         'tf_child_frame_id': 'base_footprint',
        #         'use_sim_time': use_sim_time,
        #     }],
        #     remappings=[
        #         ('/tf', 'tf'),
        #         ('/tf_static', 'tf_static'),
        #     ],
        # ))

        ld.add_action(IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                os.path.join(nav_launch_dir, 'rviz_launch.py')
            ),
            launch_arguments={
                'use_sim_time': use_sim_time,
                'namespace': namespace,
                'use_namespace': 'True',
                'rviz_config': rviz_config_file,
                'log_level': 'warn',
            }.items(),
            condition=IfCondition(enable_rviz)
        ))

        

    return ld