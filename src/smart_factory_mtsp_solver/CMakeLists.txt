cmake_minimum_required(VERSION 3.8)
project(smart_factory_mtsp_solver)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(nav2_msgs REQUIRED)

include_directories(include)

add_library(mtsp_solver_core
  src/distance_matrix.cpp
  src/genome.cpp
  src/individual.cpp
  src/population.cpp
  src/genetic_algorithm.cpp
  src/mtsp_solver_node.cpp
  src/nav2_path_cost_provider.cpp
)

ament_target_dependencies(mtsp_solver_core
  rclcpp
  rclcpp_action
  geometry_msgs
  nav_msgs
  nav2_msgs
  std_msgs
)

add_executable(mtsp_solver_node
  src/main.cpp
)

target_link_libraries(mtsp_solver_node
  mtsp_solver_core
)

ament_target_dependencies(mtsp_solver_node
  rclcpp
  rclcpp_action
  geometry_msgs
  nav_msgs
  nav2_msgs
  std_msgs
)

install(TARGETS
  mtsp_solver_core
  mtsp_solver_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include/
)

install(DIRECTORY config/
  DESTINATION share/${PROJECT_NAME}/config
)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)

  ament_add_gtest(test_distance_matrix_and_individual
    test/test_distance_matrix_and_individual.cpp
  )
  target_link_libraries(test_distance_matrix_and_individual
    mtsp_solver_core
  )
  ament_target_dependencies(test_distance_matrix_and_individual
    rclcpp
    rclcpp_action
    geometry_msgs
    nav_msgs
    nav2_msgs
    std_msgs
  )

  ament_add_gtest(test_genetic_algorithm_euclidean_integration
    test/test_ga_euclidean_integration.cpp
  )
  target_link_libraries(test_genetic_algorithm_euclidean_integration
    mtsp_solver_core
  )
  ament_target_dependencies(test_genetic_algorithm_euclidean_integration
    rclcpp
    rclcpp_action
    geometry_msgs
    nav_msgs
    nav2_msgs
    std_msgs
  )
endif()

ament_package()