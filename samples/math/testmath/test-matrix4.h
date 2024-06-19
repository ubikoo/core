//
// test-matrix4.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef TEST_MATH_MATRIX4_H_
#define TEST_MATH_MATRIX4_H_

#include "core/math/math.h"
#include "common.h"

///
/// @brief mat4 test client.
///
template<typename T>
void test_matrix4(void)
{
    // Constructor and assignment.
    {
        math::mat4<T> a{};
        math::mat4<T> b = {
            static_cast<T>(1),
            static_cast<T>(1),
            static_cast<T>(1),
            static_cast<T>(1),

            static_cast<T>(1),
            static_cast<T>(1),
            static_cast<T>(1),
            static_cast<T>(1),

            static_cast<T>(1),
            static_cast<T>(1),
            static_cast<T>(1),
            static_cast<T>(1),

            static_cast<T>(1),
            static_cast<T>(1),
            static_cast<T>(1),
            static_cast<T>(1)};
        math::mat4<T> c = math::mat4<T>::zeros;
        math::mat4<T> d = math::mat4<T>::ones;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == c[i]);
            REQUIRE(b[i] == d[i]);
        }
    }

    // Compound assignment operators (matrix).
    {
        math::mat4<T> a = {};
        a += math::mat4<T>::ones;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(1));
        }

        a -= math::mat4<T>::ones;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(0));
        }

        math::mat4<T> b = {
            static_cast<T>(2),
            static_cast<T>(2),
            static_cast<T>(2),
            static_cast<T>(2),

            static_cast<T>(2),
            static_cast<T>(2),
            static_cast<T>(2),
            static_cast<T>(2),

            static_cast<T>(2),
            static_cast<T>(2),
            static_cast<T>(2),
            static_cast<T>(2),

            static_cast<T>(2),
            static_cast<T>(2),
            static_cast<T>(2),
            static_cast<T>(2)};
        a = math::mat4<T>::ones;
        a *= b;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(2));
        }

        a /= b;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(1));
        }
    }

    // Compound assignment operators (scalar).
    {
        math::mat4<T> a = {};
        a += static_cast<T>(1);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(1));
        }

        a -= static_cast<T>(1);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(0));
        }

        a = math::mat4<T>::ones;
        a *= static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(2));
        }

        a /= static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(1));
        }
    }


    // Arithmetic operators (matrix).
    {
        math::mat4<T> a = math::mat4<T>::ones * static_cast<T>(2);
        math::mat4<T> b = math::mat4<T>::ones * static_cast<T>(4);
        math::mat4<T> c = a + b;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(6));
        }

        c = b - a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(2));
        }

        c = b * a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(8));
        }

        c = b / a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(2));
        }
    }

    // Arithmetic operators (scalar).
    {
        math::mat4<T> a = math::mat4<T>::ones * static_cast<T>(4);
        math::mat4<T> c = a + static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(6));
        }

        c = a - static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(2));
        }

        c = a * static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(8));
        }

        c = a / static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(2));
        }
    }

    // Arithmetic operators (scalar).
    {
        math::mat4<T> a = static_cast<T>(2) * math::mat4<T>::ones;
        math::mat4<T> c = static_cast<T>(2) + a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(4));
        }

        c = static_cast<T>(4) - a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(2));
        }

        c = static_cast<T>(2) * a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(4));
        }

        c = static_cast<T>(2) / a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(1));
        }
    }

    // Unary and increment operators
    {
        math::mat4<T> a = math::mat4<T>::ones * static_cast<T>(2);

        math::mat4<T> c = +a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(2));
        }

        c = -a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(-2));
        }

        c = a++;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(2));
        }

        c = a--;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(3));
        }

        c = ++a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(3));
        }

        c = --a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(2));
        }
    }
}

#endif // TEST_MATH_MATRIX4_H_
