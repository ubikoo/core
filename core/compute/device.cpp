//
// context.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <iostream>
#include <vector>

#include "common.h"
#include "helpers.h"
#include "device.h"

namespace Compute {

///
/// @brief OpenCL context error callback function.
///
static void CL_CALLBACK ContextCallback(
    const char *error_info,
    const void *private_info,
    size_t cb,
    void *user_data)
{
    std::cerr << "OpenCL context error: " << error_info << std::endl;
}

///
/// @brief Create a device with the specified index on the first platform.
///
Device CreateDevice(const size_t device_index)
{
    DeviceObject *device = new DeviceObject;

    // Get the id of the device with the specified index in the first platform.
    {
        auto platform_ids = GetPlatformIDs();
        device->platform = platform_ids[0];

        auto device_ids = GetDeviceIDs(device->platform);
        ThrowIfNot(device_index < device_ids.size());
        device->id = device_ids[device_index];
    }

    // Create an context with the OpenCL device.
    {
        const cl_context_properties context_properties[] = {
            CL_CONTEXT_PLATFORM,
            (cl_context_properties) device->platform,
            (cl_context_properties) NULL};

        cl_int err;
        device->context = clCreateContext(
            context_properties,         // specify the platform to use
            1,                          // only one device id.
            &device->id,                // pointer to the device handle.
            &ContextCallback,           // register log callback function
            NULL,
            &err);
        ThrowIfFailed(err);
    }

    //Create a command queue on the OpenCL device wiht in-order execution.
    {
        static const cl_command_queue_properties queue_properties = 0;

        cl_int err;
        device->queue = clCreateCommandQueue(
            device->context,
            device->id,
            queue_properties,
            &err);
        ThrowIfFailed(err);
    }

    return Device(device, DeviceDeleter());
}

/// -----------------------------------------------------------------------------
/// @brief Issues all previously queued commands to the device.
///
void DeviceObject::FlushQueue() const
{
    ThrowIfFailed(clFlush(queue));
}

///
/// @brief Blocks until all previously queued commands to the device to complete.
///
void DeviceObject::FinishQueue() const
{
    ThrowIfFailed(clFinish(queue));
}

///
/// @brief Execute a marker command waiting for a list of events to complete,
/// or for all previously enqueued commands to complete.
///
void DeviceObject::QueueMarker(
    const std::vector<cl_event> *wait_list,
    cl_event *event) const
{
    bool has_wait_list = (wait_list && !wait_list->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueMarkerWithWaitList(
        queue,
        has_wait_list ? static_cast<cl_uint>(wait_list->size()) : 0,
        has_wait_list ? wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

///
/// @brief Execute a barrier command which waits for a list of events to complete,
/// or if the list is empty, waits for all commands previously enqueued in the
/// command queue to complete.
///
void DeviceObject::QueueBarrier(
    const std::vector<cl_event> *wait_list,
    cl_event *event) const
{
    bool has_wait_list = (wait_list && !wait_list->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueBarrierWithWaitList(
        queue,
        has_wait_list ? static_cast<cl_uint>(wait_list->size()) : 0,
        has_wait_list ? wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

} // Compute