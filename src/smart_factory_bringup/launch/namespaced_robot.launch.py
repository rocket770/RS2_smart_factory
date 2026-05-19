#!/usr/bin/env python3
#
# Copyright 2019 ROBOTIS CO., LTD.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Authors: Darby Lim

#!/usr/bin/env python3

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import Command, LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue

def generate_launch_description():
    TURTLEBOT3_MODEL = os.environ['TURTLEBOT3_MODEL']

    # Launch arguments
    namespace = LaunchConfiguration('namespace')
    usb_port = LaunchConfiguration('usb_port', default='/dev/ttyACM0')
    lidar_port = LaunchConfiguration('port', default='/dev/ttyUSB0')
    frame_id = LaunchConfiguration('frame_id', default='base_scan')
    use_sim_time = LaunchConfiguration('use_sim_time', default='false')
    odom_frame = [namespace, '/odom']
    base_footprint_frame = [namespace, '/base_footprint']
    tb3_param_dir = LaunchConfiguration(
        'tb3_param_dir',
        default=os.path.join(
            get_package_share_directory('turtlebot3_bringup'),
            'param',
            TURTLEBOT3_MODEL + '.yaml'))

    remappings = []#[('/tf', 'tf'), ('/tf_static', 'tf_static')]

    # Declare launch arguments
    ld = LaunchDescription([
        DeclareLaunchArgument('use_sim_time', default_value=use_sim_time,
                              description='Use simulation (Gazebo) clock if true'),
        DeclareLaunchArgument('usb_port', default_value=usb_port,
                              description='Connected USB port with OpenCR'),
        DeclareLaunchArgument('tb3_param_dir', default_value=tb3_param_dir,
                              description='Full path to turtlebot3 parameter file to load'),
        DeclareLaunchArgument('namespace', default_value='',
                              description='Namespace for nodes'),
        DeclareLaunchArgument('port', default_value=lidar_port,
                              description='USB port for lidar'),
        DeclareLaunchArgument('frame_id', default_value=frame_id,
                              description='Frame ID of lidar (default: laser)'),
        DeclareLaunchArgument('x_pose', default_value='0.0'),
        DeclareLaunchArgument('y_pose', default_value='0.0'),
        DeclareLaunchArgument('z_pose', default_value='0.0'),
        DeclareLaunchArgument('yaw', default_value='0.0'),
    ])

    urdf_file_name = f'turtlebot3_waffle.urdf.xacro'
    print(f'urdf_file_name : {urdf_file_name}')
    urdf = os.path.join(
        get_package_share_directory('multi_robot_bringup'),
        'urdf',
        urdf_file_name)

    robot_description = ParameterValue(
        Command([
            'xacro ',
            urdf,
            ' robot_ns:=', namespace,
        ]),
        value_type=str
    )

    # robot_state_publisher
    state_publisher = Node(
        package='robot_state_publisher',
        namespace=namespace,
        executable='robot_state_publisher',
        output='screen',
        parameters=[
            {
                'use_sim_time': use_sim_time,
                'publish_frequency': 10.0,
                'robot_description': robot_description,
            }
        ],
        remappings=remappings,
    )
    ld.add_action(state_publisher)

    # Laser scanner driver
    laser_scan = Node(
        package='hls_lfcd_lds_driver',
        namespace=namespace,
        executable='hlds_laser_publisher',
        name='hlds_laser_publisher',
        parameters=[
            {
                'port': lidar_port,
                'frame_id': [namespace, '/base_scan'],
                'qos_overrides': {
                    ('/', namespace, '/scan'): {
                        'publisher': {
                            'reliability': 'best_effort',
                            'history': 'keep_last',
                            'depth': 5,
                        },
                    },
                    'scan': {
                        'publisher': {
                            'reliability': 'best_effort',
                            'history': 'keep_last',
                            'depth': 5,
                        },
                    },
                },
            }
        ],
        output='screen'
    )
    ld.add_action(laser_scan)

    # TurtleBot3 core node
    node = Node(
        package='turtlebot3_node',
        namespace=namespace,
        executable='turtlebot3_ros',
        parameters=[
            tb3_param_dir,
            {
                'odometry.frame_id': odom_frame,
                'odometry.child_frame_id': base_footprint_frame,
            }
        ],
        arguments=['-i', usb_port],
        remappings=remappings,
        output='screen'
    )
    ld.add_action(node)


    x_pose = LaunchConfiguration('x_pose')
    y_pose = LaunchConfiguration('y_pose')
    z_pose = LaunchConfiguration('z_pose')
    yaw = LaunchConfiguration('yaw')

    ld.add_action(Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        namespace=namespace,
        name='map_anchor',
        arguments=[
            x_pose, y_pose, z_pose,
            yaw, '0', '0',
            'map',
            [namespace, '/map']
        ]
    ))

    return ld
