// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from smart_factory_fleet_msgs:srv/GetFleetState.idl
// generated code does not contain a copyright notice

#ifndef SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__GET_FLEET_STATE__BUILDER_HPP_
#define SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__GET_FLEET_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "smart_factory_fleet_msgs/srv/detail/get_fleet_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace smart_factory_fleet_msgs
{

namespace srv
{


}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::smart_factory_fleet_msgs::srv::GetFleetState_Request>()
{
  return ::smart_factory_fleet_msgs::srv::GetFleetState_Request(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace smart_factory_fleet_msgs


namespace smart_factory_fleet_msgs
{

namespace srv
{

namespace builder
{

class Init_GetFleetState_Response_tasks
{
public:
  explicit Init_GetFleetState_Response_tasks(::smart_factory_fleet_msgs::srv::GetFleetState_Response & msg)
  : msg_(msg)
  {}
  ::smart_factory_fleet_msgs::srv::GetFleetState_Response tasks(::smart_factory_fleet_msgs::srv::GetFleetState_Response::_tasks_type arg)
  {
    msg_.tasks = std::move(arg);
    return std::move(msg_);
  }

private:
  ::smart_factory_fleet_msgs::srv::GetFleetState_Response msg_;
};

class Init_GetFleetState_Response_robots
{
public:
  Init_GetFleetState_Response_robots()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GetFleetState_Response_tasks robots(::smart_factory_fleet_msgs::srv::GetFleetState_Response::_robots_type arg)
  {
    msg_.robots = std::move(arg);
    return Init_GetFleetState_Response_tasks(msg_);
  }

private:
  ::smart_factory_fleet_msgs::srv::GetFleetState_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::smart_factory_fleet_msgs::srv::GetFleetState_Response>()
{
  return smart_factory_fleet_msgs::srv::builder::Init_GetFleetState_Response_robots();
}

}  // namespace smart_factory_fleet_msgs

#endif  // SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__GET_FLEET_STATE__BUILDER_HPP_
