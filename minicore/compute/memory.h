//
// memory.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef COMPUTE_MEMORY_H_
#define COMPUTE_MEMORY_H_

#include <memory>
#include <array>
#include <vector>
#include "common.h"

namespace Compute {

/// -----------------------------------------------------------------------------
/// @brief Create an interface to a OpenCL buffer memory object on the device.
///
struct BufferObject {
    DeviceObject *mDevice{NULL};
    cl_mem mId{NULL};
    size_t mSize{};

    /// @brief Read from the buffer object to host memory.
    void Read(
        size_t bufferOffset,
        size_t bufferSize,
        void *ptr,
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL);

    void Read(
        void *ptr,
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL);

    /// @brief Write to the buffer object from host memory.
    void Write(
        size_t bufferOffset,
        size_t bufferSize,
        void *ptr,
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL);

    void Write(
        void *ptr,
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL);

    /// @brief Fill the buffer object with a pattern of a specified size.
    void Fill(
        const void *pattern,
        size_t patternSize,
        size_t bufferOffset,
        size_t bufferSize,
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL);

    void Fill(
        const void *pattern,
        size_t patternSize,
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL);

    /// @brief Map an entire buffer object into a host address space.
    void *Map(
        cl_map_flags flags,
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL);

    /// @brief Unmap a previously mapped buffer.
    void Unmap(
        void *pointer,
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL);
};
struct BufferDeleter {
    void operator()(BufferObject *obj) {
        if (obj) { clReleaseMemObject(obj->mId); };
        delete obj;
    }
};
using Buffer = std::unique_ptr<BufferObject, BufferDeleter>;
Buffer CreateBuffer(const Device &device, size_t size, cl_mem_flags flags);

/// -----------------------------------------------------------------------------
/// @brief Create an interface to a OpenCL image memory object on the device.
///
struct ImageObject {
    DeviceObject *mDevice{NULL};
    cl_mem mId{NULL};
    std::array<size_t,3> mOrigin{};
    std::array<size_t,3> mRegion{};

    /// @brief Read image object to host memory data.
    void Read(
        void *ptr,
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL);

    /// @brief Write host memory dataw to image object.
    void Write(
        void *ptr,
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL);

    /// @brief Fill an image object with a specified color.
    void Fill(
        const cl_float4 fillColor,
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL);

    /// @brief Fill an image object with a specified color.
    void Fill(
        const cl_int4 fillColor,
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL);

    /// @brief Fill an image object with a specified color.
    void Fill(
        const cl_uint4 fillColor,
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL);

    /// @brief Map an entire image object into a host address space.
    void *Map(
        cl_map_flags flags,
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL);

    /// @brief Unmap a previously mapped image.
    void Unmap(
        void *pointer,
        const std::vector<cl_event> *waitList = NULL,
        cl_event *event = NULL);
};
struct ImageDeleter {
    void operator()(ImageObject *obj) {
        if (obj) { clReleaseMemObject(obj->mId); };
        delete obj;
    }
};
using Image = std::unique_ptr<ImageObject, ImageDeleter>;

/// @brief Create a 1d-image memory object with a specified width.
Image CreateImage1d(
    const Device &device,
    size_t width,
    const cl_image_format &format,
    cl_mem_flags flags);

/// @brief Create a 2d-image memory object with specified width and height.
Image CreateImage2d(
    const Device &device,
    size_t width,
    size_t height,
    const cl_image_format &format,
    cl_mem_flags flags);

/// @brief Create a 3d-image memory object with specified width, height and depth.
Image CreateImage3d(
    const Device &device,
    size_t width,
    size_t height,
    size_t depth,
    const cl_image_format &format,
    cl_mem_flags flags);

} // namespace Compute

#endif // COMPUTE_MEMORY_H_
