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
        event->device = device.get();

        cl_int err;
        event->id = clCreateUserEvent(device->context, &err);
        ThrowIfFailed(err);
    }
    return Event(event, EventDeleter());
}

///
/// @brief Wait for commands identified by all event objects to complete.
///
void WaitForEvents(const std::vector<cl_event> &events)
{
    ThrowIfFailed(clWaitForEvents(
        static_cast<cl_uint>(events.size()),
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
    cl_int command_exec_callback_type,
  	void (CL_CALLBACK *pfn_event_notify) (
        cl_event event,
        cl_int event_command_exec_status,
        void *user_data),
  	void *user_data)
{
    ThrowIfFailed(clSetEventCallback(
        event,
        command_exec_callback_type,
        pfn_event_notify,
        user_data));
}

///
/// @brief Return a 64-bit value representing the current time counter in
/// nanoseconds when the command identified by the event starts execution.
///
cl_ulong GetCommandStart(const cl_event &event)
{
    cl_ulong time_start;
    ThrowIfFailed(clGetEventProfilingInfo(
        event,
        CL_PROFILING_COMMAND_START,
        sizeof(time_start),
        &time_start,
        NULL));
    return time_start;
}

///
/// @brief Return a 64-bit value representing the current time counter in
/// nanoseconds when the command identified by the event finishes execution.
///
cl_ulong GetCommandEnd(const cl_event &event)
{
    cl_ulong time_end;
    ThrowIfFailed(clGetEventProfilingInfo(
        event,
        CL_PROFILING_COMMAND_END,
        sizeof(time_end),
        &time_end,
        NULL));
    return time_end;
}

} // Compute
