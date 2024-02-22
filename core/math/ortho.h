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
#include <type_traits>
#include "vector.h"
#include "matrix.h"

namespace math {

/// -----------------------------------------------------------------------------
/// @brief Plain data type representing an orthonormal basis.
///
template<typename T>
struct ortho {
    static_assert(std::is_floating_point<T>::value, "non floating point");

    // Create an orthonormal basis from a single fixed vector.
    static ortho<T> create_from_u(const vec3<T> &e_u);
    static ortho<T> create_from_v(const vec3<T> &e_v);
    static ortho<T> create_from_w(const vec3<T> &e_w);

    //
    // Create an orthonormal basis from two vectors. The first vector is fixed,
    // the second is projected in a direction orthogonal to the first vector.
    //
    static ortho<T> create_from_uv(const vec3<T> &e_u, const vec3<T> &e_v);
    static ortho<T> create_from_vu(const vec3<T> &e_v, const vec3<T> &e_u);

    static ortho<T> create_from_vw(const vec3<T> &e_v, const vec3<T> &e_w);
    static ortho<T> create_from_wv(const vec3<T> &e_w, const vec3<T> &e_v);

    static ortho<T> create_from_wu(const vec3<T> &e_w, const vec3<T> &e_u);
    static ortho<T> create_from_uw(const vec3<T> &e_u, const vec3<T> &e_w);

    // Return the projection of the vector from world to local space.
    vec3<T> world_to_local(const vec3<T> &a) const;

    // Return the projection of the vector from local to world space.
    vec3<T> local_to_world(const vec3<T> &a) const;

    // Orthonormal basis vectors.
    vec3<T> u;
    vec3<T> v;
    vec3<T> w;
};

typedef ortho<float>  orthof;
typedef ortho<double> orthod;

/// -----------------------------------------------------------------------------
/// ortho<T>::create_from_u
/// @brief Create an orthonormal basis set given the fixed vector u.
///
template<typename T>
inline ortho<T> ortho<T>::create_from_u(const vec3<T> &e_u)
{
    static const T eps = std::sqrt(std::numeric_limits<T>::epsilon());
    static const vec3<T> e_1{(T) 1, (T) 0, (T) 0};
    static const vec3<T> e_2{(T) 0, (T) 1, (T) 0};

    ortho<T> o;
    o.u = normalize(e_u);               // u = u / |u|
    o.v = cross(o.u, e_1);              // v = u x e_1
    if (norm(o.v) < eps) {
        o.v = cross(o.u, e_2);          // v = u x e_2
    }
    o.v = normalize(o.v);
    o.w = normalize(cross(o.u, o.v));   // w = <u x v>
    return o;
}

///
/// ortho<T>::create_from_v
/// @brief Create an orthonormal basis set given the fixed vector v.
///
template<typename T>
inline ortho<T> ortho<T>::create_from_v(const vec3<T> &e_v)
{
    static const T eps = std::sqrt(std::numeric_limits<T>::epsilon());
    static const vec3<T> e_1{(T) 1, (T) 0, (T) 0};
    static const vec3<T> e_2{(T) 0, (T) 1, (T) 0};

    ortho<T> o;
    o.v = normalize(e_v);               // v = v / |v|
    o.w = cross(o.v, e_1);              // w = v x e_1
    if (norm(o.w) < eps) {
        o.w = cross(o.v, e_2);          // w = v x e_2
    }
    o.w = normalize(o.w);
    o.u = normalize(cross(o.v, o.w));   // u = <v x w>
    return o;
}

///
/// ortho<T>::create_from_w
/// @brief Create an orthonormal basis set given the fixed vector w.
///
template<typename T>
inline ortho<T> ortho<T>::create_from_w(const vec3<T> &e_w)
{
    static const T eps = std::sqrt(std::numeric_limits<T>::epsilon());
    static const vec3<T> e_1{(T) 1, (T) 0, (T) 0};
    static const vec3<T> e_2{(T) 0, (T) 1, (T) 0};

    ortho<T> o;
    o.w = normalize(e_w);               // w = w / |w|
    o.u = cross(o.w, e_1);              // u = w x e_1
    if (norm(o.u) < eps) {
        o.u = cross(o.w, e_2);          // u = w x e_2
    }
    o.u = normalize(o.u);
    o.v = normalize(cross(o.w, o.u));   // v = <w x u>
    return o;
}

/// -----------------------------------------------------------------------------
/// ortho<T>::create_from_uv
/// @brief Create an orthonormal basis set from two vectors:
///  u is fixed.
///  v is orthogonalized using a Gram-Schmidt process.
///
template<typename T>
inline ortho<T> ortho<T>::create_from_uv(
    const vec3<T> &e_u, const vec3<T> &e_v)
{
    ortho<T> o;
    o.u = normalize(e_u);                           // u = u / |u|
    o.v = normalize(e_v - (o.u * dot(e_v, o.u)));   // v = <v - (v.u)*u>
    o.w = normalize(cross(o.u, o.v));               // w = u x v
    return o;
}

///
/// ortho<T>::create_from_vu
/// @brief Create an orthonormal basis set from two vectors:
///  v is fixed.
///  u is orthogonalized using a Gram-Schmidt process.
///
template<typename T>
inline ortho<T> ortho<T>::create_from_vu(
    const vec3<T> &e_v, const vec3<T> &e_u)
{
    ortho<T> o;
    o.v = normalize(e_v);                           // v = v / |v|
    o.u = normalize(e_u - (o.v * dot(e_u, o.v)));   // u = <u - (u.v)*v>
    o.w = normalize(cross(o.u, o.v));               // w = u x v
    return o;
}

/// -----------------------------------------------------------------------------
/// ortho<T>::create_from_vw
/// @brief Create an orthonormal basis set from two vectors:
///  v is fixed.
///  w is orthogonalized using a Gram-Schmidt process.
///
template<typename T>
inline ortho<T> ortho<T>::create_from_vw(
    const vec3<T> &e_v, const vec3<T> &e_w)
{
    ortho<T> o;
    o.v = normalize(e_v);                           // v = v / |v|
    o.w = normalize(e_w - (o.v * dot(e_w, o.v)));   // w = <w - (w.v)*v>
    o.u = normalize(cross(o.v, o.w));               // u = <v x w>
    return o;
}

///
/// ortho<T>::create_from_wv
/// @brief Create an orthonormal basis set from two vectors:
///  w is fixed.
///  v is orthogonalized using a Gram-Schmidt process.
///
template<typename T>
inline ortho<T> ortho<T>::create_from_wv(
    const vec3<T> &e_w, const vec3<T> &e_v)
{
    ortho<T> o;
    o.w = normalize(e_w);                           // w = w / |w|
    o.v = normalize(e_v - (o.w * dot(e_v, o.w)));   // v = <v - (v.w)*w>
    o.u = normalize(cross(o.v, o.w));               // u = <v x w>
    return o;
}

/// -----------------------------------------------------------------------------
/// ortho<T>::create_from_wu
/// @brief Create an orthonormal basis set from two vectors:
///  w is fixed.
///  u is orthogonalized using a Gram-Schmidt process.
///
template<typename T>
inline ortho<T> ortho<T>::create_from_wu(
    const vec3<T> &e_w, const vec3<T> &e_u)
{
    ortho<T> o;
    o.w = normalize(e_w);                           // w = w / |w|
    o.u = normalize(e_u - (o.w * dot(e_u, o.w)));   // u = <u - (u.w)*w>
    o.v = normalize(cross(o.w, o.u));               // v = <w x u>
    return o;
}

///
/// ortho<T>::create_from_uw
/// @brief Create an orthonormal basis set from two vectors:
///  u is fixed.
///  w is orthogonalized using a Gram-Schmidt process.
///
template<typename T>
inline ortho<T> ortho<T>::create_from_uw(
    const vec3<T> &e_u, const vec3<T> &e_w)
{
    ortho<T> o;
    o.u = normalize(e_u);                           // u = u / |u|
    o.w = normalize(e_w - (o.u * dot(e_w, o.u)));   // w = <w - (u.w)*u>
    o.v = normalize(cross(o.w, o.u));               // v = <w x u>
    return o;
}

/// -----------------------------------------------------------------------------
/// ortho<T>::world_to_local
/// @brief Project a vector coordinates from world to local
/// reference frame defined by the orthonormal basis set.
///
template<typename T>
inline vec3<T> ortho<T>::world_to_local(const vec3<T> &a) const
{
    return {u.x * a.x + u.y * a.y + u.z * a.z,
            v.x * a.x + v.y * a.y + v.z * a.z,
            w.x * a.x + w.y * a.y + w.z * a.z};
}

///
/// ortho<T>::local_to_world
/// @brief Project a vector coordinates from local to world
/// reference frame defined by the orthonormal basis set.
///
template<typename T>
inline vec3<T> ortho<T>::local_to_world(const vec3<T> &a) const
{
    return {u.x * a.x + v.x * a.y + w.x * a.z,
            u.y * a.x + v.y * a.y + w.y * a.z,
            u.z * a.x + v.z * a.y + w.z * a.z};
}

} // math

#endif // MATH_ORTHO_H_
