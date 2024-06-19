//
// test-algebra-vector2.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef TEST_MATH_ALGEBRA_VECTOR2_H_
#define TEST_MATH_ALGEBRA_VECTOR2_H_

#include "core/math/math.h"
#include "common.h"

///
/// @brief vec2 algebra test client.
///
template<typename T>
void test_algebra_vector2_run(const size_t n_iters)
{
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_real_distribution<T> dist(0.0, 1.0);

    for (size_t iter = 0; iter < n_iters; ++iter) {
        math::vec2<T> arr_a = {dist(rng),  dist(rng)};
        math::vec2<T> arr_b = -arr_a;
        math::vec2<T> arr_c = math::vec2<T>::zeros;

        // Test arithmetic assignment
        (arr_c = arr_a) += arr_b;
        for (size_t j = 0; j < arr_c.length; ++j) {
            REQUIRE(math::iseq(arr_c[j], static_cast<T>(0)));
        }

        (arr_c = arr_a) -= arr_b;
        for (size_t j = 0; j < arr_c.length; ++j) {
            REQUIRE(math::iseq(arr_c[j], static_cast<T>(2) * arr_a[j]));
        }

        arr_c = arr_a + arr_b;
        for (size_t j = 0; j < arr_c.length; ++j) {
            REQUIRE(math::iseq(arr_c[j], static_cast<T>(0)));
        }

        arr_c = arr_a - arr_b;
        for (size_t j = 0; j < arr_c.length; ++j) {
            REQUIRE(math::iseq(arr_c[j], static_cast<T>(2) * arr_a[j]));
        }

        // Test dot, norm and normalize
        arr_c = arr_a - arr_b;

        T d_0 = math::dot(arr_c, arr_c);
        T d_1 = 4.0 * math::dot(arr_a, arr_a);
        REQUIRE(math::iseq(d_0, d_1));

        T d_2 = math::norm(arr_c);
        T d_3 = 2.0 * std::sqrt(math::dot(arr_a, arr_a));
        REQUIRE(math::iseq(d_2, d_3));

        T d_4 = math::norm(math::normalize(arr_c));
        REQUIRE(math::iseq(d_4, static_cast<T>(1)));
    }
}

template<typename T>
void test_algebra_vector2(const size_t n_iters)
{
    // Test dot
    {
        const T zero = (T) 0;
        const T one = (T) 1;
        const T two = (T) 2;

        math::vec2<T> a = { 1.0, 1.0};
        math::vec2<T> b = { 0.0, 1.0};
        math::vec2<T> c = {-1.0, 1.0};

        REQUIRE(math::iseq(dot(a,a), two));
        REQUIRE(math::iseq(dot(b,b), one));
        REQUIRE(math::iseq(dot(c,c), two));

        REQUIRE(math::iseq(dot(a,b), one));
        REQUIRE(math::iseq(dot(a,c), zero));
        REQUIRE(math::iseq(dot(b,c), one));
    }

    // Test norm
    {
        const T sqrt2 = (T) std::sqrt(2.0);
        const T one = (T) 1;

        math::vec2<T> a = { 1.0, 1.0};
        math::vec2<T> b = { 0.0, 1.0};
        math::vec2<T> c = {-1.0, 1.0};

        REQUIRE(math::iseq(norm(a), sqrt2));
        REQUIRE(math::iseq(norm(b), one));
        REQUIRE(math::iseq(norm(c), sqrt2));
    }

    // Test normalize
    {
        const T one = (T) 1;

        math::vec2<T> a = { 1.0, 1.0};
        math::vec2<T> b = { 0.0, 1.0};
        math::vec2<T> c = {-1.0, 1.0};

        math::vec2<T> norm_a = normalize(a);
        math::vec2<T> norm_b = normalize(b);
        math::vec2<T> norm_c = normalize(c);

        REQUIRE(math::iseq(norm(norm_a), one));
        REQUIRE(math::iseq(norm(norm_b), one));
        REQUIRE(math::iseq(norm(norm_c), one));
    }

    // Test distance
    {
        const T zero = (T) 0;

        math::vec2<T> a = { 1.0, 1.0};
        math::vec2<T> b = { 0.0, 1.0};
        math::vec2<T> c = {-1.0, 1.0};

        math::vec2<T> ab  = a - b;
        math::vec2<T> ac  = a - c;
        math::vec2<T> cb1 = c - b;
        math::vec2<T> cb2 = ab - ac;

        REQUIRE(math::iseq(norm(ab), distance(a,b)));
        REQUIRE(math::iseq(distance(b,a), distance(a,b)));

        REQUIRE(math::iseq(norm(ac), distance(a,c)));
        REQUIRE(math::iseq(distance(a,c), distance(a,c)));

        REQUIRE(math::iseq(norm(cb1), norm(cb2)));
        REQUIRE(math::iseq(norm(cb1), distance(c,b)));
        REQUIRE(math::iseq(distance(b,c), distance(c,b)));
        REQUIRE(math::iseq(distance(cb1,cb2), zero));
    }

    // Test random vectors
    {
        #pragma omp parallel default(none) shared(n_iters)
        {
            size_t n_threads = omp_get_num_threads();
            #pragma omp parallel for schedule(static)
            for (size_t ix = 0; ix < n_threads; ++ix) {
                test_algebra_vector2_run<T>(n_iters);
            }
        }
    }
}

#endif // TEST_MATH_ALGEBRA_VECTOR2_H_
