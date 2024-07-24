//
// common.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef COMMON_H_
#define COMMON_H_

#include "minicore/compute/compute.h"

/// @brief OpenCL parameters.
static const cl_ulong kDeviceIndex = 3;
static const cl_ulong kWorkGroupSize = 256;

/// @brief Model parameters
static const cl_uint kNumIters = 8;
static const cl_uint kLoadFactor = 2;
static const cl_uint kNumPoints = 1048576;
static const cl_uint kCapacity = kLoadFactor * kNumPoints;

static const cl_float3 kDomainLo = {-1.0f, -1.0f, -1.0f};
static const cl_float3 kDomainHi = { 1.0f,  1.0f,  1.0f};
static const cl_uint kNumCells = 16;

static const cl_uint kEmpty = 0xffffffff;   // Empty slot flag
static const cl_uint kMinBits = 3;          // Min capacity 8 items
static const cl_uint kMaxBits = 31;         // Max capacity 2147483648 items
static const cl_uint kMinSize = 1 << kMinBits;
static const cl_uint kMaxSize = 1 << kMaxBits;

/// @brief Common data types.
struct Point {
    cl_float3 pos;
    cl_float3 col;
};

struct KeyValue {
    cl_uint key;
    cl_uint value;
};

#endif // COMMON_H_
