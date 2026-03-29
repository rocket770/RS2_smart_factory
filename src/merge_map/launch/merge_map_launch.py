import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    pkg_share = get_package_share_directory('merge_map')

    rviz_file = os.path.join(pkg_share, 'config', 'merge_map.rviz')
    params_file = os.path.join(pkg_share, 'config', 'map_merge_params.yaml')

    return LaunchDescription([
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            output='screen',
            arguments=['-d', rviz_file],
            parameters=[{'use_sim_time': True}],
        ),
        Node(
            package='merge_map',
            executable='merge_map',
            name='merge_map',
            output='screen',
            parameters=[params_file],
        ),
    ])