//
// error.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_HELPERS_H_
#define GRAPHICS_HELPERS_H_

#include <exception>
#include "common.h"

namespace Graphics {

/// -----------------------------------------------------------------------------
/// @brief Fast-fail helper function.
///
inline void ThrowIf(bool result) {
#ifndef NDEBUG
    if (result) {
        throw std::exception();
    }
#endif
}

inline void ThrowIfNot(bool result) {
    ThrowIf(!result);
}

/// -----------------------------------------------------------------------------
/// @brief Check the OpenGL error queue.
///
GLenum GetError();

/// @brief Does the OpenGL error queue have an error?
bool HasError();

/// @brief Clear the OpenGL error queue of any errors.
void ClearError();

/// -----------------------------------------------------------------------------
/// @brief Get/Set the current viewport.
///
struct Viewport {
    GLfloat x;
    GLfloat y;
    GLfloat width;
    GLfloat height;
};

Viewport GetViewport();
void SetViewport(const Viewport &viewport);

} // Graphics

#endif // GRAPHICS_HELPERS_H_
