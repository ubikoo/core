//
// math.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "math.h"

namespace Math {

/// Vec2 explicit instantiation
template struct Vec2<int16_t>;
template struct Vec2<int32_t>;
template struct Vec2<int64_t>;
template struct Vec2<uint16_t>;
template struct Vec2<uint32_t>;
template struct Vec2<uint64_t>;
template struct Vec2<float>;
template struct Vec2<double>;

/// Vec3 explicit instantiation
template struct Vec3<int16_t>;
template struct Vec3<int32_t>;
template struct Vec3<int64_t>;
template struct Vec3<uint16_t>;
template struct Vec3<uint32_t>;
template struct Vec3<uint64_t>;
template struct Vec3<float>;
template struct Vec3<double>;

/// Vec4 explicit instantiation
template struct Vec4<int16_t>;
template struct Vec4<int32_t>;
template struct Vec4<int64_t>;
template struct Vec4<uint16_t>;
template struct Vec4<uint32_t>;
template struct Vec4<uint64_t>;
template struct Vec4<float>;
template struct Vec4<double>;

/// Mat2 explicit instantiation
template struct Mat2<int16_t>;
template struct Mat2<int32_t>;
template struct Mat2<int64_t>;
template struct Mat2<uint16_t>;
template struct Mat2<uint32_t>;
template struct Mat2<uint64_t>;
template struct Mat2<float>;
template struct Mat2<double>;

/// Mat3 explicit instantiation
template struct Mat3<int16_t>;
template struct Mat3<int32_t>;
template struct Mat3<int64_t>;
template struct Mat3<uint16_t>;
template struct Mat3<uint32_t>;
template struct Mat3<uint64_t>;
template struct Mat3<float>;
template struct Mat3<double>;

/// Mat4 explicit instantiation
template struct Mat4<int16_t>;
template struct Mat4<int32_t>;
template struct Mat4<int64_t>;
template struct Mat4<uint16_t>;
template struct Mat4<uint32_t>;
template struct Mat4<uint64_t>;
template struct Mat4<float>;
template struct Mat4<double>;

/// ortho explicit instantiation
template struct Ortho<float>;
template struct Ortho<double>;

} // namespace Math
