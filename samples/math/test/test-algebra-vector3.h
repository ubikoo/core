//
// test-algebra-vector3.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef TEST_MATH_ALGEBRA_VECTOR3_H_
#define TEST_MATH_ALGEBRA_VECTOR3_H_

#include "minicore/math/math.h"
#include "common.h"

///
/// @brief Vec3 algebra test client.
///
template<typename T>
void test_algebra_vector3_run(const size_t n_iters)
{
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_real_distribution<T> dist(0.0, 1.0);

    for (size_t iter = 0; iter < n_iters; ++iter) {
        Math::Vec3<T> arr_a = {dist(rng),  dist(rng), dist(rng)};
        Math::Vec3<T> arr_b = -arr_a;
        Math::Vec3<T> arr_c = Math::Vec3<T>::Zeros;

        // Test arithmetic assignment
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

        // Test Dot, norm and normalize
        arr_c = arr_a - arr_b;

        T d_0 = Math::Dot(arr_c, arr_c);
        T d_1 = 4.0 * Math::Dot(arr_a, arr_a);
        REQUIRE(Math::IsEq(d_0, d_1));

        T d_2 = Math::Norm(arr_c);
        T d_3 = 2.0 * std::sqrt(Math::Dot(arr_a, arr_a));
        REQUIRE(Math::IsEq(d_2, d_3));

        T d_4 = Math::Norm(Math::Normalize(arr_c));
        REQUIRE(Math::IsEq(d_4, static_cast<T>(1)));
    }
}

template<typename T>
void test_algebra_vector3(const size_t n_iters)
{
    // Test Dot
    {
        const T zero = (T) 0;
        const T one = (T) 1;
        const T two = (T) 2;

        Math::Vec3<T> a = {0.0, 1.0, 1.0};
        Math::Vec3<T> b = {0.0, 0.0, 1.0};
        Math::Vec3<T> c = {0.0,-1.0, 1.0};

        REQUIRE(Math::IsEq(Dot(a,a), two));
        REQUIRE(Math::IsEq(Dot(b,b), one));
        REQUIRE(Math::IsEq(Dot(c,c), two));

        REQUIRE(Math::IsEq(Dot(a,b), one));
        REQUIRE(Math::IsEq(Dot(a,c), zero));
        REQUIRE(Math::IsEq(Dot(b,c), one));
    }

    // Test Norm
    {
        const T sqrt2 = (T) std::sqrt(2.0);
        const T one = (T) 1;

        Math::Vec3<T> a = {0.0, 1.0, 1.0};
        Math::Vec3<T> b = {0.0, 0.0, 1.0};
        Math::Vec3<T> c = {0.0,-1.0, 1.0};

        REQUIRE(Math::IsEq(Norm(a), sqrt2));
        REQUIRE(Math::IsEq(Norm(b), one));
        REQUIRE(Math::IsEq(Norm(c), sqrt2));
    }

    // Test Normalize
    {
        const T one = (T) 1;

        Math::Vec3<T> a = {0.0, 1.0, 1.0};
        Math::Vec3<T> b = {0.0, 0.0, 1.0};
        Math::Vec3<T> c = {0.0,-1.0, 1.0};

        Math::Vec3<T> norm_a = Normalize(a);
        Math::Vec3<T> norm_b = Normalize(b);
        Math::Vec3<T> norm_c = Normalize(c);

        REQUIRE(Math::IsEq(Norm(norm_a), one));
        REQUIRE(Math::IsEq(Norm(norm_b), one));
        REQUIRE(Math::IsEq(Norm(norm_c), one));
    }

    // Test Distance
    {
        const T zero = (T) 0;

        Math::Vec3<T> a = {0.0, 1.0, 1.0};
        Math::Vec3<T> b = {0.0, 0.0, 1.0};
        Math::Vec3<T> c = {0.0,-1.0, 1.0};

        Math::Vec3<T> ab  = a - b;
        Math::Vec3<T> ac  = a - c;
        Math::Vec3<T> cb1 = c - b;
        Math::Vec3<T> cb2 = ab - ac;

        REQUIRE(Math::IsEq(Norm(ab), Distance(a,b)));
        REQUIRE(Math::IsEq(Distance(b,a), Distance(a,b)));

        REQUIRE(Math::IsEq(Norm(ac), Distance(a,c)));
        REQUIRE(Math::IsEq(Distance(a,c), Distance(a,c)));

        REQUIRE(Math::IsEq(Norm(cb1), Norm(cb2)));
        REQUIRE(Math::IsEq(Norm(cb1), Distance(c,b)));
        REQUIRE(Math::IsEq(Distance(b,c), Distance(c,b)));
        REQUIRE(Math::IsEq(Distance(cb1,cb2), zero));
    }

    // Test Cross
    {
        const T zero = (T) 0;

        Math::Vec3<T> x = {1.0, 0.0, 0.0};
        Math::Vec3<T> y = {0.0, 1.0, 0.0};
        Math::Vec3<T> z = {0.0, 0.0, 1.0};

        Math::Vec3<T> cross_xy = Cross(x,y);
        Math::Vec3<T> cross_zx = Cross(z,x);
        Math::Vec3<T> cross_yz = Cross(y,z);

        REQUIRE(Math::IsEq(Distance(cross_xy, z), zero));
        REQUIRE(Math::IsEq(Distance(cross_zx, y), zero));
        REQUIRE(Math::IsEq(Distance(cross_yz, x), zero));
    }

    // Test Random Vectors
    {
        #pragma omp parallel default(none) shared(n_iters)
        {
            size_t n_threads = omp_get_num_threads();
            #pragma omp parallel for schedule(static)
            for (size_t ix = 0; ix < n_threads; ++ix) {
                test_algebra_vector3_run<T>(n_iters);
            }
        }
    }
}

#endif // TEST_MATH_ALGEBRA_VECTOR3_H_
