//
// test-ortho.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef TEST_MATH_ORTHO_H_
#define TEST_MATH_ORTHO_H_

#include <iostream>
#include "minicore/math/math.h"
#include "common.h"

///
/// @brief ortho test client.
///
template<typename T>
inline bool test_ortho_is_valid(const Math::Ortho<T> &o)
{
    // Check norm
    {
        T norm_u = std::fabs(Math::Norm(o.u));
        if (!Math::IsEq(norm_u, (T) 1)) {
            std::cerr << "norm_u " << norm_u << "\n";
            return false;
        }

        T norm_v = std::fabs(Math::Norm(o.v));
        if (!Math::IsEq(norm_v, (T) 1)) {
            std::cerr << "norm_v " << norm_v << "\n";
            return false;
        }

        T norm_w = std::fabs(Math::Norm(o.w));
        if (!Math::IsEq(norm_w, (T) 1)) {
            std::cerr << "norm_w " << norm_w << "\n";
            return false;
        }
    }

    // Check orthogonality
    {
        T dot_uv = std::fabs(Math::Dot(o.u, o.v));
        if (!Math::IsEq(dot_uv, (T) 0)) {
            std::cerr << "dot_uv " << dot_uv << "\n";
            return false;
        }

        T dot_uw = std::fabs(Math::Dot(o.u, o.w));
        if (!Math::IsEq(dot_uw, (T) 0)) {
            std::cerr << "dot_uw " << dot_uw << "\n";
            return false;
        }

        T dot_vw = std::fabs(Math::Dot(o.v, o.w));
        if (!Math::IsEq(dot_vw, (T) 0)) {
            std::cerr << "dot_vw " << dot_vw << "\n";
            return false;
        }
    }

    // Check cross products
    {
        T dot_wuv = Math::Dot(o.w, Cross(o.u, o.v));
        if (!Math::IsEq(dot_wuv, (T) 1)) {
            std::cerr << "dot_wuv " << dot_wuv << "\n";
            return false;
        }

        T dot_uvw = Math::Dot(o.u, Cross(o.v, o.w));
        if (!Math::IsEq(dot_uvw, (T) 1)) {
            std::cerr << "dot_uvw " << dot_uvw << "\n";
            return false;
        }

        T dot_vwu = Math::Dot(o.v, Cross(o.w, o.u));
        if (!Math::IsEq(dot_vwu, (T) 1)) {
            std::cerr << "dot_vwu " << dot_vwu << "\n";
            return false;
        }
    }

    return true;
}

template<typename T>
inline bool test_ortho_is_nan(const Math::Vec3<T> &v)
{
    if (std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z)) {
        std::cerr << "isnan " << Math::ToString(v) << "\n";
        return true;
    }
    return false;
}

template<typename T>
inline void test_ortho_run(const size_t n_iters)
{
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_real_distribution<T> dist(-1.0, 1.0);

    static const T zero = static_cast<T>(0);
    static const T two = static_cast<T>(2);

    for (size_t iter = 0; iter < n_iters; ++iter) {
        Math::Vec3<T> arr_u = {two + dist(rng), dist(rng), dist(rng)};
        Math::Vec3<T> arr_v = {dist(rng), two + dist(rng), dist(rng)};
        Math::Vec3<T> arr_w = {dist(rng), dist(rng), two + dist(rng)};

        // Check vector samples
        REQUIRE(!test_ortho_is_nan(arr_u));
        REQUIRE(!test_ortho_is_nan(arr_v));
        REQUIRE(!test_ortho_is_nan(arr_w));

        // Error message
        auto errmsg1 = [] (
            const std::string msg,
            const Math::Vec3<T> &a,
            const Math::Ortho<T> &ortho) -> void {
            std::cerr << msg << "\n";
            std::cerr << "a " << Math::ToString(a) << "\n";
            std::cerr << "ortho " <<  Math::ToString(ortho) << "\n";
        };

        auto errmsg2 = [] (
            const std::string msg,
            const Math::Vec3<T> &a,
            const Math::Vec3<T> &b,
            const Math::Ortho<T> &ortho) -> void {
            std::cerr << msg << "\n";
            std::cerr << "a " << Math::ToString(a) << "\n";
            std::cerr << "b " <<  Math::ToString(b) << "\n";
            std::cerr << "ortho " <<  Math::ToString(ortho) << "\n";
        };

        // Check CreateFromU
        {
            Math::Ortho<T> ortho = Math::Ortho<T>::CreateFromU(arr_u);
            bool isValid = test_ortho_is_valid(ortho);
            if (!isValid) {
                errmsg1("CreateFromU", arr_u, ortho);
            }
            REQUIRE(isValid);
        }

        // Check CreateFromV
        {
            Math::Ortho<T> ortho = Math::Ortho<T>::CreateFromV(arr_v);
            bool isValid = test_ortho_is_valid(ortho);
            if (!isValid) {
                errmsg1("CreateFromV", arr_v, ortho);
            }
            REQUIRE(isValid);
        }

        // Check CreateFromW
        {
            Math::Ortho<T> ortho = Math::Ortho<T>::CreateFromW(arr_w);
            bool isValid = test_ortho_is_valid(ortho);
            if (!isValid) {
                errmsg1("CreateFromW", arr_w, ortho);
            }
            REQUIRE(isValid);
        }

        // Check CreateFromUV
        {
            Math::Ortho<T> ortho = Math::Ortho<T>::CreateFromUV(arr_u, arr_v);
            bool isValid = test_ortho_is_valid(ortho);
            if (!isValid) {
                errmsg2("CreateFromUV", arr_u, arr_v, ortho);
            }
            REQUIRE(isValid);
        }

        // Check CreateFromVU
        {
            Math::Ortho<T> ortho = Math::Ortho<T>::CreateFromVU(arr_v, arr_u);
            bool isValid = test_ortho_is_valid(ortho);
            if (!isValid) {
                errmsg2("CreateFromVU", arr_v, arr_u, ortho);
            }
            REQUIRE(isValid);
        }

        // Check CreateFromVW
        {
            Math::Ortho<T> ortho = Math::Ortho<T>::CreateFromVW(arr_v, arr_w);
            bool isValid = test_ortho_is_valid(ortho);
            if (!isValid) {
                errmsg2("CreateFromVW", arr_v, arr_w, ortho);
            }
            REQUIRE(isValid);
        }

        // Check CreateFromWV
        {
            Math::Ortho<T> ortho = Math::Ortho<T>::CreateFromWV(arr_w, arr_v);
            bool isValid = test_ortho_is_valid(ortho);
            if (!isValid) {
                errmsg2("CreateFromWV", arr_w, arr_v, ortho);
            }
            REQUIRE(isValid);
        }

        // Check CreateFromWU
        {
            Math::Ortho<T> ortho = Math::Ortho<T>::CreateFromWU(arr_w, arr_u);
            bool isValid = test_ortho_is_valid(ortho);
            if (!isValid) {
                errmsg2("CreateFromWU", arr_w, arr_u, ortho);
            }
            REQUIRE(isValid);
        }

        // Check CreateFromUW
        {
            Math::Ortho<T> ortho = Math::Ortho<T>::CreateFromUW(arr_u, arr_w);
            bool isValid = test_ortho_is_valid(ortho);
            if (!isValid) {
                errmsg2("CreateFromUW", arr_u, arr_w, ortho);
            }
            REQUIRE(isValid);
        }

        // Check WorldToLocal and LocalToWorld
        {
            Math::Ortho<T> ortho = Math::Ortho<T>::CreateFromU(arr_u);
            Math::Vec3<T> a = {dist(rng), dist(rng), dist(rng)};
            Math::Vec3<T> b = ortho.WorldToLocal(a);
            Math::Vec3<T> e = a - ortho.LocalToWorld(b);
            bool isValid = Math::IsEq(Norm(e), zero);
            if (!isValid) {
                std::cerr << "WorldToLocal -> LocalToWorld\n";
                std::cerr << "o: " << Math::ToString(ortho) << "\n";
                std::cerr << "a: " << Math::ToString(a) << "\n";
                std::cerr << "b: " << Math::ToString(b) << "\n";
                std::cerr << "e: " << Math::ToString(e) << "\n";
                std::cerr << "norm e: " << Math::ToString(Norm(e)) << "\n";
            }
            REQUIRE(isValid);
        }
    }
}

template<typename T>
inline void test_ortho(const size_t n_iters)
{
    // Test random orthonormal basis
    #pragma omp parallel default(none) shared(n_iters)
    {
        size_t n_threads = omp_get_num_threads();
        #pragma omp parallel for schedule(static)
        for (size_t ix = 0; ix < n_threads; ++ix) {
            test_ortho_run<T>(n_iters);
        }
    }
}

#endif // TEST_MATH_ORTHO_H_
