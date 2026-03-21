#ifndef SMART_FACTORY_MTSP_SOLVER__NAV2_PATH_COST_PROVIDER_HPP_
#define SMART_FACTORY_MTSP_SOLVER__NAV2_PATH_COST_PROVIDER_HPP_

#include "path_cost_provider.hpp"

#include <chrono>
#include <limits>
#include <memory>
#include <string>
#include <mutex>

#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav2_msgs/action/compute_path_to_pose.hpp"
#include "nav_msgs/msg/path.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

namespace smart_factory_mtsp_solver
{

class Nav2PathCostProvider : public PathCostProvider
{
public:
  using ComputePathToPose = nav2_msgs::action::ComputePathToPose;
  using GoalHandleComputePathToPose = rclcpp_action::ClientGoalHandle<ComputePathToPose>;

  explicit Nav2PathCostProvider(
    const rclcpp::Node::SharedPtr & node,
    const std::string & action_name = "/tb2/compute_path_to_pose",
    const std::string & global_frame = "map",
    const std::string & planner_id = "",
    std::chrono::milliseconds server_timeout = std::chrono::milliseconds(5000),
    std::chrono::milliseconds result_timeout = std::chrono::milliseconds(10000));

  bool wait_until_ready();
  double compute_cost(const Point2D & start, const Point2D & goal) override;

private:
  geometry_msgs::msg::PoseStamped make_pose(const Point2D & pt) const;
  double compute_path_length(const nav_msgs::msg::Path & path) const;

  rclcpp::Node::SharedPtr node_;
  std::string action_name_;
  std::string global_frame_;
  std::string planner_id_;
  std::chrono::milliseconds server_timeout_;
  std::chrono::milliseconds result_timeout_;

  std::mutex request_mutex_;

  rclcpp_action::Client<ComputePathToPose>::SharedPtr client_;
};

}  // namespace smart_factory_mtsp_solver

#endif  // SMART_FACTORY_MTSP_SOLVER__NAV2_PATH_COST_PROVIDER_HPP_