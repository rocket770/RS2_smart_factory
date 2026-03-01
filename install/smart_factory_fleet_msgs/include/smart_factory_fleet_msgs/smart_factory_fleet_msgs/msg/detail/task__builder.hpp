// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from smart_factory_fleet_msgs:msg/Task.idl
// generated code does not contain a copyright notice

#ifndef SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__TASK__BUILDER_HPP_
#define SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__TASK__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "smart_factory_fleet_msgs/msg/detail/task__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace smart_factory_fleet_msgs
{

namespace msg
{

namespace builder
{

class Init_Task_status
{
public:
  explicit Init_Task_status(::smart_factory_fleet_msgs::msg::Task & msg)
  : msg_(msg)
  {}
  ::smart_factory_fleet_msgs::msg::Task status(::smart_factory_fleet_msgs::msg::Task::_status_type arg)
  {
    msg_.status = std::move(arg);
    return std::move(msg_);
  }

private:
  ::smart_factory_fleet_msgs::msg::Task msg_;
};

class Init_Task_assigned_robot
{
public:
  explicit Init_Task_assigned_robot(::smart_factory_fleet_msgs::msg::Task & msg)
  : msg_(msg)
  {}
  Init_Task_status assigned_robot(::smart_factory_fleet_msgs::msg::Task::_assigned_robot_type arg)
  {
    msg_.assigned_robot = std::move(arg);
    return Init_Task_status(msg_);
  }

private:
  ::smart_factory_fleet_msgs::msg::Task msg_;
};

class Init_Task_dropoff_pose
{
public:
  explicit Init_Task_dropoff_pose(::smart_factory_fleet_msgs::msg::Task & msg)
  : msg_(msg)
  {}
  Init_Task_assigned_robot dropoff_pose(::smart_factory_fleet_msgs::msg::Task::_dropoff_pose_type arg)
  {
    msg_.dropoff_pose = std::move(arg);
    return Init_Task_assigned_robot(msg_);
  }

private:
  ::smart_factory_fleet_msgs::msg::Task msg_;
};

class Init_Task_pickup_pose
{
public:
  explicit Init_Task_pickup_pose(::smart_factory_fleet_msgs::msg::Task & msg)
  : msg_(msg)
  {}
  Init_Task_dropoff_pose pickup_pose(::smart_factory_fleet_msgs::msg::Task::_pickup_pose_type arg)
  {
    msg_.pickup_pose = std::move(arg);
    return Init_Task_dropoff_pose(msg_);
  }

private:
  ::smart_factory_fleet_msgs::msg::Task msg_;
};

class Init_Task_priority
{
public:
  explicit Init_Task_priority(::smart_factory_fleet_msgs::msg::Task & msg)
  : msg_(msg)
  {}
  Init_Task_pickup_pose priority(::smart_factory_fleet_msgs::msg::Task::_priority_type arg)
  {
    msg_.priority = std::move(arg);
    return Init_Task_pickup_pose(msg_);
  }

private:
  ::smart_factory_fleet_msgs::msg::Task msg_;
};

class Init_Task_type
{
public:
  explicit Init_Task_type(::smart_factory_fleet_msgs::msg::Task & msg)
  : msg_(msg)
  {}
  Init_Task_priority type(::smart_factory_fleet_msgs::msg::Task::_type_type arg)
  {
    msg_.type = std::move(arg);
    return Init_Task_priority(msg_);
  }

private:
  ::smart_factory_fleet_msgs::msg::Task msg_;
};

class Init_Task_task_id
{
public:
  explicit Init_Task_task_id(::smart_factory_fleet_msgs::msg::Task & msg)
  : msg_(msg)
  {}
  Init_Task_type task_id(::smart_factory_fleet_msgs::msg::Task::_task_id_type arg)
  {
    msg_.task_id = std::move(arg);
    return Init_Task_type(msg_);
  }

private:
  ::smart_factory_fleet_msgs::msg::Task msg_;
};

class Init_Task_stamp
{
public:
  Init_Task_stamp()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Task_task_id stamp(::smart_factory_fleet_msgs::msg::Task::_stamp_type arg)
  {
    msg_.stamp = std::move(arg);
    return Init_Task_task_id(msg_);
  }

private:
  ::smart_factory_fleet_msgs::msg::Task msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::smart_factory_fleet_msgs::msg::Task>()
{
  return smart_factory_fleet_msgs::msg::builder::Init_Task_stamp();
}

}  // namespace smart_factory_fleet_msgs

#endif  // SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__TASK__BUILDER_HPP_
