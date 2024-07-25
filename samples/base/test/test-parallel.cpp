//
// test-memory.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "external/catch2/catch.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include "test-parallel.h"

static constexpr uint64_t kNumThreads = 16;
static constexpr uint64_t kNumIntervals = 1 << 4;
static constexpr uint64_t kNumIntervalSteps = 1 << 24;
static double gPiIntegral = 0.0;
static pthread_mutex_t gPiIntegralMutex;

void test_base_parallel(void)
{
    pthread_mutex_init(&gPiIntegralMutex, NULL);

    Base::ThreadPool::Initialize(kNumThreads);
    Base::ParallelFor(
        [](size_t intervalId, void *data) {
            double dX = 1.0 / static_cast<double>(kNumIntervals);
            double xLo = dX * static_cast<double>(intervalId);
            double xHi = xLo + dX;

            double dStep = (xHi - xLo) / static_cast<double>(kNumIntervalSteps);
            double fSum = 4.0 / (1.0 + xLo * xLo);
            fSum += 4.0 / (1.0 + xHi * xHi);
            for (size_t k = 1; k < kNumIntervalSteps - 1; ++k) {
                double x = xLo + static_cast<double>(k) * dStep;
                fSum += 4.0 / (1.0 + x * x);
            }

            pthread_mutex_lock(&gPiIntegralMutex);
            gPiIntegral += dStep * fSum;
            std::cout << "gPiIntegral " << std::setprecision(15)
                << gPiIntegral << ", "
                << M_PI << " "
                << std::abs(gPiIntegral - M_PI)
                << std::endl;
            pthread_mutex_unlock(&gPiIntegralMutex);
        },
        kNumIntervals,
        nullptr
    );
    Base::ThreadPool::Terminate();

    REQUIRE(std::abs(gPiIntegral - M_PI) < 1E-8);

    exit(EXIT_SUCCESS);
}

/// -----------------------------------------------------------------------------
TEST_CASE("BaseParallel") {
    test_base_parallel();
}
