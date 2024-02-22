//
// compute.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef COMPUTE_H_
#define COMPUTE_H_

///
/// @brief Helper functions providing an interface to OpenCL.
///
/// The interface is divided in two layers:
///  1. Platform layer
///     Query platform
///     Create device
///     Create command queue
///
///  2. Runtime layer
///     Compile program
///     Build kernel
///     Create buffers
///
/// A typical runtime would have the form:
///     Write buffer data
///     Set kernel arguments
///     Execute kernel
///     Read buffer data
///
#include "common.h"
#include "device.h"
#include "event.h"
#include "helpers.h"
#include "interop.h"
#include "kernel.h"
#include "math.h"
#include "memory.h"
#include "ndrange.h"
#include "program.h"
#include "sampler.h"

#endif // COMPUTE_H_
