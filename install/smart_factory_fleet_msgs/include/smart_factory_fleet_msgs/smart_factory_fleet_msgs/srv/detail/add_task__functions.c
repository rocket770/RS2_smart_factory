// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from smart_factory_fleet_msgs:srv/AddTask.idl
// generated code does not contain a copyright notice
#include "smart_factory_fleet_msgs/srv/detail/add_task__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

// Include directives for member types
// Member `type`
#include "rosidl_runtime_c/string_functions.h"
// Member `pickup_pose`
// Member `dropoff_pose`
#include "geometry_msgs/msg/detail/pose_stamped__functions.h"

bool
smart_factory_fleet_msgs__srv__AddTask_Request__init(smart_factory_fleet_msgs__srv__AddTask_Request * msg)
{
  if (!msg) {
    return false;
  }
  // type
  if (!rosidl_runtime_c__String__init(&msg->type)) {
    smart_factory_fleet_msgs__srv__AddTask_Request__fini(msg);
    return false;
  }
  // priority
  // pickup_pose
  if (!geometry_msgs__msg__PoseStamped__init(&msg->pickup_pose)) {
    smart_factory_fleet_msgs__srv__AddTask_Request__fini(msg);
    return false;
  }
  // dropoff_pose
  if (!geometry_msgs__msg__PoseStamped__init(&msg->dropoff_pose)) {
    smart_factory_fleet_msgs__srv__AddTask_Request__fini(msg);
    return false;
  }
  return true;
}

void
smart_factory_fleet_msgs__srv__AddTask_Request__fini(smart_factory_fleet_msgs__srv__AddTask_Request * msg)
{
  if (!msg) {
    return;
  }
  // type
  rosidl_runtime_c__String__fini(&msg->type);
  // priority
  // pickup_pose
  geometry_msgs__msg__PoseStamped__fini(&msg->pickup_pose);
  // dropoff_pose
  geometry_msgs__msg__PoseStamped__fini(&msg->dropoff_pose);
}

bool
smart_factory_fleet_msgs__srv__AddTask_Request__are_equal(const smart_factory_fleet_msgs__srv__AddTask_Request * lhs, const smart_factory_fleet_msgs__srv__AddTask_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // type
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->type), &(rhs->type)))
  {
    return false;
  }
  // priority
  if (lhs->priority != rhs->priority) {
    return false;
  }
  // pickup_pose
  if (!geometry_msgs__msg__PoseStamped__are_equal(
      &(lhs->pickup_pose), &(rhs->pickup_pose)))
  {
    return false;
  }
  // dropoff_pose
  if (!geometry_msgs__msg__PoseStamped__are_equal(
      &(lhs->dropoff_pose), &(rhs->dropoff_pose)))
  {
    return false;
  }
  return true;
}

bool
smart_factory_fleet_msgs__srv__AddTask_Request__copy(
  const smart_factory_fleet_msgs__srv__AddTask_Request * input,
  smart_factory_fleet_msgs__srv__AddTask_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // type
  if (!rosidl_runtime_c__String__copy(
      &(input->type), &(output->type)))
  {
    return false;
  }
  // priority
  output->priority = input->priority;
  // pickup_pose
  if (!geometry_msgs__msg__PoseStamped__copy(
      &(input->pickup_pose), &(output->pickup_pose)))
  {
    return false;
  }
  // dropoff_pose
  if (!geometry_msgs__msg__PoseStamped__copy(
      &(input->dropoff_pose), &(output->dropoff_pose)))
  {
    return false;
  }
  return true;
}

smart_factory_fleet_msgs__srv__AddTask_Request *
smart_factory_fleet_msgs__srv__AddTask_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  smart_factory_fleet_msgs__srv__AddTask_Request * msg = (smart_factory_fleet_msgs__srv__AddTask_Request *)allocator.allocate(sizeof(smart_factory_fleet_msgs__srv__AddTask_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(smart_factory_fleet_msgs__srv__AddTask_Request));
  bool success = smart_factory_fleet_msgs__srv__AddTask_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
smart_factory_fleet_msgs__srv__AddTask_Request__destroy(smart_factory_fleet_msgs__srv__AddTask_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    smart_factory_fleet_msgs__srv__AddTask_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
smart_factory_fleet_msgs__srv__AddTask_Request__Sequence__init(smart_factory_fleet_msgs__srv__AddTask_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  smart_factory_fleet_msgs__srv__AddTask_Request * data = NULL;

  if (size) {
    data = (smart_factory_fleet_msgs__srv__AddTask_Request *)allocator.zero_allocate(size, sizeof(smart_factory_fleet_msgs__srv__AddTask_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = smart_factory_fleet_msgs__srv__AddTask_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        smart_factory_fleet_msgs__srv__AddTask_Request__fini(&data[i - 1]);
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
smart_factory_fleet_msgs__srv__AddTask_Request__Sequence__fini(smart_factory_fleet_msgs__srv__AddTask_Request__Sequence * array)
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
      smart_factory_fleet_msgs__srv__AddTask_Request__fini(&array->data[i]);
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

smart_factory_fleet_msgs__srv__AddTask_Request__Sequence *
smart_factory_fleet_msgs__srv__AddTask_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  smart_factory_fleet_msgs__srv__AddTask_Request__Sequence * array = (smart_factory_fleet_msgs__srv__AddTask_Request__Sequence *)allocator.allocate(sizeof(smart_factory_fleet_msgs__srv__AddTask_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = smart_factory_fleet_msgs__srv__AddTask_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
smart_factory_fleet_msgs__srv__AddTask_Request__Sequence__destroy(smart_factory_fleet_msgs__srv__AddTask_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    smart_factory_fleet_msgs__srv__AddTask_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
smart_factory_fleet_msgs__srv__AddTask_Request__Sequence__are_equal(const smart_factory_fleet_msgs__srv__AddTask_Request__Sequence * lhs, const smart_factory_fleet_msgs__srv__AddTask_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!smart_factory_fleet_msgs__srv__AddTask_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
smart_factory_fleet_msgs__srv__AddTask_Request__Sequence__copy(
  const smart_factory_fleet_msgs__srv__AddTask_Request__Sequence * input,
  smart_factory_fleet_msgs__srv__AddTask_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(smart_factory_fleet_msgs__srv__AddTask_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    smart_factory_fleet_msgs__srv__AddTask_Request * data =
      (smart_factory_fleet_msgs__srv__AddTask_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!smart_factory_fleet_msgs__srv__AddTask_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          smart_factory_fleet_msgs__srv__AddTask_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!smart_factory_fleet_msgs__srv__AddTask_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `task_id`
// Member `message`
// already included above
// #include "rosidl_runtime_c/string_functions.h"

bool
smart_factory_fleet_msgs__srv__AddTask_Response__init(smart_factory_fleet_msgs__srv__AddTask_Response * msg)
{
  if (!msg) {
    return false;
  }
  // accepted
  // task_id
  if (!rosidl_runtime_c__String__init(&msg->task_id)) {
    smart_factory_fleet_msgs__srv__AddTask_Response__fini(msg);
    return false;
  }
  // message
  if (!rosidl_runtime_c__String__init(&msg->message)) {
    smart_factory_fleet_msgs__srv__AddTask_Response__fini(msg);
    return false;
  }
  return true;
}

void
smart_factory_fleet_msgs__srv__AddTask_Response__fini(smart_factory_fleet_msgs__srv__AddTask_Response * msg)
{
  if (!msg) {
    return;
  }
  // accepted
  // task_id
  rosidl_runtime_c__String__fini(&msg->task_id);
  // message
  rosidl_runtime_c__String__fini(&msg->message);
}

bool
smart_factory_fleet_msgs__srv__AddTask_Response__are_equal(const smart_factory_fleet_msgs__srv__AddTask_Response * lhs, const smart_factory_fleet_msgs__srv__AddTask_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // accepted
  if (lhs->accepted != rhs->accepted) {
    return false;
  }
  // task_id
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->task_id), &(rhs->task_id)))
  {
    return false;
  }
  // message
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->message), &(rhs->message)))
  {
    return false;
  }
  return true;
}

bool
smart_factory_fleet_msgs__srv__AddTask_Response__copy(
  const smart_factory_fleet_msgs__srv__AddTask_Response * input,
  smart_factory_fleet_msgs__srv__AddTask_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // accepted
  output->accepted = input->accepted;
  // task_id
  if (!rosidl_runtime_c__String__copy(
      &(input->task_id), &(output->task_id)))
  {
    return false;
  }
  // message
  if (!rosidl_runtime_c__String__copy(
      &(input->message), &(output->message)))
  {
    return false;
  }
  return true;
}

smart_factory_fleet_msgs__srv__AddTask_Response *
smart_factory_fleet_msgs__srv__AddTask_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  smart_factory_fleet_msgs__srv__AddTask_Response * msg = (smart_factory_fleet_msgs__srv__AddTask_Response *)allocator.allocate(sizeof(smart_factory_fleet_msgs__srv__AddTask_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(smart_factory_fleet_msgs__srv__AddTask_Response));
  bool success = smart_factory_fleet_msgs__srv__AddTask_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
smart_factory_fleet_msgs__srv__AddTask_Response__destroy(smart_factory_fleet_msgs__srv__AddTask_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    smart_factory_fleet_msgs__srv__AddTask_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
smart_factory_fleet_msgs__srv__AddTask_Response__Sequence__init(smart_factory_fleet_msgs__srv__AddTask_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  smart_factory_fleet_msgs__srv__AddTask_Response * data = NULL;

  if (size) {
    data = (smart_factory_fleet_msgs__srv__AddTask_Response *)allocator.zero_allocate(size, sizeof(smart_factory_fleet_msgs__srv__AddTask_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = smart_factory_fleet_msgs__srv__AddTask_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        smart_factory_fleet_msgs__srv__AddTask_Response__fini(&data[i - 1]);
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
smart_factory_fleet_msgs__srv__AddTask_Response__Sequence__fini(smart_factory_fleet_msgs__srv__AddTask_Response__Sequence * array)
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
      smart_factory_fleet_msgs__srv__AddTask_Response__fini(&array->data[i]);
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

smart_factory_fleet_msgs__srv__AddTask_Response__Sequence *
smart_factory_fleet_msgs__srv__AddTask_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  smart_factory_fleet_msgs__srv__AddTask_Response__Sequence * array = (smart_factory_fleet_msgs__srv__AddTask_Response__Sequence *)allocator.allocate(sizeof(smart_factory_fleet_msgs__srv__AddTask_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = smart_factory_fleet_msgs__srv__AddTask_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
smart_factory_fleet_msgs__srv__AddTask_Response__Sequence__destroy(smart_factory_fleet_msgs__srv__AddTask_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    smart_factory_fleet_msgs__srv__AddTask_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
smart_factory_fleet_msgs__srv__AddTask_Response__Sequence__are_equal(const smart_factory_fleet_msgs__srv__AddTask_Response__Sequence * lhs, const smart_factory_fleet_msgs__srv__AddTask_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!smart_factory_fleet_msgs__srv__AddTask_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
smart_factory_fleet_msgs__srv__AddTask_Response__Sequence__copy(
  const smart_factory_fleet_msgs__srv__AddTask_Response__Sequence * input,
  smart_factory_fleet_msgs__srv__AddTask_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(smart_factory_fleet_msgs__srv__AddTask_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    smart_factory_fleet_msgs__srv__AddTask_Response * data =
      (smart_factory_fleet_msgs__srv__AddTask_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!smart_factory_fleet_msgs__srv__AddTask_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          smart_factory_fleet_msgs__srv__AddTask_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!smart_factory_fleet_msgs__srv__AddTask_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
