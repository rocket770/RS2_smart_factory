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

    package_dir = get_package_share_directory('smart_factory_bringup')
    use_sim_time = LaunchConfiguration('use_sim_time')

    ld.add_action(DeclareLaunchArgument(
        'use_sim_time',
        default_value='false',
        description='Use simulator time'
    ))

    settings_file = os.path.join(package_dir, 'params', 'general_settings.yaml')
    with open(settings_file, 'r') as f:
        settings = yaml.safe_load(f)

    robots = settings['robots']

    default_slam_params_file = os.path.join(
        get_package_share_directory('slam_toolbox'),
        'config',
        'mapper_params_online_async.yaml'
    )
    slam_overlay_params_file = os.path.join(
        package_dir,
        'params',
        'slam_toolbox_dynamic_overlay.yaml'
    )

    for robot in robots:
        namespace = robot['name']

        map_frame = f'{namespace}/map'
        odom_frame = f'{namespace}/odom'
        base_frame = f'{namespace}/base_footprint'

        ld.add_action(Node(
            package='slam_toolbox',
            executable='async_slam_toolbox_node',
            name='slam_toolbox',
            namespace=namespace,
            output='screen',
            parameters=[
                default_slam_params_file,
                slam_overlay_params_file,
                {
                    'use_sim_time': use_sim_time,
                    'odom_frame': odom_frame,
                    'base_frame': base_frame,
                    'map_frame': map_frame,
                    'scan_topic': f'/{namespace}/scan',
                    'qos_overrides': {
                        f'/{namespace}/scan': {
                            'subscription': {'reliability': 'best_effort'},
                        },
                        f'/{namespace}/map': {
                            'publisher': {'reliability': 'best_effort'},
                        },
                        f'/{namespace}/map_metadata': {
                            'publisher': {'reliability': 'best_effort'},
                        },
                        f'/{namespace}/map_updates': {
                            'publisher': {'reliability': 'best_effort'},
                        },
                    },
                    # 'transform_timeout': 0.5,
                    # 'tf_buffer_duration': 30.0,
                }
            ],
            remappings=[
                ('/scan', f'/{namespace}/scan'),
                ('/map', f'/{namespace}/map'),
                ('/map_metadata', f'/{namespace}/map_metadata'),
                ('/map_updates', f'/{namespace}/map_updates'),
            ]
        ))
    
    ld.add_action(Node(
        package='nav2_map_server',
        executable='map_saver_server',
        name='map_saver',
        output='screen',
        parameters=[{
            'use_sim_time': use_sim_time,
            'save_map_timeout': 5000.0,
            'free_thresh_default': 0.25,
            'occupied_thresh_default': 0.65,
            'qos_overrides': {
                '/map': {
                    'subscription': {'reliability': 'best_effort'},
                },
            },
        }]
    ))

    ld.add_action(Node(
        package='nav2_lifecycle_manager',
        executable='lifecycle_manager',
        name='lifecycle_manager_map_saver',
        output='screen',
        parameters=[{
            'use_sim_time': use_sim_time,
            'autostart': True,
            'node_names': ['map_saver']
        }]
    ))

    return ld
