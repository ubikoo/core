//
// ortho.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MATH_ORTHO_H_
#define MATH_ORTHO_H_

#include <cmath>
#include <limits>
#include <type_traits>
#include "vector.h"
#include "matrix.h"

namespace Math {

/// -----------------------------------------------------------------------------
/// @brief Plain data type representing an orthonormal basis.
///
template<typename T>
struct Ortho {
    static_assert(std::is_floating_point<T>::value, "non floating point");

    // Create an orthonormal basis from a single fixed vector.
    static Ortho<T> CreateFromU(const Vec3<T> &e_u);
    static Ortho<T> CreateFromV(const Vec3<T> &e_v);
    static Ortho<T> CreateFromW(const Vec3<T> &e_w);

    //
    // Create an orthonormal basis from two vectors. The first vector is fixed,
    // the second is projected in a direction orthogonal to the first vector.
    //
    static Ortho<T> CreateFromUV(const Vec3<T> &e_u, const Vec3<T> &e_v);
    static Ortho<T> CreateFromVU(const Vec3<T> &e_v, const Vec3<T> &e_u);

    static Ortho<T> CreateFromVW(const Vec3<T> &e_v, const Vec3<T> &e_w);
    static Ortho<T> CreateFromWV(const Vec3<T> &e_w, const Vec3<T> &e_v);

    static Ortho<T> CreateFromWU(const Vec3<T> &e_w, const Vec3<T> &e_u);
    static Ortho<T> CreateFromUW(const Vec3<T> &e_u, const Vec3<T> &e_w);

    // Return the projection of the vector from world to local space.
    Vec3<T> WorldToLocal(const Vec3<T> &a) const;

    // Return the projection of the vector from local to world space.
    Vec3<T> LocalToWorld(const Vec3<T> &a) const;

    // Orthonormal basis vectors.
    Vec3<T> u;
    Vec3<T> v;
    Vec3<T> w;
};

typedef Ortho<float>  Orthof;
typedef Ortho<double> Orthod;

/// -----------------------------------------------------------------------------
/// @brief Create an orthonormal basis set given the fixed vector u.
///
template<typename T>
inline Ortho<T> Ortho<T>::CreateFromU(const Vec3<T> &e_u)
{
    static const T eps = std::sqrt(std::numeric_limits<T>::epsilon());
    static const Vec3<T> e_1{(T) 1, (T) 0, (T) 0};
    static const Vec3<T> e_2{(T) 0, (T) 1, (T) 0};

    Ortho<T> o;
    o.u = Normalize(e_u);               // u = u / |u|
    o.v = Cross(o.u, e_1);              // v = u x e_1
    if (Norm(o.v) < eps) {
        o.v = Cross(o.u, e_2);          // v = u x e_2
    }
    o.v = Normalize(o.v);
    o.w = Normalize(Cross(o.u, o.v));   // w = <u x v>
    return o;
}

///
/// @brief Create an orthonormal basis set given the fixed vector v.
///
template<typename T>
inline Ortho<T> Ortho<T>::CreateFromV(const Vec3<T> &e_v)
{
    static const T eps = std::sqrt(std::numeric_limits<T>::epsilon());
    static const Vec3<T> e_1{(T) 1, (T) 0, (T) 0};
    static const Vec3<T> e_2{(T) 0, (T) 1, (T) 0};

    Ortho<T> o;
    o.v = Normalize(e_v);               // v = v / |v|
    o.w = Cross(o.v, e_1);              // w = v x e_1
    if (Norm(o.w) < eps) {
        o.w = Cross(o.v, e_2);          // w = v x e_2
    }
    o.w = Normalize(o.w);
    o.u = Normalize(Cross(o.v, o.w));   // u = <v x w>
    return o;
}

///
/// @brief Create an orthonormal basis set given the fixed vector w.
///
template<typename T>
inline Ortho<T> Ortho<T>::CreateFromW(const Vec3<T> &e_w)
{
    static const T eps = std::sqrt(std::numeric_limits<T>::epsilon());
    static const Vec3<T> e_1{(T) 1, (T) 0, (T) 0};
    static const Vec3<T> e_2{(T) 0, (T) 1, (T) 0};

    Ortho<T> o;
    o.w = Normalize(e_w);               // w = w / |w|
    o.u = Cross(o.w, e_1);              // u = w x e_1
    if (Norm(o.u) < eps) {
        o.u = Cross(o.w, e_2);          // u = w x e_2
    }
    o.u = Normalize(o.u);
    o.v = Normalize(Cross(o.w, o.u));   // v = <w x u>
    return o;
}

/// -----------------------------------------------------------------------------
/// @brief Create an orthonormal basis set from two vectors:
///  u is fixed.
///  v is orthogonalized using a Gram-Schmidt process.
///
template<typename T>
inline Ortho<T> Ortho<T>::CreateFromUV(const Vec3<T> &e_u, const Vec3<T> &e_v)
{
    Ortho<T> o;
    o.u = Normalize(e_u);                           // u = u / |u|
    o.v = Normalize(e_v - (o.u * Dot(e_v, o.u)));   // v = <v - (v.u)*u>
    o.w = Normalize(Cross(o.u, o.v));               // w = u x v
    return o;
}

///
/// @brief Create an orthonormal basis set from two vectors:
///  v is fixed.
///  u is orthogonalized using a Gram-Schmidt process.
///
template<typename T>
inline Ortho<T> Ortho<T>::CreateFromVU(
    const Vec3<T> &e_v, const Vec3<T> &e_u)
{
    Ortho<T> o;
    o.v = Normalize(e_v);                           // v = v / |v|
    o.u = Normalize(e_u - (o.v * Dot(e_u, o.v)));   // u = <u - (u.v)*v>
    o.w = Normalize(Cross(o.u, o.v));               // w = u x v
    return o;
}

/// -----------------------------------------------------------------------------
/// @brief Create an orthonormal basis set from two vectors:
///  v is fixed.
///  w is orthogonalized using a Gram-Schmidt process.
///
template<typename T>
inline Ortho<T> Ortho<T>::CreateFromVW(const Vec3<T> &e_v, const Vec3<T> &e_w)
{
    Ortho<T> o;
    o.v = Normalize(e_v);                           // v = v / |v|
    o.w = Normalize(e_w - (o.v * Dot(e_w, o.v)));   // w = <w - (w.v)*v>
    o.u = Normalize(Cross(o.v, o.w));               // u = <v x w>
    return o;
}

///
/// @brief Create an orthonormal basis set from two vectors:
///  w is fixed.
///  v is orthogonalized using a Gram-Schmidt process.
///
template<typename T>
inline Ortho<T> Ortho<T>::CreateFromWV(
    const Vec3<T> &e_w, const Vec3<T> &e_v)
{
    Ortho<T> o;
    o.w = Normalize(e_w);                           // w = w / |w|
    o.v = Normalize(e_v - (o.w * Dot(e_v, o.w)));   // v = <v - (v.w)*w>
    o.u = Normalize(Cross(o.v, o.w));               // u = <v x w>
    return o;
}

/// -----------------------------------------------------------------------------
/// @brief Create an orthonormal basis set from two vectors:
///  w is fixed.
///  u is orthogonalized using a Gram-Schmidt process.
///
template<typename T>
inline Ortho<T> Ortho<T>::CreateFromWU(
    const Vec3<T> &e_w, const Vec3<T> &e_u)
{
    Ortho<T> o;
    o.w = Normalize(e_w);                           // w = w / |w|
    o.u = Normalize(e_u - (o.w * Dot(e_u, o.w)));   // u = <u - (u.w)*w>
    o.v = Normalize(Cross(o.w, o.u));               // v = <w x u>
    return o;
}

///
/// @brief Create an orthonormal basis set from two vectors:
///  u is fixed.
///  w is orthogonalized using a Gram-Schmidt process.
///
template<typename T>
inline Ortho<T> Ortho<T>::CreateFromUW(const Vec3<T> &e_u, const Vec3<T> &e_w)
{
    Ortho<T> o;
    o.u = Normalize(e_u);                           // u = u / |u|
    o.w = Normalize(e_w - (o.u * Dot(e_w, o.u)));   // w = <w - (u.w)*u>
    o.v = Normalize(Cross(o.w, o.u));               // v = <w x u>
    return o;
}

/// -----------------------------------------------------------------------------
/// @brief Project a vector coordinates from world to local
/// reference frame defined by the orthonormal basis set.
///
template<typename T>
inline Vec3<T> Ortho<T>::WorldToLocal(const Vec3<T> &a) const
{
    return {u.x * a.x + u.y * a.y + u.z * a.z,
            v.x * a.x + v.y * a.y + v.z * a.z,
            w.x * a.x + w.y * a.y + w.z * a.z};
}

///
/// @brief Project a vector coordinates from local to world
/// reference frame defined by the orthonormal basis set.
///
template<typename T>
inline Vec3<T> Ortho<T>::LocalToWorld(const Vec3<T> &a) const
{
    return {u.x * a.x + v.x * a.y + w.x * a.z,
            u.y * a.x + v.y * a.y + w.y * a.z,
            u.z * a.x + v.z * a.y + w.z * a.z};
}

} // namespace Math

#endif // MATH_ORTHO_H_
