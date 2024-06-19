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

#include "core/math/math.h"
#include "common.h"

///
/// @brief vec2 arithmetic test client.
///
template<typename T>
void test_arithmetic2(void)
{
    // round
    {
        math::vec2<T> a = {static_cast<T>(-1.1), static_cast<T>(-0.9)};
        math::vec2<T> round_a = math::round<T>(a);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(math::iseq(round_a[i], std::round(a[i])));
        }

        math::vec2<T> b = {static_cast<T>(1.1), static_cast<T>(0.9)};
        math::vec2<T> round_b = math::round<T>(b);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(math::iseq(round_b[i], std::round(b[i])));
        }
    }

    // floor
    {
        math::vec2<T> a = {static_cast<T>(-1.1), static_cast<T>(-0.9)};
        math::vec2<T> floor_a = math::floor<T>(a);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(math::iseq(floor_a[i], std::floor(a[i])));
        }

        math::vec2<T> b = {static_cast<T>(1.1), static_cast<T>(0.9)};
        math::vec2<T> floor_b = math::floor<T>(b);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(math::iseq(floor_b[i], std::floor(b[i])));
        }
    }

    // ceil
    {
        math::vec2<T> a = {static_cast<T>(-1.1), static_cast<T>(-0.9)};
        math::vec2<T> ceil_a = math::ceil<T>(a);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(math::iseq(ceil_a[i], std::ceil(a[i])));
        }

        math::vec2<T> b = {static_cast<T>(1.1), static_cast<T>(0.9)};
        math::vec2<T> ceil_b = math::ceil<T>(b);
        for (size_t i = 0; i < a.length; i++) {
            REQUIRE(math::iseq(ceil_b[i], std::ceil(b[i])));
        }
    }

    // mod
    {
        math::vec2<T> a = {static_cast<T>(2.5), static_cast<T>(1.5)};
        math::vec2<T> m = math::vec2<T>::ones * static_cast<T>(2);
        math::vec2<T> r = math::mod<T>(a, m);
        REQUIRE(math::iseq(r.x, static_cast<T>(0.5)));
        REQUIRE(math::iseq(r.y, static_cast<T>(1.5)));
    }

    // dirac
    {
        T e = static_cast<T>(1);
        math::vec2<T> u = math::vec2<T>::ones * static_cast<T>(0.1);
        math::vec2<T> a = math::dirac<T>(e, u);
        REQUIRE(math::iseq(a.x, static_cast<T>(1)));
        REQUIRE(math::iseq(a.y, static_cast<T>(1)));
    }

    // step
    {
        T epsilon = std::numeric_limits<T>::epsilon();
        math::vec2<T> u = {epsilon, -epsilon};
        math::vec2<T> step = math::step<T>(u);
        REQUIRE(math::iseq(step.x, static_cast<T>(1)));
        REQUIRE(math::iseq(step.y, static_cast<T>(0)));
    }

    // smoothstep
    {
        math::vec2<T> lo = math::vec2<T>::ones * static_cast<T>(-1);
        math::vec2<T> hi = math::vec2<T>::ones * static_cast<T>( 1);

        math::vec2<T> u = {-2.0, 2.0};
        math::vec2<T> step = math::smoothstep<T>(lo, hi, u);
        REQUIRE(math::iseq(step.x, static_cast<T>(0)));
        REQUIRE(math::iseq(step.y, static_cast<T>(1)));

        u = (lo + hi) * static_cast<T>(0.5);
        step = math::smoothstep<T>(lo, hi, u);
        REQUIRE(math::iseq(step.x, static_cast<T>(0.5)));
        REQUIRE(math::iseq(step.y, static_cast<T>(0.5)));
    }

    // lerp
    {
        auto lerp_fun = [] (
            const math::vec2<T> &lo,
            const math::vec2<T> &hi,
            const T u) -> math::vec2<T> {
            return (lo * (static_cast<T>(1) - u) + hi * u);
        };

        math::vec2<T> lo = math::vec2<T>::ones * static_cast<T>(-1);
        math::vec2<T> hi = math::vec2<T>::ones * static_cast<T>( 1);

        T delta = static_cast<T>(0.01);
        T u = 0.0;
        while (u < 1.0) {
            math::vec2<T> a = math::lerp(lo,  hi, {u, u});
            math::vec2<T> b = lerp_fun(lo,  hi, u);
            math::vec2<T> e = math::abs(a - b);
            REQUIRE(math::iseq(e.x, static_cast<T>(0.0)));
            REQUIRE(math::iseq(e.y, static_cast<T>(0.0)));
            u += delta;
        }
    }

    // radians/degrees
    {
        math::vec2<T> deg = {static_cast<T>(90.0), static_cast<T>(-90.0)};

        math::vec2<T> rad = math::radians<T>(deg);
        REQUIRE(math::iseq(rad.x, static_cast<T>( 0.5*M_PI)));
        REQUIRE(math::iseq(rad.y, static_cast<T>(-0.5*M_PI)));

        deg = math::vec2<T>::zeros;
        deg = math::degrees<T>(rad);
        REQUIRE(math::iseq(deg.x, static_cast<T>( 90.0)));
        REQUIRE(math::iseq(deg.y, static_cast<T>(-90.0)));
    }

    // swap
    {
        math::vec2<T> a = math::vec2<T>::zeros;
        math::vec2<T> b = math::vec2<T>::ones;
        math::swap<T>(a, b);
        REQUIRE(math::iseq(a.x, static_cast<T>(1)));
        REQUIRE(math::iseq(a.y, static_cast<T>(1)));
        REQUIRE(math::iseq(b.x, static_cast<T>(0)));
        REQUIRE(math::iseq(b.y, static_cast<T>(0)));
    }

    // sign
    {
        math::vec2<T> a = {static_cast<T>(2), static_cast<T>(-2)};
        math::vec2<T> b = math::sign<T>(a);
        REQUIRE(math::iseq(b.x, static_cast<T>( 1)));
        REQUIRE(math::iseq(b.y, static_cast<T>(-1)));
    }


    // abs
    {
        math::vec2<T> a = {static_cast<T>(-1), static_cast<T>(-2)};
        math::vec2<T> b = math::abs<T>(a);
        REQUIRE(math::iseq(b.x, static_cast<T>(1)));
        REQUIRE(math::iseq(b.y, static_cast<T>(2)));
    }

    // min
    {
        math::vec2<T> a = {static_cast<T>(-1), static_cast<T>( 2)};
        math::vec2<T> b = {static_cast<T>( 1), static_cast<T>(-2)};
        math::vec2<T> c = math::min<T>(a, b);
        REQUIRE(math::iseq(c.x, static_cast<T>(-1)));
        REQUIRE(math::iseq(c.y, static_cast<T>(-2)));
    }

    // max
    {
        math::vec2<T> a = {static_cast<T>(-1), static_cast<T>( 2)};
        math::vec2<T> b = {static_cast<T>( 1), static_cast<T>(-2)};
        math::vec2<T> c = math::max<T>(a, b);
        REQUIRE(math::iseq(c.x, static_cast<T>(1)));
        REQUIRE(math::iseq(c.y, static_cast<T>(2)));
    }

    // clamp
    {
        math::vec2<T> a = {static_cast<T>(-2), static_cast<T>( 2)};
        math::vec2<T> lo = math::vec2<T>::ones * static_cast<T>(-1);
        math::vec2<T> hi = math::vec2<T>::ones * static_cast<T>( 1);
        math::vec2<T> b = math::clamp<T>(a, lo, hi);
        REQUIRE(math::iseq(b.x, static_cast<T>(-1)));
        REQUIRE(math::iseq(b.y, static_cast<T>( 1)));
    }
}

#endif // TEST_MATH_ARITHMETIC2_H_
