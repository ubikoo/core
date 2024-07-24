//
// helpers.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef COMPUTE_HELPERS_H_
#define COMPUTE_HELPERS_H_

#include <string>
#include <vector>
#include <exception>
#include "common.h"
#include "minicore/base/error.h"

namespace Compute {

using Base::ThrowIf;
using Base::ThrowIfNot;

/// -----------------------------------------------------------------------------
/// @brief Fast-fail helper function.
///
inline void ThrowIfFailed(cl_int result) {
    Base::ThrowIfNot(result == CL_SUCCESS);
}

/// -----------------------------------------------------------------------------
/// @brief Return a vector of all platforms currently available.
std::vector<cl_platform_id> GetPlatformIDs();

/// @brief Query information about a specified platform.
struct PlatformInfo {
    std::string profile;            // OpenCL profile name.
    std::string version;            // OpenCL version string.
    std::string name;               // Platform name string.
    std::string vendor;             // Platform vendor string.
    std::string extensions;         // List of supported extensions.
};
PlatformInfo GetPlatformInfo(const cl_platform_id &platform);

/// @brief Return the platform information in reader friendly string format.
std::string ToString(const PlatformInfo &info);

/// -----------------------------------------------------------------------------
/// @brief Return all devices of a given type in the specified platform.
std::vector<cl_device_id> GetDeviceIDs(
    const cl_platform_id &platform,
    cl_device_type type = CL_DEVICE_TYPE_ALL);

/// @brief Return the id of the first device with a given name in the platform.
cl_device_id GetDeviceID(const cl_platform_id &platform, std::string name);

/// @brief Query information about a specified device.
struct DeviceInfo {
    std::string deviceName;                         // CL_DEVICE_NAME
    std::string deviceVendor;                       // CL_DEVICE_VENDOR
    std::string deviceVersion;                      // CL_DEVICE_VERSION
    std::string deviceExtensions;                   // CL_DEVICE_EXTENSIONS
    cl_device_type deviceType;                      // CL_DEVICE_TYPE

    cl_ulong globalMemCacheSize;                    // CL_DEVICE_GLOBAL_MEM_CACHE_SIZE
    cl_device_mem_cache_type globalMemCacheType;    // CL_DEVICE_GLOBAL_MEM_CACHE_TYPE
    cl_uint globalMemCachelineSize;                 // CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE
    cl_ulong globalMemSize;                         // CL_DEVICE_GLOBAL_MEM_SIZE
    cl_ulong localMemSize;                          // CL_DEVICE_LOCAL_MEM_SIZE
    cl_device_local_mem_type localMemType;          // CL_DEVICE_LOCAL_MEM_TYPE

    cl_uint maxClockFrequency;                      // CL_DEVICE_MAX_CLOCK_FREQUENCY
    cl_ulong maxConstantBufferSize;                 // CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE
    cl_ulong maxMemAllocSize;                       // CL_DEVICE_MAX_MEM_ALLOC_SIZE
    cl_ulong maxParameterSize;                      // CL_DEVICE_MAX_PARAMETER_SIZE
    cl_uint maxComputeUnits;                        // CL_DEVICE_MAX_COMPUTE_UNITS
    size_t maxWorkGroupSize;                        // CL_DEVICE_MAX_WORK_GROUP_SIZE
    cl_uint maxWorkItemDimensions;                  // CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS
    std::vector<size_t> maxWorkItemSizes;           // CL_DEVICE_MAX_WORK_ITEM_SIZES

    bool imageSupport;                              // CL_DEVICE_IMAGE_SUPPORT
    size_t image2dMaxWidth;                         // CL_DEVICE_IMAGE2D_MAX_WIDTH
    size_t image2dMaxHeight;                        // CL_DEVICE_IMAGE2D_MAX_HEIGHT
    size_t image3dMaxWidth;                         // CL_DEVICE_IMAGE3D_MAX_WIDTH
    size_t image3dMaxHeight;                        // CL_DEVICE_IMAGE3D_MAX_HEIGHT
    size_t image3dMaxDepth;                         // CL_DEVICE_IMAGE3D_MAX_DEPTH
    size_t imageMaxBufferSize;                      // CL_DEVICE_IMAGE_MAX_BUFFER_SIZE
    size_t imageMaxArraySize;                       // CL_DEVICE_IMAGE_MAX_ARRAY_SIZE
};
DeviceInfo GetDeviceInfo(const cl_device_id &device);

/// @brief Return the device information in reader friendly string format.
std::string ToString(const DeviceInfo &info);

/// -----------------------------------------------------------------------------
/// @brief Return all devices in the specified context.
std::vector<cl_device_id> GetContextDevices(const cl_context &context);

/// -----------------------------------------------------------------------------
/// @brief Return the image bitmap as normalized OpenCL floating point data type.
std::vector<cl_float> ImageAsFloat(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp);

/// @brief Return the image bitmap as normalized OpenCL 2d-vector data type.
std::vector<cl_float2> ImageAsFloat2(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp);

/// @brief Return the image bitmap as normalized OpenCL 3d-vector data type.
///
std::vector<cl_float3> ImageAsFloat3(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp);

/// @brief Return the image bitmap as normalized OpenCL 4d-vector data type.
std::vector<cl_float4> ImageAsFloat4(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp);

} // namespace Compute

#endif // COMPUTE_HELPERS_H_
