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

namespace math {

/// -----------------------------------------------------------------------------
/// @brief Translate the matrix by d.
///
template<typename T>
inline mat4<T> translate(const vec3<T> d)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {(T) 1, (T) 0, (T) 0, d.x,
            (T) 0, (T) 1, (T) 0, d.y,
            (T) 0, (T) 0, (T) 1, d.z,
            (T) 0, (T) 0, (T) 0, (T) 1};
}

template<typename T>
inline mat4<T> translate(const mat4<T> m, const vec3<T> d)
{
    return dot(translate(d), m);
}

/// -----------------------------------------------------------------------------
/// @brief Scale the matrix by s.
///
template<typename T>
inline mat4<T> scale(const vec3<T> s)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {  s.x, (T) 0, (T) 0, (T) 0,
            (T) 0,   s.y, (T) 0, (T) 0,
            (T) 0, (T) 0,   s.z, (T) 0,
            (T) 0, (T) 0, (T ) 0, (T) 1};
}

template<typename T>
inline mat4<T> scale(const mat4<T> m, const vec3<T> s)
{
    return dot(scale(s), m);
}

/// -----------------------------------------------------------------------------
/// @brief Rotate the matrix around n-axis using Rodrigues formula.
///
template<typename T>
inline mat4<T> rotate(vec3<T> n, const T theta)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");

    // Compute cross product matrix
    n = normalize(n);
    mat4<T> R = { (T) 0,   -n.z,    n.y, (T) 0,
                    n.z,  (T) 0,   -n.x, (T) 0,
                   -n.y,    n.x,  (T) 0, (T) 0,
                  (T) 0,  (T) 0,  (T) 0, (T) 0};

    // Compute rotation matrix
    mat4<T> result = mat4<T>::eye;
    result += R * std::sin(theta);
    result += dot(R, R) * ((T) 1 - std::cos(theta));
    return result;
}

template<typename T>
inline mat4<T> rotate(const mat4<T> m, vec3<T> n, const T theta)
{
    return dot(rotate(n, theta), m);
}

/// -----------------------------------------------------------------------------
/// @brief Return the matrix that maps direction vector-a to vector-b.
///
template<typename T>
inline mat4<T> align(vec3<T> a, vec3<T> b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");

    // Compute the cross product
    a = normalize(a);
    b = normalize(b);
    T cos_theta = dot(a,b);
    vec3<T> n = cross(a, b);

    // Return identity if the direction vectors are already aligned.
    if (iseq(cos_theta, -(T) 1)) {
        return mat4<T>::eye * (-(T) 1);
    }

    if (iseq(cos_theta, (T) 1)) {
        return mat4<T>::eye;
    }

    // Return the rotation matrix using Rodrigues formula.
    return rotate(n, std::acos(cos_theta));
}

template<typename T>
inline mat4<T> align(const mat4<T> m, vec3<T> a, vec3<T> b)
{
    return dot(align(a, b), m);
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
inline mat4<T> lookat(const vec3<T> &eye, const vec3<T> &ctr, const vec3<T> &up)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");

    // Compute the lookat basis set s-u-f.
    vec3<T> f = normalize(eye - ctr);       // f = eye - ctr
    vec3<T> s = normalize(cross(up, f));    // s = up x f
    vec3<T> u = normalize(cross(f, s));     // u = s x f

    // Compute the lookat projection matrix with eye translation.
    mat4<T> M{   s.x,    s.y,    s.z,   -dot(s,eye),
                 u.x,    u.y,    u.z,   -dot(u,eye),
                 f.x,    f.y,    f.z,   -dot(f,eye),
               (T) 0,  (T) 0,  (T) 0,    (T) 1};
    return M;
}

template<typename T>
inline mat4<T> lookat(
    const mat4<T> m,
    const vec3<T> &eye,
    const vec3<T> &ctr,
    const vec3<T> &up)
{
    return dot(lookat(eye, ctr, up), m);
}

/// -----------------------------------------------------------------------------
/// @brief Compute the perspective projection matrix.
///
template<typename T>
inline mat4<T> perspective(
    const T fovy,
    const T aspect,
    const T znear,
    const T zfar)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");

    const T tan_half_fovy = std::tan(fovy / (T) 2);

    mat4<T> result{};
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
inline mat4<T> perspective(
    const mat4<T> m,
    const T fovy,
    const T aspect,
    const T znear,
    const T zfar)
{
    return dot(perspective(fovy, aspect, znear, zfar), m);
}

/// -----------------------------------------------------------------------------
/// @brief Compute the orthographic projection matrix.
///
template<typename T>
inline mat4<T> orthographic(
    const T left,
    const T right,
    const T bottom,
    const T top,
    const T znear,
    const T zfar)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    mat4<T> result{};
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
inline mat4<T> orthographic(
    const mat4<T> m,
    const T left,
    const T right,
    const T bottom,
    const T top,
    const T znear,
    const T zfar)
{
    return dot(orthographic(left, right, bottom, top, znear, zfar), m);
}

} // math

/// ---- simd implementations ------------------------------------------------
#ifdef __AVX__
#include "simd/transform.h"
#endif

#endif // MATH_TRANSFORM_H_
