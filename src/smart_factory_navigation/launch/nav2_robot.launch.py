from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, GroupAction
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution, TextSubstitution
from launch_ros.actions import Node, PushRosNamespace
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    ns = LaunchConfiguration("ns")
    map_yaml = LaunchConfiguration("map")
    use_sim_time = LaunchConfiguration("use_sim_time")
    params_file = LaunchConfiguration("params_file")

    default_params = PathJoinSubstitution(
        [FindPackageShare("smart_factory_navigation"), "config", "nav2_common.yaml"]
    )

    # Fully-qualified TF frame IDs
    base_link_fq = [ns, TextSubstitution(text="/base_link")]
    base_footprint_fq = [ns, TextSubstitution(text="/base_footprint")]
    odom_fq = [ns, TextSubstitution(text="/odom")]

    # Common dotted overrides applied per node
    # (Dotted keys are reliably applied; nested dicts often aren't.)
    amcl_overrides = {
        "use_sim_time": use_sim_time,
        "global_frame_id": "map",
        "odom_frame_id": odom_fq,
        "base_frame_id": base_footprint_fq,
        "scan_topic": "scan",
    }

    # Costmaps + controller must use fully-qualified robot_base_frame
    controller_overrides = {
        "use_sim_time": use_sim_time,

        # Force controller plugin to RPP (avoids DWB critics issue)
        "controller_plugins": ["FollowPath"],
        "FollowPath.plugin": "nav2_regulated_pure_pursuit_controller::RegulatedPurePursuitController",
        "FollowPath.desired_linear_vel": 0.25,
        "FollowPath.lookahead_dist": 0.6,
        "FollowPath.min_lookahead_dist": 0.3,
        "FollowPath.max_lookahead_dist": 1.0,
        "FollowPath.rotate_to_heading_angular_vel": 0.8,
        "FollowPath.use_velocity_scaled_lookahead_dist": True,
        "FollowPath.use_rotate_to_heading": True,
        "FollowPath.allow_reversing": False,

        # Local costmap frames
        "local_costmap.local_costmap.ros__parameters.global_frame": odom_fq,
        "local_costmap.local_costmap.ros__parameters.robot_base_frame": base_link_fq,

        # Global costmap frames
        "global_costmap.global_costmap.ros__parameters.global_frame": "map",
        "global_costmap.global_costmap.ros__parameters.robot_base_frame": base_link_fq,
    }

    planner_overrides = {
        "use_sim_time": use_sim_time,

        # Global costmap frames used by planner
        "global_costmap.global_costmap.ros__parameters.global_frame": "map",
        "global_costmap.global_costmap.ros__parameters.robot_base_frame": base_link_fq,
    }

    bt_nav_overrides = {
        "use_sim_time": use_sim_time,
        "global_frame": "map",
        "robot_base_frame": base_link_fq,
        "odom_topic": "odom",
    }

    behavior_overrides = {
        "use_sim_time": use_sim_time,
        "global_frame": "map",
        "robot_base_frame": base_link_fq,
    }

    map_server_overrides = {
        "use_sim_time": use_sim_time,
        "yaml_filename": map_yaml,
    }

    lifecycle_overrides = {
        "use_sim_time": use_sim_time,
        "autostart": True,
        "node_names": [
            "map_server",
            "amcl",
            "controller_server",
            "planner_server",
            "behavior_server",
            "bt_navigator",
        ],
    }

    return LaunchDescription(
        [
            DeclareLaunchArgument("ns", default_value="TB3_1"),
            DeclareLaunchArgument("map", default_value=""),
            DeclareLaunchArgument("use_sim_time", default_value="true"),
            DeclareLaunchArgument("params_file", default_value=default_params),

            GroupAction(
                [
                    PushRosNamespace(ns),

                    Node(
                        package="nav2_map_server",
                        executable="map_server",
                        name="map_server",
                        output="screen",
                        parameters=[params_file, map_server_overrides],
                    ),

                    Node(
                        package="nav2_amcl",
                        executable="amcl",
                        name="amcl",
                        output="screen",
                        parameters=[params_file, amcl_overrides],
                    ),

                    Node(
                        package="nav2_controller",
                        executable="controller_server",
                        name="controller_server",
                        output="screen",
                        parameters=[params_file, controller_overrides],
                    ),

                    Node(
                        package="nav2_planner",
                        executable="planner_server",
                        name="planner_server",
                        output="screen",
                        parameters=[params_file, planner_overrides],
                    ),

                    Node(
                        package="nav2_behaviors",
                        executable="behavior_server",
                        name="behavior_server",
                        output="screen",
                        parameters=[params_file, behavior_overrides],
                    ),

                    Node(
                        package="nav2_bt_navigator",
                        executable="bt_navigator",
                        name="bt_navigator",
                        output="screen",
                        parameters=[params_file, bt_nav_overrides],
                    ),

                    Node(
                        package="nav2_lifecycle_manager",
                        executable="lifecycle_manager",
                        name="lifecycle_manager",
                        output="screen",
                        parameters=[lifecycle_overrides],
                    ),
                ]
            ),
        ]
    )