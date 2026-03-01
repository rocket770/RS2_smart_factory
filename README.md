# RSW_smart_factory

SO FAR:
ros2 launch smart_factory_sim two_tb3.launch.py
ros2 launch smart_factory_navigation slam_tb3_1.launch.py
ros2 launch smart_factory_navigation slam_tb3_2.launch.py
ros2 run multirobot_map_merge map_merge --ros-args --params-file   /home/nick/smart_factory_ws/src/smart_factory_navigation/config/map_merge_tb3.yaml

and then the last part is to run nav2
ros2 launch smart_factory_navigation tb3_1_nav2.launch.py

but this prints error 

local_costmap.local_costmap: Timed out waiting for transform from TB3_1/base_link to TB3_1/odom to become available, tf error: Invalid frame ID "TB3_1/odom" passed to canTransform argument target_frame - frame does not exist

we also have to force the namespace in the launch for it to work - it seems to be an issue with nav2 using namespacing