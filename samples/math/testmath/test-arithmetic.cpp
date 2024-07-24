//
// test-arithmetic.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "external/catch2/catch.hpp"
#include "test-arithmetic2.h"
#include "test-arithmetic3.h"
#include "test-arithmetic4.h"

///
/// @brief Vector arithmetic test client.
///
TEST_CASE("Arithmetic") {
    SECTION("Vec2") {
        test_arithmetic2<float>();
        test_arithmetic2<double>();
    }
    SECTION("Vec3") {
        test_arithmetic3<float>();
        test_arithmetic3<double>();
    }
    SECTION("Vec4") {
        test_arithmetic4<float>();
        test_arithmetic4<double>();
    }
}
