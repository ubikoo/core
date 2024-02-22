//
// program.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef COMPUTE_PROGRAM_H_
#define COMPUTE_PROGRAM_H_

#include <memory>
#include <string>
#include "common.h"

namespace Compute {

///
/// @brief Create an interface to a OpenCL program object on the device.
///
struct ProgramObject {
    DeviceObject *device{NULL};
    cl_program id{NULL};

    std::vector<cl_device_id> GetDevices() const;
    std::string GetSource() const;
    std::string GetKernelNames() const;
};
struct ProgramDeleter {
    void operator()(ProgramObject *obj) {
        if (obj) { clReleaseProgram(obj->id); };
        delete obj;
    }
};
using Program = std::unique_ptr<ProgramObject, ProgramDeleter>;

/// @brief Create a program object from the specified source.
Program CreateProgramWithSource(
    const Device &device,
    const std::string &source,
    const std::string &options = "");

/// @brief Create a program object from the specified filename.
Program CreateProgramWithFile(
    const Device &device,
    const std::string &filename,
    const std::string &options = "");

/// @brief Load program source from the specified filename.
std::string LoadProgramSource(const std::string &filename);

} // Compute

#endif // COMPUTE_PROGRAM_H_
