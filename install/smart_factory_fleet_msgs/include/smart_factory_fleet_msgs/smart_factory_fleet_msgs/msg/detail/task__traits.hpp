// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from smart_factory_fleet_msgs:msg/Task.idl
// generated code does not contain a copyright notice

#ifndef SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__TASK__TRAITS_HPP_
#define SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__TASK__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "smart_factory_fleet_msgs/msg/detail/task__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__traits.hpp"
// Member 'pickup_pose'
// Member 'dropoff_pose'
#include "geometry_msgs/msg/detail/pose_stamped__traits.hpp"

namespace smart_factory_fleet_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const Task & msg,
  std::ostream & out)
{
  out << "{";
  // member: stamp
  {
    out << "stamp: ";
    to_flow_style_yaml(msg.stamp, out);
    out << ", ";
  }

  // member: task_id
  {
    out << "task_id: ";
    rosidl_generator_traits::value_to_yaml(msg.task_id, out);
    out << ", ";
  }

  // member: type
  {
    out << "type: ";
    rosidl_generator_traits::value_to_yaml(msg.type, out);
    out << ", ";
  }

  // member: priority
  {
    out << "priority: ";
    rosidl_generator_traits::value_to_yaml(msg.priority, out);
    out << ", ";
  }

  // member: pickup_pose
  {
    out << "pickup_pose: ";
    to_flow_style_yaml(msg.pickup_pose, out);
    out << ", ";
  }

  // member: dropoff_pose
  {
    out << "dropoff_pose: ";
    to_flow_style_yaml(msg.dropoff_pose, out);
    out << ", ";
  }

  // member: assigned_robot
  {
    out << "assigned_robot: ";
    rosidl_generator_traits::value_to_yaml(msg.assigned_robot, out);
    out << ", ";
  }

  // member: status
  {
    out << "status: ";
    rosidl_generator_traits::value_to_yaml(msg.status, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Task & msg,
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

  // member: task_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "task_id: ";
    rosidl_generator_traits::value_to_yaml(msg.task_id, out);
    out << "\n";
  }

  // member: type
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "type: ";
    rosidl_generator_traits::value_to_yaml(msg.type, out);
    out << "\n";
  }

  // member: priority
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "priority: ";
    rosidl_generator_traits::value_to_yaml(msg.priority, out);
    out << "\n";
  }

  // member: pickup_pose
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "pickup_pose:\n";
    to_block_style_yaml(msg.pickup_pose, out, indentation + 2);
  }

  // member: dropoff_pose
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "dropoff_pose:\n";
    to_block_style_yaml(msg.dropoff_pose, out, indentation + 2);
  }

  // member: assigned_robot
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "assigned_robot: ";
    rosidl_generator_traits::value_to_yaml(msg.assigned_robot, out);
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
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Task & msg, bool use_flow_style = false)
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
  const smart_factory_fleet_msgs::msg::Task & msg,
  std::ostream & out, size_t indentation = 0)
{
  smart_factory_fleet_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use smart_factory_fleet_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const smart_factory_fleet_msgs::msg::Task & msg)
{
  return smart_factory_fleet_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<smart_factory_fleet_msgs::msg::Task>()
{
  return "smart_factory_fleet_msgs::msg::Task";
}

template<>
inline const char * name<smart_factory_fleet_msgs::msg::Task>()
{
  return "smart_factory_fleet_msgs/msg/Task";
}

template<>
struct has_fixed_size<smart_factory_fleet_msgs::msg::Task>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<smart_factory_fleet_msgs::msg::Task>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<smart_factory_fleet_msgs::msg::Task>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__TASK__TRAITS_HPP_
