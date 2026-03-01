// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from smart_factory_fleet_msgs:srv/AddTask.idl
// generated code does not contain a copyright notice

#ifndef SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__ADD_TASK__BUILDER_HPP_
#define SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__ADD_TASK__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "smart_factory_fleet_msgs/srv/detail/add_task__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace smart_factory_fleet_msgs
{

namespace srv
{

namespace builder
{

class Init_AddTask_Request_dropoff_pose
{
public:
  explicit Init_AddTask_Request_dropoff_pose(::smart_factory_fleet_msgs::srv::AddTask_Request & msg)
  : msg_(msg)
  {}
  ::smart_factory_fleet_msgs::srv::AddTask_Request dropoff_pose(::smart_factory_fleet_msgs::srv::AddTask_Request::_dropoff_pose_type arg)
  {
    msg_.dropoff_pose = std::move(arg);
    return std::move(msg_);
  }

private:
  ::smart_factory_fleet_msgs::srv::AddTask_Request msg_;
};

class Init_AddTask_Request_pickup_pose
{
public:
  explicit Init_AddTask_Request_pickup_pose(::smart_factory_fleet_msgs::srv::AddTask_Request & msg)
  : msg_(msg)
  {}
  Init_AddTask_Request_dropoff_pose pickup_pose(::smart_factory_fleet_msgs::srv::AddTask_Request::_pickup_pose_type arg)
  {
    msg_.pickup_pose = std::move(arg);
    return Init_AddTask_Request_dropoff_pose(msg_);
  }

private:
  ::smart_factory_fleet_msgs::srv::AddTask_Request msg_;
};

class Init_AddTask_Request_priority
{
public:
  explicit Init_AddTask_Request_priority(::smart_factory_fleet_msgs::srv::AddTask_Request & msg)
  : msg_(msg)
  {}
  Init_AddTask_Request_pickup_pose priority(::smart_factory_fleet_msgs::srv::AddTask_Request::_priority_type arg)
  {
    msg_.priority = std::move(arg);
    return Init_AddTask_Request_pickup_pose(msg_);
  }

private:
  ::smart_factory_fleet_msgs::srv::AddTask_Request msg_;
};

class Init_AddTask_Request_type
{
public:
  Init_AddTask_Request_type()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_AddTask_Request_priority type(::smart_factory_fleet_msgs::srv::AddTask_Request::_type_type arg)
  {
    msg_.type = std::move(arg);
    return Init_AddTask_Request_priority(msg_);
  }

private:
  ::smart_factory_fleet_msgs::srv::AddTask_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::smart_factory_fleet_msgs::srv::AddTask_Request>()
{
  return smart_factory_fleet_msgs::srv::builder::Init_AddTask_Request_type();
}

}  // namespace smart_factory_fleet_msgs


namespace smart_factory_fleet_msgs
{

namespace srv
{

namespace builder
{

class Init_AddTask_Response_message
{
public:
  explicit Init_AddTask_Response_message(::smart_factory_fleet_msgs::srv::AddTask_Response & msg)
  : msg_(msg)
  {}
  ::smart_factory_fleet_msgs::srv::AddTask_Response message(::smart_factory_fleet_msgs::srv::AddTask_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::smart_factory_fleet_msgs::srv::AddTask_Response msg_;
};

class Init_AddTask_Response_task_id
{
public:
  explicit Init_AddTask_Response_task_id(::smart_factory_fleet_msgs::srv::AddTask_Response & msg)
  : msg_(msg)
  {}
  Init_AddTask_Response_message task_id(::smart_factory_fleet_msgs::srv::AddTask_Response::_task_id_type arg)
  {
    msg_.task_id = std::move(arg);
    return Init_AddTask_Response_message(msg_);
  }

private:
  ::smart_factory_fleet_msgs::srv::AddTask_Response msg_;
};

class Init_AddTask_Response_accepted
{
public:
  Init_AddTask_Response_accepted()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_AddTask_Response_task_id accepted(::smart_factory_fleet_msgs::srv::AddTask_Response::_accepted_type arg)
  {
    msg_.accepted = std::move(arg);
    return Init_AddTask_Response_task_id(msg_);
  }

private:
  ::smart_factory_fleet_msgs::srv::AddTask_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::smart_factory_fleet_msgs::srv::AddTask_Response>()
{
  return smart_factory_fleet_msgs::srv::builder::Init_AddTask_Response_accepted();
}

}  // namespace smart_factory_fleet_msgs

#endif  // SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__ADD_TASK__BUILDER_HPP_
