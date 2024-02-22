//
// math.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "math.h"

namespace math {

/// vec2 explicit instantiation
template struct vec2<int16_t>;
template struct vec2<int32_t>;
template struct vec2<int64_t>;
template struct vec2<uint16_t>;
template struct vec2<uint32_t>;
template struct vec2<uint64_t>;
template struct vec2<float>;
template struct vec2<double>;

/// vec3 explicit instantiation
template struct vec3<int16_t>;
template struct vec3<int32_t>;
template struct vec3<int64_t>;
template struct vec3<uint16_t>;
template struct vec3<uint32_t>;
template struct vec3<uint64_t>;
template struct vec3<float>;
template struct vec3<double>;

/// vec4 explicit instantiation
template struct vec4<int16_t>;
template struct vec4<int32_t>;
template struct vec4<int64_t>;
template struct vec4<uint16_t>;
template struct vec4<uint32_t>;
template struct vec4<uint64_t>;
template struct vec4<float>;
template struct vec4<double>;

/// mat2 explicit instantiation
template struct mat2<int16_t>;
template struct mat2<int32_t>;
template struct mat2<int64_t>;
template struct mat2<uint16_t>;
template struct mat2<uint32_t>;
template struct mat2<uint64_t>;
template struct mat2<float>;
template struct mat2<double>;

/// mat3 explicit instantiation
template struct mat3<int16_t>;
template struct mat3<int32_t>;
template struct mat3<int64_t>;
template struct mat3<uint16_t>;
template struct mat3<uint32_t>;
template struct mat3<uint64_t>;
template struct mat3<float>;
template struct mat3<double>;

/// mat4 explicit instantiation
template struct mat4<int16_t>;
template struct mat4<int32_t>;
template struct mat4<int64_t>;
template struct mat4<uint16_t>;
template struct mat4<uint32_t>;
template struct mat4<uint64_t>;
template struct mat4<float>;
template struct mat4<double>;

/// ortho explicit instantiation
template struct ortho<float>;
template struct ortho<double>;

} // math