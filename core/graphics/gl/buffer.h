//
// buffer.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_GL_BUFFER_H_
#define GRAPHICS_GL_BUFFER_H_

#include "core/graphics/common.h"

namespace Graphics {

///
/// @brief Create a buffer bound to a target, with size in bytes, and usage.
///
struct BufferCreateInfo {
    GLenum target;
    GLsizeiptr size;
    GLenum usage;
};
GLuint CreateBuffer(const BufferCreateInfo &info);

///
/// @brief Delete a buffer object.
///
void DestroyBuffer(const GLuint &buffer);

} // Graphics

#endif // GRAPHICS_GL_BUFFER_H_
