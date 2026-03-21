#include "../include/smart_factory_mtsp_solver/nav2_path_cost_provider.hpp"

#include <chrono>
#include <cmath>
#include <future>
#include <limits>
#include <mutex>

namespace smart_factory_mtsp_solver
{

namespace
{

double pose_distance(
  const geometry_msgs::msg::PoseStamped & a,
  const geometry_msgs::msg::PoseStamped & b)
{
  const double dx = a.pose.position.x - b.pose.position.x;
  const double dy = a.pose.position.y - b.pose.position.y;
  return std::sqrt(dx * dx + dy * dy);
}

}  // namespace

Nav2PathCostProvider::Nav2PathCostProvider(
  const rclcpp::Node::SharedPtr & node,
  const std::string & action_name,
  const std::string & global_frame,
  const std::string & planner_id,
  std::chrono::milliseconds server_timeout,
  std::chrono::milliseconds result_timeout)
: node_(node),
  action_name_(action_name),
  global_frame_(global_frame),
  planner_id_(planner_id),
  server_timeout_(server_timeout),
  result_timeout_(result_timeout)
{
  client_ = rclcpp_action::create_client<ComputePathToPose>(node_, action_name_);
}

bool Nav2PathCostProvider::wait_until_ready()
{
  if (!client_) {
    RCLCPP_ERROR(node_->get_logger(), "Nav2 path cost client was not created");
    return false;
  }

  RCLCPP_INFO(
    node_->get_logger(),
    "Waiting for Nav2 planner action server: %s",
    action_name_.c_str());

  if (!client_->wait_for_action_server(server_timeout_)) {
    RCLCPP_ERROR(
      node_->get_logger(),
      "Timed out waiting for Nav2 planner action server: %s",
      action_name_.c_str());
    return false;
  }

  RCLCPP_INFO(
    node_->get_logger(),
    "Connected to Nav2 planner action server: %s",
    action_name_.c_str());

  return true;
}

geometry_msgs::msg::PoseStamped Nav2PathCostProvider::make_pose(const Point2D & pt) const
{
  geometry_msgs::msg::PoseStamped pose;

  pose.header.frame_id = global_frame_;

  pose.pose.position.x = pt.x;
  pose.pose.position.y = pt.y;
  pose.pose.position.z = 0.0;

  pose.pose.orientation.x = 0.0;
  pose.pose.orientation.y = 0.0;
  pose.pose.orientation.z = 0.0;
  pose.pose.orientation.w = 1.0;

  return pose;
}

double Nav2PathCostProvider::compute_path_length(const nav_msgs::msg::Path & path) const
{
  if (path.poses.empty()) {
    return std::numeric_limits<double>::infinity();
  }

  if (path.poses.size() == 1) {
    return 0.0;
  }

  double total = 0.0;
  for (std::size_t i = 1; i < path.poses.size(); ++i) {
    total += pose_distance(path.poses[i - 1], path.poses[i]);
  }

  return total;
}

double Nav2PathCostProvider::compute_cost(const Point2D & start, const Point2D & goal)
{
  std::lock_guard<std::mutex> lock(request_mutex_);

  if (!client_) {
    RCLCPP_ERROR(node_->get_logger(), "Planner action client is null");
    return std::numeric_limits<double>::infinity();
  }

  ComputePathToPose::Goal action_goal;
  action_goal.start = make_pose(start);
  action_goal.goal = make_pose(goal);
  action_goal.planner_id = planner_id_.empty() ? "GridBased" : planner_id_;
  action_goal.use_start = true;

  auto goal_promise =
    std::make_shared<std::promise<GoalHandleComputePathToPose::SharedPtr>>();
  auto result_promise =
    std::make_shared<std::promise<GoalHandleComputePathToPose::WrappedResult>>();

  auto goal_done = std::make_shared<std::atomic_bool>(false);
  auto result_done = std::make_shared<std::atomic_bool>(false);

  auto send_goal_options =
    rclcpp_action::Client<ComputePathToPose>::SendGoalOptions();

  send_goal_options.goal_response_callback =
    [goal_promise, goal_done](GoalHandleComputePathToPose::SharedPtr goal_handle) {
      if (!goal_done->exchange(true)) {
        goal_promise->set_value(goal_handle);
      }
    };

  send_goal_options.result_callback =
    [result_promise, result_done](const GoalHandleComputePathToPose::WrappedResult & result) {
      if (!result_done->exchange(true)) {
        result_promise->set_value(result);
      }
    };

  RCLCPP_WARN(
    node_->get_logger(),
    "SEND start=(%.2f, %.2f) goal=(%.2f, %.2f) frame=%s use_start=%d planner=%s",
    action_goal.start.pose.position.x, action_goal.start.pose.position.y,
    action_goal.goal.pose.position.x, action_goal.goal.pose.position.y,
    action_goal.start.header.frame_id.c_str(),
    action_goal.use_start,
    action_goal.planner_id.c_str());

  client_->async_send_goal(action_goal, send_goal_options);

  auto goal_future = goal_promise->get_future();
  while (rclcpp::ok()) {
    if (goal_future.wait_for(std::chrono::milliseconds(50)) == std::future_status::ready) {
      break;
    }
  }

  if (!rclcpp::ok()) {
    RCLCPP_WARN(node_->get_logger(), "Shutdown requested while waiting for goal response");
    return std::numeric_limits<double>::infinity();
  }

  auto goal_handle = goal_future.get();
  if (!goal_handle) {
    RCLCPP_WARN(
      node_->get_logger(),
      "Planner rejected path request from (%.3f, %.3f) to (%.3f, %.3f)",
      start.x, start.y, goal.x, goal.y);
    return std::numeric_limits<double>::infinity();
  }

  auto result_future = result_promise->get_future();
  auto start_wait = std::chrono::steady_clock::now();

  while (rclcpp::ok()) {
    if (result_future.wait_for(std::chrono::milliseconds(50)) == std::future_status::ready) {
      break;
    }

    if (std::chrono::steady_clock::now() - start_wait > result_timeout_) {
      RCLCPP_WARN(
        node_->get_logger(),
        "Timed out waiting for path result from (%.3f, %.3f) to (%.3f, %.3f); canceling goal",
        start.x, start.y, goal.x, goal.y);
      client_->async_cancel_goal(goal_handle);
      return std::numeric_limits<double>::infinity();
    }
  }

  if (!rclcpp::ok()) {
    RCLCPP_WARN(node_->get_logger(), "Shutdown requested while waiting for path result");
    client_->async_cancel_goal(goal_handle);
    return std::numeric_limits<double>::infinity();
  }

  const auto wrapped_result = result_future.get();

  if (wrapped_result.code != rclcpp_action::ResultCode::SUCCEEDED) {
    RCLCPP_WARN(
      node_->get_logger(),
      "Planner failed from (%.3f, %.3f) to (%.3f, %.3f), code=%d",
      start.x, start.y, goal.x, goal.y,
      static_cast<int>(wrapped_result.code));
    return std::numeric_limits<double>::infinity();
  }

  if (!wrapped_result.result) {
    RCLCPP_WARN(
      node_->get_logger(),
      "Planner returned null result from (%.3f, %.3f) to (%.3f, %.3f)",
      start.x, start.y, goal.x, goal.y);
    return std::numeric_limits<double>::infinity();
  }

  return compute_path_length(wrapped_result.result->path);
}

}  // namespace smart_factory_mtsp_solver