from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, TimerAction, GroupAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node, PushRosNamespace
from launch_ros.substitutions import FindPackageShare


def nav2_nav_group(ns: LaunchConfiguration, params_file: LaunchConfiguration,
                   use_sim_time: LaunchConfiguration, autostart: LaunchConfiguration):
    nav2_navigation_launch = PathJoinSubstitution(
        [FindPackageShare("nav2_bringup"), "launch", "navigation_launch.py"]
    )

    return GroupAction([
        PushRosNamespace(ns),

        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(nav2_navigation_launch),
            launch_arguments={
                "use_sim_time": use_sim_time,
                "autostart": autostart,
                "params_file": params_file,
                # IMPORTANT: do NOT rely on nav2_bringup's namespace args
                "use_namespace": "True",
            }.items(),
        ),
    ])


def amcl_group(ns: LaunchConfiguration, params_file: LaunchConfiguration,
              use_sim_time: LaunchConfiguration):
    return GroupAction([
        PushRosNamespace(ns),
        Node(
            package="nav2_amcl",
            executable="amcl",
            name="amcl",
            output="screen",
            parameters=[params_file, {"use_sim_time": use_sim_time}],
        )
    ])


def generate_launch_description():
    use_sim_time = LaunchConfiguration("use_sim_time")
    autostart = LaunchConfiguration("autostart")

    tb3_1_ns = LaunchConfiguration("tb3_1_ns")
    tb3_2_ns = LaunchConfiguration("tb3_2_ns")

    params_tb3_1 = LaunchConfiguration("params_tb3_1")
    params_tb3_2 = LaunchConfiguration("params_tb3_2")

    slam_params = LaunchConfiguration("slam_params")

    slam_delay = LaunchConfiguration("slam_delay")
    nav2_delay = LaunchConfiguration("nav2_delay")

    slam_toolbox_launch = PathJoinSubstitution(
        [FindPackageShare("slam_toolbox"), "launch", "online_async_launch.py"]
    )

    ld = LaunchDescription()

    ld.add_action(DeclareLaunchArgument("use_sim_time", default_value="True"))
    ld.add_action(DeclareLaunchArgument("autostart", default_value="True"))

    ld.add_action(DeclareLaunchArgument("tb3_1_ns", default_value="TB3_1"))
    ld.add_action(DeclareLaunchArgument("tb3_2_ns", default_value="TB3_2"))

    ld.add_action(DeclareLaunchArgument("params_tb3_1", default_value=""))
    ld.add_action(DeclareLaunchArgument("params_tb3_2", default_value=""))

    ld.add_action(DeclareLaunchArgument("slam_params", default_value=""))

    ld.add_action(DeclareLaunchArgument("slam_delay", default_value="2.0"))
    ld.add_action(DeclareLaunchArgument("nav2_delay", default_value="10.0"))

    # Shared SLAM toolbox (global /map)
    ld.add_action(
        TimerAction(
            period=slam_delay,
            actions=[
                IncludeLaunchDescription(
                    PythonLaunchDescriptionSource(slam_toolbox_launch),
                    launch_arguments={
                        "use_sim_time": use_sim_time,
                        "params_file": slam_params,
                    }.items(),
                )
            ],
        )
    )

    # AMCL per robot (namespaced)
    ld.add_action(amcl_group(tb3_1_ns, params_tb3_1, use_sim_time))
    ld.add_action(amcl_group(tb3_2_ns, params_tb3_2, use_sim_time))

    # Nav2 navigation per robot (namespaced) AFTER delay
    ld.add_action(
        TimerAction(
            period=nav2_delay,
            actions=[
                nav2_nav_group(tb3_1_ns, params_tb3_1, use_sim_time, autostart),
                nav2_nav_group(tb3_2_ns, params_tb3_2, use_sim_time, autostart),
            ],
        )
    )

    return ld