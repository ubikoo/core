//
// ndrange.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef COMPUTE_NDRANGE_H_
#define COMPUTE_NDRANGE_H_

#include <array>
#include "common.h"

namespace Compute {

struct NDRange {
    cl_uint mDimension{};
    std::array<size_t,3> mGlobal{};
    std::array<size_t,3> mLocal{};
    std::array<size_t,3> mOffset{};
};

/// @brief Round up global work size to the next multiple of local work size.
size_t RoundupRange(size_t globalWorkSize, size_t localWorkSize);

/// @brief Factory functions.
NDRange CreateNDRange(size_t global_0, size_t local_0, size_t offset_0);

NDRange CreateNDRange(
    size_t global_0,
    size_t global_1,
    size_t  local_0,
    size_t  local_1,
    size_t offset_0,
    size_t offset_1);

NDRange CreateNDRange(
    size_t global_0,
    size_t global_1,
    size_t global_2,
    size_t  local_0,
    size_t  local_1,
    size_t  local_2,
    size_t offset_0,
    size_t offset_1,
    size_t offset_2);

} // namespace Compute

#endif // COMPUTE_NDRANGE_H_
