//
// modelcpu.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

#include "common.h"
#include "hashmap.h"
#include "modelcpu.h"

///
/// @brief Initialize the cpu model.
///
void ModelCpu::Initialize()
{
    // Initialize model data.
    mHashmap = Hashmap::Create(kLoadFactor * kNumPoints);
    mKeys.resize(kNumPoints, {0, 0});
}

///
/// @brief Cleanup the cpu model.
///
void ModelCpu::Cleanup()
{}

///
/// @brief Run the cpu model.
///
void ModelCpu::Run(std::vector<Point> &points)
{
    // Compute the hash value of a cell index.
    auto hash = [&](const cl_uint3 &v) -> uint32_t {
        const uint32_t c1 = static_cast<uint32_t>(73856093);
        const uint32_t c2 = static_cast<uint32_t>(19349663);
        const uint32_t c3 = static_cast<uint32_t>(83492791);

        uint32_t h1 = c1 * v.s[0];
        uint32_t h2 = c2 * v.s[1];
        uint32_t h3 = c3 * v.s[2];
        return (h1 ^ h2 ^ h3);
        // return (7*h1 + 503*h2 + 24847*h3);
    };

    // Compute the cell index of a given point.
    auto CellId = [&](const cl_float3 &p) -> cl_uint3 {
        cl_float3 u = (p - kDomainLo);
        u /= (kDomainHi - kDomainLo);
        u *= (cl_float) kNumCells;

        const uint32_t v1 = static_cast<uint32_t>(u.s[0]);
        const uint32_t v2 = static_cast<uint32_t>(u.s[1]);
        const uint32_t v3 = static_cast<uint32_t>(u.s[2]);
        return (cl_uint3) {v1, v2, v3};
    };

    // Create the hashmap from the array of particles.
    mHashmap.clear();
    for (size_t i = 0; i < kNumPoints; i++) {
         mHashmap.insert(hash(CellId(points[i].pos)), (uint32_t) i);
    }

    // Query the hashmap for all valid key-value pairs.
    {
        std::fill(mKeys.begin(), mKeys.end(), std::make_pair(0, 0));
        for (auto &slot : mHashmap.data()) {
            if (slot.key != mHashmap.end()) {
                mKeys[slot.value] = std::make_pair(
                    slot.key, slot.key % kCapacity);
            }
        }
    }
}
