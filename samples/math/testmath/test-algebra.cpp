//
// test-algebra.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "external/catch2/catch.hpp"
#include "test-algebra-vector2.h"
#include "test-algebra-vector3.h"
#include "test-algebra-vector4.h"
#include "test-algebra-matrix2.h"
#include "test-algebra-matrix3.h"
#include "test-algebra-matrix4.h"

///
/// @brief Vector algebra test client.
///
TEST_CASE("Algebra") {
    static const size_t n_iters = 8192;

    SECTION("vec2") {
        test_algebra_vector2<float>(n_iters);
        test_algebra_vector2<double>(n_iters);
        test_algebra_matrix2<float>(n_iters);
        test_algebra_matrix2<double>(n_iters);
    }
    SECTION("vec3") {
        test_algebra_vector3<float>(n_iters);
        test_algebra_vector3<double>(n_iters);
        test_algebra_matrix3<float>(n_iters);
        test_algebra_matrix3<double>(n_iters);
    }
    SECTION("vec4") {
        test_algebra_vector4<float>(n_iters);
        test_algebra_vector4<double>(n_iters);
        test_algebra_matrix4<float>(n_iters);
        test_algebra_matrix4<double>(n_iters);
    }
}
