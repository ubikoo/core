//
// test-vector.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "catch2/catch.hpp"
#include "test-vector2.h"
#include "test-vector3.h"
#include "test-vector4.h"

///
/// @brief Vector test client.
///
TEST_CASE("Vector") {
    SECTION("vec2") {
        test_vector2<int32_t>();
        test_vector2<int64_t>();
        test_vector2<uint32_t>();
        test_vector2<uint64_t>();
        test_vector2<float>();
        test_vector2<double>();
    }

    SECTION("vec3") {
        test_vector3<int32_t>();
        test_vector3<int64_t>();
        test_vector3<uint32_t>();
        test_vector3<uint64_t>();
        test_vector3<float>();
        test_vector3<double>();
    }

    SECTION("vec4") {
        test_vector4<int32_t>();
        test_vector4<int64_t>();
        test_vector4<uint32_t>();
        test_vector4<uint64_t>();
        test_vector4<float>();
        test_vector4<double>();
    }
}
