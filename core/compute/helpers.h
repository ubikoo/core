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
#include "common.h"

namespace Compute {

/// -----------------------------------------------------------------------------
/// @brief Fast-fail helper function.
///
inline void ThrowIf(bool result) {
#ifndef NDEBUG
    if (result) {
        throw std::exception();
    }
#endif
}

inline void ThrowIfNot(bool result) {
    ThrowIf(!result);
}

inline void ThrowIfFailed(cl_int result) {
    ThrowIfNot(result == CL_SUCCESS);
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
    std::string name;                               // CL_DEVICE_NAME
    std::string vendor;                             // CL_DEVICE_VENDOR
    std::string version;                            // CL_DEVICE_VERSION
    std::string extensions;                         // CL_DEVICE_EXTENSIONS
    cl_device_type type;                            // CL_DEVICE_TYPE

    cl_ulong global_mem_cache_size;                 // CL_DEVICE_GLOBAL_MEM_CACHE_SIZE
    cl_device_mem_cache_type global_mem_cache_type; // CL_DEVICE_GLOBAL_MEM_CACHE_TYPE
    cl_uint global_mem_cacheline_size;              // CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE
    cl_ulong global_mem_size;                       // CL_DEVICE_GLOBAL_MEM_SIZE
    cl_ulong local_mem_size;                        // CL_DEVICE_LOCAL_MEM_SIZE
    cl_device_local_mem_type local_mem_type;        // CL_DEVICE_LOCAL_MEM_TYPE

    cl_uint max_clock_frequency;                    // CL_DEVICE_MAX_CLOCK_FREQUENCY
    cl_ulong max_constant_buffer_size;              // CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE
    cl_ulong max_mem_alloc_size;                    // CL_DEVICE_MAX_MEM_ALLOC_SIZE
    cl_ulong max_parameter_size;                    // CL_DEVICE_MAX_PARAMETER_SIZE
    cl_uint max_compute_units;                      // CL_DEVICE_MAX_COMPUTE_UNITS
    size_t max_work_group_size;                     // CL_DEVICE_MAX_WORK_GROUP_SIZE
    cl_uint max_work_item_dimensions;               // CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS
    std::vector<size_t> max_work_item_sizes;        // CL_DEVICE_MAX_WORK_ITEM_SIZES

    bool image_support;                             // CL_DEVICE_IMAGE_SUPPORT
    size_t image2d_max_width;                       // CL_DEVICE_IMAGE2D_MAX_WIDTH
    size_t image2d_max_height;                      // CL_DEVICE_IMAGE2D_MAX_HEIGHT
    size_t image3d_max_width;                       // CL_DEVICE_IMAGE3D_MAX_WIDTH
    size_t image3d_max_height;                      // CL_DEVICE_IMAGE3D_MAX_HEIGHT
    size_t image3d_max_depth;                       // CL_DEVICE_IMAGE3D_MAX_DEPTH
    size_t image_max_buffer_size;                   // CL_DEVICE_IMAGE_MAX_BUFFER_SIZE
    size_t image_max_array_size;                    // CL_DEVICE_IMAGE_MAX_ARRAY_SIZE
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

} // Compute

#endif // COMPUTE_HELPERS_H_
