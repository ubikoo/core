//
// program.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "common.h"
#include "helpers.h"
#include "device.h"
#include "program.h"

namespace Compute {

///
/// @brief Create a program object from the specified source and build the
/// executable for the OpenCL device. The options parameter specifies build
/// options, preprocessor defines, optimization and code generation options.
///
Program CreateProgramWithSource(
    const Device &device,
    const std::string &source,
    const std::string &options)
{
    ThrowIf(source.empty());

    // Create program with source.
    ProgramObject *program = new ProgramObject;
    {
        // Store the device object.
        program->device = device.get();

        // Create the program object.
        const char *str = source.c_str();
        size_t length = source.length();
        cl_int err;
        program->id = clCreateProgramWithSource(
            device->context,    // OpenCL context
            1,                  // one null-terminated character string
            &str,               // pointer to the character string
            &length,            // length of the character string
            &err);
        ThrowIfFailed(err);
    }

    // Build the program executable on the OpenCL device.
    {
        cl_int err = clBuildProgram(
            program->id,
            1,
            &device->id,
            options.c_str(),
            NULL,               // no callback, wait until completion
            NULL);              // don't pass callback user data

        if (err != CL_SUCCESS) {
            size_t infolen;
            clGetProgramBuildInfo(
                program->id,
                device->id,
                CL_PROGRAM_BUILD_LOG,
                0,
                NULL,
                &infolen);

            std::string infolog(infolen+1, '\0');
            clGetProgramBuildInfo(
                program->id,
                device->id,
                CL_PROGRAM_BUILD_LOG,
                infolen,
                &infolog[0],
                NULL);

            std::cerr << "failed to link program:\n\n" << infolog << "\n";
        }
        ThrowIfFailed(err);
    }
    return Program(program, ProgramDeleter());
}

///
/// @brief Create a program object from the specified filename.
///
Program CreateProgramWithFile(
    const Device &device,
    const std::string &filename,
    const std::string &options)
{
    return CreateProgramWithSource(device, LoadProgramSource(filename),options);
}

///
/// @brief Load program source from the specified filename.
///
std::string LoadProgramSource(const std::string &filename)
{
    std::stringstream ss(std::ios::out);
    std::ifstream file(filename);
    if (file) {
        ss << file.rdbuf();
    }
    return ss.str();
}

///
/// @brief Return the list of devices associated with the program object.
///
std::vector<cl_device_id> ProgramObject::GetDevices() const
{
    size_t param_size;
    ThrowIfFailed(clGetProgramInfo(id, CL_PROGRAM_DEVICES,
        0, NULL, &param_size));

    size_t devices_size = param_size / sizeof(cl_device_id);
    ThrowIfNot(devices_size > 0);

    std::vector<cl_device_id> devices(devices_size);
    ThrowIfFailed(clGetProgramInfo(id, CL_PROGRAM_DEVICES,
        param_size, &devices[0], NULL));

    return devices;
}

///
/// @brief Return the program source code specified by clCreateProgramWithSource.
///
std::string ProgramObject::GetSource() const
{
    size_t param_size;
    ThrowIfFailed(clGetProgramInfo(id, CL_PROGRAM_SOURCE,
        0, NULL, &param_size));

    size_t source_size = param_size / sizeof(char);
    ThrowIfNot(source_size > 0);

    std::string source(source_size, '\0');
    ThrowIfFailed(clGetProgramInfo(id, CL_PROGRAM_SOURCE,
        param_size, &source[0], NULL));

    return source;
}

///
/// @brief Return a semi-colon separated list of kernel names in program.
///
std::string ProgramObject::GetKernelNames() const
{
    size_t param_size;
    ThrowIfFailed(clGetProgramInfo(id, CL_PROGRAM_KERNEL_NAMES,
        0, NULL, &param_size));

    size_t kernel_names_size = param_size / sizeof(char);
    ThrowIfNot(kernel_names_size > 0);

    std::string kernel_names(kernel_names_size, '\0');
    ThrowIfFailed(clGetProgramInfo(id, CL_PROGRAM_KERNEL_NAMES,
        param_size, &kernel_names[0], NULL));

    return kernel_names;
}

} // Compute
