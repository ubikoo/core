//
// common.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef COMPUTE_COMMON_H_
#define COMPUTE_COMMON_H_

#if defined(__APPLE__)
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif // __APPLE__

#if defined(ENABLE_OPENCL_INTEROP)
#include "glad/glad.h"

#if defined(__APPLE__)
#include <OpenCL/cl_gl.h>
#include <OpenCL/cl_gl_ext.h>
#include <OpenGL/CGLTypes.h>
#include <OpenGL/CGLDevice.h>       // CGLGetShareGroup
#include <OpenGL/CGLCurrent.h>      // CGLGetCurrentContext
#else
#include <CL/cl_gl.h>
#include <GL/glx.h>
#endif // __APPLE__
#endif // ENABLE_OPENCL_INTEROP

#endif // COMPUTE_COMMON_H_
