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

namespace Math {

/// ---- Vector data types ----------------------------------------------------
template<typename T>
struct Vec2 {
    static const Vec2 Zeros;
    static const Vec2 Ones;
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
struct Vec3 {
    static const Vec3 Zeros;
    static const Vec3 Ones;
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
struct Vec4 {
    static const Vec4 Zeros;
    static const Vec4 Ones;
    static const size_t length = 4;

    union {
        alignas(32) T data[length];
        struct { T s0, s1, s2, s3; };
        struct { T x, y, z, w; };
    };

    T &operator[](size_t i) { return data[i]; }
    const T &operator[](size_t i) const { return data[i]; }
};

typedef Vec2<int16_t>   Vec2i16;
typedef Vec2<int32_t>   Vec2i32;
typedef Vec2<int64_t>   Vec2i64;
typedef Vec2<int32_t>   Vec2i;
typedef Vec2<uint16_t>  Vec2u16;
typedef Vec2<uint32_t>  Vec2u32;
typedef Vec2<uint64_t>  Vec2u64;
typedef Vec2<uint32_t>  Vec2u;
typedef Vec2<float>     Vec2f;
typedef Vec2<double>    Vec2d;

typedef Vec3<int16_t>   Vec3i16;
typedef Vec3<int32_t>   Vec3i32;
typedef Vec3<int64_t>   Vec3i64;
typedef Vec3<int32_t>   Vec3i;
typedef Vec3<uint16_t>  Vec3u16;
typedef Vec3<uint32_t>  Vec3u32;
typedef Vec3<uint64_t>  Vec3u64;
typedef Vec3<uint32_t>  Vec3u;
typedef Vec3<float>     Vec3f;
typedef Vec3<double>    Vec3d;

typedef Vec4<int16_t>   Vec4i16;
typedef Vec4<int32_t>   Vec4i32;
typedef Vec4<int64_t>   Vec4i64;
typedef Vec4<int32_t>   Vec4i;
typedef Vec4<uint16_t>  Vec4u16;
typedef Vec4<uint32_t>  Vec4u32;
typedef Vec4<uint64_t>  Vec4u64;
typedef Vec4<uint32_t>  Vec4u;
typedef Vec4<float>     Vec4f;
typedef Vec4<double>    Vec4d;

/// ---- Special vectors ------------------------------------------------------
template<typename T>
const Vec2<T> Vec2<T>::Zeros = {(T) 0, (T) 0};
template<typename T>
const Vec2<T> Vec2<T>::Ones = {(T) 1, (T) 1};

template<typename T>
const Vec3<T> Vec3<T>::Zeros = {(T) 0, (T) 0, (T) 0};
template<typename T>
const Vec3<T> Vec3<T>::Ones = {(T) 1, (T) 1, (T) 1};

template<typename T>
const Vec4<T> Vec4<T>::Zeros = {(T) 0, (T) 0, (T) 0, (T) 0};
template<typename T>
const Vec4<T> Vec4<T>::Ones = {(T) 1, (T) 1, (T) 1, (T) 1};

/// ---- Vec2 declarations ----------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T>
inline Vec2<T> &operator+=(Vec2<T> &lhs, const Vec2<T> &rhs);
template<typename T> inline Vec2<T> &operator-=(Vec2<T> &lhs, const Vec2<T> &rhs);
template<typename T> inline Vec2<T> &operator*=(Vec2<T> &lhs, const Vec2<T> &rhs);
template<typename T> inline Vec2<T> &operator/=(Vec2<T> &lhs, const Vec2<T> &rhs);

template<typename T> inline Vec2<T> &operator+=(Vec2<T> &lhs, const T scalar);
template<typename T> inline Vec2<T> &operator-=(Vec2<T> &lhs, const T scalar);
template<typename T> inline Vec2<T> &operator*=(Vec2<T> &lhs, const T scalar);
template<typename T> inline Vec2<T> &operator/=(Vec2<T> &lhs, const T scalar);

/// Arithmetic operators.
template<typename T> inline Vec2<T> operator+(Vec2<T> lhs, const Vec2<T> &rhs);
template<typename T> inline Vec2<T> operator-(Vec2<T> lhs, const Vec2<T> &rhs);
template<typename T> inline Vec2<T> operator*(Vec2<T> lhs, const Vec2<T> &rhs);
template<typename T> inline Vec2<T> operator/(Vec2<T> lhs, const Vec2<T> &rhs);

template<typename T> inline Vec2<T> operator+(Vec2<T> lhs, const T scalar);
template<typename T> inline Vec2<T> operator-(Vec2<T> lhs, const T scalar);
template<typename T> inline Vec2<T> operator*(Vec2<T> lhs, const T scalar);
template<typename T> inline Vec2<T> operator/(Vec2<T> lhs, const T scalar);

template<typename T> inline Vec2<T> operator+(const T scalar, Vec2<T> rhs);
template<typename T> inline Vec2<T> operator-(const T scalar, Vec2<T> rhs);
template<typename T> inline Vec2<T> operator*(const T scalar, Vec2<T> rhs);
template<typename T> inline Vec2<T> operator/(const T scalar, Vec2<T> rhs);

/// Unary operators.
template<typename T> inline Vec2<T> operator+(Vec2<T> lhs);
template<typename T> inline Vec2<T> operator-(Vec2<T> lhs);

/// Increment operators.
template<typename T> inline Vec2<T> &operator++(Vec2<T> &lhs);
template<typename T> inline Vec2<T> &operator--(Vec2<T> &lhs);
template<typename T> inline Vec2<T> operator++(Vec2<T> &lhs, int);
template<typename T> inline Vec2<T> operator--(Vec2<T> &lhs, int);

/// ---- Vec3 declarations ----------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T> inline Vec3<T> &operator+=(Vec3<T> &lhs, const Vec3<T> &rhs);
template<typename T> inline Vec3<T> &operator-=(Vec3<T> &lhs, const Vec3<T> &rhs);
template<typename T> inline Vec3<T> &operator*=(Vec3<T> &lhs, const Vec3<T> &rhs);
template<typename T> inline Vec3<T> &operator/=(Vec3<T> &lhs, const Vec3<T> &rhs);

template<typename T> inline Vec3<T> &operator+=(Vec3<T> &lhs, const T scalar);
template<typename T> inline Vec3<T> &operator-=(Vec3<T> &lhs, const T scalar);
template<typename T> inline Vec3<T> &operator*=(Vec3<T> &lhs, const T scalar);
template<typename T> inline Vec3<T> &operator/=(Vec3<T> &lhs, const T scalar);

/// Arithmetic operators.
template<typename T> inline Vec3<T> operator+(Vec3<T> lhs, const Vec3<T> &rhs);
template<typename T> inline Vec3<T> operator-(Vec3<T> lhs, const Vec3<T> &rhs);
template<typename T> inline Vec3<T> operator*(Vec3<T> lhs, const Vec3<T> &rhs);
template<typename T> inline Vec3<T> operator/(Vec3<T> lhs, const Vec3<T> &rhs);

template<typename T> inline Vec3<T> operator+(Vec3<T> lhs, const T scalar);
template<typename T> inline Vec3<T> operator-(Vec3<T> lhs, const T scalar);
template<typename T> inline Vec3<T> operator*(Vec3<T> lhs, const T scalar);
template<typename T> inline Vec3<T> operator/(Vec3<T> lhs, const T scalar);

template<typename T> inline Vec3<T> operator+(const T scalar, Vec3<T> rhs);
template<typename T> inline Vec3<T> operator-(const T scalar, Vec3<T> rhs);
template<typename T> inline Vec3<T> operator*(const T scalar, Vec3<T> rhs);
template<typename T> inline Vec3<T> operator/(const T scalar, Vec3<T> rhs);

/// Unary operators.
template<typename T> inline Vec3<T> operator+(Vec3<T> lhs);
template<typename T> inline Vec3<T> operator-(Vec3<T> lhs);

/// Increment operators.
template<typename T> inline Vec3<T> &operator++(Vec3<T> &lhs);
template<typename T> inline Vec3<T> &operator--(Vec3<T> &lhs);
template<typename T> inline Vec3<T> operator++(Vec3<T> &lhs, int);
template<typename T> inline Vec3<T> operator--(Vec3<T> &lhs, int);

/// ---- Vec4 declarations ----------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T> inline Vec4<T> &operator+=(Vec4<T> &lhs, const Vec4<T> &rhs);
template<typename T> inline Vec4<T> &operator-=(Vec4<T> &lhs, const Vec4<T> &rhs);
template<typename T> inline Vec4<T> &operator*=(Vec4<T> &lhs, const Vec4<T> &rhs);
template<typename T> inline Vec4<T> &operator/=(Vec4<T> &lhs, const Vec4<T> &rhs);

template<typename T> inline Vec4<T> &operator+=(Vec4<T> &lhs, const T scalar);
template<typename T> inline Vec4<T> &operator-=(Vec4<T> &lhs, const T scalar);
template<typename T> inline Vec4<T> &operator*=(Vec4<T> &lhs, const T scalar);
template<typename T> inline Vec4<T> &operator/=(Vec4<T> &lhs, const T scalar);

/// Arithmetic operators.
template<typename T> inline Vec4<T> operator+(Vec4<T> lhs, const Vec4<T> &rhs);
template<typename T> inline Vec4<T> operator-(Vec4<T> lhs, const Vec4<T> &rhs);
template<typename T> inline Vec4<T> operator*(Vec4<T> lhs, const Vec4<T> &rhs);
template<typename T> inline Vec4<T> operator/(Vec4<T> lhs, const Vec4<T> &rhs);

template<typename T> inline Vec4<T> operator+(Vec4<T> lhs, const T scalar);
template<typename T> inline Vec4<T> operator-(Vec4<T> lhs, const T scalar);
template<typename T> inline Vec4<T> operator*(Vec4<T> lhs, const T scalar);
template<typename T> inline Vec4<T> operator/(Vec4<T> lhs, const T scalar);

template<typename T> inline Vec4<T> operator+(const T scalar, Vec4<T> rhs);
template<typename T> inline Vec4<T> operator-(const T scalar, Vec4<T> rhs);
template<typename T> inline Vec4<T> operator*(const T scalar, Vec4<T> rhs);
template<typename T> inline Vec4<T> operator/(const T scalar, Vec4<T> rhs);

/// Unary operators.
template<typename T> inline Vec4<T> operator+(Vec4<T> lhs);
template<typename T> inline Vec4<T> operator-(Vec4<T> lhs);

/// Increment operators.
template<typename T> inline Vec4<T> &operator++(Vec4<T> &lhs);
template<typename T> inline Vec4<T> &operator--(Vec4<T> &lhs);
template<typename T> inline Vec4<T> operator++(Vec4<T> &lhs, int);
template<typename T> inline Vec4<T> operator--(Vec4<T> &lhs, int);

/// ---- Vec2 implementation --------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T>
inline Vec2<T> &operator+=(Vec2<T> &lhs, const Vec2<T> &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

template<typename T>
inline Vec2<T> &operator-=(Vec2<T> &lhs, const Vec2<T> &rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

template<typename T>
inline Vec2<T> &operator*=(Vec2<T> &lhs, const Vec2<T> &rhs)
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    return lhs;
}

template<typename T>
inline Vec2<T> &operator/=(Vec2<T> &lhs, const Vec2<T> &rhs)
{
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    return lhs;
}

template<typename T>
inline Vec2<T> &operator+=(Vec2<T> &lhs, const T scalar)
{
    lhs.x += scalar;
    lhs.y += scalar;
    return lhs;
}

template<typename T>
inline Vec2<T> &operator-=(Vec2<T> &lhs, const T scalar)
{
    lhs.x -= scalar;
    lhs.y -= scalar;
    return lhs;
}

template<typename T>
inline Vec2<T> &operator*=(Vec2<T> &lhs, const T scalar)
{
    lhs.x *= scalar;
    lhs.y *= scalar;
    return lhs;
}

template<typename T>
inline Vec2<T> &operator/=(Vec2<T> &lhs, const T scalar)
{
    lhs.x /= scalar;
    lhs.y /= scalar;
    return lhs;
}

///
/// Arithmetic operators.
///
template<typename T>
inline Vec2<T> operator+(Vec2<T> lhs, const Vec2<T> &rhs) { return lhs += rhs; }
template<typename T>
inline Vec2<T> operator-(Vec2<T> lhs, const Vec2<T> &rhs) { return lhs -= rhs; }
template<typename T>
inline Vec2<T> operator*(Vec2<T> lhs, const Vec2<T> &rhs) { return lhs *= rhs; }
template<typename T>
inline Vec2<T> operator/(Vec2<T> lhs, const Vec2<T> &rhs) { return lhs /= rhs; }

template<typename T>
inline Vec2<T> operator+(Vec2<T> lhs, const T scalar) { return lhs += scalar; }
template<typename T>
inline Vec2<T> operator-(Vec2<T> lhs, const T scalar) { return lhs -= scalar; }
template<typename T>
inline Vec2<T> operator*(Vec2<T> lhs, const T scalar) { return lhs *= scalar; }
template<typename T>
inline Vec2<T> operator/(Vec2<T> lhs, const T scalar) { return lhs /= scalar; }

template<typename T>
inline Vec2<T> operator+(const T scalar, Vec2<T> rhs) { return rhs += scalar; }
template<typename T>
inline Vec2<T> operator-(const T scalar, Vec2<T> rhs) {
    Vec2<T> lhs = -rhs;
    return lhs += scalar;
}
template<typename T>
inline Vec2<T> operator*(const T scalar, Vec2<T> rhs) { return rhs *= scalar; }
template<typename T>
inline Vec2<T> operator/(const T scalar, Vec2<T> rhs) {
    return {scalar / rhs.x, scalar / rhs.y};
}

///
/// Unary operators.
///
template<typename T>
inline Vec2<T> operator+(Vec2<T> lhs) { return lhs; }

template<typename T>
inline Vec2<T> operator-(Vec2<T> lhs) { return lhs *= (T) -1; }

///
/// Increment operators.
///
template<typename T>
inline Vec2<T> &operator++(Vec2<T> &lhs) { return lhs += (T) 1; }

template<typename T>
inline Vec2<T> &operator--(Vec2<T> &lhs) { return lhs -= (T) 1; }

template<typename T>
inline Vec2<T> operator++(Vec2<T> &lhs, int)
{
    Vec2<T> result = lhs;
    ++lhs;
    return result;

}

template<typename T>
inline Vec2<T> operator--(Vec2<T> &lhs, int)
{
    Vec2<T> result = lhs;
    --lhs;
    return result;
}

/// ---- Vec3 implementation --------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T>
inline Vec3<T> &operator+=(Vec3<T> &lhs, const Vec3<T> &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

template<typename T>
inline Vec3<T> &operator-=(Vec3<T> &lhs, const Vec3<T> &rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;

}

template<typename T>
inline Vec3<T> &operator*=(Vec3<T> &lhs, const Vec3<T> &rhs)
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    return lhs;
}

template<typename T>
inline Vec3<T> &operator/=(Vec3<T> &lhs, const Vec3<T> &rhs)
{
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    lhs.z /= rhs.z;
    return lhs;
}

template<typename T>
inline Vec3<T> &operator+=(Vec3<T> &lhs, const T scalar)
{
    lhs.x += scalar;
    lhs.y += scalar;
    lhs.z += scalar;
    return lhs;
}

template<typename T>
inline Vec3<T> &operator-=(Vec3<T> &lhs, const T scalar)
{
    lhs.x -= scalar;
    lhs.y -= scalar;
    lhs.z -= scalar;
    return lhs;
}

template<typename T>
inline Vec3<T> &operator*=(Vec3<T> &lhs, const T scalar)
{
    lhs.x *= scalar;
    lhs.y *= scalar;
    lhs.z *= scalar;
    return lhs;
}

template<typename T>
inline Vec3<T> &operator/=(Vec3<T> &lhs, const T scalar)
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
inline Vec3<T> operator+(Vec3<T> lhs, const Vec3<T> &rhs) { return lhs += rhs; }
template<typename T>
inline Vec3<T> operator-(Vec3<T> lhs, const Vec3<T> &rhs) { return lhs -= rhs; }
template<typename T>
inline Vec3<T> operator*(Vec3<T> lhs, const Vec3<T> &rhs) { return lhs *= rhs; }
template<typename T>
inline Vec3<T> operator/(Vec3<T> lhs, const Vec3<T> &rhs) { return lhs /= rhs; }

template<typename T>
inline Vec3<T> operator+(Vec3<T> lhs, const T scalar) { return lhs += scalar; }
template<typename T>
inline Vec3<T> operator-(Vec3<T> lhs, const T scalar) { return lhs -= scalar; }
template<typename T>
inline Vec3<T> operator*(Vec3<T> lhs, const T scalar) { return lhs *= scalar; }
template<typename T>
inline Vec3<T> operator/(Vec3<T> lhs, const T scalar) { return lhs /= scalar; }

template<typename T>
inline Vec3<T> operator+(const T scalar, Vec3<T> rhs) { return rhs += scalar; }
template<typename T>
inline Vec3<T> operator-(const T scalar, Vec3<T> rhs) {
    Vec3<T> lhs = -rhs;
    return lhs += scalar;
}
template<typename T>
inline Vec3<T> operator*(const T scalar, Vec3<T> rhs) { return rhs *= scalar; }
template<typename T>
inline Vec3<T> operator/(const T scalar, Vec3<T> rhs) {
    return {scalar / rhs.x, scalar / rhs.y, scalar / rhs.z};
}

///
/// Unary operators.
///
template<typename T>
inline Vec3<T> operator+(Vec3<T> lhs) { return lhs; }

template<typename T>
inline Vec3<T> operator-(Vec3<T> lhs) { return lhs *= (T) -1; }

///
/// Increment operators.
///
template<typename T>
inline Vec3<T> &operator++(Vec3<T> &lhs) { return lhs += (T) 1; }

template<typename T>
inline Vec3<T> &operator--(Vec3<T> &lhs) { return lhs -= (T) 1; }

template<typename T>
inline Vec3<T> operator++(Vec3<T> &lhs, int)
{
    Vec3<T> result = lhs;
    ++lhs;
    return result;

}

template<typename T>
inline Vec3<T> operator--(Vec3<T> &lhs, int)
{
    Vec3<T> result = lhs;
    --lhs;
    return result;
}

/// ---- Vec4 implementation --------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T>
inline Vec4<T> &operator+=(Vec4<T> &lhs, const Vec4<T> &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;
    return lhs;
}

template<typename T>
inline Vec4<T> &operator-=(Vec4<T> &lhs, const Vec4<T> &rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    lhs.w -= rhs.w;
    return lhs;
}

template<typename T>
inline Vec4<T> &operator*=(Vec4<T> &lhs, const Vec4<T> &rhs)
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    lhs.w *= rhs.w;
    return lhs;
}

template<typename T>
inline Vec4<T> &operator/=(Vec4<T> &lhs, const Vec4<T> &rhs)
{
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    lhs.z /= rhs.z;
    lhs.w /= rhs.w;
    return lhs;
}

template<typename T>
inline Vec4<T> &operator+=(Vec4<T> &lhs, const T scalar)
{
    lhs.x += scalar;
    lhs.y += scalar;
    lhs.z += scalar;
    lhs.w += scalar;
    return lhs;
}

template<typename T>
inline Vec4<T> &operator-=(Vec4<T> &lhs, const T scalar)
{
    lhs.x -= scalar;
    lhs.y -= scalar;
    lhs.z -= scalar;
    lhs.w -= scalar;
    return lhs;
}

template<typename T>
inline Vec4<T> &operator*=(Vec4<T> &lhs, const T scalar)
{
    lhs.x *= scalar;
    lhs.y *= scalar;
    lhs.z *= scalar;
    lhs.w *= scalar;
    return lhs;
}

template<typename T>
inline Vec4<T> &operator/=(Vec4<T> &lhs, const T scalar)
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
inline Vec4<T> operator+(Vec4<T> lhs, const Vec4<T> &rhs) { return lhs += rhs; }
template<typename T>
inline Vec4<T> operator-(Vec4<T> lhs, const Vec4<T> &rhs) { return lhs -= rhs; }
template<typename T>
inline Vec4<T> operator*(Vec4<T> lhs, const Vec4<T> &rhs) { return lhs *= rhs; }
template<typename T>
inline Vec4<T> operator/(Vec4<T> lhs, const Vec4<T> &rhs) { return lhs /= rhs; }

template<typename T>
inline Vec4<T> operator+(Vec4<T> lhs, const T scalar) { return lhs += scalar; }
template<typename T>
inline Vec4<T> operator-(Vec4<T> lhs, const T scalar) { return lhs -= scalar; }
template<typename T>
inline Vec4<T> operator*(Vec4<T> lhs, const T scalar) { return lhs *= scalar; }
template<typename T>
inline Vec4<T> operator/(Vec4<T> lhs, const T scalar) { return lhs /= scalar; }

template<typename T>
inline Vec4<T> operator+(const T scalar, Vec4<T> rhs) { return rhs += scalar; }
template<typename T>
inline Vec4<T> operator-(const T scalar, Vec4<T> rhs) {
    Vec4<T> lhs = -rhs;
    return lhs += scalar;
}
template<typename T>
inline Vec4<T> operator*(const T scalar, Vec4<T> rhs) { return rhs *= scalar; }
template<typename T>
inline Vec4<T> operator/(const T scalar, Vec4<T> rhs) {
    return {scalar / rhs.x, scalar / rhs.y, scalar / rhs.z, scalar / rhs.w};
}

///
/// Unary operators.
///
template<typename T>
inline Vec4<T> operator+(Vec4<T> lhs) { return lhs; }

template<typename T>
inline Vec4<T> operator-(Vec4<T> lhs) { return lhs *= (T) -1; }

///
/// Increment operators.
///
template<typename T>
inline Vec4<T> &operator++(Vec4<T> &lhs) { return lhs += (T) 1; }

template<typename T>
inline Vec4<T> &operator--(Vec4<T> &lhs) { return lhs -= (T) 1; }

template<typename T>
inline Vec4<T> operator++(Vec4<T> &lhs, int)
{
    Vec4<T> result = lhs;
    ++lhs;
    return result;
}

template<typename T>
inline Vec4<T> operator--(Vec4<T> &lhs, int)
{
    Vec4<T> result = lhs;
    --lhs;
    return result;
}

} // namespace Math

/// ---- simd implementations ------------------------------------------------
#ifdef __AVX__
#include "simd/vector.h"
#endif

#endif // MATH_VECTOR_H_

