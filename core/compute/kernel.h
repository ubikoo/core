//
// kernel.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef COMPUTE_KERNEL_H_
#define COMPUTE_KERNEL_H_

#include <memory>
#include <array>
#include <vector>
#include "common.h"
#include "program.h"
#include "ndrange.h"

namespace Compute {

///
/// @brief Create an interface to a OpenCL kernel object on the device.
///
struct KernelObject {
    DeviceObject *device{NULL};
    cl_kernel id{NULL};
    NDRange ndrange{};

    void Run(const std::vector<cl_event> *wait_list = NULL,
        cl_event *event = NULL);
    void Task(const std::vector<cl_event> *wait_list = NULL,
        cl_event *event = NULL);

    void SetRanges1d(
        const std::array<size_t,1> &num_work_items,
        const std::array<size_t,1> &work_group_size);
    void SetRanges2d(
        const std::array<size_t,2> &num_work_items,
        const std::array<size_t,2> &work_group_size);
    void SetRanges3d(
        const std::array<size_t,3> &num_work_items,
        const std::array<size_t,3> &work_group_size);

    void SetArg(cl_uint index, size_t size, const void *value);
    void SetArg(cl_uint index, const cl_mem *mem);
    void SetArg(cl_uint index, const cl_sampler *sampler);
    template<typename T>
    void SetArg(cl_uint index, const T *value) {
        SetArg(index, sizeof(T), static_cast<const void *>(value));
    }
};
struct KernelDeleter {
    void operator()(KernelObject *obj) {
        if (obj) { clReleaseKernel(obj->id); };
        delete obj;
    }
};
using Kernel = std::unique_ptr<KernelObject, KernelDeleter>;
Kernel CreateKernel(const Program &program, const std::string &name);

} // Compute

#endif // COMPUTE_KERNEL_H_
