//
// event.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef COMPUTE_EVENT_H_
#define COMPUTE_EVENT_H_

#include <vector>
#include "common.h"

namespace Compute {

///
/// @brief Create an interface to a OpenCL synchronization event object.
///
struct EventObject {
    DeviceObject *mDevice{NULL};
    cl_event mId{NULL};
};
struct EventDeleter {
    void operator()(EventObject *obj) {
        if (obj) { clReleaseEvent(obj->mId); };
        delete obj;
    }
};
using Event = std::unique_ptr<EventObject, EventDeleter>;
Event CreateEvent();

/// @brief Wait for commands identified by all event objects to complete.
void WaitForEvents(const std::vector<cl_event> &events);

/// @brief Wait for commands identified by the event object to complete.
void WaitForEvent(const cl_event &event);

/// @brief Register a callback function for a specific command execution status.
void SetEventCallback(
    const cl_event &event,
    cl_int commandExecCallbackType,
    void (CL_CALLBACK *pfn_event_notify) (
        cl_event event,
        cl_int eventCommandExecStatus,
        void *userData),
    void *userData);

/// @brief Return a 64-bit value representing the current time counter in
/// nanoseconds when the command identified by the event starts execution.
cl_ulong GetCommandStart(const cl_event &event);

/// @brief Return a 64-bit value representing the current time counter in
/// nanoseconds when the command identified by the event finishes execution.
cl_ulong GetCommandEnd(const cl_event &event);

} // Compute

#endif // COMPUTE_EVENT_H_
