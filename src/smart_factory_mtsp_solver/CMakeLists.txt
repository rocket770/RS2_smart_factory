cmake_minimum_required(VERSION 3.8)
project(smart_factory_mtsp_solver)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)

include_directories(include)

add_executable(mtsp_solver_node
  src/main.cpp
  src/distance_matrix.cpp
  src/genome.cpp
  src/individual.cpp
  src/population.cpp
  src/genetic_algorithm.cpp
  src/mtsp_solver_node.cpp
)

ament_target_dependencies(mtsp_solver_node
  rclcpp
  geometry_msgs
  std_msgs
)

install(TARGETS
  mtsp_solver_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include/
)

install(DIRECTORY config/
  DESTINATION share/${PROJECT_NAME}/config
)

ament_package()