cmake_minimum_required(VERSION 3.8)
project(smart_factory_nav2_bt_plugins)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(behaviortree_cpp_v3 REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav2_behavior_tree REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)

add_library(smart_factory_compute_recovery_pose_bt_node SHARED
  src/compute_recovery_pose_action.cpp
)

target_compile_features(smart_factory_compute_recovery_pose_bt_node PUBLIC cxx_std_17)
target_compile_definitions(smart_factory_compute_recovery_pose_bt_node PRIVATE BT_PLUGIN_EXPORT)

ament_target_dependencies(smart_factory_compute_recovery_pose_bt_node
  behaviortree_cpp_v3
  geometry_msgs
  nav2_behavior_tree
  nav2_msgs
  rclcpp
  tf2
  tf2_geometry_msgs
  tf2_ros
)

install(TARGETS smart_factory_compute_recovery_pose_bt_node
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

ament_export_libraries(smart_factory_compute_recovery_pose_bt_node)
ament_export_dependencies(
  behaviortree_cpp_v3
  geometry_msgs
  nav2_behavior_tree
  nav2_msgs
  rclcpp
  tf2
  tf2_geometry_msgs
  tf2_ros
)

ament_package()
