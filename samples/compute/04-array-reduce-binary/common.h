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

static const cl_ulong kDeviceIndex = 3;
static const cl_ulong kWorkGroupSize = 256;
static const cl_ulong kNumPoints = 128 * 1024 * 1024;
static const cl_ulong kNumWorkItems = Compute::RoundupRange(
    kNumPoints, kWorkGroupSize);
static const cl_ulong kNumWorkGroups = kNumWorkItems / kWorkGroupSize;

#endif // COMMON_H_
