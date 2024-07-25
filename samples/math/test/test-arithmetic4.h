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

#include "minicore/math/math.h"
#include "common.h"

///
/// @brief Vec4 arithmetic test client.
///
template<typename T>
void test_arithmetic4(void)
{
    // round
    {
        Math::Vec4<T> a = {
            static_cast<T>(-1.1),
            static_cast<T>(-0.9),
            static_cast<T>( 0.9),
            static_cast<T>( 1.0)};
        Math::Vec4<T> round_a = Math::Round<T>(a);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(Math::IsEq(round_a[i], std::round(a[i])));
        }
    }

    // floor
    {
        Math::Vec4<T> a = {
            static_cast<T>(-1.1),
            static_cast<T>(-0.9),
            static_cast<T>( 0.9),
            static_cast<T>( 1.0)};
        Math::Vec4<T> floor_a = Math::Floor<T>(a);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(Math::IsEq(floor_a[i], std::floor(a[i])));
        }
    }

    // ceil
    {
        Math::Vec4<T> a = {
            static_cast<T>(-1.1),
            static_cast<T>(-0.9),
            static_cast<T>( 0.9),
            static_cast<T>( 1.0)};
        Math::Vec4<T> ceil_a = Math::Ceil<T>(a);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(Math::IsEq(ceil_a[i], std::ceil(a[i])));
        }
    }

    // mod
    {
        Math::Vec4<T> a = {
            static_cast<T>(2.5),
            static_cast<T>(1.5),
            static_cast<T>(3.5),
            static_cast<T>(4.5)};
        Math::Vec4<T> m = Math::Vec4<T>::Ones * static_cast<T>(2);
        Math::Vec4<T> r = Math::Mod<T>(a, m);
        REQUIRE(Math::IsEq(r.x, static_cast<T>(0.5)));
        REQUIRE(Math::IsEq(r.y, static_cast<T>(1.5)));
        REQUIRE(Math::IsEq(r.z, static_cast<T>(1.5)));
        REQUIRE(Math::IsEq(r.w, static_cast<T>(0.5)));
    }

    // dirac
    {
        T e = static_cast<T>(1);
        Math::Vec4<T> u = Math::Vec4<T>::Ones * static_cast<T>(0.1);
        Math::Vec4<T> a = Math::Dirac<T>(e, u);
        REQUIRE(Math::IsEq(a.x, static_cast<T>(1)));
        REQUIRE(Math::IsEq(a.y, static_cast<T>(1)));
        REQUIRE(Math::IsEq(a.z, static_cast<T>(1)));
        REQUIRE(Math::IsEq(a.w, static_cast<T>(1)));
    }

    // step
    {
        T epsilon = std::numeric_limits<T>::epsilon();
        Math::Vec4<T> u = {
            static_cast<T>(2) * epsilon,
            epsilon,
            -epsilon,
            static_cast<T>(-2) * epsilon};
        Math::Vec4<T> step = Math::Step<T>(u);
        REQUIRE(Math::IsEq(step.x, static_cast<T>(1)));
        REQUIRE(Math::IsEq(step.y, static_cast<T>(1)));
        REQUIRE(Math::IsEq(step.z, static_cast<T>(0)));
        REQUIRE(Math::IsEq(step.w, static_cast<T>(0)));
    }

    // smoothstep
    {
        Math::Vec4<T> lo = Math::Vec4<T>::Ones * static_cast<T>(-1);
        Math::Vec4<T> hi = Math::Vec4<T>::Ones * static_cast<T>( 1);

        Math::Vec4<T> u = {-2.0, -1.0, 1.0,  2.0};
        Math::Vec4<T> step = Math::SmoothStep<T>(lo, hi, u);
        REQUIRE(Math::IsEq(step.x, static_cast<T>(0)));
        REQUIRE(Math::IsEq(step.y, static_cast<T>(0)));
        REQUIRE(Math::IsEq(step.z, static_cast<T>(1)));
        REQUIRE(Math::IsEq(step.w, static_cast<T>(1)));

        u = (lo + hi) * static_cast<T>(0.5);
        step = Math::SmoothStep<T>(lo, hi, u);
        REQUIRE(Math::IsEq(step.x, static_cast<T>(0.5)));
        REQUIRE(Math::IsEq(step.y, static_cast<T>(0.5)));
        REQUIRE(Math::IsEq(step.z, static_cast<T>(0.5)));
        REQUIRE(Math::IsEq(step.w, static_cast<T>(0.5)));
    }

    // lerp
    {
        auto lerp_fun = [] (
            const Math::Vec4<T> &lo,
            const Math::Vec4<T> &hi,
            const T u) -> Math::Vec4<T> {
            return (lo * (static_cast<T>(1) - u) + hi * u);
        };

        Math::Vec4<T> lo = Math::Vec4<T>::Ones * static_cast<T>(-1);
        Math::Vec4<T> hi = Math::Vec4<T>::Ones * static_cast<T>( 1);

        T delta = static_cast<T>(0.01);
        T u = 0.0;
        while (u < 1.0) {
            Math::Vec4<T> a = Math::Lerp(lo,  hi, {u, u, u, u});
            Math::Vec4<T> b = lerp_fun(lo,  hi, u);
            Math::Vec4<T> e = Math::Abs(a - b);
            REQUIRE(Math::IsEq(e.x, static_cast<T>(0)));
            REQUIRE(Math::IsEq(e.y, static_cast<T>(0)));
            REQUIRE(Math::IsEq(e.z, static_cast<T>(0)));
            REQUIRE(Math::IsEq(e.w, static_cast<T>(0)));
            u += delta;
        }
    }

    // radians/degrees
    {
        Math::Vec4<T> deg = {
            static_cast<T>( 180.0),
            static_cast<T>(  90.0),
            static_cast<T>( -90.0),
            static_cast<T>(-180.0)};

        Math::Vec4<T> rad = Math::Radians<T>(deg);
        REQUIRE(Math::IsEq(rad.x, static_cast<T>(     M_PI)));
        REQUIRE(Math::IsEq(rad.y, static_cast<T>( 0.5*M_PI)));
        REQUIRE(Math::IsEq(rad.z, static_cast<T>(-0.5*M_PI)));
        REQUIRE(Math::IsEq(rad.w, static_cast<T>(    -M_PI)));

        deg = Math::Vec4<T>::Zeros;
        deg = Math::Degrees<T>(rad);
        REQUIRE(Math::IsEq(deg.x, static_cast<T>( 180.0)));
        REQUIRE(Math::IsEq(deg.y, static_cast<T>(  90.0)));
        REQUIRE(Math::IsEq(deg.z, static_cast<T>( -90.0)));
        REQUIRE(Math::IsEq(deg.w, static_cast<T>(-180.0)));
    }

    // swap
    {
        Math::Vec4<T> a = Math::Vec4<T>::Zeros;
        Math::Vec4<T> b = Math::Vec4<T>::Ones;
        Math::Swap<T>(a, b);
        REQUIRE(Math::IsEq(a.x, static_cast<T>(1)));
        REQUIRE(Math::IsEq(a.y, static_cast<T>(1)));
        REQUIRE(Math::IsEq(a.z, static_cast<T>(1)));
        REQUIRE(Math::IsEq(a.w, static_cast<T>(1)));
        REQUIRE(Math::IsEq(b.x, static_cast<T>(0)));
        REQUIRE(Math::IsEq(b.y, static_cast<T>(0)));
        REQUIRE(Math::IsEq(b.z, static_cast<T>(0)));
        REQUIRE(Math::IsEq(b.w, static_cast<T>(0)));
    }

    // sign
    {
        Math::Vec4<T> a = {
            static_cast<T>( 2),
            static_cast<T>( 1),
            static_cast<T>(-1),
            static_cast<T>(-2)};
        Math::Vec4<T> b = Math::Sign<T>(a);
        REQUIRE(Math::IsEq(b.x, static_cast<T>( 1)));
        REQUIRE(Math::IsEq(b.y, static_cast<T>( 1)));
        REQUIRE(Math::IsEq(b.z, static_cast<T>(-1)));
        REQUIRE(Math::IsEq(b.w, static_cast<T>(-1)));
    }


    // abs
    {
        Math::Vec4<T> a = {
            static_cast<T>( 2),
            static_cast<T>( 1),
            static_cast<T>(-1),
            static_cast<T>(-2)};
        Math::Vec4<T> b = Math::Abs<T>(a);
        REQUIRE(Math::IsEq(b.x, static_cast<T>(2)));
        REQUIRE(Math::IsEq(b.y, static_cast<T>(1)));
        REQUIRE(Math::IsEq(b.z, static_cast<T>(1)));
        REQUIRE(Math::IsEq(b.w, static_cast<T>(2)));
    }

    // min
    {
        Math::Vec4<T> a = {
            static_cast<T>( 1),
            static_cast<T>(-1),
            static_cast<T>(-2),
            static_cast<T>( 2)};
        Math::Vec4<T> b = {
            static_cast<T>(-1),
            static_cast<T>( 1),
            static_cast<T>( 2),
            static_cast<T>(-2)};
        Math::Vec4<T> c = Math::Min<T>(a, b);
        REQUIRE(Math::IsEq(c.x, static_cast<T>(-1)));
        REQUIRE(Math::IsEq(c.y, static_cast<T>(-1)));
        REQUIRE(Math::IsEq(c.z, static_cast<T>(-2)));
        REQUIRE(Math::IsEq(c.w, static_cast<T>(-2)));
    }

    // max
    {
        Math::Vec4<T> a = {
            static_cast<T>( 1),
            static_cast<T>(-1),
            static_cast<T>(-2),
            static_cast<T>( 2)};
        Math::Vec4<T> b = {
            static_cast<T>(-1),
            static_cast<T>( 1),
            static_cast<T>( 2),
            static_cast<T>(-2)};
        Math::Vec4<T> c = Math::Max<T>(a, b);
        REQUIRE(Math::IsEq(c.x, static_cast<T>(1)));
        REQUIRE(Math::IsEq(c.y, static_cast<T>(1)));
        REQUIRE(Math::IsEq(c.z, static_cast<T>(2)));
        REQUIRE(Math::IsEq(c.w, static_cast<T>(2)));
    }

    // clamp
    {
        Math::Vec4<T> a = {
            static_cast<T>(-1),
            static_cast<T>(-2),
            static_cast<T>( 1),
            static_cast<T>( 2)};
        Math::Vec4<T> lo = Math::Vec4<T>::Ones * static_cast<T>(-1);
        Math::Vec4<T> hi = Math::Vec4<T>::Ones * static_cast<T>( 1);
        Math::Vec4<T> b = Math::Clamp<T>(a, lo, hi);
        REQUIRE(Math::IsEq(b.x, static_cast<T>(-1)));
        REQUIRE(Math::IsEq(b.y, static_cast<T>(-1)));
        REQUIRE(Math::IsEq(b.z, static_cast<T>( 1)));
        REQUIRE(Math::IsEq(b.w, static_cast<T>( 1)));
    }
}

#endif // TEST_MATH_ARITHMETIC4_H
