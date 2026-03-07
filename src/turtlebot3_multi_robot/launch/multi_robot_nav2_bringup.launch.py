from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
import os
import yaml

def generate_launch_description():
    package_dir = get_package_share_directory('turtlebot3_multi_robot')
    nav_launch_dir = os.path.join(package_dir, 'launch', 'nav2_bringup')

    use_sim_time = LaunchConfiguration('use_sim_time')
    enable_rviz = LaunchConfiguration('enable_rviz')
    params_file = LaunchConfiguration('nav_params_file')
    rviz_config_file = LaunchConfiguration('rviz_config_file')

    ld = LaunchDescription()

    ld.add_action(DeclareLaunchArgument('use_sim_time', default_value='true'))
    ld.add_action(DeclareLaunchArgument('enable_rviz', default_value='false'))
    ld.add_action(DeclareLaunchArgument(
        'nav_params_file',
        default_value=os.path.join(package_dir, 'params', 'nav2_slam_params.yaml')
    ))
    ld.add_action(DeclareLaunchArgument(
        'rviz_config_file',
        default_value=os.path.join(package_dir, 'rviz', 'multi_nav2_default_view.rviz')
    ))

    setting_path = os.path.join(package_dir, 'params', 'general_settings.yaml')
    with open(setting_path, 'r') as f:
        settings = yaml.safe_load(f)

    for robot in settings['robots']:
        namespace = robot['name']

        ld.add_action(IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                os.path.join(nav_launch_dir, 'navigation_launch.py')
            ),
            launch_arguments={
                'namespace': namespace,
                'use_namespace': 'True',
                'params_file': params_file,
                'autostart': 'true',
                'use_sim_time': use_sim_time,
                'log_level': 'warn',
            }.items()
        ))

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