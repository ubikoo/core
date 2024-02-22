//
// error.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_GL_ERROR_H_
#define GRAPHICS_GL_ERROR_H_

#include "core/graphics/common.h"

namespace Graphics {

///
/// @brief Check the OpenGL error queue.
///
GLenum GetError();

///
/// @brief Does the OpenGL error queue have an error?
///
bool HasError();

///
/// @brief Clear the OpenGL error queue of any errors.
///
void ClearError();

} // Graphics

#endif // GRAPHICS_GL_ERROR_H_
