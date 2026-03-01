// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from smart_factory_fleet_msgs:srv/GetFleetState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "smart_factory_fleet_msgs/srv/detail/get_fleet_state__rosidl_typesupport_introspection_c.h"
#include "smart_factory_fleet_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "smart_factory_fleet_msgs/srv/detail/get_fleet_state__functions.h"
#include "smart_factory_fleet_msgs/srv/detail/get_fleet_state__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void smart_factory_fleet_msgs__srv__GetFleetState_Request__rosidl_typesupport_introspection_c__GetFleetState_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  smart_factory_fleet_msgs__srv__GetFleetState_Request__init(message_memory);
}

void smart_factory_fleet_msgs__srv__GetFleetState_Request__rosidl_typesupport_introspection_c__GetFleetState_Request_fini_function(void * message_memory)
{
  smart_factory_fleet_msgs__srv__GetFleetState_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember smart_factory_fleet_msgs__srv__GetFleetState_Request__rosidl_typesupport_introspection_c__GetFleetState_Request_message_member_array[1] = {
  {
    "structure_needs_at_least_one_member",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(smart_factory_fleet_msgs__srv__GetFleetState_Request, structure_needs_at_least_one_member),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers smart_factory_fleet_msgs__srv__GetFleetState_Request__rosidl_typesupport_introspection_c__GetFleetState_Request_message_members = {
  "smart_factory_fleet_msgs__srv",  // message namespace
  "GetFleetState_Request",  // message name
  1,  // number of fields
  sizeof(smart_factory_fleet_msgs__srv__GetFleetState_Request),
  smart_factory_fleet_msgs__srv__GetFleetState_Request__rosidl_typesupport_introspection_c__GetFleetState_Request_message_member_array,  // message members
  smart_factory_fleet_msgs__srv__GetFleetState_Request__rosidl_typesupport_introspection_c__GetFleetState_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  smart_factory_fleet_msgs__srv__GetFleetState_Request__rosidl_typesupport_introspection_c__GetFleetState_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t smart_factory_fleet_msgs__srv__GetFleetState_Request__rosidl_typesupport_introspection_c__GetFleetState_Request_message_type_support_handle = {
  0,
  &smart_factory_fleet_msgs__srv__GetFleetState_Request__rosidl_typesupport_introspection_c__GetFleetState_Request_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_smart_factory_fleet_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, smart_factory_fleet_msgs, srv, GetFleetState_Request)() {
  if (!smart_factory_fleet_msgs__srv__GetFleetState_Request__rosidl_typesupport_introspection_c__GetFleetState_Request_message_type_support_handle.typesupport_identifier) {
    smart_factory_fleet_msgs__srv__GetFleetState_Request__rosidl_typesupport_introspection_c__GetFleetState_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &smart_factory_fleet_msgs__srv__GetFleetState_Request__rosidl_typesupport_introspection_c__GetFleetState_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "smart_factory_fleet_msgs/srv/detail/get_fleet_state__rosidl_typesupport_introspection_c.h"
// already included above
// #include "smart_factory_fleet_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "smart_factory_fleet_msgs/srv/detail/get_fleet_state__functions.h"
// already included above
// #include "smart_factory_fleet_msgs/srv/detail/get_fleet_state__struct.h"


// Include directives for member types
// Member `robots`
#include "smart_factory_fleet_msgs/msg/robot_state.h"
// Member `robots`
#include "smart_factory_fleet_msgs/msg/detail/robot_state__rosidl_typesupport_introspection_c.h"
// Member `tasks`
#include "smart_factory_fleet_msgs/msg/task.h"
// Member `tasks`
#include "smart_factory_fleet_msgs/msg/detail/task__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__GetFleetState_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  smart_factory_fleet_msgs__srv__GetFleetState_Response__init(message_memory);
}

void smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__GetFleetState_Response_fini_function(void * message_memory)
{
  smart_factory_fleet_msgs__srv__GetFleetState_Response__fini(message_memory);
}

size_t smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__size_function__GetFleetState_Response__robots(
  const void * untyped_member)
{
  const smart_factory_fleet_msgs__msg__RobotState__Sequence * member =
    (const smart_factory_fleet_msgs__msg__RobotState__Sequence *)(untyped_member);
  return member->size;
}

const void * smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__get_const_function__GetFleetState_Response__robots(
  const void * untyped_member, size_t index)
{
  const smart_factory_fleet_msgs__msg__RobotState__Sequence * member =
    (const smart_factory_fleet_msgs__msg__RobotState__Sequence *)(untyped_member);
  return &member->data[index];
}

void * smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__get_function__GetFleetState_Response__robots(
  void * untyped_member, size_t index)
{
  smart_factory_fleet_msgs__msg__RobotState__Sequence * member =
    (smart_factory_fleet_msgs__msg__RobotState__Sequence *)(untyped_member);
  return &member->data[index];
}

void smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__fetch_function__GetFleetState_Response__robots(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const smart_factory_fleet_msgs__msg__RobotState * item =
    ((const smart_factory_fleet_msgs__msg__RobotState *)
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__get_const_function__GetFleetState_Response__robots(untyped_member, index));
  smart_factory_fleet_msgs__msg__RobotState * value =
    (smart_factory_fleet_msgs__msg__RobotState *)(untyped_value);
  *value = *item;
}

void smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__assign_function__GetFleetState_Response__robots(
  void * untyped_member, size_t index, const void * untyped_value)
{
  smart_factory_fleet_msgs__msg__RobotState * item =
    ((smart_factory_fleet_msgs__msg__RobotState *)
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__get_function__GetFleetState_Response__robots(untyped_member, index));
  const smart_factory_fleet_msgs__msg__RobotState * value =
    (const smart_factory_fleet_msgs__msg__RobotState *)(untyped_value);
  *item = *value;
}

bool smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__resize_function__GetFleetState_Response__robots(
  void * untyped_member, size_t size)
{
  smart_factory_fleet_msgs__msg__RobotState__Sequence * member =
    (smart_factory_fleet_msgs__msg__RobotState__Sequence *)(untyped_member);
  smart_factory_fleet_msgs__msg__RobotState__Sequence__fini(member);
  return smart_factory_fleet_msgs__msg__RobotState__Sequence__init(member, size);
}

size_t smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__size_function__GetFleetState_Response__tasks(
  const void * untyped_member)
{
  const smart_factory_fleet_msgs__msg__Task__Sequence * member =
    (const smart_factory_fleet_msgs__msg__Task__Sequence *)(untyped_member);
  return member->size;
}

const void * smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__get_const_function__GetFleetState_Response__tasks(
  const void * untyped_member, size_t index)
{
  const smart_factory_fleet_msgs__msg__Task__Sequence * member =
    (const smart_factory_fleet_msgs__msg__Task__Sequence *)(untyped_member);
  return &member->data[index];
}

void * smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__get_function__GetFleetState_Response__tasks(
  void * untyped_member, size_t index)
{
  smart_factory_fleet_msgs__msg__Task__Sequence * member =
    (smart_factory_fleet_msgs__msg__Task__Sequence *)(untyped_member);
  return &member->data[index];
}

void smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__fetch_function__GetFleetState_Response__tasks(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const smart_factory_fleet_msgs__msg__Task * item =
    ((const smart_factory_fleet_msgs__msg__Task *)
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__get_const_function__GetFleetState_Response__tasks(untyped_member, index));
  smart_factory_fleet_msgs__msg__Task * value =
    (smart_factory_fleet_msgs__msg__Task *)(untyped_value);
  *value = *item;
}

void smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__assign_function__GetFleetState_Response__tasks(
  void * untyped_member, size_t index, const void * untyped_value)
{
  smart_factory_fleet_msgs__msg__Task * item =
    ((smart_factory_fleet_msgs__msg__Task *)
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__get_function__GetFleetState_Response__tasks(untyped_member, index));
  const smart_factory_fleet_msgs__msg__Task * value =
    (const smart_factory_fleet_msgs__msg__Task *)(untyped_value);
  *item = *value;
}

bool smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__resize_function__GetFleetState_Response__tasks(
  void * untyped_member, size_t size)
{
  smart_factory_fleet_msgs__msg__Task__Sequence * member =
    (smart_factory_fleet_msgs__msg__Task__Sequence *)(untyped_member);
  smart_factory_fleet_msgs__msg__Task__Sequence__fini(member);
  return smart_factory_fleet_msgs__msg__Task__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__GetFleetState_Response_message_member_array[2] = {
  {
    "robots",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(smart_factory_fleet_msgs__srv__GetFleetState_Response, robots),  // bytes offset in struct
    NULL,  // default value
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__size_function__GetFleetState_Response__robots,  // size() function pointer
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__get_const_function__GetFleetState_Response__robots,  // get_const(index) function pointer
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__get_function__GetFleetState_Response__robots,  // get(index) function pointer
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__fetch_function__GetFleetState_Response__robots,  // fetch(index, &value) function pointer
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__assign_function__GetFleetState_Response__robots,  // assign(index, value) function pointer
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__resize_function__GetFleetState_Response__robots  // resize(index) function pointer
  },
  {
    "tasks",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(smart_factory_fleet_msgs__srv__GetFleetState_Response, tasks),  // bytes offset in struct
    NULL,  // default value
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__size_function__GetFleetState_Response__tasks,  // size() function pointer
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__get_const_function__GetFleetState_Response__tasks,  // get_const(index) function pointer
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__get_function__GetFleetState_Response__tasks,  // get(index) function pointer
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__fetch_function__GetFleetState_Response__tasks,  // fetch(index, &value) function pointer
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__assign_function__GetFleetState_Response__tasks,  // assign(index, value) function pointer
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__resize_function__GetFleetState_Response__tasks  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__GetFleetState_Response_message_members = {
  "smart_factory_fleet_msgs__srv",  // message namespace
  "GetFleetState_Response",  // message name
  2,  // number of fields
  sizeof(smart_factory_fleet_msgs__srv__GetFleetState_Response),
  smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__GetFleetState_Response_message_member_array,  // message members
  smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__GetFleetState_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__GetFleetState_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__GetFleetState_Response_message_type_support_handle = {
  0,
  &smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__GetFleetState_Response_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_smart_factory_fleet_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, smart_factory_fleet_msgs, srv, GetFleetState_Response)() {
  smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__GetFleetState_Response_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, smart_factory_fleet_msgs, msg, RobotState)();
  smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__GetFleetState_Response_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, smart_factory_fleet_msgs, msg, Task)();
  if (!smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__GetFleetState_Response_message_type_support_handle.typesupport_identifier) {
    smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__GetFleetState_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &smart_factory_fleet_msgs__srv__GetFleetState_Response__rosidl_typesupport_introspection_c__GetFleetState_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "smart_factory_fleet_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "smart_factory_fleet_msgs/srv/detail/get_fleet_state__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers smart_factory_fleet_msgs__srv__detail__get_fleet_state__rosidl_typesupport_introspection_c__GetFleetState_service_members = {
  "smart_factory_fleet_msgs__srv",  // service namespace
  "GetFleetState",  // service name
  // these two fields are initialized below on the first access
  NULL,  // request message
  // smart_factory_fleet_msgs__srv__detail__get_fleet_state__rosidl_typesupport_introspection_c__GetFleetState_Request_message_type_support_handle,
  NULL  // response message
  // smart_factory_fleet_msgs__srv__detail__get_fleet_state__rosidl_typesupport_introspection_c__GetFleetState_Response_message_type_support_handle
};

static rosidl_service_type_support_t smart_factory_fleet_msgs__srv__detail__get_fleet_state__rosidl_typesupport_introspection_c__GetFleetState_service_type_support_handle = {
  0,
  &smart_factory_fleet_msgs__srv__detail__get_fleet_state__rosidl_typesupport_introspection_c__GetFleetState_service_members,
  get_service_typesupport_handle_function,
};

// Forward declaration of request/response type support functions
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, smart_factory_fleet_msgs, srv, GetFleetState_Request)();

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, smart_factory_fleet_msgs, srv, GetFleetState_Response)();

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_smart_factory_fleet_msgs
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, smart_factory_fleet_msgs, srv, GetFleetState)() {
  if (!smart_factory_fleet_msgs__srv__detail__get_fleet_state__rosidl_typesupport_introspection_c__GetFleetState_service_type_support_handle.typesupport_identifier) {
    smart_factory_fleet_msgs__srv__detail__get_fleet_state__rosidl_typesupport_introspection_c__GetFleetState_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)smart_factory_fleet_msgs__srv__detail__get_fleet_state__rosidl_typesupport_introspection_c__GetFleetState_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, smart_factory_fleet_msgs, srv, GetFleetState_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, smart_factory_fleet_msgs, srv, GetFleetState_Response)()->data;
  }

  return &smart_factory_fleet_msgs__srv__detail__get_fleet_state__rosidl_typesupport_introspection_c__GetFleetState_service_type_support_handle;
}
