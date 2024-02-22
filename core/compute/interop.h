//
// interop.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#if defined(ENABLE_OPENCL_INTEROP)
#ifndef COMPUTE_INTEROP_H_
#define COMPUTE_INTEROP_H_

#include <vector>
#include "common.h"
#include "device.h"
#include "memory.h"

namespace Compute {

/// @brief Create a device with a shared OpenCL/OpenGL context.
Device CreateDeviceFromGLContext(const size_t device_index);

/// @brief Create an OpenCL buffer object from an OpenGL buffer object.
Buffer CreateFromGLBuffer(
    const Device &device,
    GLuint gl_buffer,
    cl_mem_flags flags);

/// @brief Create an OpenCL image object from an OpenGL texture object.
Image CreateFromGLTexture(
    const Device &device,
    GLenum texture_target,
    GLint miplevel,
    GLuint gl_texture,
  	cl_mem_flags flags);

/// @brief Create an OpenCL image object from an OpenGL renderbuffer object.
Image CreateFromGLRenderbuffer(
    const Device &device,
    GLuint gl_renderbuffer,
    cl_mem_flags flags);

/// @brief Query a OpenGL memory object used to create an OpenCL memory object.
void GetGLObjectInfo(
    const cl_mem &mem_object,
    cl_gl_object_type *gl_object_type,
    GLuint *gl_object_name);

/// @brief Query a OpenGL texture object associated with an OpenCL memory object.
void GetGLTextureInfo(
    const cl_mem &mem_object,
    cl_gl_texture_info param_name,
    size_t param_value_size,
    void *param_value,
    size_t *param_value_size_ret);

/// @brief Acquire OpenCL memory objects that were created from OpenGL objects.
void AcquireGLObjects(
    const Device &device,
    const std::vector<cl_mem> &mem_objects,
    const std::vector<cl_event> *wait_list = NULL,
    cl_event *event = NULL);

/// @brief Release OpenCL memory objects that were created from OpenGL objects.
void ReleaseGLObjects(
    const Device &device,
    const std::vector<cl_mem> &mem_objects,
    const std::vector<cl_event> *wait_list = NULL,
    cl_event *event = NULL);
} // Compute

#endif // COMPUTE_INTEROP_H_
#endif // ENABLE_OPENCL_INTEROP
