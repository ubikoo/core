//
// vertexarray.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_GL_VERTEXARRAY_H_
#define GRAPHICS_GL_VERTEXARRAY_H_

#include "core/graphics/common.h"

namespace Graphics {

///
/// @brief Generate a new vertex array object and bind it to the context.
///
GLuint CreateVertexArray();

///
/// @brief Delete a vertex array object.
///
void DestroyVertexArray(const GLuint &array);

} // Graphics

#endif // GRAPHICS_GL_VERTEXARRAY_H_
