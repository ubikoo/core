//
// sampler.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "common.h"
#include "helpers.h"
#include "device.h"
#include "sampler.h"

namespace Compute {

///
/// @brief Create an interface to a OpenCL sampler object on the device.
/// @param normalizedCoords Are image coordinates are normalized?
/// @param addressingMode Specify how out-of-range image coordinates are
/// handled when reading from an image:
///  CL_ADDRESS_MIRRORED_REPEAT, CL_ADDRESS_REPEAT, CL_ADDRESS_CLAMP_TO_EDGE,
///  CL_ADDRESS_CLAMP, CL_ADDRESS_NONE.
/// @param filterMode Specify the filter type when reading an image:
///  CL_FILTER_NEAREST, CL_FILTER_LINEAR.
///
Sampler CreateSampler(
    const Device &device,
    cl_bool normalizedCoords,
    cl_addressing_mode addressingMode,
    cl_filter_mode filterMode)
{
    SamplerObject *sampler = new SamplerObject;
    {
        // Store the device object.
        sampler->mDevice = device.get();

        // Create the sampler object.
        cl_int err;
        sampler->mId = clCreateSampler(
            device->mContext,
            normalizedCoords,
            addressingMode,
            filterMode,
            &err);
        ThrowIfFailed(err);
    }
    return Sampler(sampler, SamplerDeleter());
}

} // namespace Compute
