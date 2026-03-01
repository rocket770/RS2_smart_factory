from launch import LaunchDescription
from launch.actions import GroupAction, IncludeLaunchDescription
from launch_ros.actions import PushRosNamespace
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    nav2_bringup_dir = get_package_share_directory('nav2_bringup')
    nav2_launch = os.path.join(nav2_bringup_dir, 'launch', 'navigation_launch.py')

    return LaunchDescription([
        GroupAction([
            PushRosNamespace('TB3_2'),

            SetRemap(src='/TB3_2/tf', dst='/tf'),
            SetRemap(src='/TB3_2/tf_static', dst='/tf_static'),

            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(nav2_launch),
                launch_arguments={
                    # IMPORTANT: prevent RewrittenYaml root_key behavior
                    'use_namespace': 'False',
                    'namespace': '',

                    'use_sim_time': 'True',
                    'autostart': 'True',
                    'params_file': '/home/nick/smart_factory_ws/src/smart_factory_navigation/config/nav2_tb3_2.yaml',
                }.items()
            ),
        ])
    ])