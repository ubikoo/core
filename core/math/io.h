//
// io.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MATH_IO_H_
#define MATH_IO_H_

#include <string>
#include <iostream>
#include <sstream>
#include "vector.h"
#include "matrix.h"
#include "ortho.h"

namespace math {

/// -----------------------------------------------------------------------------
/// @brief Vector and matrix output stream operators.
///
template<typename T>
inline std::ostream &operator<< (std::ostream &os, const vec2<T> &v)
{
    os << v.x << " " << v.y << "\n";
    return os;
}

template<typename T>
inline std::ostream &operator<< (std::ostream &os, const vec3<T> &v)
{
    os << v.x << " " << v.y << " " << v.z << "\n";
    return os;
}

template<typename T>
inline std::ostream &operator<< (std::ostream &os, const vec4<T> &v)
{
    os << v.x << " " << v.y << " " << v.z << " " << v.w << "\n";
    return os;
}

template<typename T>
inline std::ostream &operator<< (std::ostream &os, const mat2<T> &a)
{
    os << a.xx << " " << a.xy << "\n"
       << a.yx << " " << a.yy << "\n";
    return os;
}

template<typename T>
inline std::ostream &operator<< (std::ostream &os, const mat3<T> &a)
{
    os << a.xx << " " << a.xy << " " << a.xz << "\n"
       << a.yx << " " << a.yy << " " << a.yz << "\n"
       << a.zx << " " << a.zy << " " << a.zz << "\n";
    return os;
}

template<typename T>
inline std::ostream &operator<< (std::ostream &os, const mat4<T> &a)
{
    os << a.xx << " " << a.xy << " " << a.xz << " " << a.xw << "\n"
       << a.yx << " " << a.yy << " " << a.yz << " " << a.yw << "\n"
       << a.zx << " " << a.zy << " " << a.zz << " " << a.zw << "\n"
       << a.wx << " " << a.wy << " " << a.wz << " " << a.ww << "\n";
   return os;
}

/// -----------------------------------------------------------------------------
/// @brief Vector and matrix string serialization functions.
///
template<typename T>
inline std::string to_string(const T v)
{
    return std::to_string(v);
}

template<typename T>
inline std::string to_string(const vec2<T> &v)
{
    std::ostringstream ss;
    ss << to_string(v.x) << " ";
    ss << to_string(v.y) << " ";
    return ss.str();
}

template<typename T>
inline std::string to_string(const vec3<T> &v)
{
    std::ostringstream ss;
    ss << to_string(v.x) << " ";
    ss << to_string(v.y) << " ";
    ss << to_string(v.z) << " ";
    return ss.str();
}

template<typename T>
inline std::string to_string(const vec4<T> &v)
{
    std::ostringstream ss;
    ss << to_string(v.x) << " ";
    ss << to_string(v.y) << " ";
    ss << to_string(v.z) << " ";
    ss << to_string(v.w) << " ";
    return ss.str();
}

template<typename T>
inline std::string to_string(const mat2<T> &a)
{
    std::ostringstream ss;
    ss << to_string(a.xx) << " ";
    ss << to_string(a.xy) << "\n";

    ss << to_string(a.yx) << " ";
    ss << to_string(a.yy) << " ";
    return ss.str();
}

template<typename T>
inline std::string to_string(const mat3<T> &a)
{
    std::ostringstream ss;
    ss << to_string(a.xx) << " ";
    ss << to_string(a.xy) << " ";
    ss << to_string(a.xz) << "\n";

    ss << to_string(a.yx) << " ";
    ss << to_string(a.yy) << " ";
    ss << to_string(a.yz) << "\n";

    ss << to_string(a.zx) << " ";
    ss << to_string(a.zy) << " ";
    ss << to_string(a.zz) << " ";
    return ss.str();
}

template<typename T>
inline std::string to_string(const mat4<T> &a)
{
    std::ostringstream ss;
    ss << to_string(a.xx) << " ";
    ss << to_string(a.xy) << " ";
    ss << to_string(a.xz) << " ";
    ss << to_string(a.xw) << "\n";

    ss << to_string(a.yx) << " ";
    ss << to_string(a.yy) << " ";
    ss << to_string(a.yz) << " ";
    ss << to_string(a.yw) << "\n";

    ss << to_string(a.zx) << " ";
    ss << to_string(a.zy) << " ";
    ss << to_string(a.zz) << " ";
    ss << to_string(a.zw) << "\n";

    ss << to_string(a.wx) << " ";
    ss << to_string(a.wy) << " ";
    ss << to_string(a.wz) << " ";
    ss << to_string(a.ww) << " ";
    return ss.str();
}

///
/// @brief Return a string serialization of the orthonormal vectors.
///
template<typename T>
inline std::string to_string(const ortho<T> &o)
{
    std::ostringstream ss;
    ss << to_string(o.u) << "\n";
    ss << to_string(o.v) << "\n";
    ss << to_string(o.w) << "\n";
    return ss.str();
}
} // math

#endif // MATH_IO_H_
