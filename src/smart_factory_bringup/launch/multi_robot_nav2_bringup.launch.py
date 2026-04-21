#!/usr/bin/env python3

import os
import yaml

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.conditions import IfCondition, UnlessCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    ld = LaunchDescription()

    package_dir = get_package_share_directory('smart_factory_bringup')

    use_sim_time = LaunchConfiguration('use_sim_time')
    enable_rviz = LaunchConfiguration('enable_rviz')
    rviz_config_file = LaunchConfiguration('rviz_config_file')
    params_file = LaunchConfiguration('nav_params_file')
    map_file = LaunchConfiguration('map')
    use_slam = LaunchConfiguration('use_slam')

    ld.add_action(DeclareLaunchArgument(
        'use_sim_time',
        default_value='false',
        description='Use simulator time'
    ))

    ld.add_action(DeclareLaunchArgument(
        'enable_rviz',
        default_value='true',
        description='Enable rviz launch'
    ))

    ld.add_action(DeclareLaunchArgument(
        'use_slam',
        default_value='false',
        description='If true, launch multi_robot_slam_toolbox.launch.py. If false, run Nav2 with AMCL.'
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

    ld.add_action(DeclareLaunchArgument(
        'map',
        default_value=os.path.join(
            package_dir, 'map', 'factory_merged.yaml'
        ),
        description='Map yaml path for localization mode; can be overridden at launch'
    ))

    setting_path = os.path.join(package_dir, 'params', 'general_settings.yaml')
    with open(setting_path, 'r') as file:
        settings = yaml.safe_load(file)

    robots = settings["robots"]
    nav_launch_dir = os.path.join(package_dir, 'launch', 'nav2_bringup')

    ld.add_action(IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(package_dir, 'launch', 'multi_robot_slam_toolbox.launch.py')
        ),
        launch_arguments={
            'use_sim_time': use_sim_time,
        }.items(),
        condition=IfCondition(use_slam)
    ))

    ld.add_action(IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory('merge_map'),
                'launch',
                'merge_map_launch.py'
            )
        ),
        launch_arguments={
            'use_sim_time': use_sim_time,
        }.items(),
        condition=IfCondition(use_slam)
    ))

    ld.add_action(IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory('smart_factory_navigation'),
                'launch',
                'multi_robot_explorer.launch.py'
            )
        ),
        launch_arguments={
            'map_topic': '/map',
        }.items(),
        condition=IfCondition(use_slam)
    ))

    ld.add_action(Node(
        package='nav2_map_server',
        executable='map_server',
        name='map_server',
        output='screen',
        parameters=[{
            'use_sim_time': use_sim_time,
            'yaml_filename': map_file,
            'frame_id': 'map',
        }],
        condition=UnlessCondition(use_slam)
    ))

    ld.add_action(Node(
        package='nav2_lifecycle_manager',
        executable='lifecycle_manager',
        name='lifecycle_manager_map_server',
        output='screen',
        parameters=[{
            'use_sim_time': use_sim_time,
            'autostart': True,
            'node_names': ['map_server'],
        }],
        condition=UnlessCondition(use_slam)
    ))

    for robot in robots:
        namespace = robot['name']
        initial_pose_x = str(robot.get('initial_pose_x', robot['x_pose']))
        initial_pose_y = str(robot.get('initial_pose_y', robot['y_pose']))
        initial_pose_yaw = str(robot.get('initial_pose_yaw', robot.get('yaw', 0.0)))

        ld.add_action(IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                os.path.join(nav_launch_dir, 'bringup_launch.py')
            ),
            launch_arguments={
                'use_slam': use_slam,
                'namespace': namespace,
                'use_namespace': 'True',
                'map_server': 'False',
                'map': map_file,
                'params_file': params_file,
                'autostart': 'True',
                'use_sim_time': use_sim_time,
                'log_level': 'warn',
                'initial_pose_x': initial_pose_x,
                'initial_pose_y': initial_pose_y,
                'initial_pose_yaw': initial_pose_yaw,
            }.items(),
        ))

        # ld.add_action(Node(
        #     package='tf2_ros',
        #     executable='static_transform_publisher',
        #     name=f'{namespace}_map_to_local_map',
        #     arguments=[
        #         '0', '0', '0',
        #         '0', '0', '0',
        #         'map',
        #         f'{namespace}/map',
        #     ],
        #     output='screen',
        #     condition=IfCondition(use_slam)
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
