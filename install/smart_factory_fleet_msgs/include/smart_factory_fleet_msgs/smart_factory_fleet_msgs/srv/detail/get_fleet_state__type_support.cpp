// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from smart_factory_fleet_msgs:srv/GetFleetState.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "smart_factory_fleet_msgs/srv/detail/get_fleet_state__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace smart_factory_fleet_msgs
{

namespace srv
{

namespace rosidl_typesupport_introspection_cpp
{

void GetFleetState_Request_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) smart_factory_fleet_msgs::srv::GetFleetState_Request(_init);
}

void GetFleetState_Request_fini_function(void * message_memory)
{
  auto typed_message = static_cast<smart_factory_fleet_msgs::srv::GetFleetState_Request *>(message_memory);
  typed_message->~GetFleetState_Request();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember GetFleetState_Request_message_member_array[1] = {
  {
    "structure_needs_at_least_one_member",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(smart_factory_fleet_msgs::srv::GetFleetState_Request, structure_needs_at_least_one_member),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers GetFleetState_Request_message_members = {
  "smart_factory_fleet_msgs::srv",  // message namespace
  "GetFleetState_Request",  // message name
  1,  // number of fields
  sizeof(smart_factory_fleet_msgs::srv::GetFleetState_Request),
  GetFleetState_Request_message_member_array,  // message members
  GetFleetState_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  GetFleetState_Request_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t GetFleetState_Request_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &GetFleetState_Request_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace srv

}  // namespace smart_factory_fleet_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<smart_factory_fleet_msgs::srv::GetFleetState_Request>()
{
  return &::smart_factory_fleet_msgs::srv::rosidl_typesupport_introspection_cpp::GetFleetState_Request_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, smart_factory_fleet_msgs, srv, GetFleetState_Request)() {
  return &::smart_factory_fleet_msgs::srv::rosidl_typesupport_introspection_cpp::GetFleetState_Request_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "array"
// already included above
// #include "cstddef"
// already included above
// #include "string"
// already included above
// #include "vector"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "smart_factory_fleet_msgs/srv/detail/get_fleet_state__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/field_types.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace smart_factory_fleet_msgs
{

namespace srv
{

namespace rosidl_typesupport_introspection_cpp
{

void GetFleetState_Response_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) smart_factory_fleet_msgs::srv::GetFleetState_Response(_init);
}

void GetFleetState_Response_fini_function(void * message_memory)
{
  auto typed_message = static_cast<smart_factory_fleet_msgs::srv::GetFleetState_Response *>(message_memory);
  typed_message->~GetFleetState_Response();
}

size_t size_function__GetFleetState_Response__robots(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<smart_factory_fleet_msgs::msg::RobotState> *>(untyped_member);
  return member->size();
}

const void * get_const_function__GetFleetState_Response__robots(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<smart_factory_fleet_msgs::msg::RobotState> *>(untyped_member);
  return &member[index];
}

void * get_function__GetFleetState_Response__robots(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<smart_factory_fleet_msgs::msg::RobotState> *>(untyped_member);
  return &member[index];
}

void fetch_function__GetFleetState_Response__robots(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const smart_factory_fleet_msgs::msg::RobotState *>(
    get_const_function__GetFleetState_Response__robots(untyped_member, index));
  auto & value = *reinterpret_cast<smart_factory_fleet_msgs::msg::RobotState *>(untyped_value);
  value = item;
}

void assign_function__GetFleetState_Response__robots(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<smart_factory_fleet_msgs::msg::RobotState *>(
    get_function__GetFleetState_Response__robots(untyped_member, index));
  const auto & value = *reinterpret_cast<const smart_factory_fleet_msgs::msg::RobotState *>(untyped_value);
  item = value;
}

void resize_function__GetFleetState_Response__robots(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<smart_factory_fleet_msgs::msg::RobotState> *>(untyped_member);
  member->resize(size);
}

size_t size_function__GetFleetState_Response__tasks(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<smart_factory_fleet_msgs::msg::Task> *>(untyped_member);
  return member->size();
}

const void * get_const_function__GetFleetState_Response__tasks(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<smart_factory_fleet_msgs::msg::Task> *>(untyped_member);
  return &member[index];
}

void * get_function__GetFleetState_Response__tasks(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<smart_factory_fleet_msgs::msg::Task> *>(untyped_member);
  return &member[index];
}

void fetch_function__GetFleetState_Response__tasks(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const smart_factory_fleet_msgs::msg::Task *>(
    get_const_function__GetFleetState_Response__tasks(untyped_member, index));
  auto & value = *reinterpret_cast<smart_factory_fleet_msgs::msg::Task *>(untyped_value);
  value = item;
}

void assign_function__GetFleetState_Response__tasks(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<smart_factory_fleet_msgs::msg::Task *>(
    get_function__GetFleetState_Response__tasks(untyped_member, index));
  const auto & value = *reinterpret_cast<const smart_factory_fleet_msgs::msg::Task *>(untyped_value);
  item = value;
}

void resize_function__GetFleetState_Response__tasks(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<smart_factory_fleet_msgs::msg::Task> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember GetFleetState_Response_message_member_array[2] = {
  {
    "robots",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<smart_factory_fleet_msgs::msg::RobotState>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(smart_factory_fleet_msgs::srv::GetFleetState_Response, robots),  // bytes offset in struct
    nullptr,  // default value
    size_function__GetFleetState_Response__robots,  // size() function pointer
    get_const_function__GetFleetState_Response__robots,  // get_const(index) function pointer
    get_function__GetFleetState_Response__robots,  // get(index) function pointer
    fetch_function__GetFleetState_Response__robots,  // fetch(index, &value) function pointer
    assign_function__GetFleetState_Response__robots,  // assign(index, value) function pointer
    resize_function__GetFleetState_Response__robots  // resize(index) function pointer
  },
  {
    "tasks",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<smart_factory_fleet_msgs::msg::Task>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(smart_factory_fleet_msgs::srv::GetFleetState_Response, tasks),  // bytes offset in struct
    nullptr,  // default value
    size_function__GetFleetState_Response__tasks,  // size() function pointer
    get_const_function__GetFleetState_Response__tasks,  // get_const(index) function pointer
    get_function__GetFleetState_Response__tasks,  // get(index) function pointer
    fetch_function__GetFleetState_Response__tasks,  // fetch(index, &value) function pointer
    assign_function__GetFleetState_Response__tasks,  // assign(index, value) function pointer
    resize_function__GetFleetState_Response__tasks  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers GetFleetState_Response_message_members = {
  "smart_factory_fleet_msgs::srv",  // message namespace
  "GetFleetState_Response",  // message name
  2,  // number of fields
  sizeof(smart_factory_fleet_msgs::srv::GetFleetState_Response),
  GetFleetState_Response_message_member_array,  // message members
  GetFleetState_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  GetFleetState_Response_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t GetFleetState_Response_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &GetFleetState_Response_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace srv

}  // namespace smart_factory_fleet_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<smart_factory_fleet_msgs::srv::GetFleetState_Response>()
{
  return &::smart_factory_fleet_msgs::srv::rosidl_typesupport_introspection_cpp::GetFleetState_Response_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, smart_factory_fleet_msgs, srv, GetFleetState_Response)() {
  return &::smart_factory_fleet_msgs::srv::rosidl_typesupport_introspection_cpp::GetFleetState_Response_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"
// already included above
// #include "smart_factory_fleet_msgs/srv/detail/get_fleet_state__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/service_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/service_type_support_decl.hpp"

namespace smart_factory_fleet_msgs
{

namespace srv
{

namespace rosidl_typesupport_introspection_cpp
{

// this is intentionally not const to allow initialization later to prevent an initialization race
static ::rosidl_typesupport_introspection_cpp::ServiceMembers GetFleetState_service_members = {
  "smart_factory_fleet_msgs::srv",  // service namespace
  "GetFleetState",  // service name
  // these two fields are initialized below on the first access
  // see get_service_type_support_handle<smart_factory_fleet_msgs::srv::GetFleetState>()
  nullptr,  // request message
  nullptr  // response message
};

static const rosidl_service_type_support_t GetFleetState_service_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &GetFleetState_service_members,
  get_service_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace srv

}  // namespace smart_factory_fleet_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<smart_factory_fleet_msgs::srv::GetFleetState>()
{
  // get a handle to the value to be returned
  auto service_type_support =
    &::smart_factory_fleet_msgs::srv::rosidl_typesupport_introspection_cpp::GetFleetState_service_type_support_handle;
  // get a non-const and properly typed version of the data void *
  auto service_members = const_cast<::rosidl_typesupport_introspection_cpp::ServiceMembers *>(
    static_cast<const ::rosidl_typesupport_introspection_cpp::ServiceMembers *>(
      service_type_support->data));
  // make sure that both the request_members_ and the response_members_ are initialized
  // if they are not, initialize them
  if (
    service_members->request_members_ == nullptr ||
    service_members->response_members_ == nullptr)
  {
    // initialize the request_members_ with the static function from the external library
    service_members->request_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::smart_factory_fleet_msgs::srv::GetFleetState_Request
      >()->data
      );
    // initialize the response_members_ with the static function from the external library
    service_members->response_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::smart_factory_fleet_msgs::srv::GetFleetState_Response
      >()->data
      );
  }
  // finally return the properly initialized service_type_support handle
  return service_type_support;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, smart_factory_fleet_msgs, srv, GetFleetState)() {
  return ::rosidl_typesupport_introspection_cpp::get_service_type_support_handle<smart_factory_fleet_msgs::srv::GetFleetState>();
}

#ifdef __cplusplus
}
#endif
