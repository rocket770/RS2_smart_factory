// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from smart_factory_fleet_msgs:msg/RobotState.idl
// generated code does not contain a copyright notice

#ifndef SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__ROBOT_STATE__STRUCT_H_
#define SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__ROBOT_STATE__STRUCT_H_

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
// Member 'robot_name'
// Member 'status'
// Member 'current_task_id'
#include "rosidl_runtime_c/string.h"
// Member 'pose'
#include "geometry_msgs/msg/detail/pose_stamped__struct.h"

/// Struct defined in msg/RobotState in the package smart_factory_fleet_msgs.
typedef struct smart_factory_fleet_msgs__msg__RobotState
{
  builtin_interfaces__msg__Time stamp;
  rosidl_runtime_c__String robot_name;
  rosidl_runtime_c__String status;
  geometry_msgs__msg__PoseStamped pose;
  float battery;
  rosidl_runtime_c__String current_task_id;
} smart_factory_fleet_msgs__msg__RobotState;

// Struct for a sequence of smart_factory_fleet_msgs__msg__RobotState.
typedef struct smart_factory_fleet_msgs__msg__RobotState__Sequence
{
  smart_factory_fleet_msgs__msg__RobotState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} smart_factory_fleet_msgs__msg__RobotState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SMART_FACTORY_FLEET_MSGS__MSG__DETAIL__ROBOT_STATE__STRUCT_H_
