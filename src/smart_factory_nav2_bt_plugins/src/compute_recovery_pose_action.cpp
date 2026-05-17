#include <algorithm>
#include <chrono>
#include <cmath>
#include <memory>
#include <string>

#include "behaviortree_cpp_v3/action_node.h"
#include "behaviortree_cpp_v3/bt_factory.h"
#include "behaviortree_cpp_v3/condition_node.h"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav2_msgs/msg/costmap.hpp"
#include "nav2_msgs/srv/get_costmap.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "tf2/utils.h"
#include "tf2_ros/buffer.h"

namespace smart_factory_nav2_bt_plugins
{

class ComputeRecoveryPoseAction : public BT::SyncActionNode
{
public:
  ComputeRecoveryPoseAction(
    const std::string & xml_tag_name,
    const BT::NodeConfiguration & conf)
  : BT::SyncActionNode(xml_tag_name, conf)
  {
    node_ = config().blackboard->get<rclcpp::Node::SharedPtr>("node");
    tf_buffer_ = config().blackboard->get<std::shared_ptr<tf2_ros::Buffer>>("tf_buffer");

    node_->get_parameter_or("global_frame", global_frame_, std::string("map"));
    node_->get_parameter_or("robot_base_frame", robot_base_frame_, std::string("base_link"));
    node_->get_parameter_or("transform_tolerance", transform_tolerance_, 0.1);

    std::string service_name;
    getInput("costmap_service", service_name);
    costmap_client_ = node_->create_client<nav2_msgs::srv::GetCostmap>(service_name);
  }

  static BT::PortsList providedPorts()
  {
    return {
      BT::OutputPort<geometry_msgs::msg::PoseStamped>(
        "pose", "Computed temporary recovery pose"),
      BT::InputPort<double>(
        "distance", 0.35, "Distance behind the robot for the temporary pose"),
      BT::InputPort<double>(
        "min_distance", 0.15, "Minimum acceptable recovery distance"),
      BT::InputPort<double>(
        "check_radius", 0.18, "Radius around candidate pose checked in the costmap"),
      BT::InputPort<int>(
        "max_cost", 252, "Maximum allowed costmap value. Unknown/lethal cells are rejected."),
      BT::InputPort<std::string>(
        "costmap_service", "global_costmap/get_costmap", "Costmap service used for bounds checks"),
      BT::InputPort<int>(
        "server_timeout_ms", 200, "Costmap service timeout in milliseconds"),
    };
  }

  BT::NodeStatus tick() override
  {
    double distance;
    double min_distance;
    double check_radius;
    int max_cost;
    int server_timeout_ms;
    getInput("distance", distance);
    getInput("min_distance", min_distance);
    getInput("check_radius", check_radius);
    getInput("max_cost", max_cost);
    getInput("server_timeout_ms", server_timeout_ms);
    const auto server_timeout = std::chrono::milliseconds(server_timeout_ms);

    if (distance <= 0.0) {
      RCLCPP_WARN(node_->get_logger(), "Recovery pose distance must be positive");
      return BT::NodeStatus::FAILURE;
    }

    geometry_msgs::msg::TransformStamped robot_tf;
    try {
      robot_tf = tf_buffer_->lookupTransform(
        global_frame_, robot_base_frame_, tf2::TimePointZero,
        tf2::durationFromSec(transform_tolerance_));
    } catch (const tf2::TransformException & ex) {
      RCLCPP_WARN(node_->get_logger(), "Could not compute recovery pose TF: %s", ex.what());
      return BT::NodeStatus::FAILURE;
    }

    auto costmap = getCostmap(server_timeout);
    if (costmap == nullptr) {
      return BT::NodeStatus::FAILURE;
    }

    const double yaw = tf2::getYaw(robot_tf.transform.rotation);
    const double start_x = robot_tf.transform.translation.x;
    const double start_y = robot_tf.transform.translation.y;
    const double step = std::max<double>(costmap->metadata.resolution, 0.05);

    for (double candidate_distance = distance;
      candidate_distance >= min_distance;
      candidate_distance -= step)
    {
      geometry_msgs::msg::PoseStamped pose;
      pose.header.stamp = node_->now();
      pose.header.frame_id = global_frame_;
      pose.pose.position.x = start_x - std::cos(yaw) * candidate_distance;
      pose.pose.position.y = start_y - std::sin(yaw) * candidate_distance;
      pose.pose.position.z = 0.0;
      pose.pose.orientation = robot_tf.transform.rotation;

      if (isPoseValid(*costmap, pose, check_radius, max_cost)) {
        setOutput("pose", pose);
        RCLCPP_INFO(
          node_->get_logger(),
          "Computed recovery pose %.2f m behind robot at (%.2f, %.2f)",
          candidate_distance, pose.pose.position.x, pose.pose.position.y);
        return BT::NodeStatus::SUCCESS;
      }
    }

    RCLCPP_WARN(node_->get_logger(), "No valid recovery pose found behind robot");
    return BT::NodeStatus::FAILURE;
  }

private:
  std::shared_ptr<nav2_msgs::msg::Costmap> getCostmap(std::chrono::milliseconds timeout)
  {
    if (!costmap_client_->wait_for_service(timeout)) {
      RCLCPP_WARN(node_->get_logger(), "Costmap service is not available");
      return nullptr;
    }

    auto request = std::make_shared<nav2_msgs::srv::GetCostmap::Request>();
    auto future = costmap_client_->async_send_request(request);
    if (rclcpp::spin_until_future_complete(node_, future, timeout) !=
      rclcpp::FutureReturnCode::SUCCESS)
    {
      RCLCPP_WARN(node_->get_logger(), "Timed out waiting for costmap response");
      return nullptr;
    }

    return std::make_shared<nav2_msgs::msg::Costmap>(future.get()->map);
  }

  bool isPoseValid(
    const nav2_msgs::msg::Costmap & costmap,
    const geometry_msgs::msg::PoseStamped & pose,
    double check_radius,
    int max_cost) const
  {
    if (costmap.metadata.resolution <= 0.0 ||
      costmap.metadata.size_x == 0 ||
      costmap.metadata.size_y == 0)
    {
      return false;
    }

    const double origin_x = costmap.metadata.origin.position.x;
    const double origin_y = costmap.metadata.origin.position.y;
    const int mx = static_cast<int>(std::floor(
        (pose.pose.position.x - origin_x) / costmap.metadata.resolution));
    const int my = static_cast<int>(std::floor(
        (pose.pose.position.y - origin_y) / costmap.metadata.resolution));

    if (mx < 0 || my < 0 ||
      mx >= static_cast<int>(costmap.metadata.size_x) ||
      my >= static_cast<int>(costmap.metadata.size_y))
    {
      return false;
    }

    const int radius_cells = std::max(
      0, static_cast<int>(std::ceil(check_radius / costmap.metadata.resolution)));
    const int clamped_max_cost = std::clamp(max_cost, 0, 255);

    for (int y = std::max(0, my - radius_cells);
      y <= std::min(static_cast<int>(costmap.metadata.size_y) - 1, my + radius_cells);
      ++y)
    {
      for (int x = std::max(0, mx - radius_cells);
        x <= std::min(static_cast<int>(costmap.metadata.size_x) - 1, mx + radius_cells);
        ++x)
      {
        const int dx = x - mx;
        const int dy = y - my;
        if (dx * dx + dy * dy > radius_cells * radius_cells) {
          continue;
        }
        const auto index = static_cast<std::size_t>(y) * costmap.metadata.size_x + x;
        if (index >= costmap.data.size()) {
          return false;
        }
        const int cost = static_cast<int>(costmap.data[index]);
        if (cost >= clamped_max_cost || cost == 255) {
          return false;
        }
      }
    }

    return true;
  }

  rclcpp::Node::SharedPtr node_;
  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  rclcpp::Client<nav2_msgs::srv::GetCostmap>::SharedPtr costmap_client_;
  std::string global_frame_;
  std::string robot_base_frame_;
  double transform_tolerance_;
};

class IsRobotTooCloseCondition : public BT::ConditionNode
{
public:
  IsRobotTooCloseCondition(
    const std::string & xml_tag_name,
    const BT::NodeConfiguration & conf)
  : BT::ConditionNode(xml_tag_name, conf)
  {
    node_ = config().blackboard->get<rclcpp::Node::SharedPtr>("node");
    tf_buffer_ = config().blackboard->get<std::shared_ptr<tf2_ros::Buffer>>("tf_buffer");

    node_->get_parameter_or("global_frame", global_frame_, std::string("map"));
    node_->get_parameter_or("robot_base_frame", robot_base_frame_, std::string("base_link"));
    node_->get_parameter_or("transform_tolerance", transform_tolerance_, 0.1);
  }

  static BT::PortsList providedPorts()
  {
    return {
      BT::InputPort<double>(
        "distance", 0.55, "Distance at which this robot should yield to another robot"),
      BT::InputPort<std::string>(
        "other_base_frame", "", "Other robot base frame. Empty derives tb1/tb2 pair from namespace."),
    };
  }

  BT::NodeStatus tick() override
  {
    double distance;
    std::string other_base_frame;
    getInput("distance", distance);
    getInput("other_base_frame", other_base_frame);

    if (other_base_frame.empty()) {
      other_base_frame = deriveOtherBaseFrame();
    }

    if (other_base_frame.empty() || distance <= 0.0) {
      return BT::NodeStatus::FAILURE;
    }

    geometry_msgs::msg::TransformStamped own_tf;
    geometry_msgs::msg::TransformStamped other_tf;
    try {
      own_tf = tf_buffer_->lookupTransform(
        global_frame_, robot_base_frame_, tf2::TimePointZero,
        tf2::durationFromSec(transform_tolerance_));
      other_tf = tf_buffer_->lookupTransform(
        global_frame_, other_base_frame, tf2::TimePointZero,
        tf2::durationFromSec(transform_tolerance_));
    } catch (const tf2::TransformException & ex) {
      RCLCPP_DEBUG(
        node_->get_logger(), "Could not check robot separation: %s", ex.what());
      return BT::NodeStatus::FAILURE;
    }

    const double dx = own_tf.transform.translation.x - other_tf.transform.translation.x;
    const double dy = own_tf.transform.translation.y - other_tf.transform.translation.y;
    const double separation = std::hypot(dx, dy);
    if (separation <= distance) {
      RCLCPP_INFO_THROTTLE(
        node_->get_logger(), *node_->get_clock(), 2000,
        "Yielding: robot %s is %.2f m away",
        other_base_frame.c_str(), separation);
      return BT::NodeStatus::SUCCESS;
    }

    return BT::NodeStatus::FAILURE;
  }

private:
  std::string deriveOtherBaseFrame() const
  {
    std::string ns = node_->get_namespace();
    if (!ns.empty() && ns.front() == '/') {
      ns.erase(0, 1);
    }

    if (ns == "tb1") {
      return "tb2/base_footprint";
    }
    if (ns == "tb2") {
      return "tb1/base_footprint";
    }

    return "";
  }

  rclcpp::Node::SharedPtr node_;
  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  std::string global_frame_;
  std::string robot_base_frame_;
  double transform_tolerance_;
};

}  // namespace smart_factory_nav2_bt_plugins

BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<smart_factory_nav2_bt_plugins::ComputeRecoveryPoseAction>(
    "ComputeRecoveryPose");
  factory.registerNodeType<smart_factory_nav2_bt_plugins::IsRobotTooCloseCondition>(
    "IsRobotTooClose");
}
