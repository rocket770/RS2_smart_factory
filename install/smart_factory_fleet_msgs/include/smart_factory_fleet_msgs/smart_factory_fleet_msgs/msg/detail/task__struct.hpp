// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from smart_factory_fleet_msgs:msg/Task.idl
// generated code does not contain a copyright notice

#ifndef SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__TASK__STRUCT_HPP_
#define SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__TASK__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__struct.hpp"
// Member 'pickup_pose'
// Member 'dropoff_pose'
#include "geometry_msgs/msg/detail/pose_stamped__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__smart_factory_fleet_msgs__msg__Task __attribute__((deprecated))
#else
# define DEPRECATED__smart_factory_fleet_msgs__msg__Task __declspec(deprecated)
#endif

namespace smart_factory_fleet_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Task_
{
  using Type = Task_<ContainerAllocator>;

  explicit Task_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : stamp(_init),
    pickup_pose(_init),
    dropoff_pose(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->task_id = "";
      this->type = "";
      this->priority = 0l;
      this->assigned_robot = "";
      this->status = "";
    }
  }

  explicit Task_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : stamp(_alloc, _init),
    task_id(_alloc),
    type(_alloc),
    pickup_pose(_alloc, _init),
    dropoff_pose(_alloc, _init),
    assigned_robot(_alloc),
    status(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->task_id = "";
      this->type = "";
      this->priority = 0l;
      this->assigned_robot = "";
      this->status = "";
    }
  }

  // field types and members
  using _stamp_type =
    builtin_interfaces::msg::Time_<ContainerAllocator>;
  _stamp_type stamp;
  using _task_id_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _task_id_type task_id;
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
  using _assigned_robot_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _assigned_robot_type assigned_robot;
  using _status_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _status_type status;

  // setters for named parameter idiom
  Type & set__stamp(
    const builtin_interfaces::msg::Time_<ContainerAllocator> & _arg)
  {
    this->stamp = _arg;
    return *this;
  }
  Type & set__task_id(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->task_id = _arg;
    return *this;
  }
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
  Type & set__assigned_robot(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->assigned_robot = _arg;
    return *this;
  }
  Type & set__status(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->status = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    smart_factory_fleet_msgs::msg::Task_<ContainerAllocator> *;
  using ConstRawPtr =
    const smart_factory_fleet_msgs::msg::Task_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<smart_factory_fleet_msgs::msg::Task_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<smart_factory_fleet_msgs::msg::Task_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      smart_factory_fleet_msgs::msg::Task_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<smart_factory_fleet_msgs::msg::Task_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      smart_factory_fleet_msgs::msg::Task_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<smart_factory_fleet_msgs::msg::Task_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<smart_factory_fleet_msgs::msg::Task_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<smart_factory_fleet_msgs::msg::Task_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__smart_factory_fleet_msgs__msg__Task
    std::shared_ptr<smart_factory_fleet_msgs::msg::Task_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__smart_factory_fleet_msgs__msg__Task
    std::shared_ptr<smart_factory_fleet_msgs::msg::Task_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Task_ & other) const
  {
    if (this->stamp != other.stamp) {
      return false;
    }
    if (this->task_id != other.task_id) {
      return false;
    }
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
    if (this->assigned_robot != other.assigned_robot) {
      return false;
    }
    if (this->status != other.status) {
      return false;
    }
    return true;
  }
  bool operator!=(const Task_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Task_

// alias to use template instance with default allocator
using Task =
  smart_factory_fleet_msgs::msg::Task_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace smart_factory_fleet_msgs

#endif  // SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__TASK__STRUCT_HPP_
