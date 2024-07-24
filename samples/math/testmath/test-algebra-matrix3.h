//
// test-algebra-matrix3.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef TEST_MATH_ALGEBRA_MATRIX3_H_
#define TEST_MATH_ALGEBRA_MATRIX3_H_

#include "minicore/math/math.h"
#include "common.h"

///
/// @brief Mat3 algebra test client.
///
template<typename T>
void test_algebra_matrix3_run(const size_t n_iters)
{
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_real_distribution<T> dist(0.0, 1.0);

    for (size_t iter = 0; iter < n_iters; ++iter) {
        // Test arithmetic functions
        {
            Math::Mat3<T> arr_a = {
                dist(rng), dist(rng), dist(rng),
                dist(rng), dist(rng), dist(rng),
                dist(rng), dist(rng), dist(rng)};
            Math::Mat3<T> arr_b = -arr_a;
            Math::Mat3<T> arr_c = Math::Mat3<T>::Zeros;

            (arr_c = arr_a) += arr_b;
            for (size_t j = 0; j < arr_c.length; ++j) {
                REQUIRE(Math::IsEq(arr_c[j], static_cast<T>(0)));
            }

            (arr_c = arr_a) -= arr_b;
            for (size_t j = 0; j < arr_c.length; ++j) {
                REQUIRE(Math::IsEq(arr_c[j], static_cast<T>(2) * arr_a[j]));
            }

            arr_c = arr_a + arr_b;
            for (size_t j = 0; j < arr_c.length; ++j) {
                REQUIRE(Math::IsEq(arr_c[j], static_cast<T>(0)));
            }

            arr_c = arr_a - arr_b;
            for (size_t j = 0; j < arr_c.length; ++j) {
                REQUIRE(Math::IsEq(arr_c[j], static_cast<T>(2) * arr_a[j]));
            }
        }

        // Matrix transpose
        {
            Math::Mat3<T> arr_a = {
                dist(rng), dist(rng), dist(rng),
                dist(rng), dist(rng), dist(rng),
                dist(rng), dist(rng), dist(rng)};
            arr_a += Math::Mat3<T>::Eye * static_cast<T>(2);
            Math::Mat3<T> arr_b = Math::Transpose(arr_a);   // b = a^t
            Math::Mat3<T> arr_c = Math::Dot(arr_a, arr_b);  // c = a * a^t

            T det_a = Math::Determinant(arr_a);
            T det_b = Math::Determinant(arr_b);
            T det_c = Math::Determinant(arr_c);

            REQUIRE(Math::IsEq(det_a, det_b));
            REQUIRE(Math::IsEq(det_a * det_a, det_c));
        }

        // Matrix inverse
        {
            Math::Mat3<T> arr_a = Math::Mat3<T>{
                dist(rng), dist(rng), dist(rng),
                dist(rng), dist(rng), dist(rng),
                dist(rng), dist(rng), dist(rng)};
            arr_a += Math::Mat3<T>::Eye * static_cast<T>(2);
            Math::Mat3<T> inv_a = Math::Inverse(arr_a);
            Math::Mat3<T> eye_a = Math::Dot(arr_a, inv_a);

            Math::Mat3<T> identity = Math::Mat3<T>::Eye;
            for (size_t j = 0; j < eye_a.length; ++j) {
                REQUIRE(Math::IsEq(eye_a[j], identity[j]));
            }
        }

        // Matrix solve, x = a^-1 * arr_b, err = a * x - b
        {
            Math::Mat3<T> arr_a = Math::Mat3<T>{
                dist(rng), dist(rng), dist(rng),
                dist(rng), dist(rng), dist(rng),
                dist(rng), dist(rng), dist(rng)};
            arr_a += Math::Mat3<T>::Eye * static_cast<T>(2);
            Math::Mat3<T> inv_a = Math::Inverse(arr_a);
            Math::Vec3<T> vec_b = {dist(rng), dist(rng), dist(rng)};
            Math::Vec3<T> vec_x = Math::Dot(inv_a, vec_b);
            Math::Vec3<T> err = vec_b - Math::Dot(arr_a, vec_x);
            REQUIRE(Math::IsEq(Math::Norm(err), static_cast<T>(0)));
        }
    }
}

template<typename T>
void test_algebra_matrix3(const size_t n_iters)
{
    // Test random matrices
    #pragma omp parallel default(none) shared(n_iters)
    {
        size_t n_threads = omp_get_num_threads();
        #pragma omp parallel for schedule(static)
        for (size_t ix = 0; ix < n_threads; ++ix) {
            test_algebra_matrix3_run<T>(n_iters);
        }
    }
}

#endif // TEST_MATH_ALGEBRA_MATRIX3_H_
