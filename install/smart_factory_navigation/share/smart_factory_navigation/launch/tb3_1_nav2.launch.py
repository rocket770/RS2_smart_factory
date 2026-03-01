from launch import LaunchDescription
from launch.actions import GroupAction, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import PushRosNamespace, SetRemap
from ament_index_python.packages import get_package_share_directory
import os
from launch_ros.actions import SetParameter

def generate_launch_description():
    nav2_bringup_dir = get_package_share_directory('nav2_bringup')
    nav2_launch = os.path.join(nav2_bringup_dir, 'launch', 'navigation_launch.py')

    return LaunchDescription([
        GroupAction([
            PushRosNamespace('TB3_1'),

            # Force Nav2 to use GLOBAL TF topics (so it can see slam_toolbox/map frames)
            SetRemap(src='/TB3_1/tf', dst='/tf'),
            SetRemap(src='/TB3_1/tf_static', dst='/tf_static'),
            # (optional but harmless)
            SetRemap(src='tf', dst='/tf'),
            SetRemap(src='tf_static', dst='/tf_static'),

            SetParameter(name='robot_base_frame', value='TB3_1/base_link'),
            SetParameter(name='global_frame', value='TB3_1/odom'),

            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(nav2_launch),
                launch_arguments={
                    'use_namespace': 'True',
                    'namespace': 'TB3_1',
                    'use_sim_time': 'True',
                    'autostart': 'True',
                    'params_file': '/home/nick/smart_factory_ws/src/smart_factory_navigation/config/nav2_tb3_1.yaml',
                }.items()
            ),
        ])
    ])