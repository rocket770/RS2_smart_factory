// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from smart_factory_fleet_msgs:msg/RobotState.idl
// generated code does not contain a copyright notice

#ifndef SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__ROBOT_STATE__BUILDER_HPP_
#define SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__ROBOT_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "smart_factory_fleet_msgs/msg/detail/robot_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace smart_factory_fleet_msgs
{

namespace msg
{

namespace builder
{

class Init_RobotState_current_task_id
{
public:
  explicit Init_RobotState_current_task_id(::smart_factory_fleet_msgs::msg::RobotState & msg)
  : msg_(msg)
  {}
  ::smart_factory_fleet_msgs::msg::RobotState current_task_id(::smart_factory_fleet_msgs::msg::RobotState::_current_task_id_type arg)
  {
    msg_.current_task_id = std::move(arg);
    return std::move(msg_);
  }

private:
  ::smart_factory_fleet_msgs::msg::RobotState msg_;
};

class Init_RobotState_battery
{
public:
  explicit Init_RobotState_battery(::smart_factory_fleet_msgs::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_current_task_id battery(::smart_factory_fleet_msgs::msg::RobotState::_battery_type arg)
  {
    msg_.battery = std::move(arg);
    return Init_RobotState_current_task_id(msg_);
  }

private:
  ::smart_factory_fleet_msgs::msg::RobotState msg_;
};

class Init_RobotState_pose
{
public:
  explicit Init_RobotState_pose(::smart_factory_fleet_msgs::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_battery pose(::smart_factory_fleet_msgs::msg::RobotState::_pose_type arg)
  {
    msg_.pose = std::move(arg);
    return Init_RobotState_battery(msg_);
  }

private:
  ::smart_factory_fleet_msgs::msg::RobotState msg_;
};

class Init_RobotState_status
{
public:
  explicit Init_RobotState_status(::smart_factory_fleet_msgs::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_pose status(::smart_factory_fleet_msgs::msg::RobotState::_status_type arg)
  {
    msg_.status = std::move(arg);
    return Init_RobotState_pose(msg_);
  }

private:
  ::smart_factory_fleet_msgs::msg::RobotState msg_;
};

class Init_RobotState_robot_name
{
public:
  explicit Init_RobotState_robot_name(::smart_factory_fleet_msgs::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_status robot_name(::smart_factory_fleet_msgs::msg::RobotState::_robot_name_type arg)
  {
    msg_.robot_name = std::move(arg);
    return Init_RobotState_status(msg_);
  }

private:
  ::smart_factory_fleet_msgs::msg::RobotState msg_;
};

class Init_RobotState_stamp
{
public:
  Init_RobotState_stamp()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_RobotState_robot_name stamp(::smart_factory_fleet_msgs::msg::RobotState::_stamp_type arg)
  {
    msg_.stamp = std::move(arg);
    return Init_RobotState_robot_name(msg_);
  }

private:
  ::smart_factory_fleet_msgs::msg::RobotState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::smart_factory_fleet_msgs::msg::RobotState>()
{
  return smart_factory_fleet_msgs::msg::builder::Init_RobotState_stamp();
}

}  // namespace smart_factory_fleet_msgs

#endif  // SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__ROBOT_STATE__BUILDER_HPP_
