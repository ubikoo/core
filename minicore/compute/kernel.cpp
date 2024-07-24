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
        kernel->mDevice = program->mDevice;

        // Create the kernel object.
        cl_int err;
        kernel->mId = clCreateKernel(program->mId, name.c_str(), &err);
        ThrowIfFailed(err);
    }
    return Kernel(kernel, KernelDeleter());
}

///
/// @brief Run the kernel object.
///
void KernelObject::Run(const std::vector<cl_event> *waitList, cl_event *event)
{
    bool hasWaitList = (waitList && !waitList->empty());

    size_t dimension = mNDRange.mDimension;
    size_t *offset = (mNDRange.mOffset[0] > 0
                   && mNDRange.mOffset[1] > 0
                   && mNDRange.mOffset[2] > 0) ? &mNDRange.mOffset[0] : NULL;
    size_t *global = &mNDRange.mGlobal[0];
    size_t *local = (mNDRange.mLocal[0] > 0
                  && mNDRange.mLocal[1] > 0
                  && mNDRange.mLocal[2] > 0) ? &mNDRange.mLocal[0] : NULL;

    cl_event tmp;
    ThrowIfFailed(clEnqueueNDRangeKernel(
        mDevice->mQueue,
        mId,
        dimension,
        offset,
        global,
        local,
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
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
void KernelObject::Task(const std::vector<cl_event> *waitList, cl_event *event)
{
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueTask(
        mDevice->mQueue,
        mId,
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

///
/// @brief Specify the 1-dimensional kernel ranges.
///
void KernelObject::SetRanges1d(
    const std::array<size_t,1> &numWorkItems,
    const std::array<size_t,1> &workGroupSize)
{
    mNDRange = CreateNDRange(
        RoundupRange(numWorkItems[0], workGroupSize[0]),
        workGroupSize[0],
        0);
}

///
/// @brief Specify the 2-dimensional kernel ranges.
///
void KernelObject::SetRanges2d(
    const std::array<size_t,2> &numWorkItems,
    const std::array<size_t,2> &workGroupSize)
{
    mNDRange = CreateNDRange(
        RoundupRange(numWorkItems[0], workGroupSize[0]),
        RoundupRange(numWorkItems[1], workGroupSize[1]),
        workGroupSize[0],
        workGroupSize[1],
        0,
        0);
}

///
/// @brief Specify the 3-dimensional kernel ranges.
///
void KernelObject::SetRanges3d(
    const std::array<size_t,3> &numWorkItems,
    const std::array<size_t,3> &workGroupSize)
{
    mNDRange = CreateNDRange(
        RoundupRange(numWorkItems[0], workGroupSize[0]),
        RoundupRange(numWorkItems[1], workGroupSize[1]),
        RoundupRange(numWorkItems[2], workGroupSize[2]),
        workGroupSize[0],
        workGroupSize[1],
        workGroupSize[2],
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
    ThrowIfFailed(clSetKernelArg(mId, index, size, value));
}

void KernelObject::SetArg(cl_uint index, const cl_mem *mem)
{
    SetArg(index, sizeof(cl_mem), static_cast<const void *>(mem));
}

void KernelObject::SetArg(cl_uint index, const cl_sampler *sampler)
{
    SetArg(index, sizeof(cl_sampler), static_cast<const void *>(sampler));
}

} // namespace Compute
