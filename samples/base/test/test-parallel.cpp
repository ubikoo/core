//
// test-memory.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "external/catch2/catch.hpp"
#include <array>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "test-parallel.h"

static constexpr uint64_t kNumThreads = 16;
static constexpr uint64_t kNumIntervals = 1 << 30;
static std::array<double, kNumThreads> gThreadSum;
static constexpr double gDeltaX = 1.0 / static_cast<double>(kNumIntervals);

void test_base_parallel(void)
{
    Base::ThreadPool::Initialize(kNumThreads);
    Base::ParallelFor(
        [](size_t intervalId, void *data) {
            double x = static_cast<double>(intervalId) * gDeltaX;
            auto tid = Base::ThreadPool::GetThreadId();
            gThreadSum[tid] += 4.0 / (1.0 + x * x);
        },
        kNumIntervals,
        nullptr
    );
    Base::ThreadPool::Terminate();

    double piIntegral = 0.0;
    for (auto &threadSum : gThreadSum) {
        piIntegral += gDeltaX * threadSum;
    }

    std::cout << "piIntegral " << std::setprecision(15)
        << piIntegral << ", "
        << M_PI << " "
        << std::abs(piIntegral - M_PI)
        << std::endl;
    REQUIRE(std::abs(piIntegral - M_PI) < 1E-8);

    exit(EXIT_SUCCESS);
}

/// -----------------------------------------------------------------------------
TEST_CASE("BaseParallel") {
    test_base_parallel();
}
