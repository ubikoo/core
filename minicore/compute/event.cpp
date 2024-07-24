//
// event.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <vector>

#include "common.h"
#include "helpers.h"
#include "device.h"
#include "event.h"

namespace Compute {

///
/// @brief Create a user event object.
///
Event CreateEvent(const Device &device)
{
    EventObject *event = new EventObject;
    {
        event->mDevice = device.get();

        cl_int err;
        event->mId = clCreateUserEvent(device->mContext, &err);
        ThrowIfFailed(err);
    }
    return Event(event, EventDeleter());
}

///
/// @brief Wait for commands identified by all event objects to complete.
///
void WaitForEvents(const std::vector<cl_event> &events)
{
    ThrowIfFailed(clWaitForEvents(static_cast<cl_uint>(events.size()),
        events.data()));
}

///
/// @brief Wait for commands identified by the event object to complete.
///
void WaitForEvent(const cl_event &event)
{
    ThrowIfFailed(clWaitForEvents(1, &event));
}

///
/// @brief Register a callback function for a specific command execution status.
///
void SetEventCallback(
    const cl_event &event,
    cl_int commandExecCallbackType,
    void (CL_CALLBACK *pfn_event_notify) (
        cl_event event,
        cl_int eventCommandExecStatus,
        void *userData),
    void *userData)
{
    ThrowIfFailed(clSetEventCallback(
        event,
        commandExecCallbackType,
        pfn_event_notify,
        userData));
}

///
/// @brief Return a 64-bit value representing the current time counter in
/// nanoseconds when the command identified by the event starts execution.
///
cl_ulong GetCommandStart(const cl_event &event)
{
    cl_ulong timeStart;
    ThrowIfFailed(clGetEventProfilingInfo(
        event,
        CL_PROFILING_COMMAND_START,
        sizeof(timeStart),
        &timeStart,
        NULL));
    return timeStart;
}

///
/// @brief Return a 64-bit value representing the current time counter in
/// nanoseconds when the command identified by the event finishes execution.
///
cl_ulong GetCommandEnd(const cl_event &event)
{
    cl_ulong timeEnb;
    ThrowIfFailed(clGetEventProfilingInfo(
        event,
        CL_PROFILING_COMMAND_END,
        sizeof(timeEnb),
        &timeEnb,
        NULL));
    return timeEnb;
}

} // namespace Compute
