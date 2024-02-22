//
// vector.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MATH_VECTOR_H_
#define MATH_VECTOR_H_

namespace math {

/// ---- Vector data types ----------------------------------------------------
template<typename T>
struct vec2 {
    static const vec2 zeros;
    static const vec2 ones;
    static const size_t length = 2;

    union {
        alignas(32) T data[length];
        struct { T s0, s1; };
        struct { T x, y; };
    };

    T &operator[](size_t i) { return data[i]; }
    const T &operator[](size_t i) const { return data[i]; }
};

template<typename T>
struct vec3 {
    static const vec3 zeros;
    static const vec3 ones;
    static const size_t length = 3;

    union {
        alignas(32) T data[length];
        struct { T s0, s1, s2; };
        struct { T x, y, z; };
    };

    T &operator[](size_t i) { return data[i]; }
    const T &operator[](size_t i) const { return data[i]; }
};

template<typename T>
struct vec4 {
    static const vec4 zeros;
    static const vec4 ones;
    static const size_t length = 4;

    union {
        alignas(32) T data[length];
        struct { T s0, s1, s2, s3; };
        struct { T x, y, z, w; };
    };

    T &operator[](size_t i) { return data[i]; }
    const T &operator[](size_t i) const { return data[i]; }
};

typedef vec2<int16_t>   vec2i16;
typedef vec2<int32_t>   vec2i32;
typedef vec2<int64_t>   vec2i64;
typedef vec2<int32_t>   vec2i;
typedef vec2<uint16_t>  vec2u16;
typedef vec2<uint32_t>  vec2u32;
typedef vec2<uint64_t>  vec2u64;
typedef vec2<uint32_t>  vec2u;
typedef vec2<float>     vec2f;
typedef vec2<double>    vec2d;

typedef vec3<int16_t>   vec3i16;
typedef vec3<int32_t>   vec3i32;
typedef vec3<int64_t>   vec3i64;
typedef vec3<int32_t>   vec3i;
typedef vec3<uint16_t>  vec3u16;
typedef vec3<uint32_t>  vec3u32;
typedef vec3<uint64_t>  vec3u64;
typedef vec3<uint32_t>  vec3u;
typedef vec3<float>     vec3f;
typedef vec3<double>    vec3d;

typedef vec4<int16_t>   vec4i16;
typedef vec4<int32_t>   vec4i32;
typedef vec4<int64_t>   vec4i64;
typedef vec4<int32_t>   vec4i;
typedef vec4<uint16_t>  vec4u16;
typedef vec4<uint32_t>  vec4u32;
typedef vec4<uint64_t>  vec4u64;
typedef vec4<uint32_t>  vec4u;
typedef vec4<float>     vec4f;
typedef vec4<double>    vec4d;

/// ---- Special vectors ------------------------------------------------------
template<typename T>
const vec2<T> vec2<T>::zeros = {(T) 0, (T) 0};
template<typename T>
const vec2<T> vec2<T>::ones = {(T) 1, (T) 1};

template<typename T>
const vec3<T> vec3<T>::zeros = {(T) 0, (T) 0, (T) 0};
template<typename T>
const vec3<T> vec3<T>::ones = {(T) 1, (T) 1, (T) 1};

template<typename T>
const vec4<T> vec4<T>::zeros = {(T) 0, (T) 0, (T) 0, (T) 0};
template<typename T>
const vec4<T> vec4<T>::ones = {(T) 1, (T) 1, (T) 1, (T) 1};

/// ---- vec2 declarations ----------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T>
inline vec2<T> &operator+=(vec2<T> &lhs, const vec2<T> &rhs);
template<typename T> inline vec2<T> &operator-=(vec2<T> &lhs, const vec2<T> &rhs);
template<typename T> inline vec2<T> &operator*=(vec2<T> &lhs, const vec2<T> &rhs);
template<typename T> inline vec2<T> &operator/=(vec2<T> &lhs, const vec2<T> &rhs);

template<typename T> inline vec2<T> &operator+=(vec2<T> &lhs, const T scalar);
template<typename T> inline vec2<T> &operator-=(vec2<T> &lhs, const T scalar);
template<typename T> inline vec2<T> &operator*=(vec2<T> &lhs, const T scalar);
template<typename T> inline vec2<T> &operator/=(vec2<T> &lhs, const T scalar);

/// Arithmetic operators.
template<typename T> inline vec2<T> operator+(vec2<T> lhs, const vec2<T> &rhs);
template<typename T> inline vec2<T> operator-(vec2<T> lhs, const vec2<T> &rhs);
template<typename T> inline vec2<T> operator*(vec2<T> lhs, const vec2<T> &rhs);
template<typename T> inline vec2<T> operator/(vec2<T> lhs, const vec2<T> &rhs);

template<typename T> inline vec2<T> operator+(vec2<T> lhs, const T scalar);
template<typename T> inline vec2<T> operator-(vec2<T> lhs, const T scalar);
template<typename T> inline vec2<T> operator*(vec2<T> lhs, const T scalar);
template<typename T> inline vec2<T> operator/(vec2<T> lhs, const T scalar);

template<typename T> inline vec2<T> operator+(const T scalar, vec2<T> rhs);
template<typename T> inline vec2<T> operator-(const T scalar, vec2<T> rhs);
template<typename T> inline vec2<T> operator*(const T scalar, vec2<T> rhs);
template<typename T> inline vec2<T> operator/(const T scalar, vec2<T> rhs);

/// Unary operators.
template<typename T> inline vec2<T> operator+(vec2<T> lhs);
template<typename T> inline vec2<T> operator-(vec2<T> lhs);

/// Increment operators.
template<typename T> inline vec2<T> &operator++(vec2<T> &lhs);
template<typename T> inline vec2<T> &operator--(vec2<T> &lhs);
template<typename T> inline vec2<T> operator++(vec2<T> &lhs, int);
template<typename T> inline vec2<T> operator--(vec2<T> &lhs, int);

/// ---- vec3 declarations ----------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T> inline vec3<T> &operator+=(vec3<T> &lhs, const vec3<T> &rhs);
template<typename T> inline vec3<T> &operator-=(vec3<T> &lhs, const vec3<T> &rhs);
template<typename T> inline vec3<T> &operator*=(vec3<T> &lhs, const vec3<T> &rhs);
template<typename T> inline vec3<T> &operator/=(vec3<T> &lhs, const vec3<T> &rhs);

template<typename T> inline vec3<T> &operator+=(vec3<T> &lhs, const T scalar);
template<typename T> inline vec3<T> &operator-=(vec3<T> &lhs, const T scalar);
template<typename T> inline vec3<T> &operator*=(vec3<T> &lhs, const T scalar);
template<typename T> inline vec3<T> &operator/=(vec3<T> &lhs, const T scalar);

/// Arithmetic operators.
template<typename T> inline vec3<T> operator+(vec3<T> lhs, const vec3<T> &rhs);
template<typename T> inline vec3<T> operator-(vec3<T> lhs, const vec3<T> &rhs);
template<typename T> inline vec3<T> operator*(vec3<T> lhs, const vec3<T> &rhs);
template<typename T> inline vec3<T> operator/(vec3<T> lhs, const vec3<T> &rhs);

template<typename T> inline vec3<T> operator+(vec3<T> lhs, const T scalar);
template<typename T> inline vec3<T> operator-(vec3<T> lhs, const T scalar);
template<typename T> inline vec3<T> operator*(vec3<T> lhs, const T scalar);
template<typename T> inline vec3<T> operator/(vec3<T> lhs, const T scalar);

template<typename T> inline vec3<T> operator+(const T scalar, vec3<T> rhs);
template<typename T> inline vec3<T> operator-(const T scalar, vec3<T> rhs);
template<typename T> inline vec3<T> operator*(const T scalar, vec3<T> rhs);
template<typename T> inline vec3<T> operator/(const T scalar, vec3<T> rhs);

/// Unary operators.
template<typename T> inline vec3<T> operator+(vec3<T> lhs);
template<typename T> inline vec3<T> operator-(vec3<T> lhs);

/// Increment operators.
template<typename T> inline vec3<T> &operator++(vec3<T> &lhs);
template<typename T> inline vec3<T> &operator--(vec3<T> &lhs);
template<typename T> inline vec3<T> operator++(vec3<T> &lhs, int);
template<typename T> inline vec3<T> operator--(vec3<T> &lhs, int);

/// ---- vec4 declarations ----------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T> inline vec4<T> &operator+=(vec4<T> &lhs, const vec4<T> &rhs);
template<typename T> inline vec4<T> &operator-=(vec4<T> &lhs, const vec4<T> &rhs);
template<typename T> inline vec4<T> &operator*=(vec4<T> &lhs, const vec4<T> &rhs);
template<typename T> inline vec4<T> &operator/=(vec4<T> &lhs, const vec4<T> &rhs);

template<typename T> inline vec4<T> &operator+=(vec4<T> &lhs, const T scalar);
template<typename T> inline vec4<T> &operator-=(vec4<T> &lhs, const T scalar);
template<typename T> inline vec4<T> &operator*=(vec4<T> &lhs, const T scalar);
template<typename T> inline vec4<T> &operator/=(vec4<T> &lhs, const T scalar);

/// Arithmetic operators.
template<typename T> inline vec4<T> operator+(vec4<T> lhs, const vec4<T> &rhs);
template<typename T> inline vec4<T> operator-(vec4<T> lhs, const vec4<T> &rhs);
template<typename T> inline vec4<T> operator*(vec4<T> lhs, const vec4<T> &rhs);
template<typename T> inline vec4<T> operator/(vec4<T> lhs, const vec4<T> &rhs);

template<typename T> inline vec4<T> operator+(vec4<T> lhs, const T scalar);
template<typename T> inline vec4<T> operator-(vec4<T> lhs, const T scalar);
template<typename T> inline vec4<T> operator*(vec4<T> lhs, const T scalar);
template<typename T> inline vec4<T> operator/(vec4<T> lhs, const T scalar);

template<typename T> inline vec4<T> operator+(const T scalar, vec4<T> rhs);
template<typename T> inline vec4<T> operator-(const T scalar, vec4<T> rhs);
template<typename T> inline vec4<T> operator*(const T scalar, vec4<T> rhs);
template<typename T> inline vec4<T> operator/(const T scalar, vec4<T> rhs);

/// Unary operators.
template<typename T> inline vec4<T> operator+(vec4<T> lhs);
template<typename T> inline vec4<T> operator-(vec4<T> lhs);

/// Increment operators.
template<typename T> inline vec4<T> &operator++(vec4<T> &lhs);
template<typename T> inline vec4<T> &operator--(vec4<T> &lhs);
template<typename T> inline vec4<T> operator++(vec4<T> &lhs, int);
template<typename T> inline vec4<T> operator--(vec4<T> &lhs, int);

/// ---- vec2 implementation --------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T>
inline vec2<T> &operator+=(vec2<T> &lhs, const vec2<T> &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

template<typename T>
inline vec2<T> &operator-=(vec2<T> &lhs, const vec2<T> &rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

template<typename T>
inline vec2<T> &operator*=(vec2<T> &lhs, const vec2<T> &rhs)
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    return lhs;
}

template<typename T>
inline vec2<T> &operator/=(vec2<T> &lhs, const vec2<T> &rhs)
{
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    return lhs;
}

template<typename T>
inline vec2<T> &operator+=(vec2<T> &lhs, const T scalar)
{
    lhs.x += scalar;
    lhs.y += scalar;
    return lhs;
}

template<typename T>
inline vec2<T> &operator-=(vec2<T> &lhs, const T scalar)
{
    lhs.x -= scalar;
    lhs.y -= scalar;
    return lhs;
}

template<typename T>
inline vec2<T> &operator*=(vec2<T> &lhs, const T scalar)
{
    lhs.x *= scalar;
    lhs.y *= scalar;
    return lhs;
}

template<typename T>
inline vec2<T> &operator/=(vec2<T> &lhs, const T scalar)
{
    lhs.x /= scalar;
    lhs.y /= scalar;
    return lhs;
}

///
/// Arithmetic operators.
///
template<typename T>
inline vec2<T> operator+(vec2<T> lhs, const vec2<T> &rhs) { return lhs += rhs; }
template<typename T>
inline vec2<T> operator-(vec2<T> lhs, const vec2<T> &rhs) { return lhs -= rhs; }
template<typename T>
inline vec2<T> operator*(vec2<T> lhs, const vec2<T> &rhs) { return lhs *= rhs; }
template<typename T>
inline vec2<T> operator/(vec2<T> lhs, const vec2<T> &rhs) { return lhs /= rhs; }

template<typename T>
inline vec2<T> operator+(vec2<T> lhs, const T scalar) { return lhs += scalar; }
template<typename T>
inline vec2<T> operator-(vec2<T> lhs, const T scalar) { return lhs -= scalar; }
template<typename T>
inline vec2<T> operator*(vec2<T> lhs, const T scalar) { return lhs *= scalar; }
template<typename T>
inline vec2<T> operator/(vec2<T> lhs, const T scalar) { return lhs /= scalar; }

template<typename T>
inline vec2<T> operator+(const T scalar, vec2<T> rhs) { return rhs += scalar; }
template<typename T>
inline vec2<T> operator-(const T scalar, vec2<T> rhs) {
    vec2<T> lhs = -rhs;
    return lhs += scalar;
}
template<typename T>
inline vec2<T> operator*(const T scalar, vec2<T> rhs) { return rhs *= scalar; }
template<typename T>
inline vec2<T> operator/(const T scalar, vec2<T> rhs) {
    return {scalar / rhs.x, scalar / rhs.y};
}

///
/// Unary operators.
///
template<typename T>
inline vec2<T> operator+(vec2<T> lhs) { return lhs; }

template<typename T>
inline vec2<T> operator-(vec2<T> lhs) { return lhs *= (T) -1; }

///
/// Increment operators.
///
template<typename T>
inline vec2<T> &operator++(vec2<T> &lhs) { return lhs += (T) 1; }

template<typename T>
inline vec2<T> &operator--(vec2<T> &lhs) { return lhs -= (T) 1; }

template<typename T>
inline vec2<T> operator++(vec2<T> &lhs, int)
{
    vec2<T> result = lhs;
    ++lhs;
    return result;

}

template<typename T>
inline vec2<T> operator--(vec2<T> &lhs, int)
{
    vec2<T> result = lhs;
    --lhs;
    return result;
}

/// ---- vec3 implementation --------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T>
inline vec3<T> &operator+=(vec3<T> &lhs, const vec3<T> &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

template<typename T>
inline vec3<T> &operator-=(vec3<T> &lhs, const vec3<T> &rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;

}

template<typename T>
inline vec3<T> &operator*=(vec3<T> &lhs, const vec3<T> &rhs)
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    return lhs;
}

template<typename T>
inline vec3<T> &operator/=(vec3<T> &lhs, const vec3<T> &rhs)
{
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    lhs.z /= rhs.z;
    return lhs;
}

template<typename T>
inline vec3<T> &operator+=(vec3<T> &lhs, const T scalar)
{
    lhs.x += scalar;
    lhs.y += scalar;
    lhs.z += scalar;
    return lhs;
}

template<typename T>
inline vec3<T> &operator-=(vec3<T> &lhs, const T scalar)
{
    lhs.x -= scalar;
    lhs.y -= scalar;
    lhs.z -= scalar;
    return lhs;
}

template<typename T>
inline vec3<T> &operator*=(vec3<T> &lhs, const T scalar)
{
    lhs.x *= scalar;
    lhs.y *= scalar;
    lhs.z *= scalar;
    return lhs;
}

template<typename T>
inline vec3<T> &operator/=(vec3<T> &lhs, const T scalar)
{
    lhs.x /= scalar;
    lhs.y /= scalar;
    lhs.z /= scalar;
    return lhs;
}

///
/// Arithmetic operators.
///
template<typename T>
inline vec3<T> operator+(vec3<T> lhs, const vec3<T> &rhs) { return lhs += rhs; }
template<typename T>
inline vec3<T> operator-(vec3<T> lhs, const vec3<T> &rhs) { return lhs -= rhs; }
template<typename T>
inline vec3<T> operator*(vec3<T> lhs, const vec3<T> &rhs) { return lhs *= rhs; }
template<typename T>
inline vec3<T> operator/(vec3<T> lhs, const vec3<T> &rhs) { return lhs /= rhs; }

template<typename T>
inline vec3<T> operator+(vec3<T> lhs, const T scalar) { return lhs += scalar; }
template<typename T>
inline vec3<T> operator-(vec3<T> lhs, const T scalar) { return lhs -= scalar; }
template<typename T>
inline vec3<T> operator*(vec3<T> lhs, const T scalar) { return lhs *= scalar; }
template<typename T>
inline vec3<T> operator/(vec3<T> lhs, const T scalar) { return lhs /= scalar; }

template<typename T>
inline vec3<T> operator+(const T scalar, vec3<T> rhs) { return rhs += scalar; }
template<typename T>
inline vec3<T> operator-(const T scalar, vec3<T> rhs) {
    vec3<T> lhs = -rhs;
    return lhs += scalar;
}
template<typename T>
inline vec3<T> operator*(const T scalar, vec3<T> rhs) { return rhs *= scalar; }
template<typename T>
inline vec3<T> operator/(const T scalar, vec3<T> rhs) {
    return {scalar / rhs.x, scalar / rhs.y, scalar / rhs.z};
}

///
/// Unary operators.
///
template<typename T>
inline vec3<T> operator+(vec3<T> lhs) { return lhs; }

template<typename T>
inline vec3<T> operator-(vec3<T> lhs) { return lhs *= (T) -1; }

///
/// Increment operators.
///
template<typename T>
inline vec3<T> &operator++(vec3<T> &lhs) { return lhs += (T) 1; }

template<typename T>
inline vec3<T> &operator--(vec3<T> &lhs) { return lhs -= (T) 1; }

template<typename T>
inline vec3<T> operator++(vec3<T> &lhs, int)
{
    vec3<T> result = lhs;
    ++lhs;
    return result;

}

template<typename T>
inline vec3<T> operator--(vec3<T> &lhs, int)
{
    vec3<T> result = lhs;
    --lhs;
    return result;
}

/// ---- vec4 implementation --------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T>
inline vec4<T> &operator+=(vec4<T> &lhs, const vec4<T> &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;
    return lhs;
}

template<typename T>
inline vec4<T> &operator-=(vec4<T> &lhs, const vec4<T> &rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    lhs.w -= rhs.w;
    return lhs;
}

template<typename T>
inline vec4<T> &operator*=(vec4<T> &lhs, const vec4<T> &rhs)
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    lhs.w *= rhs.w;
    return lhs;
}

template<typename T>
inline vec4<T> &operator/=(vec4<T> &lhs, const vec4<T> &rhs)
{
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    lhs.z /= rhs.z;
    lhs.w /= rhs.w;
    return lhs;
}

template<typename T>
inline vec4<T> &operator+=(vec4<T> &lhs, const T scalar)
{
    lhs.x += scalar;
    lhs.y += scalar;
    lhs.z += scalar;
    lhs.w += scalar;
    return lhs;
}

template<typename T>
inline vec4<T> &operator-=(vec4<T> &lhs, const T scalar)
{
    lhs.x -= scalar;
    lhs.y -= scalar;
    lhs.z -= scalar;
    lhs.w -= scalar;
    return lhs;
}

template<typename T>
inline vec4<T> &operator*=(vec4<T> &lhs, const T scalar)
{
    lhs.x *= scalar;
    lhs.y *= scalar;
    lhs.z *= scalar;
    lhs.w *= scalar;
    return lhs;
}

template<typename T>
inline vec4<T> &operator/=(vec4<T> &lhs, const T scalar)
{
    lhs.x /= scalar;
    lhs.y /= scalar;
    lhs.z /= scalar;
    lhs.w /= scalar;
    return lhs;
}

///
/// Arithmetic operators.
///
template<typename T>
inline vec4<T> operator+(vec4<T> lhs, const vec4<T> &rhs) { return lhs += rhs; }
template<typename T>
inline vec4<T> operator-(vec4<T> lhs, const vec4<T> &rhs) { return lhs -= rhs; }
template<typename T>
inline vec4<T> operator*(vec4<T> lhs, const vec4<T> &rhs) { return lhs *= rhs; }
template<typename T>
inline vec4<T> operator/(vec4<T> lhs, const vec4<T> &rhs) { return lhs /= rhs; }

template<typename T>
inline vec4<T> operator+(vec4<T> lhs, const T scalar) { return lhs += scalar; }
template<typename T>
inline vec4<T> operator-(vec4<T> lhs, const T scalar) { return lhs -= scalar; }
template<typename T>
inline vec4<T> operator*(vec4<T> lhs, const T scalar) { return lhs *= scalar; }
template<typename T>
inline vec4<T> operator/(vec4<T> lhs, const T scalar) { return lhs /= scalar; }

template<typename T>
inline vec4<T> operator+(const T scalar, vec4<T> rhs) { return rhs += scalar; }
template<typename T>
inline vec4<T> operator-(const T scalar, vec4<T> rhs) {
    vec4<T> lhs = -rhs;
    return lhs += scalar;
}
template<typename T>
inline vec4<T> operator*(const T scalar, vec4<T> rhs) { return rhs *= scalar; }
template<typename T>
inline vec4<T> operator/(const T scalar, vec4<T> rhs) {
    return {scalar / rhs.x, scalar / rhs.y, scalar / rhs.z, scalar / rhs.w};
}

///
/// Unary operators.
///
template<typename T>
inline vec4<T> operator+(vec4<T> lhs) { return lhs; }

template<typename T>
inline vec4<T> operator-(vec4<T> lhs) { return lhs *= (T) -1; }

///
/// Increment operators.
///
template<typename T>
inline vec4<T> &operator++(vec4<T> &lhs) { return lhs += (T) 1; }

template<typename T>
inline vec4<T> &operator--(vec4<T> &lhs) { return lhs -= (T) 1; }

template<typename T>
inline vec4<T> operator++(vec4<T> &lhs, int)
{
    vec4<T> result = lhs;
    ++lhs;
    return result;
}

template<typename T>
inline vec4<T> operator--(vec4<T> &lhs, int)
{
    vec4<T> result = lhs;
    --lhs;
    return result;
}

} // math

/// ---- simd implementations ------------------------------------------------
#ifdef __AVX__
#include "simd/vector.h"
#endif

#endif // MATH_VECTOR_H_

