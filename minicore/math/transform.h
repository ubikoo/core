//
// transform.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MATH_TRANSFORM_H_
#define MATH_TRANSFORM_H_

#include <cmath>
#include <type_traits>
#include "vector.h"
#include "matrix.h"

namespace Math {

/// -----------------------------------------------------------------------------
/// @brief Translate the matrix by d.
///
template<typename T>
inline Mat4<T> Translate(const Vec3<T> d)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {(T) 1, (T) 0, (T) 0, d.x,
            (T) 0, (T) 1, (T) 0, d.y,
            (T) 0, (T) 0, (T) 1, d.z,
            (T) 0, (T) 0, (T) 0, (T) 1};
}

template<typename T>
inline Mat4<T> Translate(const Mat4<T> m, const Vec3<T> d)
{
    return Dot(Translate(d), m);
}

/// -----------------------------------------------------------------------------
/// @brief Scale the matrix by s.
///
template<typename T>
inline Mat4<T> Scale(const Vec3<T> s)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {  s.x, (T) 0, (T) 0, (T) 0,
            (T) 0,   s.y, (T) 0, (T) 0,
            (T) 0, (T) 0,   s.z, (T) 0,
            (T) 0, (T) 0, (T ) 0, (T) 1};
}

template<typename T>
inline Mat4<T> Scale(const Mat4<T> m, const Vec3<T> s)
{
    return Dot(Scale(s), m);
}

/// -----------------------------------------------------------------------------
/// @brief Rotate the matrix around n-axis using Rodrigues formula.
///
template<typename T>
inline Mat4<T> Rotate(Vec3<T> n, const T theta)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");

    // Compute cross product matrix
    n = Normalize(n);
    Mat4<T> R = { (T) 0,   -n.z,    n.y, (T) 0,
                    n.z,  (T) 0,   -n.x, (T) 0,
                   -n.y,    n.x,  (T) 0, (T) 0,
                  (T) 0,  (T) 0,  (T) 0, (T) 0};

    // Compute rotation matrix
    Mat4<T> result = Mat4<T>::Eye;
    result += R * std::sin(theta);
    result += Dot(R, R) * ((T) 1 - std::cos(theta));
    return result;
}

template<typename T>
inline Mat4<T> Rotate(const Mat4<T> m, Vec3<T> n, const T theta)
{
    return Dot(Rotate(n, theta), m);
}

/// -----------------------------------------------------------------------------
/// @brief Return the matrix that aligns direction vector-a to vector-b.
///
template<typename T>
inline Mat4<T> Align(Vec3<T> a, Vec3<T> b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");

    // Compute the cross product
    a = Normalize(a);
    b = Normalize(b);
    T cos_theta = Dot(a,b);
    Vec3<T> n = Cross(a, b);

    // Return identity if the direction vectors are already aligned.
    if (IsEq(cos_theta, -(T) 1)) {
        return Mat4<T>::Eye * (-(T) 1);
    }

    if (IsEq(cos_theta, (T) 1)) {
        return Mat4<T>::Eye;
    }

    // Return the rotation matrix using Rodrigues formula.
    return Rotate(n, std::acos(cos_theta));
}

template<typename T>
inline Mat4<T> Align(const Mat4<T> m, Vec3<T> a, Vec3<T> b)
{
    return Dot(Align(a, b), m);
}

/// -----------------------------------------------------------------------------
/// @brief Compute a lookat local frame of reference given the eye position,
/// the ctr reference viewpoint and the updward directon vector up.
///
/// The convention used here is a right-handed basis set, where the z-axis is
/// represented by forward f-vector. The x-axis is represented by the sideway
/// s-vector and the y-axis is represented by the upward u-vector in the local
/// reference frame.
///
template<typename T>
inline Mat4<T> LookAt(const Vec3<T> &eye, const Vec3<T> &ctr, const Vec3<T> &up)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");

    // Compute the lookat basis set s-u-f.
    Vec3<T> f = Normalize(eye - ctr);       // f = eye - ctr
    Vec3<T> s = Normalize(Cross(up, f));    // s = up x f
    Vec3<T> u = Normalize(Cross(f, s));     // u = s x f

    // Compute the lookat projection matrix with eye translation.
    Mat4<T> M{   s.x,    s.y,    s.z,   -Dot(s, eye),
                 u.x,    u.y,    u.z,   -Dot(u, eye),
                 f.x,    f.y,    f.z,   -Dot(f, eye),
               (T) 0,  (T) 0,  (T) 0,    (T) 1};
    return M;
}

template<typename T>
inline Mat4<T> LookAt(
    const Mat4<T> m,
    const Vec3<T> &eye,
    const Vec3<T> &ctr,
    const Vec3<T> &up)
{
    return Dot(LookAt(eye, ctr, up), m);
}

/// -----------------------------------------------------------------------------
/// @brief Compute the perspective projection matrix.
///
template<typename T>
inline Mat4<T> Perspective(
    const T fovy,
    const T aspect,
    const T znear,
    const T zfar)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");

    const T tan_half_fovy = std::tan(fovy / (T) 2);

    Mat4<T> result{};
    result.xx =  (T) 1 / (tan_half_fovy * aspect);
    result.xy =  (T) 0;
    result.xz =  (T) 0;
    result.xw =  (T) 0;

    result.yx =  (T) 0;
    result.yy =  (T) 1 / tan_half_fovy;
    result.yz =  (T) 0;
    result.yw =  (T) 0;

    result.zx =  (T) 0;
    result.zy =  (T) 0;
    result.zz = -(zfar + znear) / (zfar - znear);
    result.zw = -((T) 2 * zfar * znear) / (zfar - znear);

    result.wx =  (T) 0;
    result.wy =  (T) 0;
    result.wz = -(T) 1;
    result.ww =  (T) 0;
    return result;
}

template<typename T>
inline Mat4<T> Perspective(
    const Mat4<T> m,
    const T fovy,
    const T aspect,
    const T znear,
    const T zfar)
{
    return Dot(Perspective(fovy, aspect, znear, zfar), m);
}

/// -----------------------------------------------------------------------------
/// @brief Compute the orthographic projection matrix.
///
template<typename T>
inline Mat4<T> Orthographic(
    const T left,
    const T right,
    const T bottom,
    const T top,
    const T znear,
    const T zfar)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    Mat4<T> result{};
    result.xx =  (T) 2 / (right - left);
    result.xy =  (T) 0;
    result.xz =  (T) 0;
    result.xw =  -(right + left) / (right - left);

    result.yx =  (T) 0;
    result.yy =  (T) 2 / (top - bottom);
    result.yz =  (T) 0;
    result.yw =  -(top + bottom) / (top - bottom);

    result.zx =  (T) 0;
    result.zy =  (T) 0;
    result.zz = -(T) 2 / (zfar - znear);
    result.zw = -(zfar + znear) / (zfar - znear);

    result.wx =  (T) 0;
    result.wy =  (T) 0;
    result.wz =  (T) 0;
    result.ww =  (T) 1;
    return result;
}

template<typename T>
inline Mat4<T> Orthographic(
    const Mat4<T> m,
    const T left,
    const T right,
    const T bottom,
    const T top,
    const T znear,
    const T zfar)
{
    return Dot(Orthographic(left, right, bottom, top, znear, zfar), m);
}

} // namespace Math

/// ---- simd implementations ------------------------------------------------
#ifdef __AVX__
#include "simd/transform.h"
#endif

#endif // MATH_TRANSFORM_H_
