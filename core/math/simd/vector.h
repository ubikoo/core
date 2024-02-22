//
// vector.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MATH_SIMD_VECTOR_H_
#define MATH_SIMD_VECTOR_H_

#include "common.h"

namespace math {

/// ---- simd load/store functions --------------------------------------------
/// @brief Load 128-bits (2 packed double-precision 64-bit) from a vec2 array.
///
inline __m128d simd_load(const vec2<double> &v)
{
    return _mm_load_pd(v.data);
}

///
/// @brief Store 128-bits (2 packed double-precision 64-bit) into a vec2 array.
///
inline void simd_store(vec2<double> &v, const __m128d a)
{
    _mm_store_pd(v.data, a);
}

///
/// @brief Load 192-bits (3 packed double precision 64-bit) from a vec3 array.
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
///
/// If any of the bits of the mask is set to zero, the corresponding value from
/// the memory location is not loaded and the corresponding field of the
/// destination vector is set to zero.
///
inline __m256d simd_load(const vec3<double> &v)
{
    const __m256i mask = _mm256_set_epi64x(
        0x0,
        0xffffffffffffffff,
        0xffffffffffffffff,
        0xffffffffffffffff);
    return _mm256_maskload_pd(v.data, mask);
}

///
/// @brief Store 192-bits (3 packed double precision 64-bit) into a vec3 array.
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
/// the vector-a is not loaded, and the corresponding field of the destination
/// vector-v is left unchanged.
///
inline void simd_store(vec3<double> &v, const __m256d a)
{
    const __m256i mask = _mm256_set_epi64x(
        0x0,
        0xffffffffffffffff,
        0xffffffffffffffff,
        0xffffffffffffffff);
    _mm256_maskstore_pd(v.data, mask, a);
}

///
/// @brief Load 256-bits (4 packed double precision 64-bit) from a vec4 array.
///
inline __m256d simd_load(const vec4<double> &v)
{
    return _mm256_load_pd(v.data);
}

///
/// @brief Store 256-bits (4 packed double precision 64-bit) into a vec4 array.
///
inline void simd_store(vec4<double> &v, const __m256d a)
{
    _mm256_store_pd(v.data, a);
}

/// ---- vec2 simd assignment operators ---------------------------------------
///
template<>
inline vec2<double> &operator+=(vec2<double> &lhs, const vec2<double> &rhs)
{
    const __m128d a = simd_load(lhs);
    const __m128d b = simd_load(rhs);
    simd_store(lhs, _mm_add_pd(a, b));
    return lhs;
}

template<>
inline vec2<double> &operator-=(vec2<double> &lhs, const vec2<double> &rhs)
{
    const __m128d a = simd_load(lhs);
    const __m128d b = simd_load(rhs);
    simd_store(lhs, _mm_sub_pd(a, b));
    return lhs;
}

template<>
inline vec2<double> &operator*=(vec2<double> &lhs, const vec2<double> &rhs)
{
    const __m128d a = simd_load(lhs);
    const __m128d b = simd_load(rhs);
    simd_store(lhs, _mm_mul_pd(a, b));
    return lhs;
}

template<>
inline vec2<double> &operator/=(vec2<double> &lhs, const vec2<double> &rhs)
{
    const __m128d a = simd_load(lhs);
    const __m128d b = simd_load(rhs);
    simd_store(lhs, _mm_div_pd(a, b));
    return lhs;
}

template<>
inline vec2<double> &operator+=(vec2<double> &lhs, const double scalar)
{
    const __m128d a = simd_load(lhs);
    const __m128d b = _mm_set1_pd(scalar);
    simd_store(lhs, _mm_add_pd(a, b));
    return lhs;
}

template<>
inline vec2<double> &operator-=(vec2<double> &lhs, const double scalar)
{
    const __m128d a = simd_load(lhs);
    const __m128d b = _mm_set1_pd(scalar);
    simd_store(lhs, _mm_sub_pd(a, b));
    return lhs;
}

template<>
inline vec2<double> &operator*=(vec2<double> &lhs, const double scalar)
{
    const __m128d a = simd_load(lhs);
    const __m128d b = _mm_set1_pd(scalar);
    simd_store(lhs, _mm_mul_pd(a, b));
    return lhs;
}

template<>
inline vec2<double> &operator/=(vec2<double> &lhs, const double scalar)
{
    const __m128d a = simd_load(lhs);
    const __m128d b = _mm_set1_pd(scalar);
    simd_store(lhs, _mm_div_pd(a, b));
    return lhs;
}

/// ---- vec2 simd arithmetic operators ---------------------------------------
///
template<>
inline vec2<double> operator+(const double scalar, vec2<double> rhs)
{
    const __m128d a = _mm_set1_pd(scalar);
    const __m128d b = simd_load(rhs);
    simd_store(rhs, _mm_add_pd(a, b));
    return rhs;
}

template<>
inline vec2<double> operator-(const double scalar, vec2<double> rhs)
{
    const __m128d a = _mm_set1_pd(scalar);
    const __m128d b = simd_load(rhs);
    simd_store(rhs, _mm_sub_pd(a, b));
    return rhs;
}

template<>
inline vec2<double> operator*(const double scalar, vec2<double> rhs)
{
    const __m128d a = _mm_set1_pd(scalar);
    const __m128d b = simd_load(rhs);
    simd_store(rhs, _mm_mul_pd(a, b));
    return rhs;
}

template<>
inline vec2<double> operator/(const double scalar, vec2<double> rhs)
{
    const __m128d a = _mm_set1_pd(scalar);
    const __m128d b = simd_load(rhs);
    simd_store(rhs, _mm_div_pd(a, b));
    return rhs;
}

/// ---- vec3 simd assignment operators ---------------------------------------
///
template<>
inline vec3<double> &operator+=(vec3<double> &lhs, const vec3<double> &rhs)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = simd_load(rhs);
    simd_store(lhs, _mm256_add_pd(a, b));
    return lhs;
}

template<>
inline vec3<double> &operator-=(vec3<double> &lhs, const vec3<double> &rhs)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = simd_load(rhs);
    simd_store(lhs, _mm256_sub_pd(a, b));
    return lhs;
}

template<>
inline vec3<double> &operator*=(vec3<double> &lhs, const vec3<double> &rhs)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = simd_load(rhs);
    simd_store(lhs, _mm256_mul_pd(a, b));
    return lhs;
}

template<>
inline vec3<double> &operator/=(vec3<double> &lhs, const vec3<double> &rhs)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = simd_load(rhs);
    simd_store(lhs, _mm256_div_pd(a, b));
    return lhs;
}

template<>
inline vec3<double> &operator+=(vec3<double> &lhs, const double scalar)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = _mm256_set1_pd(scalar);
    simd_store(lhs, _mm256_add_pd(a, b));
    return lhs;
}

template<>
inline vec3<double> &operator-=(vec3<double> &lhs, const double scalar)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = _mm256_set1_pd(scalar);
    simd_store(lhs, _mm256_sub_pd(a, b));
    return lhs;
}

template<>
inline vec3<double> &operator*=(vec3<double> &lhs, const double scalar)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = _mm256_set1_pd(scalar);
    simd_store(lhs, _mm256_mul_pd(a, b));
    return lhs;
}

template<>
inline vec3<double> &operator/=(vec3<double> &lhs, const double scalar)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = _mm256_set1_pd(scalar);
    simd_store(lhs, _mm256_div_pd(a, b));
    return lhs;
}

/// ---- vec3 simd arithmetic operators ---------------------------------------
///
template<>
inline vec3<double> operator+(const double scalar, vec3<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b = simd_load(rhs);
    simd_store(rhs, _mm256_add_pd(a, b));
    return rhs;
}

template<>
inline vec3<double> operator-(const double scalar, vec3<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b = simd_load(rhs);
    simd_store(rhs, _mm256_sub_pd(a, b));
    return rhs;
}

template<>
inline vec3<double> operator*(const double scalar, vec3<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b = simd_load(rhs);
    simd_store(rhs, _mm256_mul_pd(a, b));
    return rhs;
}

template<>
inline vec3<double> operator/(const double scalar, vec3<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b = simd_load(rhs);
    simd_store(rhs, _mm256_div_pd(a, b));
    return rhs;
}

/// ---- vec4 simd assignment operators ---------------------------------------
///
template<>
inline vec4<double> &operator+=(vec4<double> &lhs, const vec4<double> &rhs)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = simd_load(rhs);
    simd_store(lhs, _mm256_add_pd(a, b));
    return lhs;
}

template<>
inline vec4<double> &operator-=(vec4<double> &lhs, const vec4<double> &rhs)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = simd_load(rhs);
    simd_store(lhs, _mm256_sub_pd(a, b));
    return lhs;
}

template<>
inline vec4<double> &operator*=(vec4<double> &lhs, const vec4<double> &rhs)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = simd_load(rhs);
    simd_store(lhs, _mm256_mul_pd(a, b));
    return lhs;
}

template<>
inline vec4<double> &operator/=(vec4<double> &lhs, const vec4<double> &rhs)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = simd_load(rhs);
    simd_store(lhs, _mm256_div_pd(a, b));
    return lhs;
}

template<>
inline vec4<double> &operator+=(vec4<double> &lhs, const double scalar)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = _mm256_set1_pd(scalar);
    simd_store(lhs, _mm256_add_pd(a, b));
    return lhs;
}

template<>
inline vec4<double> &operator-=(vec4<double> &lhs, const double scalar)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = _mm256_set1_pd(scalar);
    simd_store(lhs, _mm256_sub_pd(a, b));
    return lhs;
}

template<>
inline vec4<double> &operator*=(vec4<double> &lhs, const double scalar)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = _mm256_set1_pd(scalar);
    simd_store(lhs, _mm256_mul_pd(a, b));
    return lhs;
}

template<>
inline vec4<double> &operator/=(vec4<double> &lhs, const double scalar)
{
    const __m256d a = simd_load(lhs);
    const __m256d b = _mm256_set1_pd(scalar);
    simd_store(lhs, _mm256_div_pd(a, b));
    return lhs;
}

/// ---- vec4 simd arithmetic operators ---------------------------------------
///
template<>
inline vec4<double> operator+(const double scalar, vec4<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b = simd_load(rhs);
    simd_store(rhs, _mm256_add_pd(a, b));
    return rhs;
}

template<>
inline vec4<double> operator-(const double scalar, vec4<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b = simd_load(rhs);
    simd_store(rhs, _mm256_sub_pd(a, b));
    return rhs;
}

template<>
inline vec4<double> operator*(const double scalar, vec4<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b = simd_load(rhs);
    simd_store(rhs, _mm256_mul_pd(a, b));
    return rhs;
}

template<>
inline vec4<double> operator/(const double scalar, vec4<double> rhs)
{
    const __m256d a = _mm256_set1_pd(scalar);
    const __m256d b = simd_load(rhs);
    simd_store(rhs, _mm256_div_pd(a, b));
    return rhs;
}

} // math

#endif // MATH_SIMD_VECTOR_H_
