#!/usr/bin/env python3

import os
import yaml
import tempfile
import subprocess

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, RegisterEventHandler
from launch.conditions import IfCondition
from launch.event_handlers import OnProcessExit
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch.substitutions import Command
from launch_ros.parameter_descriptions import ParameterValue

def generate_sdf_from_xacro(xacro_file: str, robot_ns: str, robot_name: str) -> str:
    tmp_dir = tempfile.gettempdir()
    out_file = os.path.join(tmp_dir, f'{robot_name}_model.sdf')

    cmd = [
        'xacro',
        xacro_file,
        f'robot_ns:={robot_ns}',
        f'robot_name:={robot_name}',
    ]

    result = subprocess.run(cmd, check=True, capture_output=True, text=True)

    with open(out_file, 'w') as f:
        f.write(result.stdout)

    return out_file


def generate_launch_description():
    ld = LaunchDescription()

    turtlebot3_model = 'waffle'

    use_sim_time = LaunchConfiguration('use_sim_time')
    enable_drive = LaunchConfiguration('enable_drive')

    ld.add_action(DeclareLaunchArgument(
        'use_sim_time',
        default_value='true',
        description='Use simulator time'
    ))

    ld.add_action(DeclareLaunchArgument(
        'enable_drive',
        default_value='false',
        description='Enable robot drive node'
    ))

    pkg_multi = get_package_share_directory('smart_factory_bringup')
    pkg_gazebo_ros = get_package_share_directory('gazebo_ros')

    urdf_xacro = os.path.join(
        pkg_multi, 'urdf', f'turtlebot3_{turtlebot3_model}.urdf.xacro'
    )

    model_xacro = os.path.join(
        pkg_multi, 'models', f'turtlebot3_{turtlebot3_model}', 'model.sdf.xacro'
    )

    settings_file = os.path.join(pkg_multi, 'params', 'general_settings.yaml')
    with open(settings_file, 'r') as f:
        settings = yaml.safe_load(f)

    robots = settings['robots']

    world = os.path.join(pkg_multi, 'worlds', 'multi_robot_world.world')

    gzserver_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_gazebo_ros, 'launch', 'gzserver.launch.py')
        ),
        launch_arguments={'world': world}.items(),
    )

    gzclient_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_gazebo_ros, 'launch', 'gzclient.launch.py')
        )
    )

    ld.add_action(gzserver_cmd)
    ld.add_action(gzclient_cmd)

    remappings = []#[('/tf', 'tf'), ('/tf_static', 'tf_static')]

    last_action = None

    for robot in robots:
        name = robot['name']
        x_pose = str(robot['x_pose'])
        y_pose = str(robot['y_pose'])
        z_pose = str(robot.get('z_pose', 0.01))

        sdf_file = generate_sdf_from_xacro(
            xacro_file=model_xacro,
            robot_ns=name,
            robot_name=name,
        )

        robot_description = ParameterValue(
            Command([
                'xacro ',
                urdf_xacro,
                ' robot_ns:=', name,
            ]),
            value_type=str
        )

        robot_state_publisher = Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            namespace=name,
            output='screen',
            parameters=[
                {
                    'use_sim_time': use_sim_time,
                    'publish_frequency': 10.0,
                    'robot_description': robot_description,
                }
            ],
        )

        spawn_robot = Node(
            package='gazebo_ros',
            executable='spawn_entity.py',
            arguments=[
                '-entity', name,
                '-file', sdf_file,
                '-robot_namespace', f'/{name}',
                '-x', x_pose,
                '-y', y_pose,
                '-z', z_pose,
                '-Y', '0.0',
                '-unpause',
            ],
            output='screen',
        )

        world_to_map = Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name=f'{name}_world_to_map',
            arguments=[
                '0', '0', '0',
                '0', '0', '0',
                'world',
                f'{name}/odom',
            ],
            output='screen',
        )

        if last_action is None:
            ld.add_action(robot_state_publisher)
            ld.add_action(spawn_robot)
            ld.add_action(world_to_map)
        else:
            ld.add_action(RegisterEventHandler(
                OnProcessExit(
                    target_action=last_action,
                    on_exit=[
                        robot_state_publisher,
                        spawn_robot,
                        world_to_map,
                    ],
                )
            ))

        last_action = spawn_robot

    for robot in robots:
        name = robot['name']

        drive_node = Node(
            package='turtlebot3_gazebo',
            executable='turtlebot3_drive',
            namespace=name,
            output='screen',
            condition=IfCondition(enable_drive),
        )

        ld.add_action(RegisterEventHandler(
            OnProcessExit(
                target_action=last_action,
                on_exit=[drive_node],
            )
        ))

    return ld