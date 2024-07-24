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

// OpenCL parameters
static const cl_ulong kDeviceIndex = 3;
static const cl_ulong kWorkGroupSize = 256;
static const cl_ulong kNumIters = 16;
static const cl_ulong kNumIntervals = 2048;
static const cl_ulong kIntervalSteps = 32768;
static const cl_ulong kNumWorkItems = Compute::RoundupRange(
    kNumIntervals, kWorkGroupSize);
static const cl_ulong kNumWorkGroups = kNumWorkItems / kWorkGroupSize;

// OpenMPI parameters
static const int kMasterId = 0;

#endif // COMMON_H_
