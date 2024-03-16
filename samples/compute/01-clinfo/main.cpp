//
// main.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <iostream>
#include <exception>
#include <vector>
#include <utility>
#include <chrono>

#include "core/compute/compute.h"

///
/// @brief Compute run driver.
///
void Run()
{
    // Query information about all available platforms.
    auto platformIds = Compute::GetPlatformIDs();
    std::cout << "Number of Platforms " << platformIds.size() << "\n\n";
    for (auto &platform : platformIds) {
        auto info = Compute::GetPlatformInfo(platform);
        std::cout << Compute::ToString(info) << "\n";

        auto deviceIds = Compute::GetDeviceIDs(platform, CL_DEVICE_TYPE_ALL);
        std::cout << "Number of Devices " << deviceIds.size() << "\n\n";
        size_t count = 0;
        for (auto &device :deviceIds) {
            auto info = Compute::GetDeviceInfo(device);
            std::cout << "Device # " << count++ << "\n"
                      << Compute::ToString(info) << "\n";
        }
    }
}

///
/// @brief main client function.
///
int main(int argc, char const *argv[])
{
    try {
        Run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
