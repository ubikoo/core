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
    cl_uint dimension{};
    std::array<size_t,3> global{};
    std::array<size_t,3> local{};
    std::array<size_t,3> offset{};
};

/// @brief Round up global work size to the next multiple of local work size.
size_t RoundupRange(size_t global_work_size, size_t local_work_size);

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

} // Compute

#endif // COMPUTE_NDRANGE_H_
