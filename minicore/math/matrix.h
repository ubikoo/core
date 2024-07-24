//
// matrix.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MATH_MATRIX_H_
#define MATH_MATRIX_H_

namespace Math {

/// ---- Matrix data types ----------------------------------------------------
///
template<typename T>
struct Mat2 {
    static const Mat2 Zeros;
    static const Mat2 Ones;
    static const Mat2 Eye;
    static const size_t dim = 2;
    static const size_t length = dim * dim;

    union {
        alignas(32) T data[length];
        struct { T s0, s1,
                   s2, s3; };
        struct { T xx, xy,
                   yx, yy; };
    };

    T &operator[](size_t i) { return data[i]; }
    const T &operator[](size_t i) const { return data[i]; }
};

template<typename T>
struct Mat3 {
    static const Mat3 Zeros;
    static const Mat3 Ones;
    static const Mat3 Eye;
    static const size_t dim = 3;
    static const size_t length = dim * dim;

    union {
        alignas(32) T data[length];
        struct { T s0, s1, s2,
                   s3, s4, s5,
                   s6, s7, s8; };
        struct { T xx, xy, xz,
                   yx, yy, yz,
                   zx, zy, zz; };
    };

    T &operator[](size_t i) { return data[i]; }
    const T &operator[](size_t i) const { return data[i]; }
};

template<typename T>
struct Mat4 {
    static const Mat4 Zeros;
    static const Mat4 Ones;
    static const Mat4 Eye;
    static const size_t dim = 4;
    static const size_t length = dim * dim;

    union {
        alignas(32) T data[length];
        struct { T s0, s1, s2, s3,
                   s4, s5, s6, s7,
                   s8, s9, s10, s11,
                   s12, s13, s14, s15; };
        struct { T xx, xy, xz, xw,
                   yx, yy, yz, yw,
                   zx, zy, zz, zw,
                   wx, wy, wz, ww; };
    };

    T &operator[](size_t i) { return data[i]; }
    const T &operator[](size_t i) const { return data[i]; }
};

typedef Mat2<int16_t>   Mat2i16;
typedef Mat2<int32_t>   Mat2i32;
typedef Mat2<int64_t>   Mat2i64;
typedef Mat2<int32_t>   Mat2i;
typedef Mat2<uint16_t>  Mat2u16;
typedef Mat2<uint32_t>  Mat2u32;
typedef Mat2<uint64_t>  Mat2u64;
typedef Mat2<uint32_t>  Mat2u;
typedef Mat2<float>     Mat2f;
typedef Mat2<double>    Mat2d;

typedef Mat3<int16_t>   Mat3i16;
typedef Mat3<int32_t>   Mat3i32;
typedef Mat3<int64_t>   Mat3i64;
typedef Mat3<int32_t>   Mat3i;
typedef Mat3<uint16_t>  Mat3u16;
typedef Mat3<uint32_t>  Mat3u32;
typedef Mat3<uint64_t>  Mat3u64;
typedef Mat3<uint32_t>  Mat3u;
typedef Mat3<float>     Mat3f;
typedef Mat3<double>    Mat3d;

typedef Mat4<int16_t>   Mat4i16;
typedef Mat4<int32_t>   Mat4i32;
typedef Mat4<int64_t>   Mat4i64;
typedef Mat4<int32_t>   Mat4i;
typedef Mat4<uint16_t>  Mat4u16;
typedef Mat4<uint32_t>  Mat4u32;
typedef Mat4<uint64_t>  Mat4u64;
typedef Mat4<uint32_t>  Mat4u;
typedef Mat4<float>     Mat4f;
typedef Mat4<double>    Mat4d;

/// ---- Special matrices -----------------------------------------------------
///
template<typename T>
const Mat2<T> Mat2<T>::Zeros = {
    (T) 0, (T) 0,
    (T) 0, (T) 0};
template<typename T>
const Mat2<T> Mat2<T>::Ones = {
    (T) 1, (T) 1,
    (T) 1, (T) 1};
template<typename T>
const Mat2<T> Mat2<T>::Eye = {
    (T) 1, (T) 0,
    (T) 0, (T) 1};

template<typename T>
const Mat3<T> Mat3<T>::Zeros = {
    (T) 0, (T) 0, (T) 0,
    (T) 0, (T) 0, (T) 0,
    (T) 0, (T) 0, (T) 0};
template<typename T>
const Mat3<T> Mat3<T>::Ones = {
    (T) 1, (T) 1, (T) 1,
    (T) 1, (T) 1, (T) 1,
    (T) 1, (T) 1, (T) 1};
template<typename T>
const Mat3<T> Mat3<T>::Eye = {
    (T) 1, (T) 0, (T) 0,
    (T) 0, (T) 1, (T) 0,
    (T) 0, (T) 0, (T) 1};

template<typename T>
const Mat4<T> Mat4<T>::Zeros = {
    (T) 0, (T) 0, (T) 0, (T) 0,
    (T) 0, (T) 0, (T) 0, (T) 0,
    (T) 0, (T) 0, (T) 0, (T) 0,
    (T) 0, (T) 0, (T) 0, (T) 0};

template<typename T>
const Mat4<T> Mat4<T>::Ones = {
    (T) 1, (T) 1, (T) 1, (T) 1,
    (T) 1, (T) 1, (T) 1, (T) 1,
    (T) 1, (T) 1, (T) 1, (T) 1,
    (T) 1, (T) 1, (T) 1, (T) 1};

template<typename T>
const Mat4<T> Mat4<T>::Eye = {
    (T) 1, (T) 0, (T) 0, (T) 0,
    (T) 0, (T) 1, (T) 0, (T) 0,
    (T) 0, (T) 0, (T) 1, (T) 0,
    (T) 0, (T) 0, (T) 0, (T) 1};

/// ---- Mat2 declarations ----------------------------------------------------
/// Compound assignment operators matrix operators.
///
template<typename T>
inline Mat2<T> &operator+=(Mat2<T> &lhs, const Mat2<T> &rhs);
template<typename T>
inline Mat2<T> &operator-=(Mat2<T> &lhs, const Mat2<T> &rhs);
template<typename T>
inline Mat2<T> &operator*=(Mat2<T> &lhs, const Mat2<T> &rhs);
template<typename T>
inline Mat2<T> &operator/=(Mat2<T> &lhs, const Mat2<T> &rhs);

template<typename T>
inline Mat2<T> &operator+=(Mat2<T> &lhs, const T scalar);
template<typename T>
inline Mat2<T> &operator-=(Mat2<T> &lhs, const T scalar);
template<typename T>
inline Mat2<T> &operator*=(Mat2<T> &lhs, const T scalar);
template<typename T>
inline Mat2<T> &operator/=(Mat2<T> &lhs, const T scalar);

/// Arithmetic operators.
template<typename T>
inline Mat2<T> operator+(Mat2<T> lhs, const Mat2<T> &rhs);
template<typename T>
inline Mat2<T> operator-(Mat2<T> lhs, const Mat2<T> &rhs);
template<typename T>
inline Mat2<T> operator*(Mat2<T> lhs, const Mat2<T> &rhs);
template<typename T>
inline Mat2<T> operator/(Mat2<T> lhs, const Mat2<T> &rhs);

template<typename T>
inline Mat2<T> operator+(Mat2<T> lhs, const T scalar);
template<typename T>
inline Mat2<T> operator-(Mat2<T> lhs, const T scalar);
template<typename T>
inline Mat2<T> operator*(Mat2<T> lhs, const T scalar);
template<typename T>
inline Mat2<T> operator/(Mat2<T> lhs, const T scalar);

template<typename T>
inline Mat2<T> operator+(const T scalar, Mat2<T> rhs);
template<typename T>
inline Mat2<T> operator-(const T scalar, Mat2<T> rhs);
template<typename T>
inline Mat2<T> operator*(const T scalar, Mat2<T> rhs);
template<typename T>
inline Mat2<T> operator/(const T scalar, Mat2<T> rhs);

/// Unary operators.
template<typename T>
inline Mat2<T> operator+(Mat2<T> lhs);
template<typename T>
inline Mat2<T> operator-(Mat2<T> lhs);

/// Increment operators.
template<typename T>
inline Mat2<T> &operator++(Mat2<T> &lhs);
template<typename T>
inline Mat2<T> &operator--(Mat2<T> &lhs);
template<typename T>
inline Mat2<T> operator++(Mat2<T> &lhs, int);
template<typename T>
inline Mat2<T> operator--(Mat2<T> &lhs, int);

/// ---- Mat3 declarations ----------------------------------------------------
/// Compound assignment operators matrix operators.
///
template<typename T>
inline Mat3<T> &operator+=(Mat3<T> &lhs, const Mat3<T> &rhs);
template<typename T>
inline Mat3<T> &operator-=(Mat3<T> &lhs, const Mat3<T> &rhs);
template<typename T>
inline Mat3<T> &operator*=(Mat3<T> &lhs, const Mat3<T> &rhs);
template<typename T>
inline Mat3<T> &operator/=(Mat3<T> &lhs, const Mat3<T> &rhs);

template<typename T>
inline Mat3<T> &operator+=(Mat3<T> &lhs, const T scalar);
template<typename T>
inline Mat3<T> &operator-=(Mat3<T> &lhs, const T scalar);
template<typename T>
inline Mat3<T> &operator*=(Mat3<T> &lhs, const T scalar);
template<typename T>
inline Mat3<T> &operator/=(Mat3<T> &lhs, const T scalar);

/// Arithmetic operators.
template<typename T>
inline Mat3<T> operator+(Mat3<T> lhs, const Mat3<T> &rhs);
template<typename T>
inline Mat3<T> operator-(Mat3<T> lhs, const Mat3<T> &rhs);
template<typename T>
inline Mat3<T> operator*(Mat3<T> lhs, const Mat3<T> &rhs);
template<typename T>
inline Mat3<T> operator/(Mat3<T> lhs, const Mat3<T> &rhs);

template<typename T>
inline Mat3<T> operator+(Mat3<T> lhs, const T scalar);
template<typename T>
inline Mat3<T> operator-(Mat3<T> lhs, const T scalar);
template<typename T>
inline Mat3<T> operator*(Mat3<T> lhs, const T scalar);
template<typename T>
inline Mat3<T> operator/(Mat3<T> lhs, const T scalar);

template<typename T>
inline Mat3<T> operator+(const T scalar, Mat3<T> rhs);
template<typename T>
inline Mat3<T> operator-(const T scalar, Mat3<T> rhs);
template<typename T>
inline Mat3<T> operator*(const T scalar, Mat3<T> rhs);
template<typename T>
inline Mat3<T> operator/(const T scalar, Mat3<T> rhs);

/// Unary operators.
template<typename T>
inline Mat3<T> operator+(Mat3<T> lhs);
template<typename T>
inline Mat3<T> operator-(Mat3<T> lhs);

/// Increment operators.
template<typename T>
inline Mat3<T> &operator++(Mat3<T> &lhs);
template<typename T>
inline Mat3<T> &operator--(Mat3<T> &lhs);
template<typename T>
inline Mat3<T> operator++(Mat3<T> &lhs, int);
template<typename T>
inline Mat3<T> operator--(Mat3<T> &lhs, int);

/// ---- Mat4 declarations ----------------------------------------------------
/// Compound assignment operators matrix operators.
///
template<typename T>
inline Mat4<T> &operator+=(Mat4<T> &lhs, const Mat4<T> &rhs);
template<typename T>
inline Mat4<T> &operator-=(Mat4<T> &lhs, const Mat4<T> &rhs);
template<typename T>
inline Mat4<T> &operator*=(Mat4<T> &lhs, const Mat4<T> &rhs);
template<typename T>
inline Mat4<T> &operator/=(Mat4<T> &lhs, const Mat4<T> &rhs);

template<typename T>
inline Mat4<T> &operator+=(Mat4<T> &lhs, const T scalar);
template<typename T>
inline Mat4<T> &operator-=(Mat4<T> &lhs, const T scalar);
template<typename T>
inline Mat4<T> &operator*=(Mat4<T> &lhs, const T scalar);
template<typename T>
inline Mat4<T> &operator/=(Mat4<T> &lhs, const T scalar);

/// Arithmetic operators.
template<typename T>
inline Mat4<T> operator+(Mat4<T> lhs, const Mat4<T> &rhs);
template<typename T>
inline Mat4<T> operator-(Mat4<T> lhs, const Mat4<T> &rhs);
template<typename T>
inline Mat4<T> operator*(Mat4<T> lhs, const Mat4<T> &rhs);
template<typename T>
inline Mat4<T> operator/(Mat4<T> lhs, const Mat4<T> &rhs);

template<typename T>
inline Mat4<T> operator+(Mat4<T> lhs, const T scalar);
template<typename T>
inline Mat4<T> operator-(Mat4<T> lhs, const T scalar);
template<typename T>
inline Mat4<T> operator*(Mat4<T> lhs, const T scalar);
template<typename T>
inline Mat4<T> operator/(Mat4<T> lhs, const T scalar);

template<typename T>
inline Mat4<T> operator+(const T scalar, Mat4<T> rhs);
template<typename T>
inline Mat4<T> operator-(const T scalar, Mat4<T> rhs);
template<typename T>
inline Mat4<T> operator*(const T scalar, Mat4<T> rhs);
template<typename T>
inline Mat4<T> operator/(const T scalar, Mat4<T> rhs);

/// Unary operators.
template<typename T>
inline Mat4<T> operator+(Mat4<T> lhs);
template<typename T>
inline Mat4<T> operator-(Mat4<T> lhs);

/// Increment operators.
template<typename T>
inline Mat4<T> &operator++(Mat4<T> &lhs);
template<typename T>
inline Mat4<T> &operator--(Mat4<T> &lhs);
template<typename T>
inline Mat4<T> operator++(Mat4<T> &lhs, int);
template<typename T>
inline Mat4<T> operator--(Mat4<T> &lhs, int);

/// ---- Mat2 implementation --------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T>
inline Mat2<T> &operator+=(Mat2<T> &lhs, const Mat2<T> &rhs)
{
    lhs.s0 += rhs.s0;
    lhs.s1 += rhs.s1;
    lhs.s2 += rhs.s2;
    lhs.s3 += rhs.s3;
    return lhs;
}

template<typename T>
inline Mat2<T> &operator-=(Mat2<T> &lhs, const Mat2<T> &rhs)
{
    lhs.s0 -= rhs.s0;
    lhs.s1 -= rhs.s1;
    lhs.s2 -= rhs.s2;
    lhs.s3 -= rhs.s3;
    return lhs;
}

template<typename T>
inline Mat2<T> &operator*=(Mat2<T> &lhs, const Mat2<T> &rhs)
{
    lhs.s0 *= rhs.s0;
    lhs.s1 *= rhs.s1;
    lhs.s2 *= rhs.s2;
    lhs.s3 *= rhs.s3;
    return lhs;
}

template<typename T>
inline Mat2<T> &operator/=(Mat2<T> &lhs, const Mat2<T> &rhs)
{
    lhs.s0 /= rhs.s0;
    lhs.s1 /= rhs.s1;
    lhs.s2 /= rhs.s2;
    lhs.s3 /= rhs.s3;
    return lhs;
}

template<typename T>
inline Mat2<T> &operator+=(Mat2<T> &lhs, const T scalar)
{
    lhs.s0 += scalar;
    lhs.s1 += scalar;
    lhs.s2 += scalar;
    lhs.s3 += scalar;
    return lhs;
}

template<typename T>
inline Mat2<T> &operator-=(Mat2<T> &lhs, const T scalar)
{
    lhs.s0 -= scalar;
    lhs.s1 -= scalar;
    lhs.s2 -= scalar;
    lhs.s3 -= scalar;
    return lhs;
}

template<typename T>
inline Mat2<T> &operator*=(Mat2<T> &lhs, const T scalar)
{
    lhs.s0 *= scalar;
    lhs.s1 *= scalar;
    lhs.s2 *= scalar;
    lhs.s3 *= scalar;
    return lhs;
}

template<typename T>
inline Mat2<T> &operator/=(Mat2<T> &lhs, const T scalar)
{
    lhs.s0 /= scalar;
    lhs.s1 /= scalar;
    lhs.s2 /= scalar;
    lhs.s3 /= scalar;
    return lhs;
}

///
/// Arithmetic operators.
///
template<typename T>
inline Mat2<T> operator+(Mat2<T> lhs, const Mat2<T> &rhs) { return lhs += rhs; }
template<typename T>
inline Mat2<T> operator-(Mat2<T> lhs, const Mat2<T> &rhs) { return lhs -= rhs; }
template<typename T>
inline Mat2<T> operator*(Mat2<T> lhs, const Mat2<T> &rhs) { return lhs *= rhs; }
template<typename T>
inline Mat2<T> operator/(Mat2<T> lhs, const Mat2<T> &rhs) { return lhs /= rhs; }

template<typename T>
inline Mat2<T> operator+(Mat2<T> lhs, const T scalar) { return lhs += scalar; }
template<typename T>
inline Mat2<T> operator-(Mat2<T> lhs, const T scalar) { return lhs -= scalar; }
template<typename T>
inline Mat2<T> operator*(Mat2<T> lhs, const T scalar) { return lhs *= scalar; }
template<typename T>
inline Mat2<T> operator/(Mat2<T> lhs, const T scalar) { return lhs /= scalar; }

template<typename T>
inline Mat2<T> operator+(const T scalar, Mat2<T> rhs) { return rhs += scalar; }
template<typename T>
inline Mat2<T> operator-(const T scalar, Mat2<T> rhs) {
    Mat2<T> lhs = -rhs;
    return lhs += scalar;
}
template<typename T>
inline Mat2<T> operator*(const T scalar, Mat2<T> rhs) { return rhs *= scalar; }
template<typename T>
inline Mat2<T> operator/(const T scalar, Mat2<T> rhs) {
    return {
        scalar / rhs.xx, scalar / rhs.xy,
        scalar / rhs.yx, scalar / rhs.yy
    };
}

///
/// Unary operators.
///
template<typename T>
inline Mat2<T> operator+(Mat2<T> lhs) { return lhs; }

template<typename T>
inline Mat2<T> operator-(Mat2<T> lhs) { return lhs *= (T) -1; }

///
/// Increment operators.
///
template<typename T>
inline Mat2<T> &operator++(Mat2<T> &lhs) { return lhs += (T) 1; }

template<typename T>
inline Mat2<T> &operator--(Mat2<T> &lhs) { return lhs -= (T) 1; }

template<typename T>
inline Mat2<T> operator++(Mat2<T> &lhs, int)
{
    Mat2<T> result = lhs;
    ++lhs;
    return result;
}

template<typename T>
inline Mat2<T> operator--(Mat2<T> &lhs, int)
{
    Mat2<T> result = lhs;
    --lhs;
    return result;
}

/// ---- Mat3 implementation --------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T>
inline Mat3<T> &operator+=(Mat3<T> &lhs, const Mat3<T> &rhs)
{
    lhs.s0 += rhs.s0;
    lhs.s1 += rhs.s1;
    lhs.s2 += rhs.s2;
    lhs.s3 += rhs.s3;
    lhs.s4 += rhs.s4;
    lhs.s5 += rhs.s5;
    lhs.s6 += rhs.s6;
    lhs.s7 += rhs.s7;
    lhs.s8 += rhs.s8;
    return lhs;
}

template<typename T>
inline Mat3<T> &operator-=(Mat3<T> &lhs, const Mat3<T> &rhs)
{
    lhs.s0 -= rhs.s0;
    lhs.s1 -= rhs.s1;
    lhs.s2 -= rhs.s2;
    lhs.s3 -= rhs.s3;
    lhs.s4 -= rhs.s4;
    lhs.s5 -= rhs.s5;
    lhs.s6 -= rhs.s6;
    lhs.s7 -= rhs.s7;
    lhs.s8 -= rhs.s8;
    return lhs;
}

template<typename T>
inline Mat3<T> &operator*=(Mat3<T> &lhs, const Mat3<T> &rhs)
{
    lhs.s0 *= rhs.s0;
    lhs.s1 *= rhs.s1;
    lhs.s2 *= rhs.s2;
    lhs.s3 *= rhs.s3;
    lhs.s4 *= rhs.s4;
    lhs.s5 *= rhs.s5;
    lhs.s6 *= rhs.s6;
    lhs.s7 *= rhs.s7;
    lhs.s8 *= rhs.s8;
    return lhs;
}

template<typename T>
inline Mat3<T> &operator/=(Mat3<T> &lhs, const Mat3<T> &rhs)
{
    lhs.s0 /= rhs.s0;
    lhs.s1 /= rhs.s1;
    lhs.s2 /= rhs.s2;
    lhs.s3 /= rhs.s3;
    lhs.s4 /= rhs.s4;
    lhs.s5 /= rhs.s5;
    lhs.s6 /= rhs.s6;
    lhs.s7 /= rhs.s7;
    lhs.s8 /= rhs.s8;
    return lhs;
}

template<typename T>
inline Mat3<T> &operator+=(Mat3<T> &lhs, const T scalar)
{
    lhs.s0 += scalar;
    lhs.s1 += scalar;
    lhs.s2 += scalar;
    lhs.s3 += scalar;
    lhs.s4 += scalar;
    lhs.s5 += scalar;
    lhs.s6 += scalar;
    lhs.s7 += scalar;
    lhs.s8 += scalar;
    return lhs;
}

template<typename T>
inline Mat3<T> &operator-=(Mat3<T> &lhs, const T scalar)
{
    lhs.s0 -= scalar;
    lhs.s1 -= scalar;
    lhs.s2 -= scalar;
    lhs.s3 -= scalar;
    lhs.s4 -= scalar;
    lhs.s5 -= scalar;
    lhs.s6 -= scalar;
    lhs.s7 -= scalar;
    lhs.s8 -= scalar;
    return lhs;
}

template<typename T>
inline Mat3<T> &operator*=(Mat3<T> &lhs, const T scalar)
{
    lhs.s0 *= scalar;
    lhs.s1 *= scalar;
    lhs.s2 *= scalar;
    lhs.s3 *= scalar;
    lhs.s4 *= scalar;
    lhs.s5 *= scalar;
    lhs.s6 *= scalar;
    lhs.s7 *= scalar;
    lhs.s8 *= scalar;
    return lhs;
}

template<typename T>
inline Mat3<T> &operator/=(Mat3<T> &lhs, const T scalar)
{
    lhs.s0 /= scalar;
    lhs.s1 /= scalar;
    lhs.s2 /= scalar;
    lhs.s3 /= scalar;
    lhs.s4 /= scalar;
    lhs.s5 /= scalar;
    lhs.s6 /= scalar;
    lhs.s7 /= scalar;
    lhs.s8 /= scalar;
    return lhs;
}

///
/// Arithmetic operators.
///
template<typename T>
inline Mat3<T> operator+(Mat3<T> lhs, const Mat3<T> &rhs) { return lhs += rhs; }
template<typename T>
inline Mat3<T> operator-(Mat3<T> lhs, const Mat3<T> &rhs) { return lhs -= rhs; }
template<typename T>
inline Mat3<T> operator*(Mat3<T> lhs, const Mat3<T> &rhs) { return lhs *= rhs; }
template<typename T>
inline Mat3<T> operator/(Mat3<T> lhs, const Mat3<T> &rhs) { return lhs /= rhs; }

template<typename T>
inline Mat3<T> operator+(Mat3<T> lhs, const T scalar) { return lhs += scalar; }
template<typename T>
inline Mat3<T> operator-(Mat3<T> lhs, const T scalar) { return lhs -= scalar; }
template<typename T>
inline Mat3<T> operator*(Mat3<T> lhs, const T scalar) { return lhs *= scalar; }
template<typename T>
inline Mat3<T> operator/(Mat3<T> lhs, const T scalar) { return lhs /= scalar; }

template<typename T>
inline Mat3<T> operator+(const T scalar, Mat3<T> rhs) { return rhs += scalar; }
template<typename T>
inline Mat3<T> operator-(const T scalar, Mat3<T> rhs) {
    Mat3<T> lhs = -rhs;
    return lhs += scalar;
}
template<typename T>
inline Mat3<T> operator*(const T scalar, Mat3<T> rhs) { return rhs *= scalar; }
template<typename T>
inline Mat3<T> operator/(const T scalar, Mat3<T> rhs) {
    return {
        scalar / rhs.xx, scalar / rhs.xy, scalar / rhs.xz,
        scalar / rhs.yx, scalar / rhs.yy, scalar / rhs.yz,
        scalar / rhs.zx, scalar / rhs.zy, scalar / rhs.zz
    };
}

///
/// Unary operators.
///
template<typename T>
inline Mat3<T> operator+(Mat3<T> lhs) { return lhs; }

template<typename T>
inline Mat3<T> operator-(Mat3<T> lhs) { return lhs *= (T) -1; }

///
/// Increment operators.
///
template<typename T>
inline Mat3<T> &operator++(Mat3<T> &lhs) { return lhs += (T) 1; }

template<typename T>
inline Mat3<T> &operator--(Mat3<T> &lhs) { return lhs -= (T) 1; }

template<typename T>
inline Mat3<T> operator++(Mat3<T> &lhs, int)
{
    Mat3<T> result = lhs;
    ++lhs;
    return result;
}

template<typename T>
inline Mat3<T> operator--(Mat3<T> &lhs, int)
{
    Mat3<T> result = lhs;
    --lhs;
    return result;
}

/// ---- Mat4 implementation --------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T>
inline Mat4<T> &operator+=(Mat4<T> &lhs, const Mat4<T> &rhs)
{
    lhs.s0  += rhs.s0;
    lhs.s1  += rhs.s1;
    lhs.s2  += rhs.s2;
    lhs.s3  += rhs.s3;
    lhs.s4  += rhs.s4;
    lhs.s5  += rhs.s5;
    lhs.s6  += rhs.s6;
    lhs.s7  += rhs.s7;
    lhs.s8  += rhs.s8;
    lhs.s9  += rhs.s9;
    lhs.s10 += rhs.s10;
    lhs.s11 += rhs.s11;
    lhs.s12 += rhs.s12;
    lhs.s13 += rhs.s13;
    lhs.s14 += rhs.s14;
    lhs.s15 += rhs.s15;
    return lhs;
}

template<typename T>
inline Mat4<T> &operator-=(Mat4<T> &lhs, const Mat4<T> &rhs)
{
    lhs.s0  -= rhs.s0;
    lhs.s1  -= rhs.s1;
    lhs.s2  -= rhs.s2;
    lhs.s3  -= rhs.s3;
    lhs.s4  -= rhs.s4;
    lhs.s5  -= rhs.s5;
    lhs.s6  -= rhs.s6;
    lhs.s7  -= rhs.s7;
    lhs.s8  -= rhs.s8;
    lhs.s9  -= rhs.s9;
    lhs.s10 -= rhs.s10;
    lhs.s11 -= rhs.s11;
    lhs.s12 -= rhs.s12;
    lhs.s13 -= rhs.s13;
    lhs.s14 -= rhs.s14;
    lhs.s15 -= rhs.s15;
    return lhs;
}

template<typename T>
inline Mat4<T> &operator*=(Mat4<T> &lhs, const Mat4<T> &rhs)
{
    lhs.s0  *= rhs.s0;
    lhs.s1  *= rhs.s1;
    lhs.s2  *= rhs.s2;
    lhs.s3  *= rhs.s3;
    lhs.s4  *= rhs.s4;
    lhs.s5  *= rhs.s5;
    lhs.s6  *= rhs.s6;
    lhs.s7  *= rhs.s7;
    lhs.s8  *= rhs.s8;
    lhs.s9  *= rhs.s9;
    lhs.s10 *= rhs.s10;
    lhs.s11 *= rhs.s11;
    lhs.s12 *= rhs.s12;
    lhs.s13 *= rhs.s13;
    lhs.s14 *= rhs.s14;
    lhs.s15 *= rhs.s15;
    return lhs;
}

template<typename T>
inline Mat4<T> &operator/=(Mat4<T> &lhs, const Mat4<T> &rhs)
{
    lhs.s0  /= rhs.s0;
    lhs.s1  /= rhs.s1;
    lhs.s2  /= rhs.s2;
    lhs.s3  /= rhs.s3;
    lhs.s4  /= rhs.s4;
    lhs.s5  /= rhs.s5;
    lhs.s6  /= rhs.s6;
    lhs.s7  /= rhs.s7;
    lhs.s8  /= rhs.s8;
    lhs.s9  /= rhs.s9;
    lhs.s10 /= rhs.s10;
    lhs.s11 /= rhs.s11;
    lhs.s12 /= rhs.s12;
    lhs.s13 /= rhs.s13;
    lhs.s14 /= rhs.s14;
    lhs.s15 /= rhs.s15;
    return lhs;
}

template<typename T>
inline Mat4<T> &operator+=(Mat4<T> &lhs, const T scalar)
{
    lhs.s0  += scalar;
    lhs.s1  += scalar;
    lhs.s2  += scalar;
    lhs.s3  += scalar;
    lhs.s4  += scalar;
    lhs.s5  += scalar;
    lhs.s6  += scalar;
    lhs.s7  += scalar;
    lhs.s8  += scalar;
    lhs.s9  += scalar;
    lhs.s10 += scalar;
    lhs.s11 += scalar;
    lhs.s12 += scalar;
    lhs.s13 += scalar;
    lhs.s14 += scalar;
    lhs.s15 += scalar;
    return lhs;
}

template<typename T>
inline Mat4<T> &operator-=(Mat4<T> &lhs, const T scalar)
{
    lhs.s0  -= scalar;
    lhs.s1  -= scalar;
    lhs.s2  -= scalar;
    lhs.s3  -= scalar;
    lhs.s4  -= scalar;
    lhs.s5  -= scalar;
    lhs.s6  -= scalar;
    lhs.s7  -= scalar;
    lhs.s8  -= scalar;
    lhs.s9  -= scalar;
    lhs.s10 -= scalar;
    lhs.s11 -= scalar;
    lhs.s12 -= scalar;
    lhs.s13 -= scalar;
    lhs.s14 -= scalar;
    lhs.s15 -= scalar;
    return lhs;
}

template<typename T>
inline Mat4<T> &operator*=(Mat4<T> &lhs, const T scalar)
{
    lhs.s0  *= scalar;
    lhs.s1  *= scalar;
    lhs.s2  *= scalar;
    lhs.s3  *= scalar;
    lhs.s4  *= scalar;
    lhs.s5  *= scalar;
    lhs.s6  *= scalar;
    lhs.s7  *= scalar;
    lhs.s8  *= scalar;
    lhs.s9  *= scalar;
    lhs.s10 *= scalar;
    lhs.s11 *= scalar;
    lhs.s12 *= scalar;
    lhs.s13 *= scalar;
    lhs.s14 *= scalar;
    lhs.s15 *= scalar;
    return lhs;
}

template<typename T>
inline Mat4<T> &operator/=(Mat4<T> &lhs, const T scalar)
{
    lhs.s0  /= scalar;
    lhs.s1  /= scalar;
    lhs.s2  /= scalar;
    lhs.s3  /= scalar;
    lhs.s4  /= scalar;
    lhs.s5  /= scalar;
    lhs.s6  /= scalar;
    lhs.s7  /= scalar;
    lhs.s8  /= scalar;
    lhs.s9  /= scalar;
    lhs.s10 /= scalar;
    lhs.s11 /= scalar;
    lhs.s12 /= scalar;
    lhs.s13 /= scalar;
    lhs.s14 /= scalar;
    lhs.s15 /= scalar;
    return lhs;
}

///
/// Arithmetic operators.
///
template<typename T>
inline Mat4<T> operator+(Mat4<T> lhs, const Mat4<T> &rhs) { return lhs += rhs; }
template<typename T>
inline Mat4<T> operator-(Mat4<T> lhs, const Mat4<T> &rhs) { return lhs -= rhs; }
template<typename T>
inline Mat4<T> operator*(Mat4<T> lhs, const Mat4<T> &rhs) { return lhs *= rhs; }
template<typename T>
inline Mat4<T> operator/(Mat4<T> lhs, const Mat4<T> &rhs) { return lhs /= rhs; }

template<typename T>
inline Mat4<T> operator+(Mat4<T> lhs, const T scalar) { return lhs += scalar; }
template<typename T>
inline Mat4<T> operator-(Mat4<T> lhs, const T scalar) { return lhs -= scalar; }
template<typename T>
inline Mat4<T> operator*(Mat4<T> lhs, const T scalar) { return lhs *= scalar; }
template<typename T>
inline Mat4<T> operator/(Mat4<T> lhs, const T scalar) { return lhs /= scalar; }

template<typename T>
inline Mat4<T> operator+(const T scalar, Mat4<T> rhs) { return rhs += scalar; }
template<typename T>
inline Mat4<T> operator-(const T scalar, Mat4<T> rhs) {
    Mat4<T> lhs = -rhs;
    return lhs += scalar;
}
template<typename T>
inline Mat4<T> operator*(const T scalar, Mat4<T> rhs) { return rhs *= scalar; }
template<typename T>
inline Mat4<T> operator/(const T scalar, Mat4<T> rhs) {
    return {
        scalar / rhs.xx, scalar / rhs.xy, scalar / rhs.xz, scalar / rhs.xw,
        scalar / rhs.yx, scalar / rhs.yy, scalar / rhs.yz, scalar / rhs.yw,
        scalar / rhs.zx, scalar / rhs.zy, scalar / rhs.zz, scalar / rhs.zw,
        scalar / rhs.wx, scalar / rhs.wy, scalar / rhs.wz, scalar / rhs.ww
    };
}

///
/// Unary operators.
///
template<typename T>
inline Mat4<T> operator+(Mat4<T> lhs) { return lhs; }

template<typename T>
inline Mat4<T> operator-(Mat4<T> lhs) { return lhs *= (T) -1; }

///
/// Increment operators.
///
template<typename T>
inline Mat4<T> &operator++(Mat4<T> &lhs) { return lhs += (T) 1; }

template<typename T>
inline Mat4<T> &operator--(Mat4<T> &lhs) { return lhs -= (T) 1; }

template<typename T>
inline Mat4<T> operator++(Mat4<T> &lhs, int)
{
    Mat4<T> result = lhs;
    ++lhs;
    return result;
}

template<typename T>
inline Mat4<T> operator--(Mat4<T> &lhs, int)
{
    Mat4<T> result = lhs;
    --lhs;
    return result;
}

} // namespace Math

/// ---- simd implementations ------------------------------------------------
#ifdef __AVX__
#include "simd/matrix.h"
#endif

#endif // MATH_MATRIX_H_

