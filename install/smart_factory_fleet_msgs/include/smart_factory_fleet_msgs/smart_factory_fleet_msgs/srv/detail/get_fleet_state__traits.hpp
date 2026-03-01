// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from smart_factory_fleet_msgs:srv/GetFleetState.idl
// generated code does not contain a copyright notice

#ifndef SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__GET_FLEET_STATE__TRAITS_HPP_
#define SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__GET_FLEET_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "smart_factory_fleet_msgs/srv/detail/get_fleet_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace smart_factory_fleet_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetFleetState_Request & msg,
  std::ostream & out)
{
  (void)msg;
  out << "null";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetFleetState_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  (void)msg;
  (void)indentation;
  out << "null\n";
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetFleetState_Request & msg, bool use_flow_style = false)
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
  const smart_factory_fleet_msgs::srv::GetFleetState_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  smart_factory_fleet_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use smart_factory_fleet_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const smart_factory_fleet_msgs::srv::GetFleetState_Request & msg)
{
  return smart_factory_fleet_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<smart_factory_fleet_msgs::srv::GetFleetState_Request>()
{
  return "smart_factory_fleet_msgs::srv::GetFleetState_Request";
}

template<>
inline const char * name<smart_factory_fleet_msgs::srv::GetFleetState_Request>()
{
  return "smart_factory_fleet_msgs/srv/GetFleetState_Request";
}

template<>
struct has_fixed_size<smart_factory_fleet_msgs::srv::GetFleetState_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<smart_factory_fleet_msgs::srv::GetFleetState_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<smart_factory_fleet_msgs::srv::GetFleetState_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

// Include directives for member types
// Member 'robots'
#include "smart_factory_fleet_msgs/msg/detail/robot_state__traits.hpp"
// Member 'tasks'
#include "smart_factory_fleet_msgs/msg/detail/task__traits.hpp"

namespace smart_factory_fleet_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetFleetState_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: robots
  {
    if (msg.robots.size() == 0) {
      out << "robots: []";
    } else {
      out << "robots: [";
      size_t pending_items = msg.robots.size();
      for (auto item : msg.robots) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: tasks
  {
    if (msg.tasks.size() == 0) {
      out << "tasks: []";
    } else {
      out << "tasks: [";
      size_t pending_items = msg.tasks.size();
      for (auto item : msg.tasks) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GetFleetState_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: robots
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.robots.size() == 0) {
      out << "robots: []\n";
    } else {
      out << "robots:\n";
      for (auto item : msg.robots) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: tasks
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.tasks.size() == 0) {
      out << "tasks: []\n";
    } else {
      out << "tasks:\n";
      for (auto item : msg.tasks) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GetFleetState_Response & msg, bool use_flow_style = false)
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
  const smart_factory_fleet_msgs::srv::GetFleetState_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  smart_factory_fleet_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use smart_factory_fleet_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const smart_factory_fleet_msgs::srv::GetFleetState_Response & msg)
{
  return smart_factory_fleet_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<smart_factory_fleet_msgs::srv::GetFleetState_Response>()
{
  return "smart_factory_fleet_msgs::srv::GetFleetState_Response";
}

template<>
inline const char * name<smart_factory_fleet_msgs::srv::GetFleetState_Response>()
{
  return "smart_factory_fleet_msgs/srv/GetFleetState_Response";
}

template<>
struct has_fixed_size<smart_factory_fleet_msgs::srv::GetFleetState_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<smart_factory_fleet_msgs::srv::GetFleetState_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<smart_factory_fleet_msgs::srv::GetFleetState_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<smart_factory_fleet_msgs::srv::GetFleetState>()
{
  return "smart_factory_fleet_msgs::srv::GetFleetState";
}

template<>
inline const char * name<smart_factory_fleet_msgs::srv::GetFleetState>()
{
  return "smart_factory_fleet_msgs/srv/GetFleetState";
}

template<>
struct has_fixed_size<smart_factory_fleet_msgs::srv::GetFleetState>
  : std::integral_constant<
    bool,
    has_fixed_size<smart_factory_fleet_msgs::srv::GetFleetState_Request>::value &&
    has_fixed_size<smart_factory_fleet_msgs::srv::GetFleetState_Response>::value
  >
{
};

template<>
struct has_bounded_size<smart_factory_fleet_msgs::srv::GetFleetState>
  : std::integral_constant<
    bool,
    has_bounded_size<smart_factory_fleet_msgs::srv::GetFleetState_Request>::value &&
    has_bounded_size<smart_factory_fleet_msgs::srv::GetFleetState_Response>::value
  >
{
};

template<>
struct is_service<smart_factory_fleet_msgs::srv::GetFleetState>
  : std::true_type
{
};

template<>
struct is_service_request<smart_factory_fleet_msgs::srv::GetFleetState_Request>
  : std::true_type
{
};

template<>
struct is_service_response<smart_factory_fleet_msgs::srv::GetFleetState_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__GET_FLEET_STATE__TRAITS_HPP_
