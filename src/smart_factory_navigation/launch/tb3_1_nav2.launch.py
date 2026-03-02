from launch import LaunchDescription
from launch.actions import GroupAction, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import PushRosNamespace, SetRemap
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    nav2_bringup_dir = get_package_share_directory('nav2_bringup')
    nav2_launch = os.path.join(nav2_bringup_dir, 'launch', 'navigation_launch.py')

    return LaunchDescription([
        GroupAction([
            PushRosNamespace('TB3_1'),
            
            SetRemap(src='tf', dst='/tf'),
            SetRemap(src='tf_static', dst='/tf_static'),

            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(nav2_launch),
                launch_arguments={
                    'use_namespace': 'False',
                    'namespace': '',
                    'use_composition': 'False',
                    'use_sim_time': 'true',
                    'autostart': 'true',
                    'params_file': '/home/nick/smart_factory_ws/src/smart_factory_navigation/config/nav2_tb3_1.yaml',
                }.items()
            ),
        ])
    ])