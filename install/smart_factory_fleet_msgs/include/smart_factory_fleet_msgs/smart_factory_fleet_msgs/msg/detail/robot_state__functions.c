// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from smart_factory_fleet_msgs:msg/RobotState.idl
// generated code does not contain a copyright notice
#include "smart_factory_fleet_msgs/msg/detail/robot_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `stamp`
#include "builtin_interfaces/msg/detail/time__functions.h"
// Member `robot_name`
// Member `status`
// Member `current_task_id`
#include "rosidl_runtime_c/string_functions.h"
// Member `pose`
#include "geometry_msgs/msg/detail/pose_stamped__functions.h"

bool
smart_factory_fleet_msgs__msg__RobotState__init(smart_factory_fleet_msgs__msg__RobotState * msg)
{
  if (!msg) {
    return false;
  }
  // stamp
  if (!builtin_interfaces__msg__Time__init(&msg->stamp)) {
    smart_factory_fleet_msgs__msg__RobotState__fini(msg);
    return false;
  }
  // robot_name
  if (!rosidl_runtime_c__String__init(&msg->robot_name)) {
    smart_factory_fleet_msgs__msg__RobotState__fini(msg);
    return false;
  }
  // status
  if (!rosidl_runtime_c__String__init(&msg->status)) {
    smart_factory_fleet_msgs__msg__RobotState__fini(msg);
    return false;
  }
  // pose
  if (!geometry_msgs__msg__PoseStamped__init(&msg->pose)) {
    smart_factory_fleet_msgs__msg__RobotState__fini(msg);
    return false;
  }
  // battery
  // current_task_id
  if (!rosidl_runtime_c__String__init(&msg->current_task_id)) {
    smart_factory_fleet_msgs__msg__RobotState__fini(msg);
    return false;
  }
  return true;
}

void
smart_factory_fleet_msgs__msg__RobotState__fini(smart_factory_fleet_msgs__msg__RobotState * msg)
{
  if (!msg) {
    return;
  }
  // stamp
  builtin_interfaces__msg__Time__fini(&msg->stamp);
  // robot_name
  rosidl_runtime_c__String__fini(&msg->robot_name);
  // status
  rosidl_runtime_c__String__fini(&msg->status);
  // pose
  geometry_msgs__msg__PoseStamped__fini(&msg->pose);
  // battery
  // current_task_id
  rosidl_runtime_c__String__fini(&msg->current_task_id);
}

bool
smart_factory_fleet_msgs__msg__RobotState__are_equal(const smart_factory_fleet_msgs__msg__RobotState * lhs, const smart_factory_fleet_msgs__msg__RobotState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // stamp
  if (!builtin_interfaces__msg__Time__are_equal(
      &(lhs->stamp), &(rhs->stamp)))
  {
    return false;
  }
  // robot_name
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->robot_name), &(rhs->robot_name)))
  {
    return false;
  }
  // status
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->status), &(rhs->status)))
  {
    return false;
  }
  // pose
  if (!geometry_msgs__msg__PoseStamped__are_equal(
      &(lhs->pose), &(rhs->pose)))
  {
    return false;
  }
  // battery
  if (lhs->battery != rhs->battery) {
    return false;
  }
  // current_task_id
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->current_task_id), &(rhs->current_task_id)))
  {
    return false;
  }
  return true;
}

bool
smart_factory_fleet_msgs__msg__RobotState__copy(
  const smart_factory_fleet_msgs__msg__RobotState * input,
  smart_factory_fleet_msgs__msg__RobotState * output)
{
  if (!input || !output) {
    return false;
  }
  // stamp
  if (!builtin_interfaces__msg__Time__copy(
      &(input->stamp), &(output->stamp)))
  {
    return false;
  }
  // robot_name
  if (!rosidl_runtime_c__String__copy(
      &(input->robot_name), &(output->robot_name)))
  {
    return false;
  }
  // status
  if (!rosidl_runtime_c__String__copy(
      &(input->status), &(output->status)))
  {
    return false;
  }
  // pose
  if (!geometry_msgs__msg__PoseStamped__copy(
      &(input->pose), &(output->pose)))
  {
    return false;
  }
  // battery
  output->battery = input->battery;
  // current_task_id
  if (!rosidl_runtime_c__String__copy(
      &(input->current_task_id), &(output->current_task_id)))
  {
    return false;
  }
  return true;
}

smart_factory_fleet_msgs__msg__RobotState *
smart_factory_fleet_msgs__msg__RobotState__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  smart_factory_fleet_msgs__msg__RobotState * msg = (smart_factory_fleet_msgs__msg__RobotState *)allocator.allocate(sizeof(smart_factory_fleet_msgs__msg__RobotState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(smart_factory_fleet_msgs__msg__RobotState));
  bool success = smart_factory_fleet_msgs__msg__RobotState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
smart_factory_fleet_msgs__msg__RobotState__destroy(smart_factory_fleet_msgs__msg__RobotState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    smart_factory_fleet_msgs__msg__RobotState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
smart_factory_fleet_msgs__msg__RobotState__Sequence__init(smart_factory_fleet_msgs__msg__RobotState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  smart_factory_fleet_msgs__msg__RobotState * data = NULL;

  if (size) {
    data = (smart_factory_fleet_msgs__msg__RobotState *)allocator.zero_allocate(size, sizeof(smart_factory_fleet_msgs__msg__RobotState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = smart_factory_fleet_msgs__msg__RobotState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        smart_factory_fleet_msgs__msg__RobotState__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
smart_factory_fleet_msgs__msg__RobotState__Sequence__fini(smart_factory_fleet_msgs__msg__RobotState__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      smart_factory_fleet_msgs__msg__RobotState__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

smart_factory_fleet_msgs__msg__RobotState__Sequence *
smart_factory_fleet_msgs__msg__RobotState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  smart_factory_fleet_msgs__msg__RobotState__Sequence * array = (smart_factory_fleet_msgs__msg__RobotState__Sequence *)allocator.allocate(sizeof(smart_factory_fleet_msgs__msg__RobotState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = smart_factory_fleet_msgs__msg__RobotState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
smart_factory_fleet_msgs__msg__RobotState__Sequence__destroy(smart_factory_fleet_msgs__msg__RobotState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    smart_factory_fleet_msgs__msg__RobotState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
smart_factory_fleet_msgs__msg__RobotState__Sequence__are_equal(const smart_factory_fleet_msgs__msg__RobotState__Sequence * lhs, const smart_factory_fleet_msgs__msg__RobotState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!smart_factory_fleet_msgs__msg__RobotState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
smart_factory_fleet_msgs__msg__RobotState__Sequence__copy(
  const smart_factory_fleet_msgs__msg__RobotState__Sequence * input,
  smart_factory_fleet_msgs__msg__RobotState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(smart_factory_fleet_msgs__msg__RobotState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    smart_factory_fleet_msgs__msg__RobotState * data =
      (smart_factory_fleet_msgs__msg__RobotState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!smart_factory_fleet_msgs__msg__RobotState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          smart_factory_fleet_msgs__msg__RobotState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!smart_factory_fleet_msgs__msg__RobotState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
