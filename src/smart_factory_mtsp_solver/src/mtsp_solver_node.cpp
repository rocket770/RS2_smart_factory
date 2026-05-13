#include "../include/smart_factory_mtsp_solver/mtsp_solver_node.hpp"

#include <chrono>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "../include/smart_factory_mtsp_solver/nav2_path_cost_provider.hpp"

namespace smart_factory_mtsp_solver
{
namespace
{

void append_json_number(std::ostringstream & ss, double value)
{
  if (std::isfinite(value)) {
    ss << value;
  } else {
    ss << "null";
  }
}

}  // namespace

  MtspSolverNode::MtspSolverNode()
  : Node("mtsp_solver_node"), started_(false)
  {
    this->declare_parameter<std::vector<double> >("robot_starts", std::vector<double>());
    this->declare_parameter<std::vector<double> >("goals", std::vector<double>());
    this->declare_parameter<int>("population_size", 100);
    this->declare_parameter<int>("generations", 200);
    this->declare_parameter<double>("mutation_rate", 0.10);
    this->declare_parameter<int>("seed", 42);
    this->declare_parameter<bool>("publish_progress", true);
    this->declare_parameter<int>("generation_delay_ms", 0);
    this->declare_parameter<double>("unused_robot_penalty", 0.0);
    this->declare_parameter<double>("route_count_balance_penalty", 0.0);
    this->declare_parameter<double>("publish_generation_delay", 25);
    this->declare_parameter<std::string>("distance_backend", "euclidean");

    this->declare_parameter<std::string>("planner_action_name", "/tb2/compute_path_to_pose");
    this->declare_parameter<std::string>("global_frame", "map");
    this->declare_parameter<std::string>("planner_id", "");
    this->declare_parameter<int>("planner_server_timeout_ms", 5000);
    this->declare_parameter<int>("planner_result_timeout_ms", 10000);

    progress_publisher_ = this->create_publisher<std_msgs::msg::String>("mtsp_best_solution", 10);

    startup_timer_ = this->create_wall_timer(
      std::chrono::milliseconds(500),
      [this]() {
        startup_timer_->cancel();
        solver_thread_ = std::thread([this]() {
          this->start_solver();
        });
      });
  }

  MtspSolverNode::~MtspSolverNode()
  {
    if (startup_timer_) {
      startup_timer_->cancel();
    }
  
    if (solver_thread_.joinable()) {
      if (solver_thread_.get_id() == std::this_thread::get_id()) {
        solver_thread_.detach();
      } else {
        solver_thread_.join();
      }
    }
  }

  void MtspSolverNode::start_solver()
  {
    if (started_) {
      return;
    }
    started_ = true;

    if (startup_timer_) {
      startup_timer_->cancel();
    }

    ProblemData problem = parse_problem_from_parameters();

    if (problem.robot_count() == 0) {
      RCLCPP_ERROR(this->get_logger(), "No robot start positions provided.");
      rclcpp::shutdown();
      return;
    }

    if (problem.goal_count() == 0) {
      RCLCPP_ERROR(this->get_logger(), "No goals provided.");
      rclcpp::shutdown();
      return;
    }

    GAParams params;
    params.population_size = this->get_parameter("population_size").as_int();
    params.generations = this->get_parameter("generations").as_int();
    params.mutation_rate = this->get_parameter("mutation_rate").as_double();
    params.seed = static_cast<unsigned int>(this->get_parameter("seed").as_int());
    params.unused_robot_penalty = this->get_parameter("unused_robot_penalty").as_double();
    params.route_count_balance_penalty = this->get_parameter("route_count_balance_penalty").as_double();
    
    const int publish_generation_delay = static_cast<int>(this->get_parameter("publish_generation_delay").as_double());
    const bool publish_progress = this->get_parameter("publish_progress").as_bool();
    const int generation_delay_ms = this->get_parameter("generation_delay_ms").as_int();
    const std::string planner_action_name = this->get_parameter("planner_action_name").as_string();
    const std::string global_frame = this->get_parameter("global_frame").as_string();
    const std::string planner_id = this->get_parameter("planner_id").as_string();
    const auto planner_server_timeout =
      std::chrono::milliseconds(this->get_parameter("planner_server_timeout_ms").as_int());
    const auto planner_result_timeout =
      std::chrono::milliseconds(this->get_parameter("planner_result_timeout_ms").as_int());

    const std::string distance_backend_str = this->get_parameter("distance_backend").as_string();

    if (distance_backend_str == "euclidean") {
      params.distance_backend = DistanceBackend::EUCLIDEAN;
    } else if (distance_backend_str == "nav2") {
      params.distance_backend = DistanceBackend::NAV2;
    } else {
      throw std::runtime_error(
              "Invalid distance_backend parameter. Expected 'euclidean' or 'nav2'");
    }

    GeneticAlgorithm ga;

    ProgressCallback callback;
    if (publish_progress || generation_delay_ms > 0) {
      callback = [this, &problem, publish_progress, generation_delay_ms, publish_generation_delay](const ProgressState & progress) {
        if (publish_progress) {
          if(progress.generation % publish_generation_delay == 0)
          publish_progress_message(progress, problem);
        }

        if (generation_delay_ms > 0) {
          std::this_thread::sleep_for(std::chrono::milliseconds(generation_delay_ms));
        }
      };
    }

    std::unique_ptr<PathCostProvider> path_cost_provider;

    if (params.distance_backend == DistanceBackend::NAV2) {
      auto nav2_provider = std::make_unique<Nav2PathCostProvider>(
        this->shared_from_this(),
        planner_action_name,
        global_frame,
        planner_id,
        planner_server_timeout,
        planner_result_timeout);

      if (!nav2_provider->wait_until_ready()) {
        throw std::runtime_error("Nav2 planner action server is not ready");
      }

      path_cost_provider = std::move(nav2_provider);
    }

    const Solution solution = ga.solve(problem, params, callback, path_cost_provider.get());

    if (publish_progress) {
      ProgressState final_progress;
      final_progress.generation = params.generations;
      final_progress.total_cost = solution.total_cost;
      final_progress.routes = solution.routes;
      publish_progress_message(final_progress, problem);
    }

    log_solution(solution, problem);

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    rclcpp::shutdown();
  }

  std::vector<double> MtspSolverNode::get_double_array_param(const std::string & name) const
  {
    return this->get_parameter(name).as_double_array();
  }

  ProblemData MtspSolverNode::parse_problem_from_parameters() const
  {
    ProblemData problem;

    const std::vector<double> robot_values = get_double_array_param("robot_starts");
    const std::vector<double> goal_values = get_double_array_param("goals");

    if (robot_values.size() % 2 != 0) {
      throw std::runtime_error("robot_starts must contain pairs of [x, y]");
    }

    if (goal_values.size() % 2 != 0) {
      throw std::runtime_error("goals must contain pairs of [x, y]");
    }

    for (std::size_t i = 0; i < robot_values.size(); i += 2) {
      problem.robot_starts.push_back(Point2D(robot_values[i], robot_values[i + 1]));
    }

    for (std::size_t i = 0; i < goal_values.size(); i += 2) {
      problem.goals.push_back(Point2D(goal_values[i], goal_values[i + 1]));
    }

    return problem;
  }

  // This was generated using chat-gpt**
  // I use this for debugging and visualizing!
  std::string MtspSolverNode::build_progress_json(
    const ProgressState & progress,
    const ProblemData & problem) const
  {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(6);

    ss << "{";
    ss << "\"generation\":" << progress.generation << ",";
    ss << "\"cost\":";
    append_json_number(ss, progress.total_cost);
    ss << ",";

    ss << "\"robot_starts\":[";
    for (std::size_t i = 0; i < problem.robot_starts.size(); ++i) {
      ss << "[" << problem.robot_starts[i].x << "," << problem.robot_starts[i].y << "]";
      if (i + 1 < problem.robot_starts.size()) {
        ss << ",";
      }
    }
    ss << "],";

    ss << "\"goals\":[";
    for (std::size_t i = 0; i < problem.goals.size(); ++i) {
      ss << "[" << problem.goals[i].x << "," << problem.goals[i].y << "]";
      if (i + 1 < problem.goals.size()) {
        ss << ",";
      }
    }
    ss << "],";

    ss << "\"routes\":[";
    for (std::size_t i = 0; i < progress.routes.size(); ++i) {
      ss << "[";
      for (std::size_t j = 0; j < progress.routes[i].size(); ++j) {
        ss << progress.routes[i][j];
        if (j + 1 < progress.routes[i].size()) {
          ss << ",";
        }
      }
      ss << "]";
      if (i + 1 < progress.routes.size()) {
        ss << ",";
      }
    }
    ss << "]";

    ss << "}";

    return ss.str();
  }

  void MtspSolverNode::publish_progress_message(
    const ProgressState & progress,
    const ProblemData & problem)
  {
    std_msgs::msg::String msg;
    msg.data = build_progress_json(progress, problem);
    progress_publisher_->publish(msg);
  }

  // This was also generated using chat-gpt**
  // I use this for debugging and visualizing!
  void MtspSolverNode::log_solution(const Solution & solution, const ProblemData & problem) const
  {
    RCLCPP_INFO(this->get_logger(), "Final best total cost: %.3f", solution.total_cost);
    RCLCPP_INFO(this->get_logger(), "Robots: %zu, Goals: %zu", problem.robot_count(), problem.goal_count());

    for (std::size_t robot = 0; robot < solution.routes.size(); ++robot) {
      std::ostringstream route_stream;
      route_stream << "Robot " << robot << " start=("
                  << problem.robot_starts[robot].x << ", "
                  << problem.robot_starts[robot].y << ") route: ";

      if (solution.routes[robot].empty()) {
        route_stream << "[no goals]";
      } else {
        for (std::size_t i = 0; i < solution.routes[robot].size(); ++i) {
          const int goal_index = solution.routes[robot][i];
          route_stream << goal_index
                      << "("
                      << problem.goals[static_cast<std::size_t>(goal_index)].x
                      << ","
                      << problem.goals[static_cast<std::size_t>(goal_index)].y
                      << ")";
          if (i + 1 < solution.routes[robot].size()) {
            route_stream << " -> ";
          }
        }
      }

      RCLCPP_INFO(this->get_logger(), "%s", route_stream.str().c_str());
    }
  }
}  // namespace smart_factory_mtsp_solver
