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
    const char *errorInfo,
    const void *privateInfo,
    size_t cb,
    void *userData)
{
    std::cerr << "OpenCL context error: " << errorInfo << std::endl;
}

///
/// @brief Create a device with the specified index on the first platform.
///
Device CreateDevice(const size_t deviceIndex)
{
    DeviceObject *device = new DeviceObject;

    // Get the id of the device with the specified index in the first platform.
    {
        auto platformIds = GetPlatformIDs();
        device->mPlatform = platformIds[0];

        auto deviceIds = GetDeviceIDs(device->mPlatform);
        ThrowIfNot(deviceIndex < deviceIds.size());
        device->mId = deviceIds[deviceIndex];
    }

    // Create an context with the OpenCL device.
    {
        const cl_context_properties kContextProperties[] = {
            CL_CONTEXT_PLATFORM,
            (cl_context_properties) device->mPlatform,
            (cl_context_properties) NULL};

        cl_int err;
        device->mContext = clCreateContext(
            kContextProperties,         // specify the platform to use
            1,                          // only one device id.
            &device->mId,                // pointer to the device handle.
            &ContextCallback,           // register log callback function
            NULL,
            &err);
        ThrowIfFailed(err);
    }

    //Create a command queue on the OpenCL device wiht in-order execution.
    {
        static const cl_command_queue_properties kQueueProperties = 0;

        cl_int err;
        device->mQueue = clCreateCommandQueue(
            device->mContext,
            device->mId,
            kQueueProperties,
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
    ThrowIfFailed(clFlush(mQueue));
}

///
/// @brief Blocks until all previously queued commands to the device to complete.
///
void DeviceObject::FinishQueue() const
{
    ThrowIfFailed(clFinish(mQueue));
}

///
/// @brief Execute a marker command waiting for a list of events to complete,
/// or for all previously enqueued commands to complete.
///
void DeviceObject::QueueMarker(
    const std::vector<cl_event> *waitList,
    cl_event *event) const
{
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueMarkerWithWaitList(
        mQueue,
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
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
    const std::vector<cl_event> *waitList,
    cl_event *event) const
{
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueBarrierWithWaitList(
        mQueue,
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

} // namespace Compute