from launch import LaunchDescription
from launch_ros.actions import Node, PushRosNamespace

def generate_launch_description():
    return LaunchDescription([
        PushRosNamespace('TB3_2'),

        Node(
            package='slam_toolbox',
            executable='async_slam_toolbox_node',
            name='slam_toolbox',
            output='screen',
            parameters=[{
                'use_sim_time': True,
                'mode': 'mapping',
                'map_frame': 'map',
                'odom_frame': 'TB3_2/odom',
                'base_frame': 'TB3_2/base_footprint',
                'scan_topic': '/TB3_2/scan',
                'tf_buffer_duration': 30.0
            }],
            remappings=[
                ('/map', 'map'),
                ('/map_metadata', 'map_metadata'),
            ]
        )
    ])