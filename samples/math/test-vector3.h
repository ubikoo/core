//
// test-vector3.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef TEST_MATH_VECTOR3_H_
#define TEST_MATH_VECTOR3_H_

#include "core/math/math.h"
#include "common.h"

///
/// @brief vec3 test client.
///
template<typename T>
void test_vector3(void)
{
    // Constructor and assignment.
    {
        math::vec3<T> a{};
        math::vec3<T> b = {
            static_cast<T>(1),
            static_cast<T>(1),
            static_cast<T>(1)};
        math::vec3<T> c = math::vec3<T>::zeros;
        math::vec3<T> d = math::vec3<T>::ones;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == c[i]);
            REQUIRE(b[i] == d[i]);
        }
    }

    // Compound assignment operators (vector).
    {
        math::vec3<T> a = {};
        a += math::vec3<T>::ones;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(1));
        }

        a -= math::vec3<T>::ones;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(0));
        }

        math::vec3<T> b = {
            static_cast<T>(2),
            static_cast<T>(2),
            static_cast<T>(2)};
        a = math::vec3<T>::ones;
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
        math::vec3<T> a = {};
        a += static_cast<T>(1);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(1));
        }

        a -= static_cast<T>(1);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(0));
        }

        a = math::vec3<T>::ones;
        a *= static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(2));
        }

        a /= static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(1));
        }
    }

    // Arithmetic operators (vector).
    {
        math::vec3<T> a = math::vec3<T>::ones * static_cast<T>(2);
        math::vec3<T> b = math::vec3<T>::ones * static_cast<T>(4);
        math::vec3<T> c = a + b;
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
        math::vec3<T> a = math::vec3<T>::ones * static_cast<T>(4);
        math::vec3<T> c = a + static_cast<T>(2);
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
        math::vec3<T> a = static_cast<T>(2) * math::vec3<T>::ones;
        math::vec3<T> c = static_cast<T>(2) + a;
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
        math::vec3<T> a = math::vec3<T>::ones * static_cast<T>(2);

        math::vec3<T> c = +a;
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

#endif // TEST_MATH_VECTOR3_H_
