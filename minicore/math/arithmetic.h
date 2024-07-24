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
#include <limits>
#include <type_traits>
#include <algorithm>
#include "vector.h"

namespace Math {

/// ---- Function declarations ------------------------------------------------
/// @brief Floating point functions.
///
template<typename T> inline bool IsEq(const T u, const T v);
template<typename T> inline bool IsLess(const T u, const T v);
template<typename T> inline bool IsGreater(const T u, const T v);

template<typename T> inline T Round(const T u);
template<typename T> inline Vec2<T> Round(const Vec2<T> &u);
template<typename T> inline Vec3<T> Round(const Vec3<T> &u);
template<typename T> inline Vec4<T> Round(const Vec4<T> &u);

template<typename T> inline T Floor(const T u);
template<typename T> inline Vec2<T> Floor(const Vec2<T> &u);
template<typename T> inline Vec3<T> Floor(const Vec3<T> &u);
template<typename T> inline Vec4<T> Floor(const Vec4<T> &u);

template<typename T> inline T Ceil(const T u);
template<typename T> inline Vec2<T> Ceil(const Vec2<T> &u);
template<typename T> inline Vec3<T> Ceil(const Vec3<T> &u);
template<typename T> inline Vec4<T> Ceil(const Vec4<T> &u);

template<typename T> inline T Mod(const T u, const T v);
template<typename T> inline Vec2<T> Mod(const Vec2<T> &u, const Vec2<T> &v);
template<typename T> inline Vec3<T> Mod(const Vec3<T> &u, const Vec3<T> &v);
template<typename T> inline Vec4<T> Mod(const Vec4<T> &u, const Vec4<T> &v);

template<typename T> inline T Dirac(const T e, const T u);
template<typename T> inline Vec2<T> Dirac(const T e, const Vec2<T> &u);
template<typename T> inline Vec3<T> Dirac(const T e, const Vec3<T> &u);
template<typename T> inline Vec4<T> Dirac(const T e, const Vec4<T> &u);

template<typename T> inline T Step(const T u);
template<typename T> inline Vec2<T> Step(const Vec2<T> &u);
template<typename T> inline Vec3<T> Step(const Vec3<T> &u);
template<typename T> inline Vec4<T> Step(const Vec4<T> &u);

template<typename T> inline T SmoothStep(const T lo, const T hi, const T u);
template<typename T> inline Vec2<T> SmoothStep(
    const Vec2<T> &lo, const Vec2<T> &hi, const Vec2<T> &u);
template<typename T> inline Vec3<T> SmoothStep(
    const Vec3<T> &lo, const Vec3<T> &hi, const Vec3<T> &u);
template<typename T> inline Vec4<T> SmoothStep(
    const Vec4<T> &lo, const Vec4<T> &hi, const Vec4<T> &u);

template<typename T> inline T Lerp(const T lo, const T hi,  T u);
template<typename T> inline Vec2<T> Lerp(
    const Vec2<T> &lo, const Vec2<T> &hi, const T u);
template<typename T> inline Vec3<T> Lerp(
    const Vec3<T> &lo, const Vec3<T> &hi, const T u);
template<typename T> inline Vec4<T> Lerp(
    const Vec4<T> &lo, const Vec4<T> &hi, const T u);

template<typename T> inline T Radians(const T u);
template<typename T> inline Vec2<T> Radians(const Vec2<T> &u);
template<typename T> inline Vec3<T> Radians(const Vec3<T> &u);
template<typename T> inline Vec4<T> Radians(const Vec4<T> &u);

template<typename T> inline T Degrees(const T u);
template<typename T> inline Vec2<T> Degrees(const Vec2<T> &u);
template<typename T> inline Vec3<T> Degrees(const Vec3<T> &u);
template<typename T> inline Vec4<T> Degrees(const Vec4<T> &u);

///
/// @brief Arithmetic functions.
///
template<typename T> inline void Swap(T &u, T &v);
template<typename T> inline void Swap(Vec2<T> &u, Vec2<T> &v);
template<typename T> inline void Swap(Vec3<T> &u, Vec3<T> &v);
template<typename T> inline void Swap(Vec4<T> &u, Vec4<T> &v);

template<typename T> inline T Sign(const T u);
template<typename T> inline Vec2<T> Sign(const Vec2<T> &u);
template<typename T> inline Vec3<T> Sign(const Vec3<T> &u);
template<typename T> inline Vec4<T> Sign(const Vec4<T> &u);

template<typename T> inline T Abs(const T u);
template<typename T> inline Vec2<T> Abs(const Vec2<T> &u);
template<typename T> inline Vec3<T> Abs(const Vec3<T> &u);
template<typename T> inline Vec4<T> Abs(const Vec4<T> &u);

template<typename T> inline T Min(const T u, const T v);
template<typename T> inline Vec2<T> Min(const Vec2<T> &u, const Vec2<T> &v);
template<typename T> inline Vec3<T> Min(const Vec3<T> &u, const Vec3<T> &v);
template<typename T> inline Vec4<T> Min(const Vec4<T> &u, const Vec4<T> &v);

template<typename T> inline T Max(const T u, const T v);
template<typename T> inline Vec2<T> Max(const Vec2<T> &u, const Vec2<T> &v);
template<typename T> inline Vec3<T> Max(const Vec3<T> &u, const Vec3<T> &v);
template<typename T> inline Vec4<T> Max(const Vec4<T> &u, const Vec4<T> &v);

template<typename T> inline T Clamp(const T u, const T lo, const T hi);
template<typename T> inline Vec2<T> Clamp(
    const Vec2<T> &u, const Vec2<T> &lo, const Vec2<T> &hi);
template<typename T> inline Vec3<T> Clamp(
    const Vec3<T> &u, const Vec3<T> &lo, const Vec3<T> &hi);
template<typename T> inline Vec4<T> Clamp(
    const Vec4<T> &u, const Vec4<T> &lo, const Vec4<T> &hi);

/// ---- Floating point functions ---------------------------------------------
/// @brief Is u approx equal to v? (u ~ v), iif |u-v| <= eps * Min(|u|,|v|)
/// If (u = 0) identically, then the relation above will be false even if
/// (v <= eps). The edge case is handled by ensuring that maxerr is never zero
/// identically (i.e. maxerr < eps), thus 0 < maxerr <= eps * Min(|u|,|v|).
///
template<typename T>
inline bool IsEq(const T u, const T v)
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
/// @brief Is u approx less than v? (u < v), iif (v-u) > eps * Max(|u|,|v|).
///
template<typename T>
inline bool IsLess(const T u, const T v)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    static const T epsilon = std::sqrt(std::numeric_limits<T>::epsilon());
    T abs_u = std::fabs(u);
    T abs_v = std::fabs(v);
    return (v - u) > (epsilon * (abs_u > abs_v ? abs_u : abs_v));
}

///
/// @brief Is u approx greater than v? (u > v), iif (u-v) > eps * Max(|u|,|v|).
///
template<typename T>
inline bool IsGreater(const T u, const T v)
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
inline T Round(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return std::round(u);
}

template<typename T>
inline Vec2<T> Round(const Vec2<T> &u)
{
    return {Round(u.x), Round(u.y)};
}

template<typename T>
inline Vec3<T> Round(const Vec3<T> &u)
{
    return {Round(u.x), Round(u.y), Round(u.z)};
}

template<typename T>
inline Vec4<T> Round(const Vec4<T> &u)
{
    return {Round(u.x), Round(u.y), Round(u.z), Round(u.w)};
}

/// -----------------------------------------------------------------------------
/// @brief Return the largest integer not greater than u (floating point).
///
template<typename T>
inline T Floor(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return std::floor(u);
}

template<typename T>
inline Vec2<T> Floor(const Vec2<T> &u)
{
    return {Floor(u.x), Floor(u.y)};
}

template<typename T>
inline Vec3<T> Floor(const Vec3<T> &u)
{
    return {Floor(u.x), Floor(u.y), Floor(u.z)};
}

template<typename T>
inline Vec4<T> Floor(const Vec4<T> &u)
{
    return {Floor(u.x), Floor(u.y), Floor(u.z), Floor(u.w)};
}

/// -----------------------------------------------------------------------------
/// @brief Return the smallest integer not smaller than u (floating point).
///
template<typename T>
inline T Ceil(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return std::ceil(u);
}

template<typename T>
inline Vec2<T> Ceil(const Vec2<T> &u)
{
    return {Ceil(u.x), Ceil(u.y)};
}

template<typename T>
inline Vec3<T> Ceil(const Vec3<T> &u)
{
    return {Ceil(u.x), Ceil(u.y), Ceil(u.z)};
}

template<typename T>
inline Vec4<T> Ceil(const Vec4<T> &u)
{
    return {Ceil(u.x), Ceil(u.y), Ceil(u.z), Ceil(u.w)};
}

/// -----------------------------------------------------------------------------
/// @brief Return modulo operation (u mod v).
///
template<typename T>
inline T Mod(const T u, const T v)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return (IsEq(v, static_cast<T>(0)) ? u : u - v * Floor(u / v));
}

template<typename T>
inline Vec2<T> Mod(const Vec2<T> &u, const Vec2<T> &v)
{
    return {Mod(u.x, v.x), Mod(u.y, v.y)};
}

template<typename T>
inline Vec3<T> Mod(const Vec3<T> &u, const Vec3<T> &v)
{
    return {Mod(u.x, v.x), Mod(u.y, v.y), Mod(u.z, v.z)};
}

template<typename T>
inline Vec4<T> Mod(const Vec4<T> &u, const Vec4<T> &v)
{
    return {Mod(u.x, v.x), Mod(u.y, v.y), Mod(u.z, v.z), Mod(u.w, v.w)};
}

/// -----------------------------------------------------------------------------
/// @brief Return the Dirac nascent function, defined as discrete pulse with
/// width e. The corresponding delta function is the asymptotic value when e->0.
///
template<typename T>
inline T Dirac(const T e, const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    if (IsGreater(e, static_cast<T>(0)) &&
        IsLess(static_cast<T>(1) * Abs(u), e)) {
        return static_cast<T>(1) / e;
    }
    return static_cast<T>(0);
}

template<typename T>
inline Vec2<T> Dirac(const T e, const Vec2<T> &u)
{
    return {Dirac(e, u.x), Dirac(e, u.y)};
}

template<typename T>
inline Vec3<T> Dirac(const T e, const Vec3<T> &u)
{
    return {Dirac(e, u.x), Dirac(e, u.y), Dirac(e, u.z)};
}

template<typename T>
inline Vec4<T> Dirac(const T e, const Vec4<T> &u)
{
    return {Dirac(e, u.x), Dirac(e, u.y), Dirac(e, u.z), Dirac(e, u.w)};
}

/// -----------------------------------------------------------------------------
/// @brief Heaviside step function.
///
template<typename T>
inline T Step(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return IsLess(u, static_cast<T>(0)) ? static_cast<T>(0) : static_cast<T>(1);
}

template<typename T>
inline Vec2<T> Step(const Vec2<T> &u)
{
    return {Step(u.x), Step(u.y)};
}

template<typename T>
inline Vec3<T> Step(const Vec3<T> &u)
{
    return {Step(u.x), Step(u.y), Step(u.z)};
}

template<typename T>
inline Vec4<T> Step(const Vec4<T> &u)
{
    return {Step(u.x), Step(u.y), Step(u.z), Step(u.w)};
}

/// -----------------------------------------------------------------------------
/// @brief Smooth step between lo and hi.
///
template<typename T>
inline T SmoothStep(const T lo, const T hi, const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    T x = Clamp((u - lo) / (hi - lo), static_cast<T>(0), static_cast<T>(1));
    return x * x * (static_cast<T>(3) - static_cast<T>(2) * x);
}

template<typename T>
inline Vec2<T> SmoothStep(
    const Vec2<T> &lo,
    const Vec2<T> &hi,
    const Vec2<T> &u)
{
    return {
        SmoothStep(lo.x, hi.x, u.x),
        SmoothStep(lo.y, hi.y, u.y)
    };
}

template<typename T>
inline Vec3<T> SmoothStep(
    const Vec3<T> &lo,
    const Vec3<T> &hi,
    const Vec3<T> &u)
{
    return {
        SmoothStep(lo.x, hi.x, u.x),
        SmoothStep(lo.y, hi.y, u.y),
        SmoothStep(lo.z, hi.z, u.z)
    };
}

template<typename T>
inline Vec4<T> SmoothStep(
    const Vec4<T> &lo,
    const Vec4<T> &hi,
    const Vec4<T> &u)
{
    return {
        SmoothStep(lo.x, hi.x, u.x),
        SmoothStep(lo.y, hi.y, u.y),
        SmoothStep(lo.z, hi.z, u.z),
        SmoothStep(lo.w, hi.w, u.w)
    };
}

/// -----------------------------------------------------------------------------
/// @brief Linear interpolation between lo and hi.
///
template<typename T>
inline T Lerp(const T lo, const T hi, const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return lo * (1 - u) + hi * u;
}

template<typename T>
inline Vec2<T> Lerp(const Vec2<T> &lo, const Vec2<T> &hi, const Vec2<T> &u)
{
    return {
        Lerp(lo.x, hi.x, u.x),
        Lerp(lo.y, hi.y, u.y)
    };
}

template<typename T>
inline Vec3<T> Lerp(const Vec3<T> &lo, const Vec3<T> &hi, const Vec3<T> &u)
{
    return {
        Lerp(lo.x, hi.x, u.x),
        Lerp(lo.y, hi.y, u.y),
        Lerp(lo.z, hi.z, u.z)
    };
}

template<typename T>
inline Vec4<T> Lerp(const Vec4<T> &lo, const Vec4<T> &hi, const Vec4<T> &u)
{
    return {
        Lerp(lo.x, hi.x, u.x),
        Lerp(lo.y, hi.y, u.y),
        Lerp(lo.z, hi.z, u.z),
        Lerp(lo.w, hi.w, u.w)
    };
}

/// -----------------------------------------------------------------------------
/// @brief Convert degrees to radians.
///
template<typename T>
inline T Radians(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return u * M_PI / 180;
}

template<typename T>
inline Vec2<T> Radians(const Vec2<T> &u)
{
    return {Radians(u.x), Radians(u.y)};
}

template<typename T>
inline Vec3<T> Radians(const Vec3<T> &u)
{
    return {Radians(u.x), Radians(u.y), Radians(u.z)};
}

template<typename T>
inline Vec4<T> Radians(const Vec4<T> &u)
{
    return {Radians(u.x), Radians(u.y), Radians(u.z), Radians(u.w)};
}

///
/// @brief Convert radians to degrees.
///
template<typename T>
inline T Degrees(const T u)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return u * static_cast<T>(180) / M_PI;
}

template<typename T>
inline Vec2<T> Degrees(const Vec2<T> &u)
{
    return {Degrees(u.x), Degrees(u.y)};
}

template<typename T>
inline Vec3<T> Degrees(const Vec3<T> &u)
{
    return {Degrees(u.x), Degrees(u.y), Degrees(u.z)};
}

template<typename T>
inline Vec4<T> Degrees(const Vec4<T> &u)
{
    return {Degrees(u.x), Degrees(u.y), Degrees(u.z), Degrees(u.w)};
}

/// ---- Arithmetic functions -------------------------------------------------
/// @brief Swap u with v.
///
template<typename T>
inline void Swap(T &u, T &v) { std::swap(u, v); }

template<typename T>
inline void Swap(Vec2<T> &u, Vec2<T> &v) { std::swap(u, v); }

template<typename T>
inline void Swap(Vec3<T> &u, Vec3<T> &v) { std::swap(u, v); }

template<typename T>
inline void Swap(Vec4<T> &u, Vec4<T> &v) { std::swap(u, v); }

///
/// @brief Return the signum function of u.
///
template<typename T>
inline T Sign(const T u)
{
    return (u < 0) ? -1 : (u > 0) ?  1 : 0;
}

template<typename T>
inline Vec2<T> Sign(const Vec2<T> &u)
{
    return {Sign(u.x), Sign(u.y)};
}

template<typename T>
inline Vec3<T> Sign(const Vec3<T> &u)
{
    return {Sign(u.x), Sign(u.y), Sign(u.z)};
}

template<typename T>
inline Vec4<T> Sign(const Vec4<T> &u)
{
    return {Sign(u.x), Sign(u.y), Sign(u.z), Sign(u.w)};
}

///
/// @brief Return the abs function of u.
///
template<typename T>
inline T Abs(const T u)
{
    return std::abs(u);
}

template<typename T>
inline Vec2<T> Abs(const Vec2<T> &u)
{
    return {abs(u.x), Abs(u.y)};
}

template<typename T>
inline Vec3<T> Abs(const Vec3<T> &u)
{
    return {Abs(u.x), Abs(u.y), Abs(u.z)};
}

template<typename T>
inline Vec4<T> Abs(const Vec4<T> &u)
{
    return {Abs(u.x), Abs(u.y), Abs(u.z), Abs(u.w)};
}

///
/// @brief Return the min between u and v.
///
template<typename T>
inline T Min(const T u, const T v)
{
    return std::min(u, v);
}

template<typename T>
inline Vec2<T> Min(const Vec2<T> &u, const Vec2<T> &v)
{
    return {Min(u.x, v.x), Min(u.y, v.y)};
}

template<typename T>
inline Vec3<T> Min(const Vec3<T> &u, const Vec3<T> &v)
{
    return {Min(u.x, v.x), Min(u.y, v.y), Min(u.z, v.z)};
}

template<typename T>
inline Vec4<T> Min(const Vec4<T> &u, const Vec4<T> &v)
{
    return {Min(u.x, v.x), Min(u.y, v.y), Min(u.z, v.z), Min(u.w, v.w)};
}

///
/// @brief Return the max between u and v.
///
template<typename T>
inline T Max(const T u, const T v)
{
    return std::max(u, v);
}

template<typename T>
inline Vec2<T> Max(const Vec2<T> &u, const Vec2<T> &v)
{
    return {Max(u.x, v.x), Max(u.y, v.y)};
}

template<typename T>
inline Vec3<T> Max(const Vec3<T> &u, const Vec3<T> &v)
{
    return {Max(u.x, v.x), Max(u.y, v.y), Max(u.z, v.z)};
}

template<typename T>
inline Vec4<T> Max(const Vec4<T> &u, const Vec4<T> &v)
{
    return {Max(u.x, v.x), Max(u.y, v.y), Max(u.z, v.z), Max(u.w, v.w)};
}

///
/// @brief Clamp u between lo and hi.
///
template<typename T>
inline T Clamp(const T u, const T lo, const T hi)
{
    return Min(Max(u, lo), hi);
}

template<typename T>
inline Vec2<T> Clamp(const Vec2<T> &u, const Vec2<T> &lo, const Vec2<T> &hi)
{
    return Min(Max(u, lo), hi);
}

template<typename T>
inline Vec3<T> Clamp(const Vec3<T> &u, const Vec3<T> &lo, const Vec3<T> &hi)
{
    return Min(Max(u, lo), hi);
}

template<typename T>
inline Vec4<T> Clamp(const Vec4<T> &u, const Vec4<T> &lo, const Vec4<T> &hi)
{
    return Min(Max(u, lo), hi);
}

} // namespace Math

/// ---- simd implementations ------------------------------------------------
#ifdef __AVX__
#include "simd/arithmetic.h"
#endif

#endif // MATH_ARITHMETIC_H_
