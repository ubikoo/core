//
// helpers.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "common.h"
#include "helpers.h"

namespace Compute {

/// -----------------------------------------------------------------------------
/// @brief Return a vector of all platforms currently available.
///
std::vector<cl_platform_id> GetPlatformIDs()
{
    cl_uint count = 0;
    ThrowIfFailed(clGetPlatformIDs(0, NULL, &count));
    ThrowIfNot(count > 0);

    std::vector<cl_platform_id> platformIds(count);
    ThrowIfFailed(clGetPlatformIDs(count, &platformIds[0], NULL));
    return platformIds;
}

///
/// @brief Query information about a specified platform.
///
PlatformInfo GetPlatformInfo(const cl_platform_id &platform)
{
    auto get_param_string = [&](
        cl_platform_info name,
        std::string &info) -> void
    {
        size_t size = 0;
        ThrowIfFailed(clGetPlatformInfo(platform, name, 0, NULL, &size));
        ThrowIfNot(size > 0);

        info.resize(size, '\0');
        ThrowIfFailed(clGetPlatformInfo(platform, name, size, &info[0], NULL));
    };

    PlatformInfo info = {};
    get_param_string(CL_PLATFORM_PROFILE, info.profile);
    get_param_string(CL_PLATFORM_VERSION, info.version);
    get_param_string(CL_PLATFORM_NAME, info.name);
    get_param_string(CL_PLATFORM_VENDOR, info.vendor);
    get_param_string(CL_PLATFORM_EXTENSIONS, info.extensions);
    return info;
}

///
/// @brief Return the platform information in reader friendly string format.
///
std::string ToString(const PlatformInfo &info)
{
    std::ostringstream ss;
    ss << "CL_PLATFORM_PROFILE " << info.profile << "\n";
    ss << "CL_PLATFORM_VERSION " << info.version << "\n";
    ss << "CL_PLATFORM_NAME " << info.name << "\n";
    ss << "CL_PLATFORM_VENDOR " << info.vendor << "\n";
    ss << "CL_PLATFORM_EXTENSIONS " << info.extensions << "\n";
    return ss.str();
}

/// -----------------------------------------------------------------------------
/// @brief Return all devices of a given type in the specified platform.
/// @param type is one of the following: CL_DEVICE_TYPE_CPU, CL_DEVICE_TYPE_GPU,
/// CL_DEVICE_TYPE_ACCELERATOR, CL_DEVICE_TYPE_DEFAULT, CL_DEVICE_TYPE_ALL.
///
std::vector<cl_device_id> GetDeviceIDs(
    const cl_platform_id &platform,
    cl_device_type type)
{
    cl_uint count = 0;
    ThrowIfFailed(clGetDeviceIDs(platform, type, 0, NULL, &count));
    ThrowIfNot(count > 0);

    std::vector<cl_device_id> deviceIds(count);
    ThrowIfFailed(clGetDeviceIDs(platform, type, count, &deviceIds[0], NULL));
    return deviceIds;
}

///
/// @brief Return the id of the first device with a given name in the platform.
///
cl_device_id GetDeviceID(const cl_platform_id &platform, std::string name)
{
    std::vector<cl_device_id> deviceIds;
    for (auto &device : GetDeviceIDs(platform, CL_DEVICE_TYPE_ALL)) {
        size_t count = 0;
        ThrowIfFailed(clGetDeviceInfo(device, CL_DEVICE_NAME, 0, NULL, &count));
        ThrowIfNot(count > 0);

        std::string deviceName(count, '\0');
        ThrowIfFailed(clGetDeviceInfo(device, CL_DEVICE_NAME,
            count, &deviceName[0], NULL));

        if (std::strcmp(deviceName.c_str(), name.c_str()) == 0) {
            return device;
        }
    }
    return NULL;
}

///
/// @brief Query information about a specified device.
///
DeviceInfo GetDeviceInfo(const cl_device_id &device)
{
    auto get_param_value = [&](
        cl_device_info name, size_t size, void *value) -> void
    {
        ThrowIfFailed(clGetDeviceInfo(device, name, size, value, NULL));
    };

    auto get_param_string = [&](cl_device_info name, std::string &info) -> void
    {
        size_t size = 0;
        ThrowIfFailed(clGetDeviceInfo(device, name, 0, NULL, &size));
        ThrowIfNot(size > 0);

        info.resize(size, '\0');
        ThrowIfFailed(clGetDeviceInfo(device, name, size, &info[0], NULL));
    };

    DeviceInfo info = {};

    get_param_string(CL_DEVICE_NAME, info.deviceName);
    get_param_string(CL_DEVICE_VENDOR, info.deviceVendor);
    get_param_string(CL_DEVICE_VERSION, info.deviceVersion);
    get_param_string(CL_DEVICE_EXTENSIONS, info.deviceExtensions);
    get_param_value(CL_DEVICE_TYPE, sizeof(cl_device_type), &info.deviceType);

    get_param_value(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
        sizeof(cl_ulong), &info.globalMemCacheSize);
    get_param_value(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
        sizeof(cl_device_mem_cache_type), &info.globalMemCacheType);
    get_param_value(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
        sizeof(cl_uint), &info.globalMemCachelineSize);
    get_param_value(CL_DEVICE_GLOBAL_MEM_SIZE,
        sizeof(cl_ulong), &info.globalMemSize);
    get_param_value(CL_DEVICE_LOCAL_MEM_SIZE,
        sizeof(cl_ulong), &info.localMemSize);
    get_param_value(CL_DEVICE_LOCAL_MEM_TYPE,
        sizeof(cl_device_local_mem_type), &info.localMemType);

    get_param_value(CL_DEVICE_MAX_CLOCK_FREQUENCY,
        sizeof(cl_uint), &info.maxClockFrequency);
    get_param_value(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
        sizeof(cl_ulong), &info.maxConstantBufferSize);
    get_param_value(CL_DEVICE_MAX_MEM_ALLOC_SIZE,
        sizeof(cl_ulong), &info.maxMemAllocSize);
    get_param_value(CL_DEVICE_MAX_PARAMETER_SIZE,
        sizeof(cl_ulong), &info.maxParameterSize);
    get_param_value(CL_DEVICE_MAX_COMPUTE_UNITS,
        sizeof(cl_uint), &info.maxComputeUnits);
    get_param_value(CL_DEVICE_MAX_WORK_GROUP_SIZE,
        sizeof(size_t), &info.maxWorkGroupSize);
    get_param_value(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
        sizeof(cl_uint), &info.maxWorkItemDimensions);

    info.maxWorkItemSizes.resize(info.maxWorkItemDimensions, 0);
    get_param_value(CL_DEVICE_MAX_WORK_ITEM_SIZES,
        info.maxWorkItemDimensions * sizeof(size_t),
        &info.maxWorkItemSizes[0]);

    get_param_value(CL_DEVICE_IMAGE_SUPPORT,
        sizeof(cl_bool), &info.imageSupport);
    get_param_value(CL_DEVICE_IMAGE2D_MAX_WIDTH,
        sizeof(size_t), &info.image2dMaxWidth);
    get_param_value(CL_DEVICE_IMAGE2D_MAX_HEIGHT,
        sizeof(size_t), &info.image2dMaxHeight);
    get_param_value(CL_DEVICE_IMAGE3D_MAX_WIDTH,
        sizeof(size_t), &info.image3dMaxWidth);
    get_param_value(CL_DEVICE_IMAGE3D_MAX_HEIGHT,
        sizeof(size_t), &info.image3dMaxHeight);
    get_param_value(CL_DEVICE_IMAGE3D_MAX_DEPTH,
        sizeof(size_t), &info.image3dMaxDepth);
    get_param_value(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE,
        sizeof(size_t), &info.imageMaxBufferSize);
    get_param_value(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE,
        sizeof(size_t), &info.imageMaxArraySize);

    return info;
}

///
/// @brief Return the device information in reader friendly string format.
///
std::string ToString(const DeviceInfo &info)
{
    std::ostringstream ss;

    ss << std::boolalpha;

    ss << "CL_DEVICE_NAME " << info.deviceName << "\n";
    ss << "CL_DEVICE_VENDOR " << info.deviceVendor << "\n";
    ss << "CL_DEVICE_VERSION " << info.deviceVersion << "\n";
    ss << "CL_DEVICE_EXTENSIONS " << info.deviceExtensions << "\n";

    ss << "CL_DEVICE_TYPE " << info.deviceType << " ";
    switch (info.deviceType) {
    case CL_DEVICE_TYPE_CPU:
        ss << "CL_DEVICE_TYPE_CPU\n";
        break;
    case CL_DEVICE_TYPE_GPU:
        ss << "CL_DEVICE_TYPE_GPU\n";
        break;
    case CL_DEVICE_TYPE_ACCELERATOR:
        ss << "CL_DEVICE_TYPE_ACCELERATOR\n";
        break;
    case CL_DEVICE_TYPE_DEFAULT:
        ss << "CL_DEVICE_TYPE_DEFAULT\n";
        break;
    default:
        ss << "UNKNOWN CL_DEVICE_TYPE\n";
        break;
    }

    ss << "CL_DEVICE_GLOBAL_MEM_CACHE_SIZE " << info.globalMemCacheSize << "\n";
    ss << "CL_DEVICE_GLOBAL_MEM_CACHE_TYPE " << info.globalMemCacheType << " ";
    switch (info.globalMemCacheType) {
    case CL_NONE:
        ss << "CL_NONE\n";
        break;
    case CL_READ_ONLY_CACHE:
        ss << "CL_READ_ONLY_CACHE\n";
        break;
    case CL_READ_WRITE_CACHE:
        ss << "CL_READ_WRITE_CACHE\n";
        break;
    default:
        ss << "UNKNOWN CL_DEVICE_GLOBAL_MEM_CACHE_TYPE\n";
        break;
    }

    ss << "CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE " << info.globalMemCachelineSize << "\n";
    ss << "CL_DEVICE_GLOBAL_MEM_SIZE " << info.globalMemSize << "\n";
    ss << "CL_DEVICE_LOCAL_MEM_SIZE " << info.localMemSize << "\n";

    ss << "CL_DEVICE_LOCAL_MEM_TYPE " << info.localMemType << " ";
    switch (info.localMemType) {
    case CL_NONE:
        ss << "CL_NONE\n";
        break;
    case CL_LOCAL:
        ss << "CL_LOCAL\n";
        break;
    case CL_GLOBAL:
        ss << "CL_GLOBAL\n";
        break;
    default:
        ss << "UNKNOWN CL_DEVICE_LOCAL_MEM_TYPE\n";
        break;
    }

    ss << "CL_DEVICE_MAX_CLOCK_FREQUENCY " << info.maxClockFrequency << "\n";
    ss << "CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE " << info.maxConstantBufferSize << "\n";
    ss << "CL_DEVICE_MAX_MEM_ALLOC_SIZE " << info.maxMemAllocSize << "\n";
    ss << "CL_DEVICE_MAX_PARAMETER_SIZE " << info.maxParameterSize << "\n";
    ss << "CL_DEVICE_MAX_COMPUTE_UNITS " << info.maxComputeUnits << "\n";
    ss << "CL_DEVICE_MAX_WORK_GROUP_SIZE " << info.maxWorkGroupSize << "\n";
    ss << "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS " << info.maxWorkItemDimensions << "\n";

    ss << "CL_DEVICE_MAX_WORK_ITEM_SIZES " << " ";
    for (auto &it : info.maxWorkItemSizes) {
        ss << it << " ";
    }
    ss << "\n";

    ss << "CL_DEVICE_IMAGE_SUPPORT " << info.imageSupport << "\n";
    ss << "CL_DEVICE_IMAGE2D_MAX_WIDTH " << info.image2dMaxWidth << "\n";
    ss << "CL_DEVICE_IMAGE2D_MAX_HEIGHT " << info.image2dMaxHeight << "\n";
    ss << "CL_DEVICE_IMAGE3D_MAX_WIDTH " << info.image3dMaxWidth << "\n";
    ss << "CL_DEVICE_IMAGE3D_MAX_HEIGHT " << info.image3dMaxHeight << "\n";
    ss << "CL_DEVICE_IMAGE3D_MAX_DEPTH " << info.image3dMaxDepth << "\n";
    ss << "CL_DEVICE_IMAGE_MAX_BUFFER_SIZE " << info.imageMaxBufferSize << "\n";
    ss << "CL_DEVICE_IMAGE_MAX_ARRAY_SIZE " << info.imageMaxArraySize << "\n";

    return ss.str();
}

/// -----------------------------------------------------------------------------
/// @brief Return all devices in the specified context.
///
std::vector<cl_device_id> GetContextDevices(const cl_context &context)
{
    size_t size = 0;
    ThrowIfFailed(clGetContextInfo(context, CL_CONTEXT_DEVICES,
        0, NULL, &size));
    ThrowIfNot(size > 0);

    size_t n_devices = size / sizeof(cl_device_id);
    std::vector<cl_device_id> devices(n_devices);
    ThrowIfFailed(clGetContextInfo(context, CL_CONTEXT_DEVICES,
        size, &devices[0], NULL));

    return devices;
}

/// -----------------------------------------------------------------------------
/// @brief Return the image bitmap as normalized OpenCL floating point data type.
///
std::vector<cl_float> ImageAsFloat(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp)
{
    const uint32_t nChannels = bpp / 8;
    const uint8_t *px = bitmap;

    std::vector<cl_float> data;
    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            for (uint32_t c = 0; c < nChannels; ++c) {
                cl_float v = bitmap[c] / 255.0f;
                data.emplace_back(v);
            }
            px += nChannels;
        }
    }
    return data;
}

///
/// @brief Return the image bitmap as normalized OpenCL 2d-vector data type.
///
std::vector<cl_float2> ImageAsFloat2(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp)
{
    const uint32_t nChannels = bpp / 8;
    const uint8_t *px = bitmap;

    std::vector<cl_float2> data;
    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            cl_float2 v = {
                nChannels > 0 ? px[0] / 255.0f : 0.0f,
                nChannels > 1 ? px[1] / 255.0f : 0.0f};
            data.emplace_back(v);
            px += nChannels;
        }
    }
    return data;
}

///
/// @brief Return the image bitmap as normalized OpenCL 3d-vector data type.
///
std::vector<cl_float3> ImageAsFloat3(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp)
{
    const uint32_t nChannels = bpp / 8;
    const uint8_t *px = bitmap;

    std::vector<cl_float3> data;
    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            cl_float3 v = {
                nChannels > 0 ? px[0] / 255.0f : 0.0f,
                nChannels > 1 ? px[1] / 255.0f : 0.0f,
                nChannels > 2 ? px[2] / 255.0f : 0.0f};
            data.emplace_back(v);
            px += nChannels;
        }
    }
    return data;
}

///
/// @brief Return the image bitmap as normalized OpenCL 4d-vector data type.
///
std::vector<cl_float4> ImageAsFloat4(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp)
{
    const uint32_t nChannels = bpp / 8;
    const uint8_t *px = bitmap;

    std::vector<cl_float4> data;
    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            cl_float4 v = {
                nChannels > 0 ? px[0] / 255.0f : 0.0f,
                nChannels > 1 ? px[1] / 255.0f : 0.0f,
                nChannels > 2 ? px[2] / 255.0f : 0.0f,
                nChannels > 3 ? px[3] / 255.0f : 0.0f};
            data.emplace_back(v);
            px += nChannels;
        }
    }
    return data;
}

} // namespace Compute
