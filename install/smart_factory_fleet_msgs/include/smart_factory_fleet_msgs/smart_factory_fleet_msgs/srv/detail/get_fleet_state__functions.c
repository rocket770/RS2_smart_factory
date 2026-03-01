// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from smart_factory_fleet_msgs:srv/GetFleetState.idl
// generated code does not contain a copyright notice
#include "smart_factory_fleet_msgs/srv/detail/get_fleet_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

bool
smart_factory_fleet_msgs__srv__GetFleetState_Request__init(smart_factory_fleet_msgs__srv__GetFleetState_Request * msg)
{
  if (!msg) {
    return false;
  }
  // structure_needs_at_least_one_member
  return true;
}

void
smart_factory_fleet_msgs__srv__GetFleetState_Request__fini(smart_factory_fleet_msgs__srv__GetFleetState_Request * msg)
{
  if (!msg) {
    return;
  }
  // structure_needs_at_least_one_member
}

bool
smart_factory_fleet_msgs__srv__GetFleetState_Request__are_equal(const smart_factory_fleet_msgs__srv__GetFleetState_Request * lhs, const smart_factory_fleet_msgs__srv__GetFleetState_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // structure_needs_at_least_one_member
  if (lhs->structure_needs_at_least_one_member != rhs->structure_needs_at_least_one_member) {
    return false;
  }
  return true;
}

bool
smart_factory_fleet_msgs__srv__GetFleetState_Request__copy(
  const smart_factory_fleet_msgs__srv__GetFleetState_Request * input,
  smart_factory_fleet_msgs__srv__GetFleetState_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // structure_needs_at_least_one_member
  output->structure_needs_at_least_one_member = input->structure_needs_at_least_one_member;
  return true;
}

smart_factory_fleet_msgs__srv__GetFleetState_Request *
smart_factory_fleet_msgs__srv__GetFleetState_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  smart_factory_fleet_msgs__srv__GetFleetState_Request * msg = (smart_factory_fleet_msgs__srv__GetFleetState_Request *)allocator.allocate(sizeof(smart_factory_fleet_msgs__srv__GetFleetState_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(smart_factory_fleet_msgs__srv__GetFleetState_Request));
  bool success = smart_factory_fleet_msgs__srv__GetFleetState_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
smart_factory_fleet_msgs__srv__GetFleetState_Request__destroy(smart_factory_fleet_msgs__srv__GetFleetState_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    smart_factory_fleet_msgs__srv__GetFleetState_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence__init(smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  smart_factory_fleet_msgs__srv__GetFleetState_Request * data = NULL;

  if (size) {
    data = (smart_factory_fleet_msgs__srv__GetFleetState_Request *)allocator.zero_allocate(size, sizeof(smart_factory_fleet_msgs__srv__GetFleetState_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = smart_factory_fleet_msgs__srv__GetFleetState_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        smart_factory_fleet_msgs__srv__GetFleetState_Request__fini(&data[i - 1]);
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
smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence__fini(smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence * array)
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
      smart_factory_fleet_msgs__srv__GetFleetState_Request__fini(&array->data[i]);
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

smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence *
smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence * array = (smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence *)allocator.allocate(sizeof(smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence__destroy(smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence__are_equal(const smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence * lhs, const smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!smart_factory_fleet_msgs__srv__GetFleetState_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence__copy(
  const smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence * input,
  smart_factory_fleet_msgs__srv__GetFleetState_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(smart_factory_fleet_msgs__srv__GetFleetState_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    smart_factory_fleet_msgs__srv__GetFleetState_Request * data =
      (smart_factory_fleet_msgs__srv__GetFleetState_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!smart_factory_fleet_msgs__srv__GetFleetState_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          smart_factory_fleet_msgs__srv__GetFleetState_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!smart_factory_fleet_msgs__srv__GetFleetState_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `robots`
#include "smart_factory_fleet_msgs/msg/detail/robot_state__functions.h"
// Member `tasks`
#include "smart_factory_fleet_msgs/msg/detail/task__functions.h"

bool
smart_factory_fleet_msgs__srv__GetFleetState_Response__init(smart_factory_fleet_msgs__srv__GetFleetState_Response * msg)
{
  if (!msg) {
    return false;
  }
  // robots
  if (!smart_factory_fleet_msgs__msg__RobotState__Sequence__init(&msg->robots, 0)) {
    smart_factory_fleet_msgs__srv__GetFleetState_Response__fini(msg);
    return false;
  }
  // tasks
  if (!smart_factory_fleet_msgs__msg__Task__Sequence__init(&msg->tasks, 0)) {
    smart_factory_fleet_msgs__srv__GetFleetState_Response__fini(msg);
    return false;
  }
  return true;
}

void
smart_factory_fleet_msgs__srv__GetFleetState_Response__fini(smart_factory_fleet_msgs__srv__GetFleetState_Response * msg)
{
  if (!msg) {
    return;
  }
  // robots
  smart_factory_fleet_msgs__msg__RobotState__Sequence__fini(&msg->robots);
  // tasks
  smart_factory_fleet_msgs__msg__Task__Sequence__fini(&msg->tasks);
}

bool
smart_factory_fleet_msgs__srv__GetFleetState_Response__are_equal(const smart_factory_fleet_msgs__srv__GetFleetState_Response * lhs, const smart_factory_fleet_msgs__srv__GetFleetState_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // robots
  if (!smart_factory_fleet_msgs__msg__RobotState__Sequence__are_equal(
      &(lhs->robots), &(rhs->robots)))
  {
    return false;
  }
  // tasks
  if (!smart_factory_fleet_msgs__msg__Task__Sequence__are_equal(
      &(lhs->tasks), &(rhs->tasks)))
  {
    return false;
  }
  return true;
}

bool
smart_factory_fleet_msgs__srv__GetFleetState_Response__copy(
  const smart_factory_fleet_msgs__srv__GetFleetState_Response * input,
  smart_factory_fleet_msgs__srv__GetFleetState_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // robots
  if (!smart_factory_fleet_msgs__msg__RobotState__Sequence__copy(
      &(input->robots), &(output->robots)))
  {
    return false;
  }
  // tasks
  if (!smart_factory_fleet_msgs__msg__Task__Sequence__copy(
      &(input->tasks), &(output->tasks)))
  {
    return false;
  }
  return true;
}

smart_factory_fleet_msgs__srv__GetFleetState_Response *
smart_factory_fleet_msgs__srv__GetFleetState_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  smart_factory_fleet_msgs__srv__GetFleetState_Response * msg = (smart_factory_fleet_msgs__srv__GetFleetState_Response *)allocator.allocate(sizeof(smart_factory_fleet_msgs__srv__GetFleetState_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(smart_factory_fleet_msgs__srv__GetFleetState_Response));
  bool success = smart_factory_fleet_msgs__srv__GetFleetState_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
smart_factory_fleet_msgs__srv__GetFleetState_Response__destroy(smart_factory_fleet_msgs__srv__GetFleetState_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    smart_factory_fleet_msgs__srv__GetFleetState_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence__init(smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  smart_factory_fleet_msgs__srv__GetFleetState_Response * data = NULL;

  if (size) {
    data = (smart_factory_fleet_msgs__srv__GetFleetState_Response *)allocator.zero_allocate(size, sizeof(smart_factory_fleet_msgs__srv__GetFleetState_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = smart_factory_fleet_msgs__srv__GetFleetState_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        smart_factory_fleet_msgs__srv__GetFleetState_Response__fini(&data[i - 1]);
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
smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence__fini(smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence * array)
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
      smart_factory_fleet_msgs__srv__GetFleetState_Response__fini(&array->data[i]);
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

smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence *
smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence * array = (smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence *)allocator.allocate(sizeof(smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence__destroy(smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence__are_equal(const smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence * lhs, const smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!smart_factory_fleet_msgs__srv__GetFleetState_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence__copy(
  const smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence * input,
  smart_factory_fleet_msgs__srv__GetFleetState_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(smart_factory_fleet_msgs__srv__GetFleetState_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    smart_factory_fleet_msgs__srv__GetFleetState_Response * data =
      (smart_factory_fleet_msgs__srv__GetFleetState_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!smart_factory_fleet_msgs__srv__GetFleetState_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          smart_factory_fleet_msgs__srv__GetFleetState_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!smart_factory_fleet_msgs__srv__GetFleetState_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
