// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from smart_factory_fleet_msgs:msg/RobotState.idl
// generated code does not contain a copyright notice

#ifndef SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__ROBOT_STATE__TRAITS_HPP_
#define SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__ROBOT_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "smart_factory_fleet_msgs/msg/detail/robot_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__traits.hpp"
// Member 'pose'
#include "geometry_msgs/msg/detail/pose_stamped__traits.hpp"

namespace smart_factory_fleet_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const RobotState & msg,
  std::ostream & out)
{
  out << "{";
  // member: stamp
  {
    out << "stamp: ";
    to_flow_style_yaml(msg.stamp, out);
    out << ", ";
  }

  // member: robot_name
  {
    out << "robot_name: ";
    rosidl_generator_traits::value_to_yaml(msg.robot_name, out);
    out << ", ";
  }

  // member: status
  {
    out << "status: ";
    rosidl_generator_traits::value_to_yaml(msg.status, out);
    out << ", ";
  }

  // member: pose
  {
    out << "pose: ";
    to_flow_style_yaml(msg.pose, out);
    out << ", ";
  }

  // member: battery
  {
    out << "battery: ";
    rosidl_generator_traits::value_to_yaml(msg.battery, out);
    out << ", ";
  }

  // member: current_task_id
  {
    out << "current_task_id: ";
    rosidl_generator_traits::value_to_yaml(msg.current_task_id, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const RobotState & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: stamp
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "stamp:\n";
    to_block_style_yaml(msg.stamp, out, indentation + 2);
  }

  // member: robot_name
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "robot_name: ";
    rosidl_generator_traits::value_to_yaml(msg.robot_name, out);
    out << "\n";
  }

  // member: status
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "status: ";
    rosidl_generator_traits::value_to_yaml(msg.status, out);
    out << "\n";
  }

  // member: pose
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "pose:\n";
    to_block_style_yaml(msg.pose, out, indentation + 2);
  }

  // member: battery
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "battery: ";
    rosidl_generator_traits::value_to_yaml(msg.battery, out);
    out << "\n";
  }

  // member: current_task_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "current_task_id: ";
    rosidl_generator_traits::value_to_yaml(msg.current_task_id, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const RobotState & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace smart_factory_fleet_msgs

namespace rosidl_generator_traits
{

[[deprecated("use smart_factory_fleet_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const smart_factory_fleet_msgs::msg::RobotState & msg,
  std::ostream & out, size_t indentation = 0)
{
  smart_factory_fleet_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use smart_factory_fleet_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const smart_factory_fleet_msgs::msg::RobotState & msg)
{
  return smart_factory_fleet_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<smart_factory_fleet_msgs::msg::RobotState>()
{
  return "smart_factory_fleet_msgs::msg::RobotState";
}

template<>
inline const char * name<smart_factory_fleet_msgs::msg::RobotState>()
{
  return "smart_factory_fleet_msgs/msg/RobotState";
}

template<>
struct has_fixed_size<smart_factory_fleet_msgs::msg::RobotState>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<smart_factory_fleet_msgs::msg::RobotState>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<smart_factory_fleet_msgs::msg::RobotState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__ROBOT_STATE__TRAITS_HPP_
