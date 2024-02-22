//
// uniform.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_GLSL_UNIFORM_H_
#define GRAPHICS_GLSL_UNIFORM_H_

#include <string>
#include <vector>
#include "core/graphics/common.h"

namespace Graphics {

///
/// @brief Update the uniform with the specified name in the shader program
/// using the corresponding glUniform* function.
///
bool SetUniform(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const void *data);

///
/// @brief Update the uniform matrix with the specified name in the shader
/// program using the corresponding glUniform* function.
///
bool SetUniformMatrix(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const GLboolean transpose,
    const void *data);

///
/// @brief Return all active uniforms in a shader program object. If an active
/// uniform is an array, it should be set manually using its name in the shader
/// program object.
/// @see https://stackoverflow.com/questions/19894839
///
struct ActiveUniform {
    std::string name;       // name in the shader program
    GLint location;         // location in the shader program
    GLsizei count;          // count in units of OpenGL type
    GLenum type;            // enumerated OpenGL type
};
std::vector<ActiveUniform> GetActiveUniforms(const GLuint &program);

} // Graphics

#endif // GRAPHICS_GLSL_UNIFORM_H_
