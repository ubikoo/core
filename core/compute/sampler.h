//
// sampler.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// // https://opensource.org/licenses/MIT.
//

#ifndef COMPUTE_SAMPLER_H_
#define COMPUTE_SAMPLER_H_

#include <memory>
#include "common.h"

namespace Compute {

///
/// @brief Create an interface to a OpenCL sampler object on the device.
///
struct SamplerObject {
    DeviceObject *mDevice{NULL};
    cl_sampler mId{NULL};
};
struct SamplerDeleter {
    void operator()(SamplerObject *obj) {
        if (obj) { clReleaseSampler(obj->mId); };
        delete obj;
    }
};
using Sampler = std::unique_ptr<SamplerObject, SamplerDeleter>;
Sampler CreateSampler(
    const Device &device,
    cl_bool normalizedCoords,
    cl_addressing_mode addressingMode,
    cl_filter_mode filterMode);

} // Compute

#endif // COMPUTE_SAMPLER_H_
