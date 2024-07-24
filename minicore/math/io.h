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

namespace Math {

/// -----------------------------------------------------------------------------
/// @brief Vector and matrix output stream operators.
///
template<typename T>
inline std::ostream &operator<< (std::ostream &os, const Vec2<T> &v)
{
    os << v.x << " " << v.y << "\n";
    return os;
}

template<typename T>
inline std::ostream &operator<< (std::ostream &os, const Vec3<T> &v)
{
    os << v.x << " " << v.y << " " << v.z << "\n";
    return os;
}

template<typename T>
inline std::ostream &operator<< (std::ostream &os, const Vec4<T> &v)
{
    os << v.x << " " << v.y << " " << v.z << " " << v.w << "\n";
    return os;
}

template<typename T>
inline std::ostream &operator<< (std::ostream &os, const Mat2<T> &a)
{
    os << a.xx << " " << a.xy << "\n"
       << a.yx << " " << a.yy << "\n";
    return os;
}

template<typename T>
inline std::ostream &operator<< (std::ostream &os, const Mat3<T> &a)
{
    os << a.xx << " " << a.xy << " " << a.xz << "\n"
       << a.yx << " " << a.yy << " " << a.yz << "\n"
       << a.zx << " " << a.zy << " " << a.zz << "\n";
    return os;
}

template<typename T>
inline std::ostream &operator<< (std::ostream &os, const Mat4<T> &a)
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
inline std::string ToString(const T v)
{
    return std::to_string(v);
}

template<typename T>
inline std::string ToString(const Vec2<T> &v)
{
    std::ostringstream ss;
    ss << ToString(v.x) << " ";
    ss << ToString(v.y) << " ";
    return ss.str();
}

template<typename T>
inline std::string ToString(const Vec3<T> &v)
{
    std::ostringstream ss;
    ss << ToString(v.x) << " ";
    ss << ToString(v.y) << " ";
    ss << ToString(v.z) << " ";
    return ss.str();
}

template<typename T>
inline std::string ToString(const Vec4<T> &v)
{
    std::ostringstream ss;
    ss << ToString(v.x) << " ";
    ss << ToString(v.y) << " ";
    ss << ToString(v.z) << " ";
    ss << ToString(v.w) << " ";
    return ss.str();
}

template<typename T>
inline std::string ToString(const Mat2<T> &a)
{
    std::ostringstream ss;
    ss << ToString(a.xx) << " ";
    ss << ToString(a.xy) << "\n";

    ss << ToString(a.yx) << " ";
    ss << ToString(a.yy) << " ";
    return ss.str();
}

template<typename T>
inline std::string ToString(const Mat3<T> &a)
{
    std::ostringstream ss;
    ss << ToString(a.xx) << " ";
    ss << ToString(a.xy) << " ";
    ss << ToString(a.xz) << "\n";

    ss << ToString(a.yx) << " ";
    ss << ToString(a.yy) << " ";
    ss << ToString(a.yz) << "\n";

    ss << ToString(a.zx) << " ";
    ss << ToString(a.zy) << " ";
    ss << ToString(a.zz) << " ";
    return ss.str();
}

template<typename T>
inline std::string ToString(const Mat4<T> &a)
{
    std::ostringstream ss;
    ss << ToString(a.xx) << " ";
    ss << ToString(a.xy) << " ";
    ss << ToString(a.xz) << " ";
    ss << ToString(a.xw) << "\n";

    ss << ToString(a.yx) << " ";
    ss << ToString(a.yy) << " ";
    ss << ToString(a.yz) << " ";
    ss << ToString(a.yw) << "\n";

    ss << ToString(a.zx) << " ";
    ss << ToString(a.zy) << " ";
    ss << ToString(a.zz) << " ";
    ss << ToString(a.zw) << "\n";

    ss << ToString(a.wx) << " ";
    ss << ToString(a.wy) << " ";
    ss << ToString(a.wz) << " ";
    ss << ToString(a.ww) << " ";
    return ss.str();
}

///
/// @brief Return a string serialization of the orthonormal vectors.
///
template<typename T>
inline std::string ToString(const Ortho<T> &o)
{
    std::ostringstream ss;
    ss << ToString(o.u) << "\n";
    ss << ToString(o.v) << "\n";
    ss << ToString(o.w) << "\n";
    return ss.str();
}
} // namespace Math

#endif // MATH_IO_H_
