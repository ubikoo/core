//
// main.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <iostream>
#include <iomanip>
#include <exception>
#include <random>

#include "common.h"
#include "modelgpu.h"
#include "modelcpu.h"

/// @brief Constants and globals.
std::vector<Point> gPoints(kNumPoints);
ModelGpu gModelGpu;
ModelCpu gModelCpu;

///
/// @brief Create a collection of points uniformly distributed inside a box.
///
void CreatePoints()
{
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_real_distribution<cl_float> urand(0.0f, 1.0f);
    std::uniform_real_distribution<cl_float> dist0(
        kDomainLo.s[0], kDomainHi.s[0]);
    std::uniform_real_distribution<cl_float> dist1(
        kDomainLo.s[1], kDomainHi.s[1]);
    std::uniform_real_distribution<cl_float> dist2(
        kDomainLo.s[2], kDomainHi.s[2]);

    for (auto &it : gPoints) {
        it.pos = {dist0(rng), dist1(rng), dist2(rng)};
        it.col = {urand(rng), urand(rng), urand(rng)};
    }
}

///
/// @brief Create a collection of points uniformly distributed inside a box.
///
void ValidateKeys()
{
    for (cl_uint i = 0; i < kNumPoints; i++) {
        bool valid = gModelGpu.key(i).first == gModelCpu.key(i).first &&
                     gModelGpu.key(i).second == gModelCpu.key(i).second;
        if (!valid) {
            std::cerr << i << ": "
                << " GPU " << std::setw(10)
                << gModelGpu.key(i).first << gModelGpu.key(i).second
                << " CPU " << std::setw(10)
                << gModelCpu.key(i).first << gModelCpu.key(i).second
                << " Points " << std::setprecision(4)
                << gPoints[i].pos.s[0]
                << gPoints[i].pos.s[1]
                << gPoints[i].pos.s[2]
                << "\n";
            throw std::runtime_error("mismatch hashmap keys");
        }
    }
}

///
/// @brief Run the simulation.
///
void Run()
{
    gModelGpu.Initialize();
    gModelCpu.Initialize();

    for (size_t i = 0; i < kNumIters; ++i) {
        std::cout << "Iteration " << i << "\n";
        CreatePoints();
        gModelGpu.Run(gPoints);
        gModelCpu.Run(gPoints);
        ValidateKeys();
    }

    gModelGpu.Cleanup();
    gModelCpu.Cleanup();
}

///
/// @brief
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
