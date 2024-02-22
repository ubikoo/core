//
// renderbuffer.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_GL_RENDERBUFFER_H_
#define GRAPHICS_GL_RENDERBUFFER_H_

#include "core/graphics/common.h"

namespace Graphics {

///
/// @brief Create a renderbuffer object with width and height.
///
struct RenderbufferCreateInfo {
      GLsizei width;
      GLsizei height;
    GLenum internalformat;
};
GLuint CreateRenderbuffer(const RenderbufferCreateInfo &info);

///
/// @brief Delete a renderbuffer object.
///
void DestroyRenderbuffer(const GLuint &renderbuffer);

} // Graphics

#endif // GRAPHICS_GL_RENDERBUFFER_H_
