//
// algebra.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MATH_ALGEBRA_H_
#define MATH_ALGEBRA_H_

#include <cmath>
#include <type_traits>
#include "vector.h"
#include "matrix.h"

namespace math {

/// ---- Vector algebra function declarations ---------------------------------
/// @brief Return the 2-dimensional dot product.
///
template<typename T> inline T dot(const vec2<T> &a, const vec2<T> &b);
template<typename T> inline vec2<T> dot(const mat2<T> &a, const vec2<T> &b);
template<typename T> inline mat2<T> dot(const mat2<T> &a, const mat2<T> &b);

///
/// @brief Return the 3-dimensional dot product.
///
template<typename T> inline T dot(const vec3<T> &a, const vec3<T> &b);
template<typename T> inline vec3<T> dot(const mat3<T> &a, const vec3<T> &b);
template<typename T> inline mat3<T> dot(const mat3<T> &a, const mat3<T> &b);

///
/// @brief Return the 4-dimensional dot product.
///
template<typename T> inline T dot(const vec4<T> &a, const vec4<T> &b);
template<typename T> inline vec4<T> dot(const mat4<T> &a, const vec4<T> &b);
template<typename T> inline mat4<T> dot(const mat4<T> &a, const mat4<T> &b);

///
/// @brief Return the norm of the specified vector.
///
template<typename T> inline T norm(const vec2<T> &a);
template<typename T> inline T norm(const vec3<T> &a);
template<typename T> inline T norm(const vec4<T> &a);

///
/// @brief Return the normalized vector.
///
template<typename T> inline vec2<T> normalize(const vec2<T> &a);
template<typename T> inline vec3<T> normalize(const vec3<T> &a);
template<typename T> inline vec4<T> normalize(const vec4<T> &a);

///
/// @brief Return the distance between two points.
///
template<typename T> inline T distance(const vec2<T> &a, const vec2<T> &b);
template<typename T> inline T distance(const vec3<T> &a, const vec3<T> &b);
template<typename T> inline T distance(const vec4<T> &a, const vec4<T> &b);

///
/// @brief Return the cross product of two vectors.
///
template<typename T> inline vec3<T> cross(
    const vec3<T> &a, const vec3<T> &b);

/// ---- Matrix algebra function declarations ---------------------------------
/// @brief Return the transpose of the matrix.
///
template<typename T> inline mat2<T> transpose(const mat2<T> &a);
template<typename T> inline mat3<T> transpose(const mat3<T> &a);
template<typename T> inline mat4<T> transpose(const mat4<T> &a);

///
/// @brief Return the determinant of the matrix.
///
template<typename T> inline T determinant(const mat2<T> &a);
template<typename T> inline T determinant(const mat3<T> &a);
template<typename T> inline T determinant(const mat4<T> &a);

///
/// @brief Return the inverse of the matrix.
///
template<typename T> inline mat2<T> inverse(const mat2<T> &a);
template<typename T> inline mat3<T> inverse(const mat3<T> &a);
template<typename T> inline mat4<T> inverse(const mat4<T> &a);

/// ---- Vector algebra function implementations ------------------------------
/// @brief Return the 2-dimensional dot product.
///
template<typename T>
inline T dot(const vec2<T> &a, const vec2<T> &b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {a.x * b.x + a.y * b.y};
}

template<typename T>
inline vec2<T> dot(const mat2<T> &a, const vec2<T> &b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {a.xx * b.x + a.xy * b.y,
            a.yx * b.x + a.yy * b.y};
}

template<typename T>
inline mat2<T> dot(const mat2<T> &a, const mat2<T> &b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {a.xx * b.xx + a.xy * b.yx,
            a.xx * b.xy + a.xy * b.yy,
            a.yx * b.xx + a.yy * b.yx,
            a.yx * b.xy + a.yy * b.yy};
}

///
/// @brief Return the 3-dimensional dot product.
///
template<typename T>
inline T dot(const vec3<T> &a, const vec3<T> &b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {a.x * b.x + a.y * b.y + a.z * b.z};
}

template<typename T>
inline vec3<T> dot(const mat3<T> &a, const vec3<T> &b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {a.xx * b.x + a.xy * b.y + a.xz * b.z,
            a.yx * b.x + a.yy * b.y + a.yz * b.z,
            a.zx * b.x + a.zy * b.y + a.zz * b.z};
}

template<typename T>
inline mat3<T> dot(const mat3<T> &a, const mat3<T> &b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {a.xx * b.xx + a.xy * b.yx + a.xz * b.zx,
            a.xx * b.xy + a.xy * b.yy + a.xz * b.zy,
            a.xx * b.xz + a.xy * b.yz + a.xz * b.zz,

            a.yx * b.xx + a.yy * b.yx + a.yz * b.zx,
            a.yx * b.xy + a.yy * b.yy + a.yz * b.zy,
            a.yx * b.xz + a.yy * b.yz + a.yz * b.zz,

            a.zx * b.xx + a.zy * b.yx + a.zz * b.zx,
            a.zx * b.xy + a.zy * b.yy + a.zz * b.zy,
            a.zx * b.xz + a.zy * b.yz + a.zz * b.zz};
}

///
/// @brief Return the 4-dimensional dot product.
///
template<typename T>
inline T dot(const vec4<T> &a, const vec4<T> &b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w};
}

template<typename T>
inline vec4<T> dot(const mat4<T> &a, const vec4<T> &b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {a.xx * b.x + a.xy * b.y + a.xz * b.z + a.xw * b.w,
            a.yx * b.x + a.yy * b.y + a.yz * b.z + a.yw * b.w,
            a.zx * b.x + a.zy * b.y + a.zz * b.z + a.zw * b.w,
            a.wx * b.x + a.wy * b.y + a.wz * b.z + a.ww * b.w};
}

template<typename T>
inline mat4<T> dot(const mat4<T> &a, const mat4<T> &b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {a.xx * b.xx + a.xy * b.yx + a.xz * b.zx + a.xw * b.wx,
            a.xx * b.xy + a.xy * b.yy + a.xz * b.zy + a.xw * b.wy,
            a.xx * b.xz + a.xy * b.yz + a.xz * b.zz + a.xw * b.wz,
            a.xx * b.xw + a.xy * b.yw + a.xz * b.zw + a.xw * b.ww,

            a.yx * b.xx + a.yy * b.yx + a.yz * b.zx + a.yw * b.wx,
            a.yx * b.xy + a.yy * b.yy + a.yz * b.zy + a.yw * b.wy,
            a.yx * b.xz + a.yy * b.yz + a.yz * b.zz + a.yw * b.wz,
            a.yx * b.xw + a.yy * b.yw + a.yz * b.zw + a.yw * b.ww,

            a.zx * b.xx + a.zy * b.yx + a.zz * b.zx + a.zw * b.wx,
            a.zx * b.xy + a.zy * b.yy + a.zz * b.zy + a.zw * b.wy,
            a.zx * b.xz + a.zy * b.yz + a.zz * b.zz + a.zw * b.wz,
            a.zx * b.xw + a.zy * b.yw + a.zz * b.zw + a.zw * b.ww,

            a.wx * b.xx + a.wy * b.yx + a.wz * b.zx + a.ww * b.wx,
            a.wx * b.xy + a.wy * b.yy + a.wz * b.zy + a.ww * b.wy,
            a.wx * b.xz + a.wy * b.yz + a.wz * b.zz + a.ww * b.wz,
            a.wx * b.xw + a.wy * b.yw + a.wz * b.zw + a.ww * b.ww};
}

///
/// @brief Return the norm of the specified vector.
///
template<typename T>
inline T norm(const vec2<T> &a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return std::sqrt(dot(a,a));
}

template<typename T>
inline T norm(const vec3<T> &a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return std::sqrt(dot(a,a));
}

template<typename T>
inline T norm(const vec4<T> &a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return std::sqrt(dot(a,a));
}

///
/// @brief Return the normalized vector.
///
template<typename T>
inline vec2<T> normalize(const vec2<T> &a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return (a / norm(a));
}

template<typename T>
inline vec3<T> normalize(const vec3<T> &a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return (a / norm(a));
}

template<typename T>
inline vec4<T> normalize(const vec4<T> &a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return (a / norm(a));
}

///
/// @brief Return the distance between two points.
///
template<typename T>
inline T distance(const vec2<T> &a, const vec2<T> &b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return norm(a - b);
}

template<typename T>
inline T distance(const vec3<T> &a, const vec3<T> &b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return norm(a - b);
}

template<typename T>
inline T distance(const vec4<T> &a, const vec4<T> &b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return norm(a - b);
}

///
/// @brief Return the cross product of two vectors.
///
template<typename T>
inline vec3<T> cross(const vec3<T> &a, const vec3<T> &b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

/// ---- Matrix algebra function implementations ------------------------------
/// @brief Return the transpose of the matrix.
///
template<typename T>
inline mat2<T> transpose(const mat2<T> &a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {a.xx, a.yx, a.xy, a.yy};
}

template<typename T>
inline mat3<T> transpose(const mat3<T> &a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {a.xx, a.yx, a.zx,
            a.xy, a.yy, a.zy,
            a.xz, a.yz, a.zz};
}

template<typename T>
inline mat4<T> transpose(const mat4<T> &a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {a.xx, a.yx, a.zx, a.wx,
            a.xy, a.yy, a.zy, a.wy,
            a.xz, a.yz, a.zz, a.wz,
            a.xw, a.yw, a.zw, a.ww};
}

///
/// @brief Return the determinant of the matrix.
///
template<typename T>
inline T determinant(const mat2<T> &a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    //
    //      M = {m0, m1,
    //           m2, m3}
    // det(M) = +m0 * m3
    //          -m1 * m2
    //
    T det = a.xx * a.yy - a.xy * a.yx;
    return det;
}

template<typename T>
inline T determinant(const mat3<T> &a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    //
    //      M = {m0, m1, m2,
    //           m3, m4, m5,
    //           m6, m7, m8}
    // det(M) = +m0 * m4 * m8
    //          -m0 * m5 * m7
    //          -m1 * m3 * m8
    //          +m1 * m5 * m6
    //          +m2 * m3 * m7
    //          -m2 * m4 * m6
    //
    T minor0 = a.yy * a.zz - a.yz * a.zy;
    T minor1 = a.yz * a.zx - a.yx * a.zz;
    T minor2 = a.yx * a.zy - a.yy * a.zx;
    T det = a.xx * minor0 + a.xy * minor1 + a.xz * minor2;
    return det;
}

template<typename T>
inline T determinant(const mat4<T> &a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    //
    //      M = {m0,  m1,  m2,  m3,
    //           m4,  m5,  m6,  m7,
    //           m8,  m9,  m10, m11,
    //           m12, m13, m14, m15}
    // det(M) = -m0  * m10 * m13 * m7
    //          +m0  * m10 * m15 * m5
    //          +m0  * m11 * m13 * m6
    //          -m0  * m11 * m14 * m5
    //          +m0  * m14 * m7  * m9
    //          -m0  * m15 * m6  * m9
    //          +m1  * m10 * m12 * m7
    //          -m1  * m10 * m15 * m4
    //          -m1  * m11 * m12 * m6
    //          +m1  * m11 * m14 * m4
    //          -m1  * m14 * m7  * m8
    //          +m1  * m15 * m6  * m8
    //          -m10 * m12 * m3  * m5
    //          +m10 * m13 * m3  * m4
    //          +m11 * m12 * m2  * m5
    //          -m11 * m13 * m2  * m4
    //          -m12 * m2  * m7  * m9
    //          +m12 * m3  * m6  * m9
    //          +m13 * m2  * m7  * m8
    //          -m13 * m3  * m6  * m8
    //          -m14 * m3  * m4  * m9
    //          +m14 * m3  * m5  * m8
    //          +m15 * m2  * m4  * m9
    //          -m15 * m2  * m5  * m8
    //
    T minor0  = a.zz * a.ww - a.zw * a.wz;
    T minor1  = a.zw * a.wy - a.zy * a.ww;
    T minor2  = a.zy * a.wz - a.zz * a.wy;

    T minor3  = a.zw * a.wz - a.zz * a.ww;
    T minor4  = a.zx * a.ww - a.zw * a.wx;
    T minor5  = a.zz * a.wx - a.zx * a.wz;

    T minor6  = a.zy * a.ww - a.zw * a.wy;
    T minor7  = a.zw * a.wx - a.zx * a.ww;
    T minor8  = a.zx * a.wy - a.zy * a.wx;

    T minor9  = a.zz * a.wy - a.zy * a.wz;
    T minor10 = a.zx * a.wz - a.zz * a.wx;
    T minor11 = a.zy * a.wx - a.zx * a.wy;

    T det = a.xx * (a.yy * minor0 + a.yz * minor1  + a.yw * minor2) +
            a.xy * (a.yx * minor3 + a.yz * minor4  + a.yw * minor5) +
            a.xz * (a.yx * minor6 + a.yy * minor7  + a.yw * minor8) +
            a.xw * (a.yx * minor9 + a.yy * minor10 + a.yz * minor11);
    return det;
}

///
/// @brief Return the inverse of the matrix.
///
template<typename T>
inline mat2<T> inverse(const mat2<T> &a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    //
    // Compute the inverse matrix from the corresponding adjugate:
    // inv(M) = adj(M) / det(M)
    // adj(M) = { m3, -m1,
    //           -m2,  m0}
    //
    mat2<T> adj = { a.yy, -a.xy,
                   -a.yx,  a.xx};

    // Compute determinant from Laplace's expansion
    T det = a.xx * adj.xx + a.xy * adj.yx;
    det = std::fabs(det) > (T) 0 ? (T) 1 / det : (T) 0;

    return (adj *= det);
}

template<typename T>
inline mat3<T> inverse(const mat3<T> &a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    //
    // Compute the inverse matrix from the corresponding adjugate:
    //  inv(M) = adj(M) / det(M)
    //  adj(M) = {+m4 * m8 - m5 * m7,
    //            +m2 * m7 - m1 * m8,
    //            +m1 * m5 - m2 * m4
    //            +m5 * m6 - m3 * m8,
    //            +m0 * m8 - m2 * m6,
    //            +m2 * m3 - m0 * m5,
    //            +m3 * m7 - m4 * m6,
    //            +m1 * m6 - m0 * m7,
    //            +m0 * m4 - m1 * m3}
    //
    mat3<T> adj = {a.yy * a.zz - a.yz * a.zy,
                   a.xz * a.zy - a.xy * a.zz,
                   a.xy * a.yz - a.xz * a.yy,

                   a.yz * a.zx - a.yx * a.zz,
                   a.xx * a.zz - a.xz * a.zx,
                   a.xz * a.yx - a.xx * a.yz,

                   a.yx * a.zy - a.yy * a.zx,
                   a.xy * a.zx - a.xx * a.zy,
                   a.xx * a.yy - a.xy * a.yx};

    // Compute determinant from Laplace's expansion
    T det = a.xx * adj.xx +
            a.xy * adj.yx +
            a.xz * adj.zx;
    det = std::fabs(det) > (T) 0 ? (T) 1 / det : (T) 0;

    return (adj *= det);
}

template<typename T>
inline mat4<T> inverse(const mat4<T> &a)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    //
    // Compute the inverse matrix from the corresponding adjugate:
    // inv(M) = adj(M) / det(M)
    // adj(M) = {
    //      -m11*m14*m5  +m10*m15*m5  +m11*m13*m6
    //      -m10*m13*m7  -m15*m6 *m9  +m14*m7 *m9,
    //
    //      +m1 *m11*m14 -m3 *m9 *m14 -m1 *m10*m15
    //      -m11*m13*m2  +m10*m13*m3  +m15*m2 *m9,
    //
    //      -m15*m2 *m5  +m14*m3 *m5  +m1 *m15*m6
    //      -m13*m3 *m6  -m1 *m14*m7  +m13*m2 *m7,
    //
    //      +m11*m2 *m5  -m10*m3 *m5  -m1 *m11*m6
    //      +m1 *m10*m7  +m3 *m6 *m9  -m2 *m7 *m9,
    //
    //      +m11*m14*m4  -m10*m15*m4  -m11*m12*m6
    //      +m10*m12*m7  +m15*m6 *m8  -m14*m7 *m8,
    //
    //      -m0 *m11*m14 +m3 *m8 *m14 +m0 *m10*m15
    //      +m11*m12*m2  -m10*m12*m3  -m15*m2 *m8,
    //
    //      +m15*m2 *m4  -m14*m3 *m4  -m0 *m15*m6
    //      +m12*m3 *m6  +m0 *m14*m7  -m12*m2 *m7,
    //
    //      -m11*m2 *m4  +m10*m3 *m4  +m0 *m11*m6
    //      -m0 *m10*m7  -m3 *m6 *m8  +m2 *m7 *m8,
    //
    //      -m11*m13*m4  +m15*m9 *m4  +m11*m12*m5
    //      -m15*m5 *m8  +m13*m7 *m8  -m12*m7 *m9,
    //
    //      -m1 *m11*m12 +m3 *m9 *m12 +m0 *m11*m13
    //      +m1 *m15*m8  -m13*m3 *m8  -m0 *m15*m9,
    //
    //      -m1 *m15*m4  +m13*m3 *m4  +m0 *m15*m5
    //      -m12*m3 *m5  +m1 *m12*m7  -m0 *m13*m7,
    //
    //      +m1 *m11*m4  -m3 *m9 *m4  -m0 *m11*m5
    //      +m3 *m5 *m8  -m1 *m7 *m8  +m0 *m7 *m9,
    //
    //      +m10*m13*m4  -m14*m9 *m4  -m10*m12*m5
    //      +m14*m5 *m8  -m13*m6 *m8  +m12*m6 *m9,
    //
    //      +m1 *m10*m12 -m2 *m9 *m12 -m0 *m10*m13
    //      -m1 *m14*m8  +m13*m2 *m8  +m0 *m14*m9,
    //
    //      +m1 *m14*m4  -m13*m2 *m4  -m0 *m14*m5
    //      +m12*m2 *m5  -m1 *m12*m6  +m0 *m13*m6,
    //
    //      -m1 *m10*m4  +m2 *m9 *m4  +m0 *m10*m5
    //      -m2 *m5 *m8  +m1 *m6 *m8  -m0 *m6 *m9}
    //
    mat4<T> adj = {a.yy * (a.zz * a.ww - a.zw * a.wz) +
                   a.yz * (a.zw * a.wy - a.zy * a.ww) +
                   a.yw * (a.zy * a.wz - a.zz * a.wy),

                   a.xy * (a.zw * a.wz - a.zz * a.ww) +
                   a.xz * (a.zy * a.ww - a.zw * a.wy) +
                   a.xw * (a.zz * a.wy - a.zy * a.wz),

                   a.yy * (a.xw * a.wz - a.xz * a.ww) +
                   a.yz * (a.xy * a.ww - a.xw * a.wy) +
                   a.yw * (a.xz * a.wy - a.xy * a.wz),

                   a.yy * (a.xz * a.zw - a.zz * a.xw) +
                   a.yz * (a.xw * a.zy - a.xy * a.zw) +
                   a.yw * (a.xy * a.zz - a.xz * a.zy),

                   a.yx * (a.zw * a.wz - a.zz * a.ww) +
                   a.yz * (a.zx * a.ww - a.zw * a.wx) +
                   a.yw * (a.zz * a.wx - a.zx * a.wz),

                   a.xx * (a.zz * a.ww - a.zw * a.wz) +
                   a.xz * (a.zw * a.wx - a.zx * a.ww) +
                   a.xw * (a.zx * a.wz - a.zz * a.wx),

                   a.xx * (a.yw * a.wz - a.yz * a.ww) +
                   a.xz * (a.yx * a.ww - a.yw * a.wx) +
                   a.xw * (a.yz * a.wx - a.yx * a.wz),

                   a.xx * (a.yz * a.zw - a.yw * a.zz) +
                   a.xz * (a.yw * a.zx - a.yx * a.zw) +
                   a.xw * (a.yx * a.zz - a.yz * a.zx),

                   a.yx * (a.zy * a.ww - a.zw * a.wy) +
                   a.yy * (a.zw * a.wx - a.zx * a.ww) +
                   a.yw * (a.zx * a.wy - a.zy * a.wx),

                   a.xx * (a.zw * a.wy - a.zy * a.ww) +
                   a.xy * (a.zx * a.ww - a.zw * a.wx) +
                   a.xw * (a.zy * a.wx - a.zx * a.wy),

                   a.xx * (a.yy * a.ww - a.yw * a.wy) +
                   a.xy * (a.yw * a.wx - a.yx * a.ww) +
                   a.xw * (a.yx * a.wy - a.yy * a.wx),

                   a.xx * (a.yw * a.zy - a.yy * a.zw) +
                   a.xy * (a.yx * a.zw - a.yw * a.zx) +
                   a.xw * (a.yy * a.zx - a.zy * a.yx),

                   a.yx * (a.zz * a.wy - a.zy * a.wz) +
                   a.yy * (a.zx * a.wz - a.zz * a.wx) +
                   a.yz * (a.zy * a.wx - a.zx * a.wy),

                   a.xx * (a.zy * a.wz - a.zz * a.wy) +
                   a.xy * (a.zz * a.wx - a.zx * a.wz) +
                   a.xz * (a.zx * a.wy - a.zy * a.wx),

                   a.xx * (a.yz * a.wy - a.yy * a.wz) +
                   a.xy * (a.yx * a.wz - a.yz * a.wx) +
                   a.xz * (a.yy * a.wx - a.yx * a.wy),

                   a.xx * (a.yy * a.zz - a.yz * a.zy) +
                   a.xy * (a.yz * a.zx - a.yx * a.zz) +
                   a.xz * (a.yx * a.zy - a.yy * a.zx)};

    // Compute inverse matrix using Laplace's expansion
    T det = a.xx * adj.xx +
            a.xy * adj.yx +
            a.xz * adj.zx +
            a.xw * adj.wx;
    det = std::fabs(det) > (T) 0 ? (T) 1 / det : (T) 0;

    return (adj *= det);
}

} // math

/// ---- simd implementations ------------------------------------------------
#ifdef __AVX__
#include "simd/algebra.h"
#endif

#endif // MATH_ALGEBRA_H_
