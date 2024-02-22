//
// interop.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#if defined(ENABLE_OPENCL_INTEROP)
#include <iostream>
#include <vector>
#include "common.h"
#include "helpers.h"
#include "device.h"
#include "memory.h"
#include "interop.h"

namespace Compute {

/// -----------------------------------------------------------------------------
/// @brief OpenCL context error callback function.
///
static void CL_CALLBACK ContextCallback(
    const char *error_info,
    const void *private_info,
    size_t cb,
    void *user_data)
{
    std::cerr << "OpenCL/OpenGL context error: " << error_info << std::endl;
}

/// -----------------------------------------------------------------------------
/// @brief Create a device with a shared OpenCL/OpenGL context.
///
Device CreateDeviceFromGLContext(const size_t device_index)
{
    DeviceObject *device = new DeviceObject;

    // Get the id of the device with the specified index in the first platform.
    {
        auto platform_ids = GetPlatformIDs();
        device->platform = platform_ids[0];

        auto device_ids = GetDeviceIDs(device->platform);
        ThrowIfNot(device_index < device_ids.size());
        device->id = device_ids[device_index];
    }

    // Create a shared OpenCL context based on the active OpenGL context.
    {
        // Get the Core OpenGL context object and sharegroup.
#if defined(__APPLE__)
        CGLContextObj cgl_context = CGLGetCurrentContext();
        CGLShareGroupObj cgl_sharegroup = CGLGetShareGroup(cgl_context);
        const cl_context_properties context_properties[] = {
            CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
            (cl_context_properties) cgl_sharegroup,
            (cl_context_properties) NULL};
#else
        const cl_context_properties context_properties[] = {
            CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
            CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay(),
            CL_CONTEXT_PLATFORM, (cl_context_properties) platform,
            (cl_context_properties) NULL};
#endif

        // Create the OpenCL context based on the OpenGL context.
        cl_int err;
        device->context = clCreateContext(
            context_properties,         // specify the platform to use
            1,                          // only one device id
            &device->id,                // pointer to the device handle
            &ContextCallback,           // register log callback function
            NULL,
            &err);
        ThrowIfFailed(err);
    }

    //Create a command queue on the OpenCL device wiht in-order execution.
    {
        static const cl_command_queue_properties queue_properties = 0;

        cl_int err;
        device->queue = clCreateCommandQueue(
            device->context,
            device->id,
            queue_properties,
            &err);
        ThrowIfFailed(err);
    }

    return Device(device, DeviceDeleter());
}

/// -----------------------------------------------------------------------------
/// @brief Create an OpenCL buffer object from an OpenGL buffer object.
///
Buffer CreateFromGLBuffer(
    const Device &device,
    GLuint gl_buffer,
    cl_mem_flags flags)
{
    ThrowIfNot(flags == CL_MEM_READ_ONLY
            || flags == CL_MEM_WRITE_ONLY
            || flags == CL_MEM_READ_WRITE);

    BufferObject *buffer = new BufferObject;
    {
        // Store the device object.
        buffer->device = device.get();

        // Create buffer object
        cl_int err;
        buffer->id = clCreateFromGLBuffer(
            device->context,
            flags,
            gl_buffer,
            &err);
        ThrowIfFailed(err);

        // Store actual size of the data store.
        ThrowIfFailed(clGetMemObjectInfo(
            buffer->id,
            CL_MEM_SIZE,
            sizeof(size_t),
            &buffer->size,
            NULL));
    }
    return Buffer(buffer, BufferDeleter());
}

///
/// @brief Create an OpenCL image object from an OpenGL texture object.
///
Image CreateFromGLTexture(
    const Device &device,
    GLenum texture_target,
    GLint miplevel,
    GLuint gl_texture,
  	cl_mem_flags flags)
{
    ThrowIfNot(texture_target == GL_TEXTURE_1D
            || texture_target == GL_TEXTURE_2D
            || texture_target == GL_TEXTURE_3D
            || texture_target == GL_TEXTURE_BUFFER);
    ThrowIfNot(miplevel == 0);
    ThrowIfNot(flags == CL_MEM_READ_ONLY
            || flags == CL_MEM_WRITE_ONLY
            || flags == CL_MEM_READ_WRITE);

    // Create an Image object
    ImageObject *image = new ImageObject;
    {
        // Store the device object.
        image->device = device.get();

        // Create image object.
        cl_int err;
        image->id = clCreateFromGLTexture(
            device->context,
            flags,
            texture_target,
            miplevel,
            gl_texture,
            &err);
        ThrowIfFailed(err);

        // Store image origin and region.
        image->origin = {0, 0, 0};
        ThrowIfFailed(clGetImageInfo(image->id, CL_IMAGE_WIDTH, sizeof(size_t),
            &image->region[0], NULL));
        ThrowIfFailed(clGetImageInfo(image->id, CL_IMAGE_HEIGHT, sizeof(size_t),
            &image->region[1], NULL));
        ThrowIfFailed(clGetImageInfo(image->id, CL_IMAGE_DEPTH, sizeof(size_t),
            &image->region[2], NULL));
    }
    return Image(image, ImageDeleter());
}

///
/// @brief Create an OpenCL image object from an OpenGL renderbuffer object.
///
Image CreateFromGLRenderbuffer(
    const Device &device,
    GLuint gl_renderbuffer,
    cl_mem_flags flags)
{
    ThrowIfNot(flags == CL_MEM_READ_ONLY
            || flags == CL_MEM_WRITE_ONLY
            || flags == CL_MEM_READ_WRITE);

    // Create an Image object
    ImageObject *image = new ImageObject;
    {
        // Store the device object.
        image->device = device.get();

        // Create image object.
        cl_int err;
        image->id = clCreateFromGLRenderbuffer(
            device->context,
            flags,
            gl_renderbuffer,
            &err);
        ThrowIfFailed(err);

        // Store image origin and region.
        image->origin = {0, 0, 0};
        ThrowIfFailed(clGetImageInfo(image->id, CL_IMAGE_WIDTH, sizeof(size_t),
            &image->region[0], NULL));
        ThrowIfFailed(clGetImageInfo(image->id, CL_IMAGE_HEIGHT, sizeof(size_t),
            &image->region[1], NULL));
        ThrowIfFailed(clGetImageInfo(image->id, CL_IMAGE_DEPTH, sizeof(size_t),
            &image->region[2], NULL));
    }
    return Image(image, ImageDeleter());
}

/// -----------------------------------------------------------------------------
/// @brief Query a OpenGL memory object used to create an OpenCL memory object.
///
void GetGLObjectInfo(
    const cl_mem &mem_object,
    cl_gl_object_type *gl_object_type,
    GLuint *gl_object_name)
{
    ThrowIfFailed(clGetGLObjectInfo(
        mem_object,
        gl_object_type,
        gl_object_name));
}

///
/// @brief Query a OpenGL texture object associated with an OpenCL memory object.
///
void GetGLTextureInfo(
    const cl_mem &mem_object,
    cl_gl_texture_info param_name,
    size_t param_value_size,
    void *param_value,
    size_t *param_value_size_ret)
{
    ThrowIfNot(param_name == CL_GL_TEXTURE_TARGET
            || param_name == CL_GL_MIPMAP_LEVEL);

    ThrowIfFailed(clGetGLTextureInfo(
        mem_object,
        param_name,
        param_value_size,
        param_value,
        param_value_size_ret));
}

/// -----------------------------------------------------------------------------
/// @brief Acquire OpenCL memory objects that were created from OpenGL objects.
///
void AcquireGLObjects(
    const Device &device,
    const std::vector<cl_mem> &mem_objects,
    const std::vector<cl_event> *wait_list,
    cl_event *event)
{
    // Ensure OpenGL commands are finished before acquiring the memory objects.
    glFinish();

    // Acquire the shared OpenGL memory objects.
    bool has_wait_list = (wait_list && !wait_list->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueAcquireGLObjects(
        device->queue,
        static_cast<cl_uint>(mem_objects.size()),
        mem_objects.data(),
        has_wait_list ? static_cast<cl_uint>(wait_list->size()) : 0,
        has_wait_list ? wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

///
/// @brief Release OpenCL memory objects that were created from OpenGL objects.
///
void ReleaseGLObjects(
    const Device &device,
    const std::vector<cl_mem> &mem_objects,
    const std::vector<cl_event> *wait_list,
    cl_event *event)
{
    // Ensure OpenCL commands are finished before releasing the memory objects.
    device->FinishQueue();

    // Release the shared OpenGL memory objects.
    bool has_wait_list = (wait_list && !wait_list->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueReleaseGLObjects(
        device->queue,
        static_cast<cl_uint>(mem_objects.size()),
        mem_objects.data(),
        has_wait_list ? static_cast<cl_uint>(wait_list->size()) : 0,
        has_wait_list ? wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

} // Compute
#endif // ENABLE_OPENCL_INTEROP
