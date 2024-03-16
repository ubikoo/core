//
// memory.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <array>
#include <vector>

#include "common.h"
#include "helpers.h"
#include "device.h"
#include "memory.h"

namespace Compute {

/// -----------------------------------------------------------------------------
/// @brief Create an interface to a OpenCL buffer memory object on the device.
/// @param size specifies the buffer memory size in bytes to be allocated.
/// @param flags specifies the buffer memory usage. For simplicity, memory can
/// only be allocated on the device, host ptr is NULL and only usage flags are
/// considered valid: CL_MEM_READ_WRITE, CL_MEM_WRITE_ONLY, CL_MEM_READ_ONLY.
///
Buffer CreateBuffer(const Device &device, size_t size, cl_mem_flags flags)
{
    ThrowIfNot(flags == CL_MEM_READ_ONLY
            || flags == CL_MEM_WRITE_ONLY
            || flags == CL_MEM_READ_WRITE);

    BufferObject *buffer = new BufferObject;
    {
        // Store the device object.
        buffer->mDevice = device.get();

        // Create the buffer memory object.
        cl_int err;
        buffer->mId = clCreateBuffer(device->mContext, flags, size, NULL, &err);
        ThrowIfFailed(err);

        // Store actual size of the buffer data store.
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
/// @brief Read from the buffer object to host memory.
/// Blocking command, it will not return until it completes.
///
void BufferObject::Read(
    size_t bufferOffset,
    size_t bufferSize,
    void *ptr,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueReadBuffer(
        mDevice->mQueue,
        mId,
        CL_TRUE,
        bufferOffset,
        bufferSize,
        ptr,
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

///
/// @brief Overloaded function. Read from a buffer object to host memory.
///
void BufferObject::Read(
    void *ptr,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    Read(0, mSize, ptr, waitList, event);
}

///
/// @brief Write to the buffer object from host memory.
/// Blocking command, it will not return until it completes.
///
void BufferObject::Write(
    size_t bufferOffset,
    size_t bufferSize,
    void *ptr,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueWriteBuffer(
        mDevice->mQueue,
        mId,
        CL_TRUE,
        bufferOffset,
        bufferSize,
        ptr,
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

///
/// @brief Overloaded function. Write to a buffer object from host memory.
///
void BufferObject::Write(
    void *ptr,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    Write(0, mSize, ptr, waitList, event);
}

///
/// @brief Fill the buffer object with a pattern of a specified size. Pattern
/// type must be an accepted OpenCL data type.
///
void BufferObject::Fill(
    const void *pattern,
    size_t patternSize,
    size_t bufferOffset,
    size_t bufferSize,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueFillBuffer(
        mDevice->mQueue,
        mId,
        pattern,
        patternSize,
        bufferOffset,
        bufferSize,
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

///
/// @brief Overloaded function. Fill the buffer object with the specified pattern.
///
void BufferObject::Fill(
    const void *pattern,
    size_t patternSize,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    Fill(pattern, patternSize, 0, mSize, waitList, event);
}

///
/// @brief Map an entire buffer object into a host address space.
/// The mapping offset is set to zero and its size is defined by the buffer.
///
void *BufferObject::Map(
    cl_map_flags flags,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    cl_int err;
    void *pointer = clEnqueueMapBuffer(
        mDevice->mQueue,
        mId,
        CL_TRUE,
        flags,
        0,
        mSize,
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
        (event != NULL) ? &tmp : NULL,
        &err);
    ThrowIfFailed(err);
    if (event != NULL) {
        *event = tmp;
    }
    return pointer;
}

///
/// @brief Unmap a previously mapped buffer.
///
void BufferObject::Unmap(
    void *pointer,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueUnmapMemObject(
        mDevice->mQueue,
        mId,
        pointer,
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

/// -----------------------------------------------------------------------------
/// @brief Create a 1d-image memory object with a specified width.
///
Image CreateImage1d(
    const Device &device,
    size_t width,
    const cl_image_format &format,
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

        // Image descriptor describing type and dimensions of a 1d-image.
        // Row pitch and slice pitch must be set to 0 when host ptr is NULL.
        cl_image_desc desc{
            CL_MEM_OBJECT_IMAGE1D,      // cl_mem_object_type image type
            width,                      // width
            0,                          // height
            0,                          // depth
            0,                          // array size (unused)
            0,                          // row pitch
            0,                          // slice pitch
            0,                          // num mip levels
            0,                          // num samples
            0};                         // cl_mem buffer, cl_mem mem_object

        // Create image object.
        cl_int err;
        image->mId = clCreateImage(device->mContext, flags, &format, &desc,
            NULL, &err);
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
/// @brief Create a 2d-image memory object with specified width and height.
///
Image CreateImage2d(
    const Device &device,
    size_t width,
    size_t height,
    const cl_image_format &format,
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

        // Image descriptor describing type and dimensions of a 2d-image.
        // Row pitch and slice pitch must be set to 0 when host ptr is NULL.
        cl_image_desc desc{
            CL_MEM_OBJECT_IMAGE2D,      // cl_mem_object_type image type
            width,                      // width
            height,                     // height
            0,                          // depth
            0,                          // array size (unused)
            0,                          // row pitch
            0,                          // slice pitch
            0,                          // num mip levels
            0,                          // num samples
            0};                         // cl_mem buffer, cl_mem mem_object

        // Create image object.
        cl_int err;
        image->mId = clCreateImage(device->mContext, flags, &format, &desc,
            NULL, &err);
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
/// @brief Create a 3d-image memory object with specified width, height and depth.
///
Image CreateImage3d(
    const Device &device,
    size_t width,
    size_t height,
    size_t depth,
    const cl_image_format &format,
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

        // Image descriptor describing type and dimensions of a 3d-image.
        // Row pitch and slice pitch must be set to 0 when host ptr is NULL.
        cl_image_desc desc{
            CL_MEM_OBJECT_IMAGE3D,      // cl_mem_object_type image type
            width,                      // width
            height,                     // height
            depth,                      // depth
            0,                          // array size (unused)
            0,                          // row pitch
            0,                          // slice pitch
            0,                          // num mip levels
            0,                          // num samples
            0};                         // cl_mem buffer, cl_mem mem_object

        // Create image object.
        cl_int err;
        image->mId = clCreateImage(device->mContext, flags, &format, &desc,
            NULL, &err);
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
/// @brief Read image object to host memory data. Appropriate row and slice
/// pitch values are calculated from image dimensions and element byte size.
///
void ImageObject::Read(
    void *ptr,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueReadImage(
        mDevice->mQueue,
        mId,
        CL_TRUE,
        &mOrigin[0],
        &mRegion[0],
        0,
        0,
        ptr,
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

///
/// @brief Write host memory dataw to image object. Appropriate row and slice
/// pitch values are calculated from image dimensions and element byte size.
///
void ImageObject::Write(
    void *ptr,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueWriteImage(
        mDevice->mQueue,
        mId,
        CL_TRUE,
        &mOrigin[0],
        &mRegion[0],
        0,
        0,
        ptr,
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

///
/// @brief Fill an image object with a specified color.
/// @param fill_color is the color to use to fill the image. This is a four
/// component RGBA floating-point color value if the image channel data type
/// is not an unnormalized signed or unsigned data type.
///
void ImageObject::Fill(
    const cl_float4 fill_color,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueFillImage(
        mDevice->mQueue,
        mId,
        static_cast<const void *>(&fill_color),
        &mOrigin[0],
        &mRegion[0],
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

///
/// @brief Fill an image object with a specified color.
/// @param fill_color is the color to use to fill the image. This is a four
/// component RGBA signed integer color value if the image channel data type
/// is an unnormalized signed integer type.
///
void ImageObject::Fill(
    const cl_int4 fill_color,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueFillImage(
        mDevice->mQueue,
        mId,
        static_cast<const void *>(&fill_color),
        &mOrigin[0],
        &mRegion[0],
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

///
/// @brief Fill an image object with a specified color.
/// @param fill_color is the color to use to fill the image. This is a four
/// component RGBA unsigned integer color value if the image channel data type
/// is an unnormalized signed integer type.
///
void ImageObject::Fill(
    const cl_uint4 fill_color,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueFillImage(
        mDevice->mQueue,
        mId,
        static_cast<const void *>(&fill_color),
        &mOrigin[0],
        &mRegion[0],
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

///
/// @brief Map an entire image object into a host address space.
/// The mapping is specified by the image origin and range values.
///
void *ImageObject::Map(
    cl_map_flags flags,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    bool hasWaitList = (waitList && !waitList->empty());

    cl_event tmp;
    cl_int err;
    void *pointer = clEnqueueMapImage(
        mDevice->mQueue,
        mId,
        CL_TRUE,
        flags,
        &mOrigin[0],
        &mRegion[0],
        0,
        0,
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
        (event != NULL) ? &tmp : NULL,
        &err);
    ThrowIfFailed(err);
    if (event != NULL) {
        *event = tmp;
    }
    return pointer;
}

///
/// @brief Unmap a previously mapped image.
///
void ImageObject::Unmap(
    void *pointer,
    const std::vector<cl_event> *waitList,
    cl_event *event)
{
    bool hasWaitList = (waitList && !waitList->empty());
    cl_event tmp;
    ThrowIfFailed(clEnqueueUnmapMemObject(
        mDevice->mQueue,
        mId,
        pointer,
        hasWaitList ? static_cast<cl_uint>(waitList->size()) : 0,
        hasWaitList ? waitList->data() : NULL,
        (event != NULL) ? &tmp : NULL));
    if (event != NULL) {
        *event = tmp;
    }
}

} // Compute
