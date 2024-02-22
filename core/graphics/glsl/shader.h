//
// shader.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_GLSL_SHADER_H_
#define GRAPHICS_GLSL_SHADER_H_

#include <string>
#include "core/graphics/common.h"

namespace Graphics {

///
/// @brief Create a new shader object of a specified type from a source string.
/// The following stages are recognised by the shader: GL_VERTEX_SHADER,
/// GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER and GL_COMPUTE_SHADER (version>=4.3).
///
GLuint CreateShaderFromSource(const GLenum type, const std::string &source);

///
/// @brief Create a new shader object of a specified type from a file.
///
GLuint CreateShaderFromFile(const GLenum type, const std::string &filename);

///
/// @brief Delete a shader program object.
///
void DestroyShader(const GLuint &shader);
void DestroyShaders(const std::vector<GLuint> &shaders);

} // Graphics

#endif // GRAPHICS_GLSL_SHADER_H_
