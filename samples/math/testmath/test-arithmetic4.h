//
// test-arithmetic4.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef TEST_MATH_ARITHMETIC4_H_
#define TEST_MATH_ARITHMETIC4_H_

#include "core/math/math.h"
#include "common.h"

///
/// @brief vec4 arithmetic test client.
///
template<typename T>
void test_arithmetic4(void)
{
    // round
    {
        math::vec4<T> a = {
            static_cast<T>(-1.1),
            static_cast<T>(-0.9),
            static_cast<T>( 0.9),
            static_cast<T>( 1.0)};
        math::vec4<T> round_a = math::round<T>(a);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(math::iseq(round_a[i], std::round(a[i])));
        }
    }

    // floor
    {
        math::vec4<T> a = {
            static_cast<T>(-1.1),
            static_cast<T>(-0.9),
            static_cast<T>( 0.9),
            static_cast<T>( 1.0)};
        math::vec4<T> floor_a = math::floor<T>(a);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(math::iseq(floor_a[i], std::floor(a[i])));
        }
    }

    // ceil
    {
        math::vec4<T> a = {
            static_cast<T>(-1.1),
            static_cast<T>(-0.9),
            static_cast<T>( 0.9),
            static_cast<T>( 1.0)};
        math::vec4<T> ceil_a = math::ceil<T>(a);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(math::iseq(ceil_a[i], std::ceil(a[i])));
        }
    }

    // mod
    {
        math::vec4<T> a = {
            static_cast<T>(2.5),
            static_cast<T>(1.5),
            static_cast<T>(3.5),
            static_cast<T>(4.5)};
        math::vec4<T> m = math::vec4<T>::ones * static_cast<T>(2);
        math::vec4<T> r = math::mod<T>(a, m);
        REQUIRE(math::iseq(r.x, static_cast<T>(0.5)));
        REQUIRE(math::iseq(r.y, static_cast<T>(1.5)));
        REQUIRE(math::iseq(r.z, static_cast<T>(1.5)));
        REQUIRE(math::iseq(r.w, static_cast<T>(0.5)));
    }

    // dirac
    {
        T e = static_cast<T>(1);
        math::vec4<T> u = math::vec4<T>::ones * static_cast<T>(0.1);
        math::vec4<T> a = math::dirac<T>(e, u);
        REQUIRE(math::iseq(a.x, static_cast<T>(1)));
        REQUIRE(math::iseq(a.y, static_cast<T>(1)));
        REQUIRE(math::iseq(a.z, static_cast<T>(1)));
        REQUIRE(math::iseq(a.w, static_cast<T>(1)));
    }

    // step
    {
        T epsilon = std::numeric_limits<T>::epsilon();
        math::vec4<T> u = {
            static_cast<T>(2) * epsilon,
            epsilon,
            -epsilon,
            static_cast<T>(-2) * epsilon};
        math::vec4<T> step = math::step<T>(u);
        REQUIRE(math::iseq(step.x, static_cast<T>(1)));
        REQUIRE(math::iseq(step.y, static_cast<T>(1)));
        REQUIRE(math::iseq(step.z, static_cast<T>(0)));
        REQUIRE(math::iseq(step.w, static_cast<T>(0)));
    }

    // smoothstep
    {
        math::vec4<T> lo = math::vec4<T>::ones * static_cast<T>(-1);
        math::vec4<T> hi = math::vec4<T>::ones * static_cast<T>( 1);

        math::vec4<T> u = {-2.0, -1.0, 1.0,  2.0};
        math::vec4<T> step = math::smoothstep<T>(lo, hi, u);
        REQUIRE(math::iseq(step.x, static_cast<T>(0)));
        REQUIRE(math::iseq(step.y, static_cast<T>(0)));
        REQUIRE(math::iseq(step.z, static_cast<T>(1)));
        REQUIRE(math::iseq(step.w, static_cast<T>(1)));

        u = (lo + hi) * static_cast<T>(0.5);
        step = math::smoothstep<T>(lo, hi, u);
        REQUIRE(math::iseq(step.x, static_cast<T>(0.5)));
        REQUIRE(math::iseq(step.y, static_cast<T>(0.5)));
        REQUIRE(math::iseq(step.z, static_cast<T>(0.5)));
        REQUIRE(math::iseq(step.w, static_cast<T>(0.5)));
    }

    // lerp
    {
        auto lerp_fun = [] (
            const math::vec4<T> &lo,
            const math::vec4<T> &hi,
            const T u) -> math::vec4<T> {
            return (lo * (static_cast<T>(1) - u) + hi * u);
        };

        math::vec4<T> lo = math::vec4<T>::ones * static_cast<T>(-1);
        math::vec4<T> hi = math::vec4<T>::ones * static_cast<T>( 1);

        T delta = static_cast<T>(0.01);
        T u = 0.0;
        while (u < 1.0) {
            math::vec4<T> a = math::lerp(lo,  hi, {u, u, u, u});
            math::vec4<T> b = lerp_fun(lo,  hi, u);
            math::vec4<T> e = math::abs(a - b);
            REQUIRE(math::iseq(e.x, static_cast<T>(0)));
            REQUIRE(math::iseq(e.y, static_cast<T>(0)));
            REQUIRE(math::iseq(e.z, static_cast<T>(0)));
            REQUIRE(math::iseq(e.w, static_cast<T>(0)));
            u += delta;
        }
    }

    // radians/degrees
    {
        math::vec4<T> deg = {
            static_cast<T>( 180.0),
            static_cast<T>(  90.0),
            static_cast<T>( -90.0),
            static_cast<T>(-180.0)};

        math::vec4<T> rad = math::radians<T>(deg);
        REQUIRE(math::iseq(rad.x, static_cast<T>(     M_PI)));
        REQUIRE(math::iseq(rad.y, static_cast<T>( 0.5*M_PI)));
        REQUIRE(math::iseq(rad.z, static_cast<T>(-0.5*M_PI)));
        REQUIRE(math::iseq(rad.w, static_cast<T>(    -M_PI)));

        deg = math::vec4<T>::zeros;
        deg = math::degrees<T>(rad);
        REQUIRE(math::iseq(deg.x, static_cast<T>( 180.0)));
        REQUIRE(math::iseq(deg.y, static_cast<T>(  90.0)));
        REQUIRE(math::iseq(deg.z, static_cast<T>( -90.0)));
        REQUIRE(math::iseq(deg.w, static_cast<T>(-180.0)));
    }

    // swap
    {
        math::vec4<T> a = math::vec4<T>::zeros;
        math::vec4<T> b = math::vec4<T>::ones;
        math::swap<T>(a, b);
        REQUIRE(math::iseq(a.x, static_cast<T>(1)));
        REQUIRE(math::iseq(a.y, static_cast<T>(1)));
        REQUIRE(math::iseq(a.z, static_cast<T>(1)));
        REQUIRE(math::iseq(a.w, static_cast<T>(1)));
        REQUIRE(math::iseq(b.x, static_cast<T>(0)));
        REQUIRE(math::iseq(b.y, static_cast<T>(0)));
        REQUIRE(math::iseq(b.z, static_cast<T>(0)));
        REQUIRE(math::iseq(b.w, static_cast<T>(0)));
    }

    // sign
    {
        math::vec4<T> a = {
            static_cast<T>( 2),
            static_cast<T>( 1),
            static_cast<T>(-1),
            static_cast<T>(-2)};
        math::vec4<T> b = math::sign<T>(a);
        REQUIRE(math::iseq(b.x, static_cast<T>( 1)));
        REQUIRE(math::iseq(b.y, static_cast<T>( 1)));
        REQUIRE(math::iseq(b.z, static_cast<T>(-1)));
        REQUIRE(math::iseq(b.w, static_cast<T>(-1)));
    }


    // abs
    {
        math::vec4<T> a = {
            static_cast<T>( 2),
            static_cast<T>( 1),
            static_cast<T>(-1),
            static_cast<T>(-2)};
        math::vec4<T> b = math::abs<T>(a);
        REQUIRE(math::iseq(b.x, static_cast<T>(2)));
        REQUIRE(math::iseq(b.y, static_cast<T>(1)));
        REQUIRE(math::iseq(b.z, static_cast<T>(1)));
        REQUIRE(math::iseq(b.w, static_cast<T>(2)));
    }

    // min
    {
        math::vec4<T> a = {
            static_cast<T>( 1),
            static_cast<T>(-1),
            static_cast<T>(-2),
            static_cast<T>( 2)};
        math::vec4<T> b = {
            static_cast<T>(-1),
            static_cast<T>( 1),
            static_cast<T>( 2),
            static_cast<T>(-2)};
        math::vec4<T> c = math::min<T>(a, b);
        REQUIRE(math::iseq(c.x, static_cast<T>(-1)));
        REQUIRE(math::iseq(c.y, static_cast<T>(-1)));
        REQUIRE(math::iseq(c.z, static_cast<T>(-2)));
        REQUIRE(math::iseq(c.w, static_cast<T>(-2)));
    }

    // max
    {
        math::vec4<T> a = {
            static_cast<T>( 1),
            static_cast<T>(-1),
            static_cast<T>(-2),
            static_cast<T>( 2)};
        math::vec4<T> b = {
            static_cast<T>(-1),
            static_cast<T>( 1),
            static_cast<T>( 2),
            static_cast<T>(-2)};
        math::vec4<T> c = math::max<T>(a, b);
        REQUIRE(math::iseq(c.x, static_cast<T>(1)));
        REQUIRE(math::iseq(c.y, static_cast<T>(1)));
        REQUIRE(math::iseq(c.z, static_cast<T>(2)));
        REQUIRE(math::iseq(c.w, static_cast<T>(2)));
    }

    // clamp
    {
        math::vec4<T> a = {
            static_cast<T>(-1),
            static_cast<T>(-2),
            static_cast<T>( 1),
            static_cast<T>( 2)};
        math::vec4<T> lo = math::vec4<T>::ones * static_cast<T>(-1);
        math::vec4<T> hi = math::vec4<T>::ones * static_cast<T>( 1);
        math::vec4<T> b = math::clamp<T>(a, lo, hi);
        REQUIRE(math::iseq(b.x, static_cast<T>(-1)));
        REQUIRE(math::iseq(b.y, static_cast<T>(-1)));
        REQUIRE(math::iseq(b.z, static_cast<T>( 1)));
        REQUIRE(math::iseq(b.w, static_cast<T>( 1)));
    }
}

#endif // TEST_MATH_ARITHMETIC4_H
