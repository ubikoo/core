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
Device CreateDeviceFromGLContext(const size_t deviceIndex);

/// @brief Create an OpenCL buffer object from an OpenGL buffer object.
Buffer CreateFromGLBuffer(
    const Device &device,
    GLuint glbufferId,
    cl_mem_flags flags);

/// @brief Create an OpenCL image object from an OpenGL texture object.
Image CreateFromGLTexture(
    const Device &device,
    GLenum textureTarget,
    GLint miplevel,
    GLuint gltextureId,
    cl_mem_flags flags);

/// @brief Create an OpenCL image object from an OpenGL renderbuffer object.
Image CreateFromGLRenderbuffer(
    const Device &device,
    GLuint glrenderbufferId,
    cl_mem_flags flags);

/// @brief Query a OpenGL memory object used to create an OpenCL memory object.
void GetGLObjectInfo(
    const cl_mem &memObject,
    cl_gl_object_type *globjectType,
    GLuint *globjectId);

/// @brief Query a OpenGL texture object associated with an OpenCL memory object.
void GetGLTextureInfo(
    const cl_mem &memObject,
    cl_gl_texture_info paramName,
    size_t paramValueSize,
    void *paraValue,
    size_t *paramValueSizeRet);

/// @brief Acquire OpenCL memory objects that were created from OpenGL objects.
void AcquireGLObjects(
    const Device &device,
    const std::vector<cl_mem> &memObjects,
    const std::vector<cl_event> *waitList = NULL,
    cl_event *event = NULL);

/// @brief Release OpenCL memory objects that were created from OpenGL objects.
void ReleaseGLObjects(
    const Device &device,
    const std::vector<cl_mem> &memObjects,
    const std::vector<cl_event> *waitList = NULL,
    cl_event *event = NULL);
} // namespace Compute

#endif // COMPUTE_INTEROP_H_
#endif // ENABLE_OPENCL_INTEROP
