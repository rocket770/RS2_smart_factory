// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from smart_factory_fleet_msgs:srv/GetFleetState.idl
// generated code does not contain a copyright notice

#ifndef SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__GET_FLEET_STATE__STRUCT_HPP_
#define SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__GET_FLEET_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__smart_factory_fleet_msgs__srv__GetFleetState_Request __attribute__((deprecated))
#else
# define DEPRECATED__smart_factory_fleet_msgs__srv__GetFleetState_Request __declspec(deprecated)
#endif

namespace smart_factory_fleet_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetFleetState_Request_
{
  using Type = GetFleetState_Request_<ContainerAllocator>;

  explicit GetFleetState_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->structure_needs_at_least_one_member = 0;
    }
  }

  explicit GetFleetState_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->structure_needs_at_least_one_member = 0;
    }
  }

  // field types and members
  using _structure_needs_at_least_one_member_type =
    uint8_t;
  _structure_needs_at_least_one_member_type structure_needs_at_least_one_member;


  // constant declarations

  // pointer types
  using RawPtr =
    smart_factory_fleet_msgs::srv::GetFleetState_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const smart_factory_fleet_msgs::srv::GetFleetState_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      smart_factory_fleet_msgs::srv::GetFleetState_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      smart_factory_fleet_msgs::srv::GetFleetState_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__smart_factory_fleet_msgs__srv__GetFleetState_Request
    std::shared_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__smart_factory_fleet_msgs__srv__GetFleetState_Request
    std::shared_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetFleetState_Request_ & other) const
  {
    if (this->structure_needs_at_least_one_member != other.structure_needs_at_least_one_member) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetFleetState_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetFleetState_Request_

// alias to use template instance with default allocator
using GetFleetState_Request =
  smart_factory_fleet_msgs::srv::GetFleetState_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace smart_factory_fleet_msgs


// Include directives for member types
// Member 'robots'
#include "smart_factory_fleet_msgs/msg/detail/robot_state__struct.hpp"
// Member 'tasks'
#include "smart_factory_fleet_msgs/msg/detail/task__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__smart_factory_fleet_msgs__srv__GetFleetState_Response __attribute__((deprecated))
#else
# define DEPRECATED__smart_factory_fleet_msgs__srv__GetFleetState_Response __declspec(deprecated)
#endif

namespace smart_factory_fleet_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct GetFleetState_Response_
{
  using Type = GetFleetState_Response_<ContainerAllocator>;

  explicit GetFleetState_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
  }

  explicit GetFleetState_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
    (void)_alloc;
  }

  // field types and members
  using _robots_type =
    std::vector<smart_factory_fleet_msgs::msg::RobotState_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<smart_factory_fleet_msgs::msg::RobotState_<ContainerAllocator>>>;
  _robots_type robots;
  using _tasks_type =
    std::vector<smart_factory_fleet_msgs::msg::Task_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<smart_factory_fleet_msgs::msg::Task_<ContainerAllocator>>>;
  _tasks_type tasks;

  // setters for named parameter idiom
  Type & set__robots(
    const std::vector<smart_factory_fleet_msgs::msg::RobotState_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<smart_factory_fleet_msgs::msg::RobotState_<ContainerAllocator>>> & _arg)
  {
    this->robots = _arg;
    return *this;
  }
  Type & set__tasks(
    const std::vector<smart_factory_fleet_msgs::msg::Task_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<smart_factory_fleet_msgs::msg::Task_<ContainerAllocator>>> & _arg)
  {
    this->tasks = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    smart_factory_fleet_msgs::srv::GetFleetState_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const smart_factory_fleet_msgs::srv::GetFleetState_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      smart_factory_fleet_msgs::srv::GetFleetState_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      smart_factory_fleet_msgs::srv::GetFleetState_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__smart_factory_fleet_msgs__srv__GetFleetState_Response
    std::shared_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__smart_factory_fleet_msgs__srv__GetFleetState_Response
    std::shared_ptr<smart_factory_fleet_msgs::srv::GetFleetState_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GetFleetState_Response_ & other) const
  {
    if (this->robots != other.robots) {
      return false;
    }
    if (this->tasks != other.tasks) {
      return false;
    }
    return true;
  }
  bool operator!=(const GetFleetState_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GetFleetState_Response_

// alias to use template instance with default allocator
using GetFleetState_Response =
  smart_factory_fleet_msgs::srv::GetFleetState_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace smart_factory_fleet_msgs

namespace smart_factory_fleet_msgs
{

namespace srv
{

struct GetFleetState
{
  using Request = smart_factory_fleet_msgs::srv::GetFleetState_Request;
  using Response = smart_factory_fleet_msgs::srv::GetFleetState_Response;
};

}  // namespace srv

}  // namespace smart_factory_fleet_msgs

#endif  // SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__GET_FLEET_STATE__STRUCT_HPP_
