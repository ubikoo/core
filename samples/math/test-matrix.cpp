//
// test-matrix.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "catch2/catch.hpp"
#include "test-matrix2.h"
#include "test-matrix3.h"
#include "test-matrix4.h"

///
/// @brief Matrix test client.
///
TEST_CASE("Matrix") {
    SECTION("mat2") {
        test_matrix2<int32_t>();
        test_matrix2<int64_t>();
        test_matrix2<uint32_t>();
        test_matrix2<uint64_t>();
        test_matrix2<float>();
        test_matrix2<double>();
    }

    SECTION("mat3") {
        test_matrix3<int32_t>();
        test_matrix3<int64_t>();
        test_matrix3<uint32_t>();
        test_matrix3<uint64_t>();
        test_matrix3<float>();
        test_matrix3<double>();
    }

    SECTION("mat4") {
        test_matrix4<int32_t>();
        test_matrix4<int64_t>();
        test_matrix4<uint32_t>();
        test_matrix4<uint64_t>();
        test_matrix4<float>();
        test_matrix4<double>();
    }
}
