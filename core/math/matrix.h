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


namespace math {

/// ---- Matrix data types ----------------------------------------------------
///
template<typename T>
struct mat2 {
    static const mat2 zeros;
    static const mat2 ones;
    static const mat2 eye;
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
struct mat3 {
    static const mat3 zeros;
    static const mat3 ones;
    static const mat3 eye;
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
struct mat4 {
    static const mat4 zeros;
    static const mat4 ones;
    static const mat4 eye;
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

typedef mat2<int16_t>   mat2i16;
typedef mat2<int32_t>   mat2i32;
typedef mat2<int64_t>   mat2i64;
typedef mat2<int32_t>   mat2i;
typedef mat2<uint16_t>  mat2u16;
typedef mat2<uint32_t>  mat2u32;
typedef mat2<uint64_t>  mat2u64;
typedef mat2<uint32_t>  mat2u;
typedef mat2<float>     mat2f;
typedef mat2<double>    mat2d;

typedef mat3<int16_t>   mat3i16;
typedef mat3<int32_t>   mat3i32;
typedef mat3<int64_t>   mat3i64;
typedef mat3<int32_t>   mat3i;
typedef mat3<uint16_t>  mat3u16;
typedef mat3<uint32_t>  mat3u32;
typedef mat3<uint64_t>  mat3u64;
typedef mat3<uint32_t>  mat3u;
typedef mat3<float>     mat3f;
typedef mat3<double>    mat3d;

typedef mat4<int16_t>   mat4i16;
typedef mat4<int32_t>   mat4i32;
typedef mat4<int64_t>   mat4i64;
typedef mat4<int32_t>   mat4i;
typedef mat4<uint16_t>  mat4u16;
typedef mat4<uint32_t>  mat4u32;
typedef mat4<uint64_t>  mat4u64;
typedef mat4<uint32_t>  mat4u;
typedef mat4<float>     mat4f;
typedef mat4<double>    mat4d;

/// ---- Special matrices -----------------------------------------------------
///
template<typename T>
const mat2<T> mat2<T>::zeros = {
    (T) 0, (T) 0,
    (T) 0, (T) 0};
template<typename T>
const mat2<T> mat2<T>::ones = {
    (T) 1, (T) 1,
    (T) 1, (T) 1};
template<typename T>
const mat2<T> mat2<T>::eye = {
    (T) 1, (T) 0,
    (T) 0, (T) 1};

template<typename T>
const mat3<T> mat3<T>::zeros = {
    (T) 0, (T) 0, (T) 0,
    (T) 0, (T) 0, (T) 0,
    (T) 0, (T) 0, (T) 0};
template<typename T>
const mat3<T> mat3<T>::ones = {
    (T) 1, (T) 1, (T) 1,
    (T) 1, (T) 1, (T) 1,
    (T) 1, (T) 1, (T) 1};
template<typename T>
const mat3<T> mat3<T>::eye = {
    (T) 1, (T) 0, (T) 0,
    (T) 0, (T) 1, (T) 0,
    (T) 0, (T) 0, (T) 1};

template<typename T>
const mat4<T> mat4<T>::zeros = {
    (T) 0, (T) 0, (T) 0, (T) 0,
    (T) 0, (T) 0, (T) 0, (T) 0,
    (T) 0, (T) 0, (T) 0, (T) 0,
    (T) 0, (T) 0, (T) 0, (T) 0};

template<typename T>
const mat4<T> mat4<T>::ones = {
    (T) 1, (T) 1, (T) 1, (T) 1,
    (T) 1, (T) 1, (T) 1, (T) 1,
    (T) 1, (T) 1, (T) 1, (T) 1,
    (T) 1, (T) 1, (T) 1, (T) 1};

template<typename T>
const mat4<T> mat4<T>::eye = {
    (T) 1, (T) 0, (T) 0, (T) 0,
    (T) 0, (T) 1, (T) 0, (T) 0,
    (T) 0, (T) 0, (T) 1, (T) 0,
    (T) 0, (T) 0, (T) 0, (T) 1};

/// ---- mat2 declarations ----------------------------------------------------
/// Compound assignment operators matrix operators.
///
template<typename T>
inline mat2<T> &operator+=(mat2<T> &lhs, const mat2<T> &rhs);
template<typename T>
inline mat2<T> &operator-=(mat2<T> &lhs, const mat2<T> &rhs);
template<typename T>
inline mat2<T> &operator*=(mat2<T> &lhs, const mat2<T> &rhs);
template<typename T>
inline mat2<T> &operator/=(mat2<T> &lhs, const mat2<T> &rhs);

template<typename T>
inline mat2<T> &operator+=(mat2<T> &lhs, const T scalar);
template<typename T>
inline mat2<T> &operator-=(mat2<T> &lhs, const T scalar);
template<typename T>
inline mat2<T> &operator*=(mat2<T> &lhs, const T scalar);
template<typename T>
inline mat2<T> &operator/=(mat2<T> &lhs, const T scalar);

/// Arithmetic operators.
template<typename T>
inline mat2<T> operator+(mat2<T> lhs, const mat2<T> &rhs);
template<typename T>
inline mat2<T> operator-(mat2<T> lhs, const mat2<T> &rhs);
template<typename T>
inline mat2<T> operator*(mat2<T> lhs, const mat2<T> &rhs);
template<typename T>
inline mat2<T> operator/(mat2<T> lhs, const mat2<T> &rhs);

template<typename T>
inline mat2<T> operator+(mat2<T> lhs, const T scalar);
template<typename T>
inline mat2<T> operator-(mat2<T> lhs, const T scalar);
template<typename T>
inline mat2<T> operator*(mat2<T> lhs, const T scalar);
template<typename T>
inline mat2<T> operator/(mat2<T> lhs, const T scalar);

template<typename T>
inline mat2<T> operator+(const T scalar, mat2<T> rhs);
template<typename T>
inline mat2<T> operator-(const T scalar, mat2<T> rhs);
template<typename T>
inline mat2<T> operator*(const T scalar, mat2<T> rhs);
template<typename T>
inline mat2<T> operator/(const T scalar, mat2<T> rhs);

/// Unary operators.
template<typename T>
inline mat2<T> operator+(mat2<T> lhs);
template<typename T>
inline mat2<T> operator-(mat2<T> lhs);

/// Increment operators.
template<typename T>
inline mat2<T> &operator++(mat2<T> &lhs);
template<typename T>
inline mat2<T> &operator--(mat2<T> &lhs);
template<typename T>
inline mat2<T> operator++(mat2<T> &lhs, int);
template<typename T>
inline mat2<T> operator--(mat2<T> &lhs, int);

/// ---- mat3 declarations ----------------------------------------------------
/// Compound assignment operators matrix operators.
///
template<typename T>
inline mat3<T> &operator+=(mat3<T> &lhs, const mat3<T> &rhs);
template<typename T>
inline mat3<T> &operator-=(mat3<T> &lhs, const mat3<T> &rhs);
template<typename T>
inline mat3<T> &operator*=(mat3<T> &lhs, const mat3<T> &rhs);
template<typename T>
inline mat3<T> &operator/=(mat3<T> &lhs, const mat3<T> &rhs);

template<typename T>
inline mat3<T> &operator+=(mat3<T> &lhs, const T scalar);
template<typename T>
inline mat3<T> &operator-=(mat3<T> &lhs, const T scalar);
template<typename T>
inline mat3<T> &operator*=(mat3<T> &lhs, const T scalar);
template<typename T>
inline mat3<T> &operator/=(mat3<T> &lhs, const T scalar);

/// Arithmetic operators.
template<typename T>
inline mat3<T> operator+(mat3<T> lhs, const mat3<T> &rhs);
template<typename T>
inline mat3<T> operator-(mat3<T> lhs, const mat3<T> &rhs);
template<typename T>
inline mat3<T> operator*(mat3<T> lhs, const mat3<T> &rhs);
template<typename T>
inline mat3<T> operator/(mat3<T> lhs, const mat3<T> &rhs);

template<typename T>
inline mat3<T> operator+(mat3<T> lhs, const T scalar);
template<typename T>
inline mat3<T> operator-(mat3<T> lhs, const T scalar);
template<typename T>
inline mat3<T> operator*(mat3<T> lhs, const T scalar);
template<typename T>
inline mat3<T> operator/(mat3<T> lhs, const T scalar);

template<typename T>
inline mat3<T> operator+(const T scalar, mat3<T> rhs);
template<typename T>
inline mat3<T> operator-(const T scalar, mat3<T> rhs);
template<typename T>
inline mat3<T> operator*(const T scalar, mat3<T> rhs);
template<typename T>
inline mat3<T> operator/(const T scalar, mat3<T> rhs);

/// Unary operators.
template<typename T>
inline mat3<T> operator+(mat3<T> lhs);
template<typename T>
inline mat3<T> operator-(mat3<T> lhs);

/// Increment operators.
template<typename T>
inline mat3<T> &operator++(mat3<T> &lhs);
template<typename T>
inline mat3<T> &operator--(mat3<T> &lhs);
template<typename T>
inline mat3<T> operator++(mat3<T> &lhs, int);
template<typename T>
inline mat3<T> operator--(mat3<T> &lhs, int);

/// ---- mat4 declarations ----------------------------------------------------
/// Compound assignment operators matrix operators.
///
template<typename T>
inline mat4<T> &operator+=(mat4<T> &lhs, const mat4<T> &rhs);
template<typename T>
inline mat4<T> &operator-=(mat4<T> &lhs, const mat4<T> &rhs);
template<typename T>
inline mat4<T> &operator*=(mat4<T> &lhs, const mat4<T> &rhs);
template<typename T>
inline mat4<T> &operator/=(mat4<T> &lhs, const mat4<T> &rhs);

template<typename T>
inline mat4<T> &operator+=(mat4<T> &lhs, const T scalar);
template<typename T>
inline mat4<T> &operator-=(mat4<T> &lhs, const T scalar);
template<typename T>
inline mat4<T> &operator*=(mat4<T> &lhs, const T scalar);
template<typename T>
inline mat4<T> &operator/=(mat4<T> &lhs, const T scalar);

/// Arithmetic operators.
template<typename T>
inline mat4<T> operator+(mat4<T> lhs, const mat4<T> &rhs);
template<typename T>
inline mat4<T> operator-(mat4<T> lhs, const mat4<T> &rhs);
template<typename T>
inline mat4<T> operator*(mat4<T> lhs, const mat4<T> &rhs);
template<typename T>
inline mat4<T> operator/(mat4<T> lhs, const mat4<T> &rhs);

template<typename T>
inline mat4<T> operator+(mat4<T> lhs, const T scalar);
template<typename T>
inline mat4<T> operator-(mat4<T> lhs, const T scalar);
template<typename T>
inline mat4<T> operator*(mat4<T> lhs, const T scalar);
template<typename T>
inline mat4<T> operator/(mat4<T> lhs, const T scalar);

template<typename T>
inline mat4<T> operator+(const T scalar, mat4<T> rhs);
template<typename T>
inline mat4<T> operator-(const T scalar, mat4<T> rhs);
template<typename T>
inline mat4<T> operator*(const T scalar, mat4<T> rhs);
template<typename T>
inline mat4<T> operator/(const T scalar, mat4<T> rhs);

/// Unary operators.
template<typename T>
inline mat4<T> operator+(mat4<T> lhs);
template<typename T>
inline mat4<T> operator-(mat4<T> lhs);

/// Increment operators.
template<typename T>
inline mat4<T> &operator++(mat4<T> &lhs);
template<typename T>
inline mat4<T> &operator--(mat4<T> &lhs);
template<typename T>
inline mat4<T> operator++(mat4<T> &lhs, int);
template<typename T>
inline mat4<T> operator--(mat4<T> &lhs, int);

/// ---- mat2 implementation --------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T>
inline mat2<T> &operator+=(mat2<T> &lhs, const mat2<T> &rhs)
{
    lhs.s0 += rhs.s0;
    lhs.s1 += rhs.s1;
    lhs.s2 += rhs.s2;
    lhs.s3 += rhs.s3;
    return lhs;
}

template<typename T>
inline mat2<T> &operator-=(mat2<T> &lhs, const mat2<T> &rhs)
{
    lhs.s0 -= rhs.s0;
    lhs.s1 -= rhs.s1;
    lhs.s2 -= rhs.s2;
    lhs.s3 -= rhs.s3;
    return lhs;
}

template<typename T>
inline mat2<T> &operator*=(mat2<T> &lhs, const mat2<T> &rhs)
{
    lhs.s0 *= rhs.s0;
    lhs.s1 *= rhs.s1;
    lhs.s2 *= rhs.s2;
    lhs.s3 *= rhs.s3;
    return lhs;
}

template<typename T>
inline mat2<T> &operator/=(mat2<T> &lhs, const mat2<T> &rhs)
{
    lhs.s0 /= rhs.s0;
    lhs.s1 /= rhs.s1;
    lhs.s2 /= rhs.s2;
    lhs.s3 /= rhs.s3;
    return lhs;
}

template<typename T>
inline mat2<T> &operator+=(mat2<T> &lhs, const T scalar)
{
    lhs.s0 += scalar;
    lhs.s1 += scalar;
    lhs.s2 += scalar;
    lhs.s3 += scalar;
    return lhs;
}

template<typename T>
inline mat2<T> &operator-=(mat2<T> &lhs, const T scalar)
{
    lhs.s0 -= scalar;
    lhs.s1 -= scalar;
    lhs.s2 -= scalar;
    lhs.s3 -= scalar;
    return lhs;
}

template<typename T>
inline mat2<T> &operator*=(mat2<T> &lhs, const T scalar)
{
    lhs.s0 *= scalar;
    lhs.s1 *= scalar;
    lhs.s2 *= scalar;
    lhs.s3 *= scalar;
    return lhs;
}

template<typename T>
inline mat2<T> &operator/=(mat2<T> &lhs, const T scalar)
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
inline mat2<T> operator+(mat2<T> lhs, const mat2<T> &rhs) { return lhs += rhs; }
template<typename T>
inline mat2<T> operator-(mat2<T> lhs, const mat2<T> &rhs) { return lhs -= rhs; }
template<typename T>
inline mat2<T> operator*(mat2<T> lhs, const mat2<T> &rhs) { return lhs *= rhs; }
template<typename T>
inline mat2<T> operator/(mat2<T> lhs, const mat2<T> &rhs) { return lhs /= rhs; }

template<typename T>
inline mat2<T> operator+(mat2<T> lhs, const T scalar) { return lhs += scalar; }
template<typename T>
inline mat2<T> operator-(mat2<T> lhs, const T scalar) { return lhs -= scalar; }
template<typename T>
inline mat2<T> operator*(mat2<T> lhs, const T scalar) { return lhs *= scalar; }
template<typename T>
inline mat2<T> operator/(mat2<T> lhs, const T scalar) { return lhs /= scalar; }

template<typename T>
inline mat2<T> operator+(const T scalar, mat2<T> rhs) { return rhs += scalar; }
template<typename T>
inline mat2<T> operator-(const T scalar, mat2<T> rhs) {
    mat2<T> lhs = -rhs;
    return lhs += scalar;
}
template<typename T>
inline mat2<T> operator*(const T scalar, mat2<T> rhs) { return rhs *= scalar; }
template<typename T>
inline mat2<T> operator/(const T scalar, mat2<T> rhs) {
    return {
        scalar / rhs.xx, scalar / rhs.xy,
        scalar / rhs.yx, scalar / rhs.yy
    };
}

///
/// Unary operators.
///
template<typename T>
inline mat2<T> operator+(mat2<T> lhs) { return lhs; }

template<typename T>
inline mat2<T> operator-(mat2<T> lhs) { return lhs *= (T) -1; }

///
/// Increment operators.
///
template<typename T>
inline mat2<T> &operator++(mat2<T> &lhs) { return lhs += (T) 1; }

template<typename T>
inline mat2<T> &operator--(mat2<T> &lhs) { return lhs -= (T) 1; }

template<typename T>
inline mat2<T> operator++(mat2<T> &lhs, int)
{
    mat2<T> result = lhs;
    ++lhs;
    return result;
}

template<typename T>
inline mat2<T> operator--(mat2<T> &lhs, int)
{
    mat2<T> result = lhs;
    --lhs;
    return result;
}

/// ---- mat3 implementation --------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T>
inline mat3<T> &operator+=(mat3<T> &lhs, const mat3<T> &rhs)
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
inline mat3<T> &operator-=(mat3<T> &lhs, const mat3<T> &rhs)
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
inline mat3<T> &operator*=(mat3<T> &lhs, const mat3<T> &rhs)
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
inline mat3<T> &operator/=(mat3<T> &lhs, const mat3<T> &rhs)
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
inline mat3<T> &operator+=(mat3<T> &lhs, const T scalar)
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
inline mat3<T> &operator-=(mat3<T> &lhs, const T scalar)
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
inline mat3<T> &operator*=(mat3<T> &lhs, const T scalar)
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
inline mat3<T> &operator/=(mat3<T> &lhs, const T scalar)
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
inline mat3<T> operator+(mat3<T> lhs, const mat3<T> &rhs) { return lhs += rhs; }
template<typename T>
inline mat3<T> operator-(mat3<T> lhs, const mat3<T> &rhs) { return lhs -= rhs; }
template<typename T>
inline mat3<T> operator*(mat3<T> lhs, const mat3<T> &rhs) { return lhs *= rhs; }
template<typename T>
inline mat3<T> operator/(mat3<T> lhs, const mat3<T> &rhs) { return lhs /= rhs; }

template<typename T>
inline mat3<T> operator+(mat3<T> lhs, const T scalar) { return lhs += scalar; }
template<typename T>
inline mat3<T> operator-(mat3<T> lhs, const T scalar) { return lhs -= scalar; }
template<typename T>
inline mat3<T> operator*(mat3<T> lhs, const T scalar) { return lhs *= scalar; }
template<typename T>
inline mat3<T> operator/(mat3<T> lhs, const T scalar) { return lhs /= scalar; }

template<typename T>
inline mat3<T> operator+(const T scalar, mat3<T> rhs) { return rhs += scalar; }
template<typename T>
inline mat3<T> operator-(const T scalar, mat3<T> rhs) {
    mat3<T> lhs = -rhs;
    return lhs += scalar;
}
template<typename T>
inline mat3<T> operator*(const T scalar, mat3<T> rhs) { return rhs *= scalar; }
template<typename T>
inline mat3<T> operator/(const T scalar, mat3<T> rhs) {
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
inline mat3<T> operator+(mat3<T> lhs) { return lhs; }

template<typename T>
inline mat3<T> operator-(mat3<T> lhs) { return lhs *= (T) -1; }

///
/// Increment operators.
///
template<typename T>
inline mat3<T> &operator++(mat3<T> &lhs) { return lhs += (T) 1; }

template<typename T>
inline mat3<T> &operator--(mat3<T> &lhs) { return lhs -= (T) 1; }

template<typename T>
inline mat3<T> operator++(mat3<T> &lhs, int)
{
    mat3<T> result = lhs;
    ++lhs;
    return result;
}

template<typename T>
inline mat3<T> operator--(mat3<T> &lhs, int)
{
    mat3<T> result = lhs;
    --lhs;
    return result;
}

/// ---- mat4 implementation --------------------------------------------------
/// Compound assignment operators vector operators.
///
template<typename T>
inline mat4<T> &operator+=(mat4<T> &lhs, const mat4<T> &rhs)
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
inline mat4<T> &operator-=(mat4<T> &lhs, const mat4<T> &rhs)
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
inline mat4<T> &operator*=(mat4<T> &lhs, const mat4<T> &rhs)
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
inline mat4<T> &operator/=(mat4<T> &lhs, const mat4<T> &rhs)
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
inline mat4<T> &operator+=(mat4<T> &lhs, const T scalar)
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
inline mat4<T> &operator-=(mat4<T> &lhs, const T scalar)
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
inline mat4<T> &operator*=(mat4<T> &lhs, const T scalar)
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
inline mat4<T> &operator/=(mat4<T> &lhs, const T scalar)
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
inline mat4<T> operator+(mat4<T> lhs, const mat4<T> &rhs) { return lhs += rhs; }
template<typename T>
inline mat4<T> operator-(mat4<T> lhs, const mat4<T> &rhs) { return lhs -= rhs; }
template<typename T>
inline mat4<T> operator*(mat4<T> lhs, const mat4<T> &rhs) { return lhs *= rhs; }
template<typename T>
inline mat4<T> operator/(mat4<T> lhs, const mat4<T> &rhs) { return lhs /= rhs; }

template<typename T>
inline mat4<T> operator+(mat4<T> lhs, const T scalar) { return lhs += scalar; }
template<typename T>
inline mat4<T> operator-(mat4<T> lhs, const T scalar) { return lhs -= scalar; }
template<typename T>
inline mat4<T> operator*(mat4<T> lhs, const T scalar) { return lhs *= scalar; }
template<typename T>
inline mat4<T> operator/(mat4<T> lhs, const T scalar) { return lhs /= scalar; }

template<typename T>
inline mat4<T> operator+(const T scalar, mat4<T> rhs) { return rhs += scalar; }
template<typename T>
inline mat4<T> operator-(const T scalar, mat4<T> rhs) {
    mat4<T> lhs = -rhs;
    return lhs += scalar;
}
template<typename T>
inline mat4<T> operator*(const T scalar, mat4<T> rhs) { return rhs *= scalar; }
template<typename T>
inline mat4<T> operator/(const T scalar, mat4<T> rhs) {
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
inline mat4<T> operator+(mat4<T> lhs) { return lhs; }

template<typename T>
inline mat4<T> operator-(mat4<T> lhs) { return lhs *= (T) -1; }

///
/// Increment operators.
///
template<typename T>
inline mat4<T> &operator++(mat4<T> &lhs) { return lhs += (T) 1; }

template<typename T>
inline mat4<T> &operator--(mat4<T> &lhs) { return lhs -= (T) 1; }

template<typename T>
inline mat4<T> operator++(mat4<T> &lhs, int)
{
    mat4<T> result = lhs;
    ++lhs;
    return result;
}

template<typename T>
inline mat4<T> operator--(mat4<T> &lhs, int)
{
    mat4<T> result = lhs;
    --lhs;
    return result;
}

} // math

/// ---- simd implementations ------------------------------------------------
#ifdef __AVX__
#include "simd/matrix.h"
#endif

#endif // MATH_MATRIX_H_

