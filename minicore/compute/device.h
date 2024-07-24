//
// device.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef COMPUTE_DEVICE_H_
#define COMPUTE_DEVICE_H_

#include <vector>
#include "common.h"

namespace Compute {

///
/// @brief Create an interface to a OpenCL device, its associated context and
/// command queue.
/// For simplicity use the first available platform. The default factory function
/// takes the index fo the device to use in the list of all platform device ids.
/// The alternative factory function takes the index of the GPU device associated
/// with the OpenGL context and creates a shared context.
/// The deleter functor takes care of releasing the OpenCL device, its associated
/// context and command queue.
///
struct DeviceObject {
    cl_platform_id mPlatform{NULL};
    cl_device_id mId{NULL};
    cl_context mContext{NULL};
    cl_command_queue mQueue{NULL};

    void FlushQueue() const;
    void FinishQueue() const;
    void QueueMarker(
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL) const;
    void QueueBarrier(
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL) const;
};
struct DeviceDeleter {
    void operator()(DeviceObject *obj) {
        if (obj) {
            clReleaseCommandQueue(obj->mQueue);
            clReleaseContext(obj->mContext);
            clReleaseDevice(obj->mId);
        };
        delete obj;
    }
};
using Device = std::unique_ptr<DeviceObject, DeviceDeleter>;

/// @brief Create a device with the specified index on the first platform.
Device CreateDevice(const size_t deviceIndex);

} // namespace Compute

#endif // COMPUTE_DEVICE_H_
