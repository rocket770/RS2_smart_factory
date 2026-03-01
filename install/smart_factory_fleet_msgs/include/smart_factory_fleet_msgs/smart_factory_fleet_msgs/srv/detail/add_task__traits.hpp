// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from smart_factory_fleet_msgs:srv/AddTask.idl
// generated code does not contain a copyright notice

#ifndef SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__ADD_TASK__TRAITS_HPP_
#define SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__ADD_TASK__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "smart_factory_fleet_msgs/srv/detail/add_task__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'pickup_pose'
// Member 'dropoff_pose'
#include "geometry_msgs/msg/detail/pose_stamped__traits.hpp"

namespace smart_factory_fleet_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const AddTask_Request & msg,
  std::ostream & out)
{
  out << "{";
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
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const AddTask_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
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
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const AddTask_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace smart_factory_fleet_msgs

namespace rosidl_generator_traits
{

[[deprecated("use smart_factory_fleet_msgs::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const smart_factory_fleet_msgs::srv::AddTask_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  smart_factory_fleet_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use smart_factory_fleet_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const smart_factory_fleet_msgs::srv::AddTask_Request & msg)
{
  return smart_factory_fleet_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<smart_factory_fleet_msgs::srv::AddTask_Request>()
{
  return "smart_factory_fleet_msgs::srv::AddTask_Request";
}

template<>
inline const char * name<smart_factory_fleet_msgs::srv::AddTask_Request>()
{
  return "smart_factory_fleet_msgs/srv/AddTask_Request";
}

template<>
struct has_fixed_size<smart_factory_fleet_msgs::srv::AddTask_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<smart_factory_fleet_msgs::srv::AddTask_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<smart_factory_fleet_msgs::srv::AddTask_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace smart_factory_fleet_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const AddTask_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: accepted
  {
    out << "accepted: ";
    rosidl_generator_traits::value_to_yaml(msg.accepted, out);
    out << ", ";
  }

  // member: task_id
  {
    out << "task_id: ";
    rosidl_generator_traits::value_to_yaml(msg.task_id, out);
    out << ", ";
  }

  // member: message
  {
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const AddTask_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: accepted
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "accepted: ";
    rosidl_generator_traits::value_to_yaml(msg.accepted, out);
    out << "\n";
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

  // member: message
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const AddTask_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace smart_factory_fleet_msgs

namespace rosidl_generator_traits
{

[[deprecated("use smart_factory_fleet_msgs::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const smart_factory_fleet_msgs::srv::AddTask_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  smart_factory_fleet_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use smart_factory_fleet_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const smart_factory_fleet_msgs::srv::AddTask_Response & msg)
{
  return smart_factory_fleet_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<smart_factory_fleet_msgs::srv::AddTask_Response>()
{
  return "smart_factory_fleet_msgs::srv::AddTask_Response";
}

template<>
inline const char * name<smart_factory_fleet_msgs::srv::AddTask_Response>()
{
  return "smart_factory_fleet_msgs/srv/AddTask_Response";
}

template<>
struct has_fixed_size<smart_factory_fleet_msgs::srv::AddTask_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<smart_factory_fleet_msgs::srv::AddTask_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<smart_factory_fleet_msgs::srv::AddTask_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<smart_factory_fleet_msgs::srv::AddTask>()
{
  return "smart_factory_fleet_msgs::srv::AddTask";
}

template<>
inline const char * name<smart_factory_fleet_msgs::srv::AddTask>()
{
  return "smart_factory_fleet_msgs/srv/AddTask";
}

template<>
struct has_fixed_size<smart_factory_fleet_msgs::srv::AddTask>
  : std::integral_constant<
    bool,
    has_fixed_size<smart_factory_fleet_msgs::srv::AddTask_Request>::value &&
    has_fixed_size<smart_factory_fleet_msgs::srv::AddTask_Response>::value
  >
{
};

template<>
struct has_bounded_size<smart_factory_fleet_msgs::srv::AddTask>
  : std::integral_constant<
    bool,
    has_bounded_size<smart_factory_fleet_msgs::srv::AddTask_Request>::value &&
    has_bounded_size<smart_factory_fleet_msgs::srv::AddTask_Response>::value
  >
{
};

template<>
struct is_service<smart_factory_fleet_msgs::srv::AddTask>
  : std::true_type
{
};

template<>
struct is_service_request<smart_factory_fleet_msgs::srv::AddTask_Request>
  : std::true_type
{
};

template<>
struct is_service_response<smart_factory_fleet_msgs::srv::AddTask_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__ADD_TASK__TRAITS_HPP_
