//
// matrix.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MATH_SIMD_MATRIX_H_
#define MATH_SIMD_MATRIX_H_

#include "common.h"

namespace math {

/// ---- simd load/store functions --------------------------------------------
/// @brief Load 128-bits (2 packed double-precision 64-bit) from the specified
/// row in the 2d-matrix.
/// @fn _mm256_extractf128_pd(__m256d a, const int mask)
///  CASE mask[7:0] of
///      0: dst[127:0] := a[127:0]
///      1: dst[127:0] := a[255:128]
///  ESAC
///
inline __m128d simd_load(const mat2<double> &mat, const size_t row)
{
    return _mm_load_pd(mat.data + row * mat.dim);
}

///
/// @brief Store 128-bits (2 packed double-precision 64-bit) into the specified
/// row in the 2d-matrix.
/// @fn _mm256_insertf128_pd(__m256d a, __m128d b, int mask)
///  dst[255:0] := a[255:0]
///  CASE mask[7:0] of
///      0: dst[127:0]   := b[127:0]
///      1: dst[255:128] := b[127:0]
///  ESAC
///
inline void simd_store(mat2<double> &mat, const size_t row, const __m128d a)
{
    _mm_store_pd(mat.data + row * mat.dim, a);
}

///
/// @brief Load 192-bits (3 packed double precision 64-bit) from the specified
/// row in the 3d-matrix.
/// @fn _mm256_set_epi64x(__int64 e3, __int64 e2, __int64 e1, __int64 e0)
///      dst[63:0]    := e0
///      dst[127:64]  := e1
///      dst[191:128] := e2
///      dst[255:192] := e3
///
/// @fn _mm256_maskload_pd(double const * v, __m256i mask)
///      dst[63:0]    := (mask[63]  == 1) ? v[63:0]    : 0
///      dst[127:64]  := (mask[127] == 1) ? v[127:64]  : 0
///      dst[191:128] := (mask[191] == 1) ? v[191:128] : 0
///      dst[255:192] := (mask[255] == 1) ? v[255:192] : 0
/// If any of the bits of the mask is set to zero, the corresponding value from
/// the memory location is not loaded and the corresponding field of the
/// destination vector is set to zero.
///
inline __m256d simd_load(const mat3<double> &mat, const size_t row)
{
    const __m256i mask = _mm256_set_epi64x(
        0x0,
        0xffffffffffffffff,
        0xffffffffffffffff,
        0xffffffffffffffff);
    return _mm256_maskload_pd(mat.data + row * mat.dim, mask);
}

///
/// @brief Store 192-bits (3 packed double precision 64-bit) into the specified
/// row in the 3d-matrix.
/// @fn _mm256_set_epi64x(__int64 e3, __int64 e2, __int64 e1, __int64 e0)
///      dst[63:0]    := e0
///      dst[127:64]  := e1
///      dst[191:128] := e2
///      dst[255:192] := e3
///
/// @fn _mm256_maskstore_pd(double * v, __m256i mask, __m256d a)
///      v[63:0]    := (mask[63]  == 1) ? a[63:0]
///      v[127:64]  := (mask[127] == 1) ? a[127:64]
///      v[191:128] := (mask[191] == 1) ? a[191:128]
///      v[255:192] := (mask[255] == 1) ? a[255:192]
///
/// If any of the bits of the mask are set to zero, the corresponding value from
/// the a vector is not loaded, and the corresponding field of the destination
/// vector v is left unchanged.
///
inline void simd_store(mat3<double> &mat, const size_t row, const __m256d a)
{
    const __m256i mask = _mm256_set_epi64x(
        0x0,
        0xffffffffffffffff,
        0xffffffffffffffff,
        0xffffffffffffffff);
    _mm256_maskstore_pd(mat.data + row * mat.dim, mask, a);
}

///
/// @brief Load 256-bits (4 packed double precision 64-bit) from the specified
/// row in the 4d-matrix.
///
inline __m256d simd_load(const mat4<double> &mat, const size_t row)
{
    return _mm256_load_pd(mat.data + row * mat.dim);
}

///
/// @brief Store 256-bits (4 packed double precision 64-bit) into the specified
/// row in the 4d-matrix.
///
inline void simd_store(mat4<double> &mat, const size_t row, const __m256d a)
{
    _mm256_store_pd(mat.data + row * mat.dim, a);
}

/// ---- mat2 simd assignment operators ---------------------------------------
///
template<>
inline mat2<double> &operator+=(mat2<double> &lhs, const mat2<double> &rhs)
{
    const __m128d a0 = simd_load(lhs, 0);
    const __m128d a1 = simd_load(lhs, 1);

    const __m128d b0 = simd_load(rhs, 0);
    const __m128d b1 = simd_load(rhs, 1);

    simd_store(lhs, 0, _mm_add_pd(a0, b0));
    simd_store(lhs, 1, _mm_add_pd(a1, b1));
    return lhs;
}

template<>
inline mat2<double> &operator-=(mat2<double> &lhs, const mat2<double> &rhs)
{
    const __m128d a0 = simd_load(lhs, 0);
    const __m128d a1 = simd_load(lhs, 1);

    const __m128d b0 = simd_load(rhs, 0);
    const __m128d b1 = simd_load(rhs, 1);

    simd_store(lhs, 0, _mm_sub_pd(a0, b0));
    simd_store(lhs, 1, _mm_sub_pd(a1, b1));
    return lhs;
}

template<>
inline mat2<double> &operator*=(mat2<double> &lhs, const mat2<double> &rhs)
{
    const __m128d a0 = simd_load(lhs, 0);
    const __m128d a1 = simd_load(lhs, 1);

    const __m128d b0 = simd_load(rhs, 0);
    const __m128d b1 = simd_load(rhs, 1);

    simd_store(lhs, 0, _mm_mul_pd(a0, b0));
    simd_store(lhs, 1, _mm_mul_pd(a1, b1));
    return lhs;
}

template<>
inline mat2<double> &operator/=(mat2<double> &lhs, const mat2<double> &rhs)
{
    const __m128d a0 = simd_load(lhs, 0);
    const __m128d a1 = simd_load(lhs, 1);

    const __m128d b0 = simd_load(rhs, 0);
    const __m128d b1 = simd_load(rhs, 1);

    simd_store(lhs, 0, _mm_div_pd(a0, b0));
    simd_store(lhs, 1, _mm_div_pd(a1, b1));
    return lhs;
}

template<>
inline mat2<double> &operator+=(mat2<double> &lhs, const double scalar)
{
    const __m128d a0 = simd_load(lhs, 0);
    const __m128d a1 = simd_load(lhs, 1);
    const __m128d b = _mm_set1_pd(scalar);

    simd_store(lhs, 0, _mm_add_pd(a0, b));
    simd_store(lhs, 1, _mm_add_pd(a1, b));
    return lhs;
}

template<>
inline mat2<double> &operator-=(mat2<double> &lhs, const double scalar)
{
    const __m128d a0 = simd_load(lhs, 0);
    const __m128d a1 = simd_load(lhs, 1);
    const __m128d b = _mm_set1_pd(scalar);

    simd_store(lhs, 0, _mm_sub_pd(a0, b));
    simd_store(lhs, 1, _mm_sub_pd(a1, b));
    return lhs;
}

template<>
inline mat2<double> &operator*=(mat2<double> &lhs, const double scalar)
{
    const __m128d a0 = simd_load(lhs, 0);
    const __m128d a1 = simd_load(lhs, 1);
    const __m128d b = _mm_set1_pd(scalar);

    simd_store(lhs, 0, _mm_mul_pd(a0, b));
    simd_store(lhs, 1, _mm_mul_pd(a1, b));
    return lhs;
}

template<>
inline mat2<double> &operator/=(mat2<double> &lhs, const double scalar)
{
    const __m128d a0 = simd_load(lhs, 0);
    const __m128d a1 = simd_load(lhs, 1);
    const __m128d b = _mm_set1_pd(scalar);

    simd_store(lhs, 0, _mm_div_pd(a0, b));
    simd_store(lhs, 1, _mm_div_pd(a1, b));
    return lhs;
}

/// ---- mat2 simd arithmetic operators ---------------------------------------
///
template<>
inline mat2<double> operator+(const double scalar, mat2<double> rhs)
{
    const __m128d a = _mm_set1_pd(scalar);
    const __m128d b0 = simd_load(rhs, 0);
    const __m128d b1 = simd_load(rhs, 1);

    simd_store(rhs, 0, _mm_add_pd(a, b0));
    simd_store(rhs, 1, _mm_add_pd(a, b1));
    return rhs;
}

template<>
inline mat2<double> operator-(const double scalar, mat2<double> rhs)
{
    const __m128d a = _mm_set1_pd(scalar);
    const __m128d b0 = simd_load(rhs, 0);
    const __m128d b1 = simd_load(rhs, 1);

    simd_store(rhs, 0, _mm_sub_pd(a, b0));
    simd_store(rhs, 1, _mm_sub_pd(a, b1));
    return rhs;
}

template<>
inline mat2<double> operator*(const double scalar, mat2<double> rhs)
{
    const __m128d a = _mm_set1_pd(scalar);
    const __m128d b0 = simd_load(rhs, 0);
    const __m128d b1 = simd_load(rhs, 1);

    simd_store(rhs, 0, _mm_mul_pd(a, b0));
    simd_store(rhs, 1, _mm_mul_pd(a, b1));
    return rhs;
}

template<>
inline mat2<double> operator/(const double scalar, mat2<double> rhs)
{
    const __m128d a = _mm_set1_pd(scalar);
    const __m128d b0 = simd_load(rhs, 0);
    const __m128d b1 = simd_load(rhs, 1);

    simd_store(rhs, 0, _mm_div_pd(a, b0));
    simd_store(rhs, 1, _mm_div_pd(a, b1));
    return rhs;
}

/// ---- mat3 simd assignment operators ---------------------------------------
///
template<>
inline mat3<double> &operator+=(mat3<double> &lhs, const mat3<double> &rhs)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);

    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);

    simd_store(lhs, 0, _mm256_add_pd(a0, b0));
    simd_store(lhs, 1, _mm256_add_pd(a1, b1));
    simd_store(lhs, 2, _mm256_add_pd(a2, b2));
    return lhs;
}

template<>
inline mat3<double> &operator-=(mat3<double> &lhs, const mat3<double> &rhs)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);

    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);

    simd_store(lhs, 0, _mm256_sub_pd(a0, b0));
    simd_store(lhs, 1, _mm256_sub_pd(a1, b1));
    simd_store(lhs, 2, _mm256_sub_pd(a2, b2));
    return lhs;
}

template<>
inline mat3<double> &operator*=(mat3<double> &lhs, const mat3<double> &rhs)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);

    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);

    simd_store(lhs, 0, _mm256_mul_pd(a0, b0));
    simd_store(lhs, 1, _mm256_mul_pd(a1, b1));
    simd_store(lhs, 2, _mm256_mul_pd(a2, b2));
    return lhs;
}

template<>
inline mat3<double> &operator/=(mat3<double> &lhs, const mat3<double> &rhs)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);

    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);

    simd_store(lhs, 0, _mm256_div_pd(a0, b0));
    simd_store(lhs, 1, _mm256_div_pd(a1, b1));
    simd_store(lhs, 2, _mm256_div_pd(a2, b2));
    return lhs;
}

template<>
inline mat3<double> &operator+=(mat3<double> &lhs, const double scalar)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);
    const __m256d b = _mm256_set1_pd(scalar);

    simd_store(lhs, 0, _mm256_add_pd(a0, b));
    simd_store(lhs, 1, _mm256_add_pd(a1, b));
    simd_store(lhs, 2, _mm256_add_pd(a2, b));
    return lhs;
}

template<>
inline mat3<double> &operator-=(mat3<double> &lhs, const double scalar)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);
    const __m256d b = _mm256_set1_pd(scalar);

    simd_store(lhs, 0, _mm256_sub_pd(a0, b));
    simd_store(lhs, 1, _mm256_sub_pd(a1, b));
    simd_store(lhs, 2, _mm256_sub_pd(a2, b));
    return lhs;
}

template<>
inline mat3<double> &operator*=(mat3<double> &lhs, const double scalar)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);
    const __m256d b = _mm256_set1_pd(scalar);

    simd_store(lhs, 0, _mm256_mul_pd(a0, b));
    simd_store(lhs, 1, _mm256_mul_pd(a1, b));
    simd_store(lhs, 2, _mm256_mul_pd(a2, b));
    return lhs;
}

template<>
inline mat3<double> &operator/=(mat3<double> &lhs, const double scalar)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);
    const __m256d b = _mm256_set1_pd(scalar);

    simd_store(lhs, 0, _mm256_div_pd(a0, b));
    simd_store(lhs, 1, _mm256_div_pd(a1, b));
    simd_store(lhs, 2, _mm256_div_pd(a2, b));
    return lhs;
}

/// ---- mat3 simd arithmetic operators ---------------------------------------
///
template<>
inline mat3<double> operator+(const double scalar, mat3<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);

    simd_store(rhs, 0, _mm256_add_pd(a, b0));
    simd_store(rhs, 1, _mm256_add_pd(a, b1));
    simd_store(rhs, 2, _mm256_add_pd(a, b2));
    return rhs;
}

template<>
inline mat3<double> operator-(const double scalar, mat3<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);

    simd_store(rhs, 0, _mm256_sub_pd(a, b0));
    simd_store(rhs, 1, _mm256_sub_pd(a, b1));
    simd_store(rhs, 2, _mm256_sub_pd(a, b2));
    return rhs;
}

template<>
inline mat3<double> operator*(const double scalar, mat3<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);

    simd_store(rhs, 0, _mm256_mul_pd(a, b0));
    simd_store(rhs, 1, _mm256_mul_pd(a, b1));
    simd_store(rhs, 2, _mm256_mul_pd(a, b2));
    return rhs;
}

template<>
inline mat3<double> operator/(const double scalar, mat3<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);

    simd_store(rhs, 0, _mm256_div_pd(a, b0));
    simd_store(rhs, 1, _mm256_div_pd(a, b1));
    simd_store(rhs, 2, _mm256_div_pd(a, b2));
    return rhs;
}

/// ---- mat4 simd assignment operators ---------------------------------------
///
template<>
inline mat4<double> &operator+=(mat4<double> &lhs, const mat4<double> &rhs)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);
    const __m256d a3 = simd_load(lhs, 3);

    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);
    const __m256d b3 = simd_load(rhs, 3);

    simd_store(lhs, 0, _mm256_add_pd(a0, b0));
    simd_store(lhs, 1, _mm256_add_pd(a1, b1));
    simd_store(lhs, 2, _mm256_add_pd(a2, b2));
    simd_store(lhs, 3, _mm256_add_pd(a3, b3));
    return lhs;
}

template<>
inline mat4<double> &operator-=(mat4<double> &lhs, const mat4<double> &rhs)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);
    const __m256d a3 = simd_load(lhs, 3);

    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);
    const __m256d b3 = simd_load(rhs, 3);

    simd_store(lhs, 0, _mm256_sub_pd(a0, b0));
    simd_store(lhs, 1, _mm256_sub_pd(a1, b1));
    simd_store(lhs, 2, _mm256_sub_pd(a2, b2));
    simd_store(lhs, 3, _mm256_sub_pd(a3, b3));
    return lhs;
}

template<>
inline mat4<double> &operator*=(mat4<double> &lhs, const mat4<double> &rhs)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);
    const __m256d a3 = simd_load(lhs, 3);

    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);
    const __m256d b3 = simd_load(rhs, 3);

    simd_store(lhs, 0, _mm256_mul_pd(a0, b0));
    simd_store(lhs, 1, _mm256_mul_pd(a1, b1));
    simd_store(lhs, 2, _mm256_mul_pd(a2, b2));
    simd_store(lhs, 3, _mm256_mul_pd(a3, b3));
    return lhs;
}

template<>
inline mat4<double> &operator/=(mat4<double> &lhs, const mat4<double> &rhs)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);
    const __m256d a3 = simd_load(lhs, 3);

    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);
    const __m256d b3 = simd_load(rhs, 3);

    simd_store(lhs, 0, _mm256_div_pd(a0, b0));
    simd_store(lhs, 1, _mm256_div_pd(a1, b1));
    simd_store(lhs, 2, _mm256_div_pd(a2, b2));
    simd_store(lhs, 3, _mm256_div_pd(a3, b3));
    return lhs;
}

template<>
inline mat4<double> &operator+=(mat4<double> &lhs, const double scalar)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);
    const __m256d a3 = simd_load(lhs, 3);
    const __m256d b = _mm256_set1_pd(scalar);

    simd_store(lhs, 0, _mm256_add_pd(a0, b));
    simd_store(lhs, 1, _mm256_add_pd(a1, b));
    simd_store(lhs, 2, _mm256_add_pd(a2, b));
    simd_store(lhs, 3, _mm256_add_pd(a3, b));
    return lhs;
}

template<>
inline mat4<double> &operator-=(mat4<double> &lhs, const double scalar)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);
    const __m256d a3 = simd_load(lhs, 3);
    const __m256d b = _mm256_set1_pd(scalar);

    simd_store(lhs, 0, _mm256_sub_pd(a0, b));
    simd_store(lhs, 1, _mm256_sub_pd(a1, b));
    simd_store(lhs, 2, _mm256_sub_pd(a2, b));
    simd_store(lhs, 3, _mm256_sub_pd(a3, b));
    return lhs;
}

template<>
inline mat4<double> &operator*=(mat4<double> &lhs, const double scalar)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);
    const __m256d a3 = simd_load(lhs, 3);
    const __m256d b = _mm256_set1_pd(scalar);

    simd_store(lhs, 0, _mm256_mul_pd(a0, b));
    simd_store(lhs, 1, _mm256_mul_pd(a1, b));
    simd_store(lhs, 2, _mm256_mul_pd(a2, b));
    simd_store(lhs, 3, _mm256_mul_pd(a3, b));
    return lhs;
}

template<>
inline mat4<double> &operator/=(mat4<double> &lhs, const double scalar)
{
    const __m256d a0 = simd_load(lhs, 0);
    const __m256d a1 = simd_load(lhs, 1);
    const __m256d a2 = simd_load(lhs, 2);
    const __m256d a3 = simd_load(lhs, 3);
    const __m256d b = _mm256_set1_pd(scalar);

    simd_store(lhs, 0, _mm256_div_pd(a0, b));
    simd_store(lhs, 1, _mm256_div_pd(a1, b));
    simd_store(lhs, 2, _mm256_div_pd(a2, b));
    simd_store(lhs, 3, _mm256_div_pd(a3, b));
    return lhs;
}

/// ---- mat4 simd arithmetic operators ---------------------------------------
///
template<>
inline mat4<double> operator+(const double scalar, mat4<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);
    const __m256d b3 = simd_load(rhs, 3);

    simd_store(rhs, 0, _mm256_add_pd(a, b0));
    simd_store(rhs, 1, _mm256_add_pd(a, b1));
    simd_store(rhs, 2, _mm256_add_pd(a, b2));
    simd_store(rhs, 3, _mm256_add_pd(a, b3));
    return rhs;
}

template<>
inline mat4<double> operator-(const double scalar, mat4<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);
    const __m256d b3 = simd_load(rhs, 3);

    simd_store(rhs, 0, _mm256_sub_pd(a, b0));
    simd_store(rhs, 1, _mm256_sub_pd(a, b1));
    simd_store(rhs, 2, _mm256_sub_pd(a, b2));
    simd_store(rhs, 3, _mm256_sub_pd(a, b3));
    return rhs;
}

template<>
inline mat4<double> operator*(const double scalar, mat4<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);
    const __m256d b3 = simd_load(rhs, 3);

    simd_store(rhs, 0, _mm256_mul_pd(a, b0));
    simd_store(rhs, 1, _mm256_mul_pd(a, b1));
    simd_store(rhs, 2, _mm256_mul_pd(a, b2));
    simd_store(rhs, 3, _mm256_mul_pd(a, b3));
    return rhs;
}

template<>
inline mat4<double> operator/(const double scalar, mat4<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b0 = simd_load(rhs, 0);
    const __m256d b1 = simd_load(rhs, 1);
    const __m256d b2 = simd_load(rhs, 2);
    const __m256d b3 = simd_load(rhs, 3);

    simd_store(rhs, 0, _mm256_div_pd(a, b0));
    simd_store(rhs, 1, _mm256_div_pd(a, b1));
    simd_store(rhs, 2, _mm256_div_pd(a, b2));
    simd_store(rhs, 3, _mm256_div_pd(a, b3));
    return rhs;
}

} // math

#endif // MATH_SIMD_MATRIX_H_
