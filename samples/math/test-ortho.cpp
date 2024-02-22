//
// test-ortho.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "catch2/catch.hpp"
#include "test-ortho.h"

TEST_CASE("Ortho") {
    static const size_t n_iters = 8192;

    auto tic = std::chrono::high_resolution_clock::now();
    test_ortho<float>(n_iters);
    test_ortho<double>(n_iters);
    auto toc = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;
    std::cout << msec.count() << " msec\n";
}
