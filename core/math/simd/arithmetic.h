//
// arithmetic.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MATH_SIMD_ARITHMETIC_H_
#define MATH_SIMD_ARITHMETIC_H_

#include "common.h"

namespace math {

/// -----------------------------------------------------------------------------
/// @brief Find the nearest integer to the vector elements.
/// @fn dst = __m128d _mm_round_pd(__m128d src, int rounding)
/// Round the packed double-precision (64-bit) floating-point elements using the
/// rounding[3:0] parameter:
///  round to nearest, and suppress exceptions
///      (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC)
///  round down, and suppress exceptions
///      (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)
///  round up, and suppress exceptions
///      (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)
///  truncate, and suppress exceptions
///      (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)
///
template<>
inline vec2<double> round(const vec2<double> &u)
{
    static constexpr int rounding = _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;
    vec2<double> result{};
    simd_store(result, _mm_round_pd(simd_load(u), rounding));
    return result;
}

template<>
inline vec3<double> round(const vec3<double> &u)
{
    static constexpr int rounding = _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;
    vec3<double> result{};
    simd_store(result, _mm256_round_pd(simd_load(u), rounding));
    return result;
}

template<>
inline vec4<double> round(const vec4<double> &u)
{
    static constexpr int rounding = _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC;
    vec4<double> result{};
    simd_store(result, _mm256_round_pd(simd_load(u), rounding));
    return result;
}

/// -----------------------------------------------------------------------------
/// @brief Find the nearest integer less than or equal to the vector elements.
///
template<>
inline vec2<double> floor(const vec2<double> &u)
{
    vec2<double> result{};
    simd_store(result, _mm_floor_pd(simd_load(u)));
    return result;
}

template<>
inline vec3<double> floor(const vec3<double> &u)
{
    vec3<double> result{};
    simd_store(result, _mm256_floor_pd(simd_load(u)));
    return result;
}

template<>
inline vec4<double> floor(const vec4<double> &u)
{
    vec4<double> result{};
    simd_store(result, _mm256_floor_pd(simd_load(u)));
    return result;
}

/// -----------------------------------------------------------------------------
/// @brief Find the nearest integer greater than or equal to the vector elements.
///
template<>
inline vec2<double> ceil(const vec2<double> &u)
{
    vec2<double> result{};
    simd_store(result, _mm_ceil_pd(simd_load(u)));
    return result;
}

template<>
inline vec3<double> ceil(const vec3<double> &u)
{
    vec3<double> result{};
    simd_store(result, _mm256_ceil_pd(simd_load(u)));
    return result;
}

template<>
inline vec4<double> ceil(const vec4<double> &u)
{
    vec4<double> result{};
    simd_store(result, _mm256_ceil_pd(simd_load(u)));
    return result;
}

/// -----------------------------------------------------------------------------
/// @brief Return modulo operation (u mod v).
///
template<>
inline vec2<double> mod(const vec2<double> &u, const vec2<double> &v)
{
    __m128d a = simd_load(u);
    __m128d b = simd_load(v);
    __m128d c = _mm_mul_pd(b, _mm_floor_pd(_mm_div_pd(a, b)));
    __m128d d = _mm_sub_pd(a, c);

    vec2<double> result{};
    simd_store(result, d);
    return result;
}

template<>
inline vec3<double> mod(const vec3<double> &u, const vec3<double> &v)
{
    __m256d a = simd_load(u);
    __m256d b = simd_load(v);
    __m256d c = _mm256_mul_pd(b, _mm256_floor_pd(_mm256_div_pd(a, b)));
    __m256d d = _mm256_sub_pd(a, c);

    vec3<double> result{};
    simd_store(result, d);
    return result;
}

template<>
inline vec4<double> mod(const vec4<double> &u, const vec4<double> &v)
{
    __m256d a = simd_load(u);
    __m256d b = simd_load(v);
    __m256d c = _mm256_mul_pd(b, _mm256_floor_pd(_mm256_div_pd(a, b)));
    __m256d d = _mm256_sub_pd(a, c);

    vec4<double> result{};
    simd_store(result, d);
    return result;
}

/// -----------------------------------------------------------------------------
/// @brief Return the Dirac nascent function, defined as discrete pulse with
/// width e. The corresponding delta function is the asymptotic value when e->0.
///
template<>
inline vec2<double> dirac(const double e, const vec2<double> &u)
{
    const __m128d zero = _mm_set1_pd(0.0);
    const __m128d one = _mm_set1_pd(1.0);

    __m128d eps = _mm_set1_pd(e);
    __m128d a = simd_load(u);
    __m128d abs_a = _mm_max_pd(_mm_sub_pd(zero, a), a);

    __m128d is_positive = _mm_cmp_pd(eps, zero, 14);    // _CMP_GT_OS
    __m128d is_inside = _mm_cmp_pd(abs_a, eps, 1);     // _CMP_LT_OS
    __m128d is_valid = _mm_and_pd(is_positive, is_inside);

    __m128d val = _mm_and_pd(is_valid, _mm_div_pd(one, eps));

    vec2<double> result{};
    simd_store(result, val);
    return result;
}

template<>
inline vec3<double> dirac(const double e, const vec3<double> &u)
{
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d one = _mm256_set1_pd(1.0);

    __m256d eps = _mm256_set1_pd(e);
    __m256d a = simd_load(u);
    __m256d abs_a = _mm256_max_pd(_mm256_sub_pd(zero, a), a);
    __m256d abs_a2 = _mm256_mul_pd(one, abs_a);

    __m256d is_positive = _mm256_cmp_pd(eps, zero, 14);    // _CMP_GT_OS
    __m256d is_inside = _mm256_cmp_pd(abs_a2, eps, 1);     // _CMP_LT_OS
    __m256d is_valid = _mm256_and_pd(is_positive, is_inside);

    __m256d val = _mm256_and_pd(is_valid, _mm256_div_pd(one, eps));

    vec3<double> result{};
    simd_store(result, val);
    return result;
}

template<>
inline vec4<double> dirac(const double e, const vec4<double> &u)
{
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d one = _mm256_set1_pd(1.0);

    __m256d eps = _mm256_set1_pd(e);
    __m256d a = simd_load(u);
    __m256d abs_a = _mm256_max_pd(_mm256_sub_pd(zero, a), a);
    __m256d abs_a2 = _mm256_mul_pd(one, abs_a);

    __m256d is_positive = _mm256_cmp_pd(eps, zero, 14);    // _CMP_GT_OS
    __m256d is_inside = _mm256_cmp_pd(abs_a2, eps, 1);     // _CMP_LT_OS
    __m256d is_valid = _mm256_and_pd(is_positive, is_inside);

    __m256d val = _mm256_and_pd(is_valid, _mm256_div_pd(one, eps));

    vec4<double> result{};
    simd_store(result, val);
    return result;
}

/// -----------------------------------------------------------------------------
/// @brief Heaviside step function.
///
template<>
inline vec2<double> step(const vec2<double> &u)
{
    const __m128d zero = _mm_set1_pd(0.0);
    const __m128d one = _mm_set1_pd(1.0);

    __m128d a = simd_load(u);
    __m128d is_greater = _mm_cmp_pd(a, zero, 14);       // _CMP_GT_OS
    __m128d val = _mm_and_pd(is_greater, one);

    vec2<double> result{};
    simd_store(result, val);
    return result;
}

template<>
inline vec3<double> step(const vec3<double> &u)
{
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d one = _mm256_set1_pd(1.0);

    __m256d a = simd_load(u);
    __m256d is_greater = _mm256_cmp_pd(a, zero, 14);    // _CMP_GT_OS
    __m256d val = _mm256_and_pd(is_greater, one);

    vec3<double> result{};
    simd_store(result, val);
    return result;
}

template<>
inline vec4<double> step(const vec4<double> &u)
{
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d one = _mm256_set1_pd(1.0);

    __m256d a = simd_load(u);
    __m256d is_greater = _mm256_cmp_pd(a, zero, 14);    // _CMP_GT_OS
    __m256d val = _mm256_and_pd(is_greater, one);

    vec4<double> result{};
    simd_store(result, val);
    return result;
}

/// -----------------------------------------------------------------------------
/// @brief Smooth step between lo and hi.
///
template<>
inline vec2<double> smoothstep(
    const vec2<double> &lo,
    const vec2<double> &hi,
    const vec2<double> &u)
{
    const __m128d zero  = _mm_set1_pd(0.0);
    const __m128d one   = _mm_set1_pd(1.0);
    const __m128d two   = _mm_set1_pd(2.0);
    const __m128d three = _mm_set1_pd(3.0);

    __m128d a_lo = simd_load(lo);
    __m128d a_hi = simd_load(hi);
    __m128d a = simd_load(u);

    __m128d numer = _mm_sub_pd(a, a_lo);
    __m128d denom = _mm_sub_pd(a_hi, a_lo);

    __m128d x = _mm_div_pd(numer, denom);
    x = _mm_min_pd(_mm_max_pd(x, zero), one);
    __m128d x2 = _mm_mul_pd(x, x);
    __m128d x3 = _mm_sub_pd(three, _mm_mul_pd(two, x));

    __m128d val = _mm_mul_pd(x2, x3);

    vec2<double> result{};
    simd_store(result, val);
    return result;
}

template<>
inline vec3<double> smoothstep(
    const vec3<double> &lo,
    const vec3<double> &hi,
    const vec3<double> &u)
{
    const __m256d zero  = _mm256_set1_pd(0.0);
    const __m256d one   = _mm256_set1_pd(1.0);
    const __m256d two   = _mm256_set1_pd(2.0);
    const __m256d three = _mm256_set1_pd(3.0);

    __m256d a_lo = simd_load(lo);
    __m256d a_hi = simd_load(hi);
    __m256d a = simd_load(u);

    __m256d numer = _mm256_sub_pd(a, a_lo);
    __m256d denom = _mm256_sub_pd(a_hi, a_lo);

    __m256d x = _mm256_div_pd(numer, denom);
    x = _mm256_min_pd(_mm256_max_pd(x, zero), one);
    __m256d x2 = _mm256_mul_pd(x, x);
    __m256d x3 = _mm256_sub_pd(three, _mm256_mul_pd(two, x));

    __m256d val = _mm256_mul_pd(x2, x3);

    vec3<double> result{};
    simd_store(result, val);
    return result;
}

template<>
inline vec4<double> smoothstep(
    const vec4<double> &lo,
    const vec4<double> &hi,
    const vec4<double> &u)
{
    const __m256d zero  = _mm256_set1_pd(0.0);
    const __m256d one   = _mm256_set1_pd(1.0);
    const __m256d two   = _mm256_set1_pd(2.0);
    const __m256d three = _mm256_set1_pd(3.0);

    __m256d a_lo = simd_load(lo);
    __m256d a_hi = simd_load(hi);
    __m256d a = simd_load(u);

    __m256d numer = _mm256_sub_pd(a, a_lo);
    __m256d denom = _mm256_sub_pd(a_hi, a_lo);

    __m256d x = _mm256_div_pd(numer, denom);
    x = _mm256_min_pd(_mm256_max_pd(x, zero), one);
    __m256d x2 = _mm256_mul_pd(x, x);
    __m256d x3 = _mm256_sub_pd(three, _mm256_mul_pd(two, x));

    __m256d val = _mm256_mul_pd(x2, x3);

    vec4<double> result{};
    simd_store(result, val);
    return result;
}

/// -----------------------------------------------------------------------------
/// @brief Linear interpolation between lo and hi.
///
template<>
inline vec2<double> lerp(
    const vec2<double> &lo,
    const vec2<double> &hi,
    const vec2<double> &u)
{
    const __m128d one = _mm_set1_pd(1.0);
    __m128d a_lo = simd_load(lo);
    __m128d a_hi = simd_load(hi);
    __m128d a = simd_load(u);

    __m128d x_lo = _mm_mul_pd(a_lo, _mm_sub_pd(one, a));
    __m128d x_hi = _mm_mul_pd(a_hi, a);
    __m128d val = _mm_add_pd(x_lo, x_hi);

    vec2<double> result{};
    simd_store(result, val);
    return result;
}

template<>
inline vec3<double> lerp(
    const vec3<double> &lo,
    const vec3<double> &hi,
    const vec3<double> &u)
{
    const __m256d one = _mm256_set1_pd(1.0);
    __m256d a_lo = simd_load(lo);
    __m256d a_hi = simd_load(hi);
    __m256d a = simd_load(u);

    __m256d x_lo = _mm256_mul_pd(a_lo, _mm256_sub_pd(one, a));
    __m256d x_hi = _mm256_mul_pd(a_hi, a);
    __m256d val = _mm256_add_pd(x_lo, x_hi);

    vec3<double> result{};
    simd_store(result, val);
    return result;
}

template<>
inline vec4<double> lerp(
    const vec4<double> &lo,
    const vec4<double> &hi,
    const vec4<double> &u)
{
    const __m256d one = _mm256_set1_pd(1.0);
    __m256d a_lo = simd_load(lo);
    __m256d a_hi = simd_load(hi);
    __m256d a = simd_load(u);

    __m256d x_lo = _mm256_mul_pd(a_lo, _mm256_sub_pd(one, a));
    __m256d x_hi = _mm256_mul_pd(a_hi, a);
    __m256d val = _mm256_add_pd(x_lo, x_hi);

    vec4<double> result{};
    simd_store(result, val);
    return result;
}

/// -----------------------------------------------------------------------------
/// @brief Convert degrees to radians.
///
template<>
inline vec2<double> radians(const vec2<double> &u)
{
    const __m128d deg_to_rad = _mm_set1_pd(M_PI / 180);
    __m128d a = simd_load(u);
    __m128d val = _mm_mul_pd(a, deg_to_rad);

    vec2<double> result{};
    simd_store(result, val);
    return result;
}

template<>
inline vec3<double> radians(const vec3<double> &u)
{
    const __m256d deg_to_rad = _mm256_set1_pd(M_PI / 180);
    __m256d a = simd_load(u);
    __m256d val = _mm256_mul_pd(a, deg_to_rad);

    vec3<double> result{};
    simd_store(result, val);
    return result;
}

template<>
inline vec4<double> radians(const vec4<double> &u)
{
    const __m256d deg_to_rad = _mm256_set1_pd(M_PI / 180);
    __m256d a = simd_load(u);
    __m256d val = _mm256_mul_pd(a, deg_to_rad);

    vec4<double> result{};
    simd_store(result, val);
    return result;
}

///
/// @brief Convert radians to degrees.
///
template<>
inline vec2<double> degrees(const vec2<double> &u)
{
    const __m128d rad_to_deg = _mm_set1_pd(180 / M_PI);
    __m128d a = simd_load(u);
    __m128d val = _mm_mul_pd(a, rad_to_deg);

    vec2<double> result{};
    simd_store(result, val);
    return result;
}

template<>
inline vec3<double> degrees(const vec3<double> &u)
{
    const __m256d rad_to_deg = _mm256_set1_pd(180 / M_PI);
    __m256d a = simd_load(u);
    __m256d val = _mm256_mul_pd(a, rad_to_deg);

    vec3<double> result{};
    simd_store(result, val);
    return result;
}

template<>
inline vec4<double> degrees(const vec4<double> &u)
{
    const __m256d rad_to_deg = _mm256_set1_pd(180 / M_PI);
    __m256d a = simd_load(u);
    __m256d val = _mm256_mul_pd(a, rad_to_deg);

    vec4<double> result{};
    simd_store(result, val);
    return result;
}

/// -----------------------------------------------------------------------------
/// @brief Return the signum function of u.
///
template<>
inline vec2<double> sign(const vec2<double> &u)
{
    const __m128d zero = _mm_set1_pd(0.0);
    const __m128d plus_one = _mm_set1_pd(1.0);
    const __m128d minus_one = _mm_set1_pd(-1.0);

    __m128d a = simd_load(u);
    __m128d is_positive = _mm_and_pd(_mm_cmpgt_pd(a, zero), plus_one);
    __m128d is_negative = _mm_and_pd(_mm_cmplt_pd(a, zero), minus_one);
    __m128d sign_a = _mm_or_pd(is_positive, is_negative);

    vec2<double> result{};
    simd_store(result, sign_a);
    return result;
}

template<>
inline vec3<double> sign(const vec3<double> &u)
{
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d plus_one = _mm256_set1_pd(1.0);
    const __m256d minus_one = _mm256_set1_pd(-1.0);

    __m256d a = simd_load(u);
    __m256d is_positive = _mm256_and_pd(_mm256_cmpgt_pd(a, zero), plus_one);
    __m256d is_negative = _mm256_and_pd(_mm256_cmplt_pd(a, zero), minus_one);
    __m256d sign_a = _mm256_or_pd(is_positive, is_negative);

    vec3<double> result{};
    simd_store(result, sign_a);
    return result;
}

template<>
inline vec4<double> sign(const vec4<double> &u)
{
    const __m256d zero = _mm256_set1_pd(0.0);
    const __m256d plus_one = _mm256_set1_pd(1.0);
    const __m256d minus_one = _mm256_set1_pd(-1.0);

    __m256d a = simd_load(u);
    __m256d is_positive = _mm256_and_pd(_mm256_cmpgt_pd(a, zero), plus_one);
    __m256d is_negative = _mm256_and_pd(_mm256_cmplt_pd(a, zero), minus_one);
    __m256d sign_a = _mm256_or_pd(is_positive, is_negative);

    vec4<double> result{};
    simd_store(result, sign_a);
    return result;
}

/// -----------------------------------------------------------------------------
/// @brief Return the absolute value of the vector elements.
/// |a| = max((0-a), a) = max(-a,a)
///
template<>
inline vec2<double> abs(const vec2<double> &u)
{
    const __m128d zero = _mm_set1_pd(0.0);

    __m128d a = simd_load(u);
    __m128d abs_a = _mm_max_pd(_mm_sub_pd(zero, a), a);

    vec2<double> result{};
    simd_store(result, abs_a);
    return result;
}

template<>
inline vec3<double> abs(const vec3<double> &u)
{
    const __m256d zero = _mm256_set1_pd(0.0);

    __m256d a = simd_load(u);
    __m256d abs_a = _mm256_max_pd(_mm256_sub_pd(zero, a), a);

    vec3<double> result{};
    simd_store(result, abs_a);
    return result;
}

template<>
inline vec4<double> abs(const vec4<double> &u)
{
    const __m256d zero = _mm256_set1_pd(0.0);

    __m256d a = simd_load(u);
    __m256d abs_a = _mm256_max_pd(_mm256_sub_pd(zero, a), a);

    vec4<double> result{};
    simd_store(result, abs_a);
    return result;
}

/// -----------------------------------------------------------------------------
/// @brief Return the min between u and v.
///
template<>
inline vec2<double> min(const vec2<double> &u, const vec2<double> &v)
{
    vec2<double> result{};
    simd_store(result, _mm_min_pd(simd_load(u), simd_load(v)));
    return result;
}

template<>
inline vec3<double> min(const vec3<double> &u, const vec3<double> &v)
{
    vec3<double> result{};
    simd_store(result, _mm256_min_pd(simd_load(u), simd_load(v)));
    return result;
}

template<>
inline vec4<double> min(const vec4<double> &u, const vec4<double> &v)
{
    vec4<double> result{};
    simd_store(result, _mm256_min_pd(simd_load(u), simd_load(v)));
    return result;
}

/// -----------------------------------------------------------------------------
/// @brief Return the max between u and v.
///
template<>
inline vec2<double> max(const vec2<double> &u, const vec2<double> &v)
{
    vec2<double> result{};
    simd_store(result, _mm_max_pd(simd_load(u), simd_load(v)));
    return result;
}

template<>
inline vec3<double> max(const vec3<double> &u, const vec3<double> &v)
{
    vec3<double> result{};
    simd_store(result, _mm256_max_pd(simd_load(u), simd_load(v)));
    return result;
}

template<>
inline vec4<double> max(const vec4<double> &u, const vec4<double> &v)
{
    vec4<double> result{};
    simd_store(result, _mm256_max_pd(simd_load(u), simd_load(v)));
    return result;
}



/// -----------------------------------------------------------------------------
/// @brief Clamp u between lo and hi.
///
template<>
inline vec2<double> clamp(
    const vec2<double> &u,
    const vec2<double> &lo,
    const vec2<double> &hi)
{
    __m128d a = simd_load(u);
    __m128d a_lo = simd_load(lo);
    __m128d a_hi = simd_load(hi);

    vec2<double> result{};
    simd_store(result, _mm_min_pd(_mm_max_pd(a, a_lo), a_hi));
    return result;
}
template<>
inline vec3<double> clamp(
    const vec3<double> &u,
    const vec3<double> &lo,
    const vec3<double> &hi)
{
    __m256d a = simd_load(u);
    __m256d a_lo = simd_load(lo);
    __m256d a_hi = simd_load(hi);

    vec3<double> result{};
    simd_store(result, _mm256_min_pd(_mm256_max_pd(a, a_lo), a_hi));
    return result;
}
template<>
inline vec4<double> clamp(
    const vec4<double> &u,
    const vec4<double> &lo,
    const vec4<double> &hi)
{
    __m256d a = simd_load(u);
    __m256d a_lo = simd_load(lo);
    __m256d a_hi = simd_load(hi);

    vec4<double> result{};
    simd_store(result, _mm256_min_pd(_mm256_max_pd(a, a_lo), a_hi));
    return result;
}

} // math

#endif // MATH_SIMD_ARITHMETIC_H_
