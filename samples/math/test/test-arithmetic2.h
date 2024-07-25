//
// test-arithmetic2.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef TEST_MATH_ARITHMETIC2_H_
#define TEST_MATH_ARITHMETIC2_H_

#include "minicore/math/math.h"
#include "common.h"

///
/// @brief Vec2 arithmetic test client.
///
template<typename T>
void test_arithmetic2(void)
{
    // round
    {
        Math::Vec2<T> a = {static_cast<T>(-1.1), static_cast<T>(-0.9)};
        Math::Vec2<T> round_a = Math::Round<T>(a);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(Math::IsEq(round_a[i], std::round(a[i])));
        }

        Math::Vec2<T> b = {static_cast<T>(1.1), static_cast<T>(0.9)};
        Math::Vec2<T> round_b = Math::Round<T>(b);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(Math::IsEq(round_b[i], std::round(b[i])));
        }
    }

    // floor
    {
        Math::Vec2<T> a = {static_cast<T>(-1.1), static_cast<T>(-0.9)};
        Math::Vec2<T> floor_a = Math::Floor<T>(a);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(Math::IsEq(floor_a[i], std::floor(a[i])));
        }

        Math::Vec2<T> b = {static_cast<T>(1.1), static_cast<T>(0.9)};
        Math::Vec2<T> floor_b = Math::Floor<T>(b);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(Math::IsEq(floor_b[i], std::floor(b[i])));
        }
    }

    // ceil
    {
        Math::Vec2<T> a = {static_cast<T>(-1.1), static_cast<T>(-0.9)};
        Math::Vec2<T> ceil_a = Math::Ceil<T>(a);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(Math::IsEq(ceil_a[i], std::ceil(a[i])));
        }

        Math::Vec2<T> b = {static_cast<T>(1.1), static_cast<T>(0.9)};
        Math::Vec2<T> ceil_b = Math::Ceil<T>(b);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(Math::IsEq(ceil_b[i], std::ceil(b[i])));
        }
    }

    // mod
    {
        Math::Vec2<T> a = {static_cast<T>(2.5), static_cast<T>(1.5)};
        Math::Vec2<T> m = Math::Vec2<T>::Ones * static_cast<T>(2);
        Math::Vec2<T> r = Math::Mod<T>(a, m);
        REQUIRE(Math::IsEq(r.x, static_cast<T>(0.5)));
        REQUIRE(Math::IsEq(r.y, static_cast<T>(1.5)));
    }

    // dirac
    {
        T e = static_cast<T>(1);
        Math::Vec2<T> u = Math::Vec2<T>::Ones * static_cast<T>(0.1);
        Math::Vec2<T> a = Math::Dirac<T>(e, u);
        REQUIRE(Math::IsEq(a.x, static_cast<T>(1)));
        REQUIRE(Math::IsEq(a.y, static_cast<T>(1)));
    }

    // step
    {
        T epsilon = std::numeric_limits<T>::epsilon();
        Math::Vec2<T> u = {epsilon, -epsilon};
        Math::Vec2<T> step = Math::Step<T>(u);
        REQUIRE(Math::IsEq(step.x, static_cast<T>(1)));
        REQUIRE(Math::IsEq(step.y, static_cast<T>(0)));
    }

    // smoothstep
    {
        Math::Vec2<T> lo = Math::Vec2<T>::Ones * static_cast<T>(-1);
        Math::Vec2<T> hi = Math::Vec2<T>::Ones * static_cast<T>( 1);

        Math::Vec2<T> u = {-2.0, 2.0};
        Math::Vec2<T> step = Math::SmoothStep<T>(lo, hi, u);
        REQUIRE(Math::IsEq(step.x, static_cast<T>(0)));
        REQUIRE(Math::IsEq(step.y, static_cast<T>(1)));

        u = (lo + hi) * static_cast<T>(0.5);
        step = Math::SmoothStep<T>(lo, hi, u);
        REQUIRE(Math::IsEq(step.x, static_cast<T>(0.5)));
        REQUIRE(Math::IsEq(step.y, static_cast<T>(0.5)));
    }

    // lerp
    {
        auto lerp_fun = [] (
            const Math::Vec2<T> &lo,
            const Math::Vec2<T> &hi,
            const T u) -> Math::Vec2<T> {
            return (lo * (static_cast<T>(1) - u) + hi * u);
        };

        Math::Vec2<T> lo = Math::Vec2<T>::Ones * static_cast<T>(-1);
        Math::Vec2<T> hi = Math::Vec2<T>::Ones * static_cast<T>( 1);

        T delta = static_cast<T>(0.01);
        T u = 0.0;
        while (u < 1.0) {
            Math::Vec2<T> a = Math::Lerp(lo,  hi, {u, u});
            Math::Vec2<T> b = lerp_fun(lo,  hi, u);
            Math::Vec2<T> e = Math::Abs(a - b);
            REQUIRE(Math::IsEq(e.x, static_cast<T>(0.0)));
            REQUIRE(Math::IsEq(e.y, static_cast<T>(0.0)));
            u += delta;
        }
    }

    // radians/degrees
    {
        Math::Vec2<T> deg = {static_cast<T>(90.0), static_cast<T>(-90.0)};

        Math::Vec2<T> rad = Math::Radians<T>(deg);
        REQUIRE(Math::IsEq(rad.x, static_cast<T>( 0.5*M_PI)));
        REQUIRE(Math::IsEq(rad.y, static_cast<T>(-0.5*M_PI)));

        deg = Math::Vec2<T>::Zeros;
        deg = Math::Degrees<T>(rad);
        REQUIRE(Math::IsEq(deg.x, static_cast<T>( 90.0)));
        REQUIRE(Math::IsEq(deg.y, static_cast<T>(-90.0)));
    }

    // swap
    {
        Math::Vec2<T> a = Math::Vec2<T>::Zeros;
        Math::Vec2<T> b = Math::Vec2<T>::Ones;
        Math::Swap<T>(a, b);
        REQUIRE(Math::IsEq(a.x, static_cast<T>(1)));
        REQUIRE(Math::IsEq(a.y, static_cast<T>(1)));
        REQUIRE(Math::IsEq(b.x, static_cast<T>(0)));
        REQUIRE(Math::IsEq(b.y, static_cast<T>(0)));
    }

    // sign
    {
        Math::Vec2<T> a = {static_cast<T>(2), static_cast<T>(-2)};
        Math::Vec2<T> b = Math::Sign<T>(a);
        REQUIRE(Math::IsEq(b.x, static_cast<T>( 1)));
        REQUIRE(Math::IsEq(b.y, static_cast<T>(-1)));
    }


    // abs
    {
        Math::Vec2<T> a = {static_cast<T>(-1), static_cast<T>(-2)};
        Math::Vec2<T> b = Math::Abs<T>(a);
        REQUIRE(Math::IsEq(b.x, static_cast<T>(1)));
        REQUIRE(Math::IsEq(b.y, static_cast<T>(2)));
    }

    // min
    {
        Math::Vec2<T> a = {static_cast<T>(-1), static_cast<T>( 2)};
        Math::Vec2<T> b = {static_cast<T>( 1), static_cast<T>(-2)};
        Math::Vec2<T> c = Math::Min<T>(a, b);
        REQUIRE(Math::IsEq(c.x, static_cast<T>(-1)));
        REQUIRE(Math::IsEq(c.y, static_cast<T>(-2)));
    }

    // max
    {
        Math::Vec2<T> a = {static_cast<T>(-1), static_cast<T>( 2)};
        Math::Vec2<T> b = {static_cast<T>( 1), static_cast<T>(-2)};
        Math::Vec2<T> c = Math::Max<T>(a, b);
        REQUIRE(Math::IsEq(c.x, static_cast<T>(1)));
        REQUIRE(Math::IsEq(c.y, static_cast<T>(2)));
    }

    // clamp
    {
        Math::Vec2<T> a = {static_cast<T>(-2), static_cast<T>( 2)};
        Math::Vec2<T> lo = Math::Vec2<T>::Ones * static_cast<T>(-1);
        Math::Vec2<T> hi = Math::Vec2<T>::Ones * static_cast<T>( 1);
        Math::Vec2<T> b = Math::Clamp<T>(a, lo, hi);
        REQUIRE(Math::IsEq(b.x, static_cast<T>(-1)));
        REQUIRE(Math::IsEq(b.y, static_cast<T>( 1)));
    }
}

#endif // TEST_MATH_ARITHMETIC2_H_
