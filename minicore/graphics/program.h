//
// program.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_SHADER_H_
#define GRAPHICS_SHADER_H_

#include <string>
#include <vector>
#include <memory>
#include "common.h"
#include "attribute.h"
#include "uniform.h"

namespace Graphics {

/// -----------------------------------------------------------------------------
/// @brief Create a new shader object of a specified type from a source string.
///
GLuint CreateShaderFromSource(const GLenum type, const std::string &source);

/// @brief Create a new shader object of a specified type from a file.
GLuint CreateShaderFromFile(const GLenum type, const std::string &filename);

/// @brief Create a shader program object from a set of shader objects.
GLuint CreateShaderProgram(const std::vector<GLuint> &shaders);

/// @brief Destroy a shader program object.
void DestroyShaderProgram(const GLuint &program);

/// @brief Return an string containing the shader program object info.
std::string GetShaderProgramInfo(const GLuint &program);

/// -----------------------------------------------------------------------------
/// @brief Interface to a shader program object.
///
struct ProgramCreateInfo {
    std::vector<GLuint> shaders;
};

struct ProgramObject {
    GLuint mId{0};

    void Use() const;
    void SetAttribute(
        const std::vector<AttributeDescription> &attributes) const;
    void SetUniform(
        const std::string &name,
        const GLenum type,
        const void *data) const;
    void SetUniformMatrix(
        const std::string &name,
        const GLenum type,
        const GLboolean transpose,
        const void *data) const;
};

struct ProgramDeleter {
    void operator()(ProgramObject *obj) {
        if (obj) { DestroyShaderProgram(obj->mId); }
        delete obj;
    }
};

using Program = std::unique_ptr<ProgramObject, ProgramDeleter>;
Program CreateProgram(const ProgramCreateInfo &info);

} // namespace Graphics

#endif // GRAPHICS_SHADER_H_
