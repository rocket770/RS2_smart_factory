// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from smart_factory_fleet_msgs:srv/AddTask.idl
// generated code does not contain a copyright notice

#ifndef SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__ADD_TASK__STRUCT_H_
#define SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__ADD_TASK__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'type'
#include "rosidl_runtime_c/string.h"
// Member 'pickup_pose'
// Member 'dropoff_pose'
#include "geometry_msgs/msg/detail/pose_stamped__struct.h"

/// Struct defined in srv/AddTask in the package smart_factory_fleet_msgs.
typedef struct smart_factory_fleet_msgs__srv__AddTask_Request
{
  rosidl_runtime_c__String type;
  int32_t priority;
  geometry_msgs__msg__PoseStamped pickup_pose;
  geometry_msgs__msg__PoseStamped dropoff_pose;
} smart_factory_fleet_msgs__srv__AddTask_Request;

// Struct for a sequence of smart_factory_fleet_msgs__srv__AddTask_Request.
typedef struct smart_factory_fleet_msgs__srv__AddTask_Request__Sequence
{
  smart_factory_fleet_msgs__srv__AddTask_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} smart_factory_fleet_msgs__srv__AddTask_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'task_id'
// Member 'message'
// already included above
// #include "rosidl_runtime_c/string.h"

/// Struct defined in srv/AddTask in the package smart_factory_fleet_msgs.
typedef struct smart_factory_fleet_msgs__srv__AddTask_Response
{
  bool accepted;
  rosidl_runtime_c__String task_id;
  rosidl_runtime_c__String message;
} smart_factory_fleet_msgs__srv__AddTask_Response;

// Struct for a sequence of smart_factory_fleet_msgs__srv__AddTask_Response.
typedef struct smart_factory_fleet_msgs__srv__AddTask_Response__Sequence
{
  smart_factory_fleet_msgs__srv__AddTask_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} smart_factory_fleet_msgs__srv__AddTask_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__ADD_TASK__STRUCT_H_
