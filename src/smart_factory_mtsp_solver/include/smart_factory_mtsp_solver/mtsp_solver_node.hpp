#ifndef SMART_FACTORY_MTSP_SOLVER_MTSP_SOLVER_NODE_HPP
#define SMART_FACTORY_MTSP_SOLVER_MTSP_SOLVER_NODE_HPP

#include <string>
#include <vector>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "genetic_algorithm.hpp"
#include "problem_data.hpp"

namespace smart_factory_mtsp_solver
{

class MtspSolverNode : public rclcpp::Node
{
public:
  MtspSolverNode();
  ~MtspSolverNode();

private:
  void start_solver();

  ProblemData parse_problem_from_parameters() const;
  std::vector<double> get_double_array_param(const std::string & name) const;
  void log_solution(const Solution & solution, const ProblemData & problem) const;

  std::string build_progress_json(
    const ProgressState & progress,
    const ProblemData & problem) const;

  void publish_progress_message(
    const ProgressState & progress,
    const ProblemData & problem);

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr progress_publisher_;
  rclcpp::TimerBase::SharedPtr startup_timer_;
  bool started_;
  std::thread solver_thread_;
};

}  // namespace smart_factory_mtsp_solver

#endif