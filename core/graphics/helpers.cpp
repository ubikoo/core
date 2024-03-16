//
// error.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef OPENGL_ERROR_H_
#define OPENGL_ERROR_H_

#include <array>
#include <string>
#include <iostream>
#include "helpers.h"

namespace Graphics {

/// -----------------------------------------------------------------------------
/// @brief Check the OpenGL error queue.
/// @see https://www.khronos.org/opengl/wiki/Example_Code
///      https://www.khronos.org/opengl/wiki/OpenGL_Error
///
GLenum GetError()
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::string message = {};
        switch (error) {
            case GL_INVALID_ENUM:
                message = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                message = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                message = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                message = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                message = "GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_UNDERFLOW:
                message = "GL_STACK_UNDERFLOW";
                break;
            case GL_STACK_OVERFLOW:
                message = "GL_STACK_OVERFLOW";
                break;
            default:
                message = "UNKNOWN";
                break;
        }
        std::cerr << "OpenGL error: " << message.c_str() << "\n";
    }
    return error;
}

///
/// @brief Does the OpenGL error queue have an error?
///
bool HasError()
{
    return (GetError() != GL_NO_ERROR);
}

///
/// @brief Clear the OpenGL error queue of any errors.
///
void ClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

/// -----------------------------------------------------------------------------
/// @brief Get/Set the current viewport.
///
Viewport GetViewport()
{
    std::array<GLint,4> viewport = {};
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);
    return {
        static_cast<GLfloat>(viewport[0]),
        static_cast<GLfloat>(viewport[1]),
        static_cast<GLfloat>(viewport[2]),
        static_cast<GLfloat>(viewport[3])};
}

void SetViewport(const Viewport &viewport)
{
    glViewport(
        static_cast<GLint>(viewport.x),
        static_cast<GLint>(viewport.y),
        static_cast<GLsizei>(viewport.width),
        static_cast<GLsizei>(viewport.height));
}

} // Graphics

#endif // OPENGL_ERROR_H_
