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
    const char *errorInfo,
    const void *privateInfo,
    size_t cb,
    void *userData)
{
    std::cerr << "OpenCL/OpenGL context error: " << errorInfo << std::endl;
}

/// -----------------------------------------------------------------------------
/// @brief Create a device with a shared OpenCL/OpenGL context.
///
Device CreateDeviceFromGLContext(const size_t deviceIndex)
{
    DeviceObject *device = new DeviceObject;

    // Get the id of the device with the specified index in the first platform.
    {
        auto platformIds = GetPlatformIDs();
        device->mPlatform = platformIds[0];

        auto deviceIds = GetDeviceIDs(device->mPlatform);
        ThrowIfNot(deviceIndex < deviceIds.size());
        device->mId = deviceIds[deviceIndex];
    }

    // Create a shared OpenCL context based on the active OpenGL context.
    {
        // Get the Core OpenGL context object and sharegroup.
#if defined(__APPLE__)
        CGLContextObj cglContext = CGLGetCurrentContext();
        CGLShareGroupObj cglShareGroup = CGLGetShareGroup(cglContext);
        const cl_context_properties kContextProperties[] = {
            CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
            (cl_context_properties) cglShareGroup,
            (cl_context_properties) NULL};
#else
        const cl_context_properties kContextProperties[] = {
            CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
            CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay(),
            CL_CONTEXT_PLATFORM, (cl_context_properties) platform,
            (cl_context_properties) NULL};
#endif

        // Create the OpenCL context based on the OpenGL context.
        cl_int err;
        device->mContext = clCreateContext(
            kContextProperties,         // specify the platform to use
            1,                          // only one device id
            &device->mId,                // pointer to the device handle
            &ContextCallback,           // register log callback function
            NULL,
            &err);
        ThrowIfFailed(err);
    }

    //Create a command queue on the OpenCL device wiht in-order execution.
    {
        static const cl_command_queue_properties queueProperties = 0;

        cl_int err;
        device->mQueue = clCreateCommandQueue(
            device->mContext,
            device->mId,
            queueProperties,
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
    GLuint glbufferId,
    cl_mem_flags flags)
{
    ThrowIfNot(flags == CL_MEM_READ_ONLY
        || flags == CL_MEM_WRITE_ONLY
        || flags == CL_MEM_READ_WRITE);

    BufferObject *buffer = new BufferObject;
    {
        // Store the device object.
        buffer->mDevice = device.get();

        // Create buffer object
        cl_int err;
        buffer->mId = clCreateFromGLBuffer(
            device->mContext,
            flags,
            glbufferId,
            &err);
        ThrowIfFailed(err);

        // Store actual size of the data store.
        ThrowIfFailed(clGetMemObjectInfo(
            buffer->mId,
            CL_MEM_SIZE,
            sizeof(size_t),
            &buffer->mSize,
            NULL));
    }
    return Buffer(buffer, BufferDeleter());
}

///
/// @brief Create an OpenCL image object from an OpenGL texture object.
///
Image CreateFromGLTexture(
    const Device &device,
    GLenum textureTarget,
    GLint miplevel,
    GLuint gltextureId,
    cl_mem_flags flags)
{
    ThrowIfNot(textureTarget == GL_TEXTURE_1D
            || textureTarget == GL_TEXTURE_2D
            || textureTarget == GL_TEXTURE_3D
            || textureTarget == GL_TEXTURE_BUFFER);
    ThrowIfNot(miplevel == 0);
    ThrowIfNot(flags == CL_MEM_READ_ONLY
            || flags == CL_MEM_WRITE_ONLY
            || flags == CL_MEM_READ_WRITE);

    // Create an Image object
    ImageObject *image = new ImageObject;
    {
        // Store the device object.
        image->mDevice = device.get();

        // Create image object.
        cl_int err;
        image->mId = clCreateFromGLTexture(
            device->mContext,
            flags,
            textureTarget,
            miplevel,
            gltextureId,
            &err);
        ThrowIfFailed(err);

        // Store image origin and region.
        image->mOrigin = {0, 0, 0};
        ThrowIfFailed(clGetImageInfo(image->mId, CL_IMAGE_WIDTH, sizeof(size_t),
            &image->mRegion[0], NULL));
        ThrowIfFailed(clGetImageInfo(image->mId, CL_IMAGE_HEIGHT, sizeof(size_t),
            &image->mRegion[1], NULL));
        ThrowIfFailed(clGetImageInfo(image->mId, CL_IMAGE_DEPTH, sizeof(size_t),
            &image->mRegion[2], NULL));
    }
    return Image(image, ImageDeleter());
}

///
/// @brief Create an OpenCL image object from an OpenGL renderbuffer object.
///
Image CreateFromGLRenderbuffer(
    const Device &device,
    GLuint glrenderbufferId,
    cl_mem_flags flags)
{
    ThrowIfNot(flags == CL_MEM_READ_ONLY
            || flags == CL_MEM_WRITE_ONLY
            || flags == CL_MEM_READ_WRITE);

    // Create an Image object
    ImageObject *image = new ImageObject;
    {
        // Store the device object.
        image->mDevice = device.get();

        // Create image object.
        cl_int err;
        image->mId = clCreateFromGLRenderbuffer(
            device->mContext,
            flags,
            glrenderbufferId,
            &err);
        ThrowIfFailed(err);

        // Store image origin and region.
        image->mOrigin = {0, 0, 0};
        ThrowIfFailed(clGetImageInfo(image->mId, CL_IMAGE_WIDTH, sizeof(size_t),
            &image->mRegion[0], NULL));
        ThrowIfFailed(clGetImageInfo(image->mId, CL_IMAGE_HEIGHT, sizeof(size_t),
            &image->mRegion[1], NULL));
        ThrowIfFailed(clGetImageInfo(image->mId, CL_IMAGE_DEPTH, sizeof(size_t),
            &image->mRegion[2], NULL));
    }
    return Image(image, ImageDeleter());
}

/// -----------------------------------------------------------------------------
/// @brief Query a OpenGL memory object used to create an OpenCL memory object.
///
void GetGLObjectInfo(
    const cl_mem &memObject,
    cl_gl_object_type *globjectType,
    GLuint *globjectId)
{
    ThrowIfFailed(clGetGLObjectInfo(memObject, globjectType, globjectId));
}

///
/// @brief Query a OpenGL texture object associated with an OpenCL memory object.
///
void GetGLTextureInfo(
    const cl_mem &memObject,
    cl_gl_texture_info paramName,
    size_t paramValueSize,
    void *paramValue,
    size_t *paramValueSizeRet)
{
    ThrowIfNot(paramName == CL_GL_TEXTURE_TARGET
            || paramName == CL_GL_MIPMAP_LEVEL);

    ThrowIfFailed(clGetGLTextureInfo(
        memObject,
        paramName,
        paramValueSize,
        paramValue,
        paramValueSizeRet));
}

/// -----------------------------------------------------------------------------
/// @brief Acquire OpenCL memory objects that were created from OpenGL objects.
///
void AcquireGLObjects(
    const Device &device,
    const std::vector<cl_mem> &memObjects,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    // Ensure OpenGL commands are finished before acquiring the memory objects.
    glFinish();

    // Acquire the shared OpenGL memory objects.
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueAcquireGLObjects(
        device->mQueue,
        static_cast<cl_uint>(memObjects.size()),
        memObjects.data(),
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
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
    const std::vector<cl_mem> &memObjects,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    // Ensure OpenCL commands are finished before releasing the memory objects.
    device->FinishQueue();

    // Release the shared OpenGL memory objects.
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueReleaseGLObjects(
        device->mQueue,
        static_cast<cl_uint>(memObjects.size()),
        memObjects.data(),
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

} // namespace Compute
#endif // ENABLE_OPENCL_INTEROP
