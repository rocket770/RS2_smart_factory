#include "../include/smart_factory_mtsp_solver/mtsp_solver_node.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  try {
    auto node = std::make_shared<smart_factory_mtsp_solver::MtspSolverNode>();

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();
  } catch (const std::exception & e) {
    fprintf(stderr, "Exception: %s\n", e.what());
  }

  rclcpp::shutdown();
  return 0;
}