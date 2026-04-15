// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from smart_factory_fleet_msgs:srv/AddTask.idl
// generated code does not contain a copyright notice

#ifndef SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__ADD_TASK__STRUCT_HPP_
#define SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__ADD_TASK__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'pickup_pose'
// Member 'dropoff_pose'
#include "geometry_msgs/msg/detail/pose_stamped__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__smart_factory_fleet_msgs__srv__AddTask_Request __attribute__((deprecated))
#else
# define DEPRECATED__smart_factory_fleet_msgs__srv__AddTask_Request __declspec(deprecated)
#endif

namespace smart_factory_fleet_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct AddTask_Request_
{
  using Type = AddTask_Request_<ContainerAllocator>;

  explicit AddTask_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : pickup_pose(_init),
    dropoff_pose(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->type = "";
      this->priority = 0l;
    }
  }

  explicit AddTask_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : type(_alloc),
    pickup_pose(_alloc, _init),
    dropoff_pose(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->type = "";
      this->priority = 0l;
    }
  }

  // field types and members
  using _type_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _type_type type;
  using _priority_type =
    int32_t;
  _priority_type priority;
  using _pickup_pose_type =
    geometry_msgs::msg::PoseStamped_<ContainerAllocator>;
  _pickup_pose_type pickup_pose;
  using _dropoff_pose_type =
    geometry_msgs::msg::PoseStamped_<ContainerAllocator>;
  _dropoff_pose_type dropoff_pose;

  // setters for named parameter idiom
  Type & set__type(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->type = _arg;
    return *this;
  }
  Type & set__priority(
    const int32_t & _arg)
  {
    this->priority = _arg;
    return *this;
  }
  Type & set__pickup_pose(
    const geometry_msgs::msg::PoseStamped_<ContainerAllocator> & _arg)
  {
    this->pickup_pose = _arg;
    return *this;
  }
  Type & set__dropoff_pose(
    const geometry_msgs::msg::PoseStamped_<ContainerAllocator> & _arg)
  {
    this->dropoff_pose = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    smart_factory_fleet_msgs::srv::AddTask_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const smart_factory_fleet_msgs::srv::AddTask_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<smart_factory_fleet_msgs::srv::AddTask_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<smart_factory_fleet_msgs::srv::AddTask_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      smart_factory_fleet_msgs::srv::AddTask_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<smart_factory_fleet_msgs::srv::AddTask_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      smart_factory_fleet_msgs::srv::AddTask_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<smart_factory_fleet_msgs::srv::AddTask_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<smart_factory_fleet_msgs::srv::AddTask_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<smart_factory_fleet_msgs::srv::AddTask_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__smart_factory_fleet_msgs__srv__AddTask_Request
    std::shared_ptr<smart_factory_fleet_msgs::srv::AddTask_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__smart_factory_fleet_msgs__srv__AddTask_Request
    std::shared_ptr<smart_factory_fleet_msgs::srv::AddTask_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const AddTask_Request_ & other) const
  {
    if (this->type != other.type) {
      return false;
    }
    if (this->priority != other.priority) {
      return false;
    }
    if (this->pickup_pose != other.pickup_pose) {
      return false;
    }
    if (this->dropoff_pose != other.dropoff_pose) {
      return false;
    }
    return true;
  }
  bool operator!=(const AddTask_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct AddTask_Request_

// alias to use template instance with default allocator
using AddTask_Request =
  smart_factory_fleet_msgs::srv::AddTask_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace smart_factory_fleet_msgs


#ifndef _WIN32
# define DEPRECATED__smart_factory_fleet_msgs__srv__AddTask_Response __attribute__((deprecated))
#else
# define DEPRECATED__smart_factory_fleet_msgs__srv__AddTask_Response __declspec(deprecated)
#endif

namespace smart_factory_fleet_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct AddTask_Response_
{
  using Type = AddTask_Response_<ContainerAllocator>;

  explicit AddTask_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->accepted = false;
      this->task_id = "";
      this->message = "";
    }
  }

  explicit AddTask_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : task_id(_alloc),
    message(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->accepted = false;
      this->task_id = "";
      this->message = "";
    }
  }

  // field types and members
  using _accepted_type =
    bool;
  _accepted_type accepted;
  using _task_id_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _task_id_type task_id;
  using _message_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _message_type message;

  // setters for named parameter idiom
  Type & set__accepted(
    const bool & _arg)
  {
    this->accepted = _arg;
    return *this;
  }
  Type & set__task_id(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->task_id = _arg;
    return *this;
  }
  Type & set__message(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->message = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    smart_factory_fleet_msgs::srv::AddTask_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const smart_factory_fleet_msgs::srv::AddTask_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<smart_factory_fleet_msgs::srv::AddTask_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<smart_factory_fleet_msgs::srv::AddTask_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      smart_factory_fleet_msgs::srv::AddTask_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<smart_factory_fleet_msgs::srv::AddTask_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      smart_factory_fleet_msgs::srv::AddTask_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<smart_factory_fleet_msgs::srv::AddTask_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<smart_factory_fleet_msgs::srv::AddTask_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<smart_factory_fleet_msgs::srv::AddTask_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__smart_factory_fleet_msgs__srv__AddTask_Response
    std::shared_ptr<smart_factory_fleet_msgs::srv::AddTask_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__smart_factory_fleet_msgs__srv__AddTask_Response
    std::shared_ptr<smart_factory_fleet_msgs::srv::AddTask_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const AddTask_Response_ & other) const
  {
    if (this->accepted != other.accepted) {
      return false;
    }
    if (this->task_id != other.task_id) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    return true;
  }
  bool operator!=(const AddTask_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct AddTask_Response_

// alias to use template instance with default allocator
using AddTask_Response =
  smart_factory_fleet_msgs::srv::AddTask_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace smart_factory_fleet_msgs

namespace smart_factory_fleet_msgs
{

namespace srv
{

struct AddTask
{
  using Request = smart_factory_fleet_msgs::srv::AddTask_Request;
  using Response = smart_factory_fleet_msgs::srv::AddTask_Response;
};

}  // namespace srv

}  // namespace smart_factory_fleet_msgs

#endif  // SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__ADD_TASK__STRUCT_HPP_
