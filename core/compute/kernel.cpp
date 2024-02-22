//
// kernel.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <array>
#include <vector>
#include <string>

#include "common.h"
#include "helpers.h"
#include "device.h"
#include "program.h"
#include "kernel.h"

namespace Compute {

///
/// @brief Create an interface to a OpenCL kernel object on the device.
///
Kernel CreateKernel(const Program &program, const std::string &name)
{
    ThrowIf(name.empty());

    KernelObject *kernel = new KernelObject;
    {
        // Store the device object.
        kernel->device = program->device;

        // Create the kernel object.
        cl_int err;
        kernel->id = clCreateKernel(program->id, name.c_str(), &err);
        ThrowIfFailed(err);
    }
    return Kernel(kernel, KernelDeleter());
}

///
/// @brief Run the kernel object.
///
void KernelObject::Run(const std::vector<cl_event> *wait_list, cl_event *event)
{
    bool has_wait_list = (wait_list && !wait_list->empty());

    size_t dimension = ndrange.dimension;
    size_t *offset = (ndrange.offset[0] > 0
                   && ndrange.offset[1] > 0
                   && ndrange.offset[2] > 0) ? &ndrange.offset[0] : NULL;
    size_t *global = &ndrange.global[0];
    size_t *local = (ndrange.local[0] > 0
                  && ndrange.local[1] > 0
                  && ndrange.local[2] > 0) ? &ndrange.local[0] : NULL;

    cl_event tmp;
    ThrowIfFailed(clEnqueueNDRangeKernel(
        device->queue,
        id,
        dimension,
        offset,
        global,
        local,
        has_wait_list ? static_cast<cl_uint>(wait_list->size()) : 0,
        has_wait_list ? wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

///
/// @brief Execute a kernel using a single work-item: equivalent to calling
/// clEnqueueNDRangeKernel with dim = 1, offset = NULL, global[0] set to 1,
/// and local[0] set to 1.
///
void KernelObject::Task(const std::vector<cl_event> *wait_list, cl_event *event)
{
    bool has_wait_list = (wait_list && !wait_list->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueTask(
        device->queue,
        id,
        has_wait_list ? static_cast<cl_uint>(wait_list->size()) : 0,
        has_wait_list ? wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

///
/// @brief Specify the 1-dimensional kernel ranges.
///
void KernelObject::SetRanges1d(
    const std::array<size_t,1> &num_work_items,
    const std::array<size_t,1> &work_group_size)
{
    ndrange = CreateNDRange(
        RoundupRange(num_work_items[0], work_group_size[0]),
        work_group_size[0],
        0);
}

///
/// @brief Specify the 2-dimensional kernel ranges.
///
void KernelObject::SetRanges2d(
    const std::array<size_t,2> &num_work_items,
    const std::array<size_t,2> &work_group_size)
{
    ndrange = CreateNDRange(
        RoundupRange(num_work_items[0], work_group_size[0]),
        RoundupRange(num_work_items[1], work_group_size[1]),
        work_group_size[0],
        work_group_size[1],
        0,
        0);
}

///
/// @brief Specify the 3-dimensional kernel ranges.
///
void KernelObject::SetRanges3d(
    const std::array<size_t,3> &num_work_items,
    const std::array<size_t,3> &work_group_size)
{
    ndrange = CreateNDRange(
        RoundupRange(num_work_items[0], work_group_size[0]),
        RoundupRange(num_work_items[1], work_group_size[1]),
        RoundupRange(num_work_items[2], work_group_size[2]),
        work_group_size[0],
        work_group_size[1],
        work_group_size[2],
        0,
        0,
        0);
}

///
/// @brief Set the value for a specific argument of the kernel. For builtin types
/// the template method is equivalent to calling SetArg(index, sizeof(T), &value).
/// Additionally, the specialized versions handle memory objects and samplers.
///
void KernelObject::SetArg(cl_uint index, size_t size, const void *value)
{
    ThrowIfFailed(clSetKernelArg(id, index, size, value));
}

void KernelObject::SetArg(cl_uint index, const cl_mem *mem)
{
    SetArg(index, sizeof(cl_mem), static_cast<const void *>(mem));
}

void KernelObject::SetArg(cl_uint index, const cl_sampler *sampler)
{
    SetArg(index, sizeof(cl_sampler), static_cast<const void *>(sampler));
}

} // Compute
