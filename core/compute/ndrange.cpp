//
// ndrange.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "ndrange.h"

namespace Compute {

///
/// @brief Round up global work size to the next multiple of local work size.
/// Compute the remainder r of globalWorkSize mod(localWorkSize).
/// If r is zero, globalWorkSize is a multiple of localWorkSize. Otherwise,
///     (globalWorkSize - r)
/// is the largest multiple of localWorkSize smaller than globalWorkSize,
///     (globalWorkSize - r) + localWorkSize
/// is the smallest multiple of localWorkSize larger than globalWorkSize.
///
size_t RoundupRange(size_t globalWorkSize, size_t localWorkSize)
{
    size_t r = globalWorkSize % localWorkSize;
    return (r == 0 ? globalWorkSize : globalWorkSize + localWorkSize - r);
}

///
/// @brief Factory functions.
///
NDRange CreateNDRange(size_t global_0, size_t local_0, size_t offset_0)
{
    return {1,
            {global_0, 0, 0},
            { local_0, 0, 0},
            {offset_0, 0, 0}};
}

NDRange CreateNDRange(
    size_t global_0,
    size_t global_1,
    size_t  local_0,
    size_t  local_1,
    size_t offset_0,
    size_t offset_1)
{
    return {2,
            {global_0, global_1, 0},
            { local_0,  local_1, 0},
            {offset_0, offset_1, 0}};
}

NDRange CreateNDRange(
    size_t global_0,
    size_t global_1,
    size_t global_2,
    size_t  local_0,
    size_t  local_1,
    size_t  local_2,
    size_t offset_0,
    size_t offset_1,
    size_t offset_2)
{
    return {3,
            {global_0, global_1, global_2},
            { local_0,  local_1,  local_1},
            {offset_0, offset_1, offset_2}};
}

} // Compute
