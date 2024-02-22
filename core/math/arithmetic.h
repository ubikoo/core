//
// arithmetic.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MATH_ARITHMETIC_H_
#define MATH_ARITHMETIC_H_

#include <cmath>
#include <type_traits>
#include <algorithm>
#include "vector.h"

namespace math {

/// ---- Function declarations ------------------------------------------------
/// @brief Floating point functions.
///
template<typename T> inline bool iseq(const T u, const T v);
template<typename T> inline bool islt(const T u, const T v);
template<typename T> inline bool isgt(const T u, const T v);

template<typename T> inline T round(const T u);
template<typename T> inline vec2<T> round(const vec2<T> &u);
template<typename T> inline vec3<T> round(const vec3<T> &u);
template<typename T> inline vec4<T> round(const vec4<T> &u);

template<typename T> inline T floor(const T u);
template<typename T> inline vec2<T> floor(const vec2<T> &u);
template<typename T> inline vec3<T> floor(const vec3<T> &u);
template<typename T> inline vec4<T> floor(const vec4<T> &u);

template<typename T> inline T ceil(const T u);
template<typename T> inline vec2<T> ceil(const vec2<T> &u);
template<typename T> inline vec3<T> ceil(const vec3<T> &u);
template<typename T> inline vec4<T> ceil(const vec4<T> &u);

template<typename T> inline T mod(const T u, const T v);
template<typename T> inline vec2<T> mod(const vec2<T> &u, const vec2<T> &v);
template<typename T> inline vec3<T> mod(const vec3<T> &u, const vec3<T> &v);
template<typename T> inline vec4<T> mod(const vec4<T> &u, const vec4<T> &v);

template<typename T> inline T dirac(const T e, const T u);
template<typename T> inline vec2<T> dirac(const T e, const vec2<T> &u);
template<typename T> inline vec3<T> dirac(const T e, const vec3<T> &u);
template<typename T> inline vec4<T> dirac(const T e, const vec4<T> &u);

template<typename T> inline T step(const T u);
template<typename T> inline vec2<T> step(const vec2<T> &u);
template<typename T> inline vec3<T> step(const vec3<T> &u);
template<typename T> inline vec4<T> step(const vec4<T> &u);

template<typename T> inline T smoothstep(const T lo, const T hi, const T u);
template<typename T> inline vec2<T> smoothstep(
    const vec2<T> &lo, const vec2<T> &hi, const vec2<T> &u);
template<typename T> inline vec3<T> smoothstep(
    const vec3<T> &lo, const vec3<T> &hi, const vec3<T> &u);
template<typename T> inline vec4<T> smoothstep(
    const vec4<T> &lo, const vec4<T> &hi, const vec4<T> &u);

template<typename T> inline T lerp(const T lo, const T hi,  T u);
template<typename T> inline vec2<T> lerp(
    const vec2<T> &lo, const vec2<T> &hi, const T u);
template<typename T> inline vec3<T> lerp(
    const vec3<T> &lo, const vec3<T> &hi, const T u);
template<typename T> inline vec4<T> lerp(
    const vec4<T> &lo, const vec4<T> &hi, const T u);

template<typename T> inline T radians(const T u);
template<typename T> inline vec2<T> radians(const vec2<T> &u);
template<typename T> inline vec3<T> radians(const vec3<T> &u);
template<typename T> inline vec4<T> radians(const vec4<T> &u);

template<typename T> inline T degrees(const T u);
template<typename T> inline vec2<T> degrees(const vec2<T> &u);
template<typename T> inline vec3<T> degrees(const vec3<T> &u);
template<typename T> inline vec4<T> degrees(const vec4<T> &u);

///
/// @brief Arithmetic functions.
///
template<typename T> inline void swap(T &u, T &v);
template<typename T> inline void swap(vec2<T> &u, vec2<T> &v);
template<typename T> inline void swap(vec3<T> &u, vec3<T> &v);
template<typename T> inline void swap(vec4<T> &u, vec4<T> &v);

template<typename T> inline T sign(const T u);
template<typename T> inline vec2<T> sign(const vec2<T> &u);
template<typename T> inline vec3<T> sign(const vec3<T> &u);
template<typename T> inline vec4<T> sign(const vec4<T> &u);

template<typename T> inline T abs(const T u);
template<typename T> inline vec2<T> abs(const vec2<T> &u);
template<typename T> inline vec3<T> abs(const vec3<T> &u);
template<typename T> inline vec4<T> abs(const vec4<T> &u);

template<typename T> inline T min(const T u, const T v);
template<typename T> inline vec2<T> min(const vec2<T> &u, const vec2<T> &v);
template<typename T> inline vec3<T> min(const vec3<T> &u, const vec3<T> &v);
template<typename T> inline vec4<T> min(const vec4<T> &u, const vec4<T> &v);

template<typename T> inline T max(const T u, const T v);
template<typename T> inline vec2<T> max(const vec2<T> &u, const vec2<T> &v);
template<typename T> inline vec3<T> max(const vec3<T> &u, const vec3<T> &v);
template<typename T> inline vec4<T> max(const vec4<T> &u, const vec4<T> &v);

template<typename T> inline T clamp(const T u, const T lo, const T hi);
template<typename T> inline vec2<T> clamp(
    const vec2<T> &u, const vec2<T> &lo, const vec2<T> &hi);
template<typename T> inline vec3<T> clamp(
    const vec3<T> &u, const vec3<T> &lo, const vec3<T> &hi);
template<typename T> inline vec4<T> clamp(
    const vec4<T> &u, const vec4<T> &lo, const vec4<T> &hi);

/// ---- Floating point functions ---------------------------------------------
/// @brief Is u approx equal to v? (u ~ v), iif |u-v| <= eps * min(|u|,|v|)
/// If (u = 0) identically, then the relation above will be false even if
/// (v <= eps). The edge case is handled by ensuring that maxerr is never zero
/// identically (i.e. maxerr < eps), thus 0 < maxerr <= eps * min(|u|,|v|).
///
template<typename T>
inline bool iseq(const T u, const T v)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    static const T epsilon = std::sqrt(std::numeric_limits<T>::epsilon());

    T abs_u = std::fabs(u);
    T abs_v = std::fabs(v);
    T maxerr = abs_u < abs_v ? abs_u : abs_v;
    maxerr = maxerr < epsilon ? epsilon : epsilon * maxerr;

    return std::fabs(u - v) <= maxerr;
}

///
/// @brief Is u approx less than v? (u < v), iif (v-u) > eps * max(|u|,|v|).
///
template<typename T>
inline bool islt(const T u, const T v)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    static const T epsilon = std::sqrt(std::numeric_limits<T>::epsilon());
    T abs_u = std::fabs(u);
    T abs_v = std::fabs(v);
    return (v - u) > (epsilon * (abs_u > abs_v ? abs_u : abs_v));
}

///
/// @brief Is u approx greater than v? (u > v), iif (u-v) > eps * max(|u|,|v|).
///
template<typename T>
inline bool isgt(const T u, const T v)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    static const T epsilon = std::sqrt(std::numeric_limits<T>::epsilon());
    T abs_u = std::fabs(u);
    T abs_v = std::fabs(v);
    return (u - v) > (epsilon * (abs_u > abs_v ? abs_u : abs_v));
}

/// -----------------------------------------------------------------------------
/// @brief Return the nearest integer to u (floating point).
///
template<typename T>
inline T round(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return std::round(u);
}

template<typename T>
inline vec2<T> round(const vec2<T> &u)
{
    return {round(u.x), round(u.y)};
}

template<typename T>
inline vec3<T> round(const vec3<T> &u)
{
    return {round(u.x), round(u.y), round(u.z)};
}

template<typename T>
inline vec4<T> round(const vec4<T> &u)
{
    return {round(u.x), round(u.y), round(u.z), round(u.w)};
}

/// -----------------------------------------------------------------------------
/// @brief Return the largest integer not greater than u (floating point).
///
template<typename T>
inline T floor(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return std::floor(u);
}

template<typename T>
inline vec2<T> floor(const vec2<T> &u)
{
    return {floor(u.x), floor(u.y)};
}

template<typename T>
inline vec3<T> floor(const vec3<T> &u)
{
    return {floor(u.x), floor(u.y), floor(u.z)};
}

template<typename T>
inline vec4<T> floor(const vec4<T> &u)
{
    return {floor(u.x), floor(u.y), floor(u.z), floor(u.w)};
}

/// -----------------------------------------------------------------------------
/// @brief Return the smallest integer not smaller than u (floating point).
///
template<typename T>
inline T ceil(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return std::ceil(u);
}

template<typename T>
inline vec2<T> ceil(const vec2<T> &u)
{
    return {ceil(u.x), ceil(u.y)};
}

template<typename T>
inline vec3<T> ceil(const vec3<T> &u)
{
    return {ceil(u.x), ceil(u.y), ceil(u.z)};
}

template<typename T>
inline vec4<T> ceil(const vec4<T> &u)
{
    return {ceil(u.x), ceil(u.y), ceil(u.z), ceil(u.w)};
}

/// -----------------------------------------------------------------------------
/// @brief Return modulo operation (u mod v).
///
template<typename T>
inline T mod(const T u, const T v)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return (iseq(v, static_cast<T>(0)) ? u : u - v * floor(u / v));
}

template<typename T>
inline vec2<T> mod(const vec2<T> &u, const vec2<T> &v)
{
    return {mod(u.x, v.x), mod(u.y, v.y)};
}

template<typename T>
inline vec3<T> mod(const vec3<T> &u, const vec3<T> &v)
{
    return {mod(u.x, v.x), mod(u.y, v.y), mod(u.z, v.z)};
}

template<typename T>
inline vec4<T> mod(const vec4<T> &u, const vec4<T> &v)
{
    return {mod(u.x, v.x), mod(u.y, v.y), mod(u.z, v.z), mod(u.w, v.w)};
}

/// -----------------------------------------------------------------------------
/// @brief Return the Dirac nascent function, defined as discrete pulse with
/// width e. The corresponding delta function is the asymptotic value when e->0.
///
template<typename T>
inline T dirac(const T e, const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    if (isgt(e, static_cast<T>(0)) && islt(static_cast<T>(1) * abs(u), e)) {
        return static_cast<T>(1) / e;
    }
    return static_cast<T>(0);
}

template<typename T>
inline vec2<T> dirac(const T e, const vec2<T> &u)
{
    return {dirac(e, u.x), dirac(e, u.y)};
}

template<typename T>
inline vec3<T> dirac(const T e, const vec3<T> &u)
{
    return {dirac(e, u.x), dirac(e, u.y), dirac(e, u.z)};
}

template<typename T>
inline vec4<T> dirac(const T e, const vec4<T> &u)
{
    return {dirac(e, u.x), dirac(e, u.y), dirac(e, u.z), dirac(e, u.w)};
}

/// -----------------------------------------------------------------------------
/// @brief Heaviside step function.
///
template<typename T>
inline T step(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return islt(u, static_cast<T>(0)) ? static_cast<T>(0) : static_cast<T>(1);
}

template<typename T>
inline vec2<T> step(const vec2<T> &u)
{
    return {step(u.x), step(u.y)};
}

template<typename T>
inline vec3<T> step(const vec3<T> &u)
{
    return {step(u.x), step(u.y), step(u.z)};
}

template<typename T>
inline vec4<T> step(const vec4<T> &u)
{
    return {step(u.x), step(u.y), step(u.z), step(u.w)};
}

/// -----------------------------------------------------------------------------
/// @brief Smooth step between lo and hi.
///
template<typename T>
inline T smoothstep(const T lo, const T hi, const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    T x = clamp((u - lo) / (hi - lo), static_cast<T>(0), static_cast<T>(1));
    return x * x * (static_cast<T>(3) - static_cast<T>(2) * x);
}

template<typename T>
inline vec2<T> smoothstep(
    const vec2<T> &lo,
    const vec2<T> &hi,
    const vec2<T> &u)
{
    return {
        smoothstep(lo.x, hi.x, u.x),
        smoothstep(lo.y, hi.y, u.y)
    };
}

template<typename T>
inline vec3<T> smoothstep(
    const vec3<T> &lo,
    const vec3<T> &hi,
    const vec3<T> &u)
{
    return {
        smoothstep(lo.x, hi.x, u.x),
        smoothstep(lo.y, hi.y, u.y),
        smoothstep(lo.z, hi.z, u.z)
    };
}

template<typename T>
inline vec4<T> smoothstep(
    const vec4<T> &lo,
    const vec4<T> &hi,
    const vec4<T> &u)
{
    return {
        smoothstep(lo.x, hi.x, u.x),
        smoothstep(lo.y, hi.y, u.y),
        smoothstep(lo.z, hi.z, u.z),
        smoothstep(lo.w, hi.w, u.w)
    };
}

/// -----------------------------------------------------------------------------
/// @brief Linear interpolation between lo and hi.
///
template<typename T>
inline T lerp(const T lo, const T hi, const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return lo * (1 - u) + hi * u;
}

template<typename T>
inline vec2<T> lerp(const vec2<T> &lo, const vec2<T> &hi, const vec2<T> &u)
{
    return {
        lerp(lo.x, hi.x, u.x),
        lerp(lo.y, hi.y, u.y)
    };
}

template<typename T>
inline vec3<T> lerp(const vec3<T> &lo, const vec3<T> &hi, const vec3<T> &u)
{
    return {
        lerp(lo.x, hi.x, u.x),
        lerp(lo.y, hi.y, u.y),
        lerp(lo.z, hi.z, u.z)
    };
}

template<typename T>
inline vec4<T> lerp(const vec4<T> &lo, const vec4<T> &hi, const vec4<T> &u)
{
    return {
        lerp(lo.x, hi.x, u.x),
        lerp(lo.y, hi.y, u.y),
        lerp(lo.z, hi.z, u.z),
        lerp(lo.w, hi.w, u.w)
    };
}

/// -----------------------------------------------------------------------------
/// @brief Convert degrees to radians.
///
template<typename T>
inline T radians(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return u * M_PI / 180;
}

template<typename T>
inline vec2<T> radians(const vec2<T> &u)
{
    return {radians(u.x), radians(u.y)};
}

template<typename T>
inline vec3<T> radians(const vec3<T> &u)
{
    return {radians(u.x), radians(u.y), radians(u.z)};
}

template<typename T>
inline vec4<T> radians(const vec4<T> &u)
{
    return {radians(u.x), radians(u.y), radians(u.z), radians(u.w)};
}

///
/// @brief Convert radians to degrees.
///
template<typename T>
inline T degrees(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return u * 180 / M_PI;
}

template<typename T>
inline vec2<T> degrees(const vec2<T> &u)
{
    return {degrees(u.x), degrees(u.y)};
}

template<typename T>
inline vec3<T> degrees(const vec3<T> &u)
{
    return {degrees(u.x), degrees(u.y), degrees(u.z)};
}

template<typename T>
inline vec4<T> degrees(const vec4<T> &u)
{
    return {degrees(u.x), degrees(u.y), degrees(u.z), degrees(u.w)};
}

/// ---- Arithmetic functions -------------------------------------------------
/// @brief Swap u with v.
///
template<typename T>
inline void swap(T &u, T &v) { std::swap(u, v); }

template<typename T>
inline void swap(vec2<T> &u, vec2<T> &v) { std::swap(u, v); }

template<typename T>
inline void swap(vec3<T> &u, vec3<T> &v) { std::swap(u, v); }

template<typename T>
inline void swap(vec4<T> &u, vec4<T> &v) { std::swap(u, v); }

///
/// @brief Return the signum function of u.
///
template<typename T>
inline T sign(const T u)
{
    return (u < 0) ? -1 : (u > 0) ?  1 : 0;
}

template<typename T>
inline vec2<T> sign(const vec2<T> &u)
{
    return {sign(u.x), sign(u.y)};
}

template<typename T>
inline vec3<T> sign(const vec3<T> &u)
{
    return {sign(u.x), sign(u.y), sign(u.z)};
}

template<typename T>
inline vec4<T> sign(const vec4<T> &u)
{
    return {sign(u.x), sign(u.y), sign(u.z), sign(u.w)};
}

///
/// @brief Return the abs function of u.
///
template<typename T>
inline T abs(const T u)
{
    return std::abs(u);
}

template<typename T>
inline vec2<T> abs(const vec2<T> &u)
{
    return {abs(u.x), abs(u.y)};
}

template<typename T>
inline vec3<T> abs(const vec3<T> &u)
{
    return {abs(u.x), abs(u.y), abs(u.z)};
}

template<typename T>
inline vec4<T> abs(const vec4<T> &u)
{
    return {abs(u.x), abs(u.y), abs(u.z), abs(u.w)};
}

///
/// @brief Return the min between u and v.
///
template<typename T>
inline T min(const T u, const T v)
{
    return std::min(u, v);
}

template<typename T>
inline vec2<T> min(const vec2<T> &u, const vec2<T> &v)
{
    return {min(u.x, v.x), min(u.y, v.y)};
}

template<typename T>
inline vec3<T> min(const vec3<T> &u, const vec3<T> &v)
{
    return {min(u.x, v.x), min(u.y, v.y), min(u.z, v.z)};
}

template<typename T>
inline vec4<T> min(const vec4<T> &u, const vec4<T> &v)
{
    return {min(u.x, v.x), min(u.y, v.y), min(u.z, v.z), min(u.w, v.w)};
}

///
/// @brief Return the max between u and v.
///
template<typename T>
inline T max(const T u, const T v)
{
    return std::max(u, v);
}

template<typename T>
inline vec2<T> max(const vec2<T> &u, const vec2<T> &v)
{
    return {max(u.x, v.x), max(u.y, v.y)};
}

template<typename T>
inline vec3<T> max(const vec3<T> &u, const vec3<T> &v)
{
    return {max(u.x, v.x), max(u.y, v.y), max(u.z, v.z)};
}

template<typename T>
inline vec4<T> max(const vec4<T> &u, const vec4<T> &v)
{
    return {max(u.x, v.x), max(u.y, v.y), max(u.z, v.z), max(u.w, v.w)};
}

///
/// @brief Clamp u between lo and hi.
///
template<typename T>
inline T clamp(const T u, const T lo, const T hi)
{
    return min(max(u, lo), hi);
}

template<typename T>
inline vec2<T> clamp(const vec2<T> &u, const vec2<T> &lo, const vec2<T> &hi)
{
    return min(max(u, lo), hi);
}

template<typename T>
inline vec3<T> clamp(const vec3<T> &u, const vec3<T> &lo, const vec3<T> &hi)
{
    return min(max(u, lo), hi);
}

template<typename T>
inline vec4<T> clamp(const vec4<T> &u, const vec4<T> &lo, const vec4<T> &hi)
{
    return min(max(u, lo), hi);
}

} // math

/// ---- simd implementations ------------------------------------------------
#ifdef __AVX__
#include "simd/arithmetic.h"
#endif

#endif // MATH_ARITHMETIC_H_
