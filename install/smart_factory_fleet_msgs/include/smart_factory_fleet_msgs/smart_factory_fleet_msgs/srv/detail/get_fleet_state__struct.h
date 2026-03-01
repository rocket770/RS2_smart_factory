// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from smart_factory_fleet_msgs:srv/GetFleetState.idl
// generated code does not contain a copyright notice

#ifndef SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__GET_FLEET_STATE__STRUCT_H_
#define SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__GET_FLEET_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/GetFleetState in the package smart_factory_fleet_msgs.
typedef struct smart_factory_fleet_msgs__srv__GetFleetState_Request
{
  uint8_t structure_needs_at_least_one_member;
} smart_factory_fleet_msgs__srv__GetFleetState_Request;

// Struct for a sequence of smart_factory_fleet_msgs__srv__GetFleetState_Request.
typedef struct smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence
{
  smart_factory_fleet_msgs__srv__GetFleetState_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'robots'
#include "smart_factory_fleet_msgs/msg/detail/robot_state__struct.h"
// Member 'tasks'
#include "smart_factory_fleet_msgs/msg/detail/task__struct.h"

/// Struct defined in srv/GetFleetState in the package smart_factory_fleet_msgs.
typedef struct smart_factory_fleet_msgs__srv__GetFleetState_Response
{
  smart_factory_fleet_msgs__msg__RobotState__Sequence robots;
  smart_factory_fleet_msgs__msg__Task__Sequence tasks;
} smart_factory_fleet_msgs__srv__GetFleetState_Response;

// Struct for a sequence of smart_factory_fleet_msgs__srv__GetFleetState_Response.
typedef struct smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence
{
  smart_factory_fleet_msgs__srv__GetFleetState_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SMART_FACTORY_FLEET_MSGS__SRV__DETAIL__GET_FLEET_STATE__STRUCT_H_
