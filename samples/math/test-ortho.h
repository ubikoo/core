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
#include "core/math/math.h"
#include "common.h"

///
/// @brief ortho test client.
///
template<typename T>
inline bool test_ortho_is_valid(const math::ortho<T> &o)
{
    // Check norm
    {
        T norm_u = std::fabs(math::norm(o.u));
        if (!math::iseq(norm_u, (T) 1)) {
            std::cerr << "norm_u " << norm_u << "\n";
            return false;
        }

        T norm_v = std::fabs(math::norm(o.v));
        if (!math::iseq(norm_v, (T) 1)) {
            std::cerr << "norm_v " << norm_v << "\n";
            return false;
        }

        T norm_w = std::fabs(math::norm(o.w));
        if (!math::iseq(norm_w, (T) 1)) {
            std::cerr << "norm_w " << norm_w << "\n";
            return false;
        }
    }

    // Check orthogonality
    {
        T dot_uv = std::fabs(math::dot(o.u, o.v));
        if (!math::iseq(dot_uv, (T) 0)) {
            std::cerr << "dot_uv " << dot_uv << "\n";
            return false;
        }

        T dot_uw = std::fabs(math::dot(o.u, o.w));
        if (!math::iseq(dot_uw, (T) 0)) {
            std::cerr << "dot_uw " << dot_uw << "\n";
            return false;
        }

        T dot_vw = std::fabs(math::dot(o.v, o.w));
        if (!math::iseq(dot_vw, (T) 0)) {
            std::cerr << "dot_vw " << dot_vw << "\n";
            return false;
        }
    }

    // Check cross products
    {
        T dot_wuv = math::dot(o.w, cross(o.u, o.v));
        if (!math::iseq(dot_wuv, (T) 1)) {
            std::cerr << "dot_wuv " << dot_wuv << "\n";
            return false;
        }

        T dot_uvw = math::dot(o.u, cross(o.v, o.w));
        if (!math::iseq(dot_uvw, (T) 1)) {
            std::cerr << "dot_uvw " << dot_uvw << "\n";
            return false;
        }

        T dot_vwu = math::dot(o.v, cross(o.w, o.u));
        if (!math::iseq(dot_vwu, (T) 1)) {
            std::cerr << "dot_vwu " << dot_vwu << "\n";
            return false;
        }
    }

    return true;
}

template<typename T>
inline bool test_ortho_is_nan(const math::vec3<T> &v)
{
    if (std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z)) {
        std::cerr << "isnan " << math::to_string(v) << "\n";
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
        math::vec3<T> arr_u = {two + dist(rng), dist(rng), dist(rng)};
        math::vec3<T> arr_v = {dist(rng), two + dist(rng), dist(rng)};
        math::vec3<T> arr_w = {dist(rng), dist(rng), two + dist(rng)};

        // Check vector samples
        REQUIRE(!test_ortho_is_nan(arr_u));
        REQUIRE(!test_ortho_is_nan(arr_v));
        REQUIRE(!test_ortho_is_nan(arr_w));

        // Error message
        auto errmsg1 = [] (
            const std::string msg,
            const math::vec3<T> &a,
            const math::ortho<T> &ortho) -> void {
            std::cerr << msg << "\n";
            std::cerr << "a " << math::to_string(a) << "\n";
            std::cerr << "ortho " <<  math::to_string(ortho) << "\n";
        };

        auto errmsg2 = [] (
            const std::string msg,
            const math::vec3<T> &a,
            const math::vec3<T> &b,
            const math::ortho<T> &ortho) -> void {
            std::cerr << msg << "\n";
            std::cerr << "a " << math::to_string(a) << "\n";
            std::cerr << "b " <<  math::to_string(b) << "\n";
            std::cerr << "ortho " <<  math::to_string(ortho) << "\n";
        };

        // Check CreateFromU
        {
            math::ortho<T> ortho = math::ortho<T>::CreateFromU(arr_u);
            bool is_valid = test_ortho_is_valid(ortho);
            if (!is_valid) {
                errmsg1("CreateFromU", arr_u, ortho);
            }
            REQUIRE(is_valid);
        }

        // Check CreateFromV
        {
            math::ortho<T> ortho = math::ortho<T>::CreateFromV(arr_v);
            bool is_valid = test_ortho_is_valid(ortho);
            if (!is_valid) {
                errmsg1("CreateFromV", arr_v, ortho);
            }
            REQUIRE(is_valid);
        }

        // Check CreateFromW
        {
            math::ortho<T> ortho = math::ortho<T>::CreateFromW(arr_w);
            bool is_valid = test_ortho_is_valid(ortho);
            if (!is_valid) {
                errmsg1("CreateFromW", arr_w, ortho);
            }
            REQUIRE(is_valid);
        }

        // Check CreateFromUV
        {
            math::ortho<T> ortho = math::ortho<T>::CreateFromUV(arr_u, arr_v);
            bool is_valid = test_ortho_is_valid(ortho);
            if (!is_valid) {
                errmsg2("CreateFromUV", arr_u, arr_v, ortho);
            }
            REQUIRE(is_valid);
        }

        // Check CreateFromVU
        {
            math::ortho<T> ortho = math::ortho<T>::CreateFromVU(arr_v, arr_u);
            bool is_valid = test_ortho_is_valid(ortho);
            if (!is_valid) {
                errmsg2("CreateFromVU", arr_v, arr_u, ortho);
            }
            REQUIRE(is_valid);
        }

        // Check CreateFromVW
        {
            math::ortho<T> ortho = math::ortho<T>::CreateFromVW(arr_v, arr_w);
            bool is_valid = test_ortho_is_valid(ortho);
            if (!is_valid) {
                errmsg2("CreateFromVW", arr_v, arr_w, ortho);
            }
            REQUIRE(is_valid);
        }

        // Check CreateFromWV
        {
            math::ortho<T> ortho = math::ortho<T>::CreateFromWV(arr_w, arr_v);
            bool is_valid = test_ortho_is_valid(ortho);
            if (!is_valid) {
                errmsg2("CreateFromWV", arr_w, arr_v, ortho);
            }
            REQUIRE(is_valid);
        }

        // Check CreateFromWU
        {
            math::ortho<T> ortho = math::ortho<T>::CreateFromWU(arr_w, arr_u);
            bool is_valid = test_ortho_is_valid(ortho);
            if (!is_valid) {
                errmsg2("CreateFromWU", arr_w, arr_u, ortho);
            }
            REQUIRE(is_valid);
        }

        // Check CreateFromUW
        {
            math::ortho<T> ortho = math::ortho<T>::CreateFromUW(arr_u, arr_w);
            bool is_valid = test_ortho_is_valid(ortho);
            if (!is_valid) {
                errmsg2("CreateFromUW", arr_u, arr_w, ortho);
            }
            REQUIRE(is_valid);
        }

        // Check WorldToLocal and LocalToWorld
        {
            math::ortho<T> ortho = math::ortho<T>::CreateFromU(arr_u);
            math::vec3<T> a = {dist(rng), dist(rng), dist(rng)};
            math::vec3<T> b = ortho.WorldToLocal(a);
            math::vec3<T> e = a - ortho.LocalToWorld(b);
            bool is_valid = math::iseq(norm(e), zero);
            if (!is_valid) {
                std::cerr << "WorldToLocal -> LocalToWorld\n";
                std::cerr << "o: " << math::to_string(ortho) << "\n";
                std::cerr << "a: " << math::to_string(a) << "\n";
                std::cerr << "b: " << math::to_string(b) << "\n";
                std::cerr << "e: " << math::to_string(e) << "\n";
                std::cerr << "norm e: " << math::to_string(norm(e)) << "\n";
            }
            REQUIRE(is_valid);
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
