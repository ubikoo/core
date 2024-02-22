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

#include <string>
#include <iostream>
#include "error.h"

namespace Graphics {

///
/// @brief Check the OpenGL error queue.
/// @see https://www.khronos.org/opengl/wiki/Example_Code
///      https://www.khronos.org/opengl/wiki/OpenGL_Error
///
GLenum GetError()
{
    GLenum error_code = glGetError();
    if (error_code != GL_NO_ERROR) {
        std::string error = {};
        switch (error_code) {
            case GL_INVALID_ENUM:
                error = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                error = "GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_UNDERFLOW:
                error = "GL_STACK_UNDERFLOW";
                break;
            case GL_STACK_OVERFLOW:
                error = "GL_STACK_OVERFLOW";
                break;
            default:
                error = "UNKNOWN";
                break;
        }
        std::cerr << "OpenGL error: " << error.c_str() << "\n";
    }
    return error_code;
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

} // Graphics

#endif // OPENGL_ERROR_H_
