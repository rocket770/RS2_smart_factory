// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from smart_factory_fleet_msgs:msg/Task.idl
// generated code does not contain a copyright notice

#ifndef SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__TASK__STRUCT_H_
#define SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__TASK__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__struct.h"
// Member 'task_id'
// Member 'type'
// Member 'assigned_robot'
// Member 'status'
#include "rosidl_runtime_c/string.h"
// Member 'pickup_pose'
// Member 'dropoff_pose'
#include "geometry_msgs/msg/detail/pose_stamped__struct.h"

/// Struct defined in msg/Task in the package smart_factory_fleet_msgs.
typedef struct smart_factory_fleet_msgs__msg__Task
{
  builtin_interfaces__msg__Time stamp;
  rosidl_runtime_c__String task_id;
  rosidl_runtime_c__String type;
  int32_t priority;
  geometry_msgs__msg__PoseStamped pickup_pose;
  geometry_msgs__msg__PoseStamped dropoff_pose;
  rosidl_runtime_c__String assigned_robot;
  rosidl_runtime_c__String status;
} smart_factory_fleet_msgs__msg__Task;

// Struct for a sequence of smart_factory_fleet_msgs__msg__Task.
typedef struct smart_factory_fleet_msgs__msg__Task__Sequence
{
  smart_factory_fleet_msgs__msg__Task * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} smart_factory_fleet_msgs__msg__Task__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__TASK__STRUCT_H_
