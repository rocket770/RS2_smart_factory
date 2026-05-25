from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument('map_topic', default_value='/map'),
        DeclareLaunchArgument('use_sim_time', default_value='false'),
        DeclareLaunchArgument('robot_namespace_regex', default_value='^tb[0-9]+$'),
        DeclareLaunchArgument('base_frame', default_value='base_link'),
        DeclareLaunchArgument('fallback_base_frame', default_value='base_footprint'),
        DeclareLaunchArgument('start_paused', default_value='true'),
        DeclareLaunchArgument('planning_period_sec', default_value='1.0'),
        DeclareLaunchArgument('discovery_period_sec', default_value='2.0'),
        DeclareLaunchArgument('reservation_radius_m', default_value='1.5'),
        DeclareLaunchArgument('frontier_min_cluster_size', default_value='6'),
        DeclareLaunchArgument('min_frontier_separation_m', default_value='1.5'),
        DeclareLaunchArgument('goal_tolerance_m', default_value='0.75'),
        DeclareLaunchArgument('robot_stale_timeout_sec', default_value='5.0'),
        DeclareLaunchArgument('assignment_timeout_sec', default_value='30.0'),
        DeclareLaunchArgument('blacklist_timeout_sec', default_value='20.0'),
        DeclareLaunchArgument('cancel_goals_on_pause', default_value='false'),
        DeclareLaunchArgument('enable_rviz_markers', default_value='true'),
        DeclareLaunchArgument('marker_topic', default_value='~/markers'),
        DeclareLaunchArgument('marker_lifetime_sec', default_value='1.5'),
        Node(
            package='smart_factory_navigation',
            executable='multi_robot_explorer',
            name='multi_robot_explorer',
            output='screen',
            parameters=[{
                'map_topic': LaunchConfiguration('map_topic'),
                'use_sim_time': LaunchConfiguration('use_sim_time'),
                'robot_namespace_regex': LaunchConfiguration('robot_namespace_regex'),
                'base_frame': LaunchConfiguration('base_frame'),
                'fallback_base_frame': LaunchConfiguration('fallback_base_frame'),
                'start_paused': LaunchConfiguration('start_paused'),
                'planning_period_sec': LaunchConfiguration('planning_period_sec'),
                'discovery_period_sec': LaunchConfiguration('discovery_period_sec'),
                'reservation_radius_m': LaunchConfiguration('reservation_radius_m'),
                'frontier_min_cluster_size': LaunchConfiguration('frontier_min_cluster_size'),
                'min_frontier_separation_m': LaunchConfiguration('min_frontier_separation_m'),
                'goal_tolerance_m': LaunchConfiguration('goal_tolerance_m'),
                'robot_stale_timeout_sec': LaunchConfiguration('robot_stale_timeout_sec'),
                'assignment_timeout_sec': LaunchConfiguration('assignment_timeout_sec'),
                'blacklist_timeout_sec': LaunchConfiguration('blacklist_timeout_sec'),
                'cancel_goals_on_pause': LaunchConfiguration('cancel_goals_on_pause'),
                'enable_rviz_markers': LaunchConfiguration('enable_rviz_markers'),
                'marker_topic': LaunchConfiguration('marker_topic'),
                'marker_lifetime_sec': LaunchConfiguration('marker_lifetime_sec'),
            }],
        ),
    ])
