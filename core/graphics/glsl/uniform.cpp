//
// uniform.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <cassert>
#include <string>
#include <iostream>

#include "datatype.h"
#include "uniform.h"

namespace Graphics {

///
/// @brief Update the uniform with the specified name in the shader program
/// using the corresponding glUniform* function.
///
bool SetUniform(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const void *data)
{
    GLint location = glGetUniformLocation(program, name.data());
    if (location == -1) {
        std::cerr << "invalid uniform location\n";
        return false;
    }

    if (data == nullptr) {
        std::cerr << "invalid uniform data\n";
        return false;
    }

    switch (type) {
    // Vector double, dvec2, dvec3, dvec4
    case GL_DOUBLE:
        glUniform1dv(location, 1, static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_VEC2:
        glUniform2dv(location, 1, static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_VEC3:
        glUniform3dv(location, 1, static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_VEC4:
        glUniform4dv(location, 1, static_cast<const GLdouble *>(data));
        break;

    // Vector float, vec2, vec3, vec4
    case GL_FLOAT:
        glUniform1fv(location, 1, static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_VEC2:
        glUniform2fv(location, 1, static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_VEC3:
        glUniform3fv(location, 1, static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_VEC4:
        glUniform4fv(location, 1, static_cast<const GLfloat *>(data));
        break;

    // Vector int, ivec2, ivec3, ivec4
    case GL_INT:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_VEC2:
        glUniform2iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_VEC3:
        glUniform3iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_VEC4:
        glUniform4iv(location, 1, static_cast<const GLint *>(data));
        break;

    // Vector unsigned int, uvec2, uvec3, uvec4
    case GL_UNSIGNED_INT:
        glUniform1uiv(location, 1, static_cast<const GLuint *>(data));
        break;
    case GL_UNSIGNED_INT_VEC2:
        glUniform2uiv(location, 1, static_cast<const GLuint *>(data));
        break;
    case GL_UNSIGNED_INT_VEC3:
        glUniform3uiv(location, 1, static_cast<const GLuint *>(data));
        break;
    case GL_UNSIGNED_INT_VEC4:
        glUniform4uiv(location, 1, static_cast<const GLuint *>(data));
        break;

    //
    // Sampler types [g]sampler1D, [g]sampler2D, [g]sampler3D, [g]samplerBuffer,
    // [g]sampler2DRect, where [g] is none for float, i for int and u for uint.
    //
    case GL_SAMPLER_1D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_SAMPLER_2D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_SAMPLER_3D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_SAMPLER_BUFFER:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_SAMPLER_2D_RECT:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_SAMPLER_1D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_SAMPLER_2D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_SAMPLER_3D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_SAMPLER_BUFFER:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_SAMPLER_2D_RECT:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_UNSIGNED_INT_SAMPLER_1D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_UNSIGNED_INT_SAMPLER_2D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_UNSIGNED_INT_SAMPLER_3D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_UNSIGNED_INT_SAMPLER_BUFFER:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;

    // Invalid uniform type.
    default:
        std::cerr << "invalid uniform type: " << type << "\n";
        return false;
    }
    return true;
}

///
/// @brief Update the uniform matrix with the specified name in the shader
/// program using the corresponding glUniform* function.
///
bool SetUniformMatrix(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const GLboolean transpose,
    const void *data)
{
    GLint location = glGetUniformLocation(program, name.data());
    if (location == -1) {
        std::cerr << "invalid uniform location\n";
        return false;
    }

    if (data == nullptr) {
        std::cerr << "invalid uniform data\n";
        return false;
    }

    switch (type) {
    // Matrix dmat2, dmat2x3, dmat2x4
    case GL_DOUBLE_MAT2:
        glUniformMatrix2dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_MAT2x3:
        glUniformMatrix2x3dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_MAT2x4:
        glUniformMatrix2x4dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;

    // Matrix dmat3x2, dmat3, dmat3x4
    case GL_DOUBLE_MAT3x2:
        glUniformMatrix3x2dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_MAT3:
        glUniformMatrix3dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_MAT3x4:
        glUniformMatrix3x4dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;

    // Matrix dmat4x2, dmat4x3, dmat4
    case GL_DOUBLE_MAT4x2:
        glUniformMatrix4x2dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_MAT4x3:
        glUniformMatrix4x3dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_MAT4:
        glUniformMatrix4dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;

    // Matrix mat2, mat2x3, mat2x4
    case GL_FLOAT_MAT2:
        glUniformMatrix2fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_MAT2x3:
        glUniformMatrix2x3fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_MAT2x4:
        glUniformMatrix2x4fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;

    // Matrix mat3x2, mat3, mat3x4
    case GL_FLOAT_MAT3x2:
        glUniformMatrix3x2fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_MAT3:
        glUniformMatrix3fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_MAT3x4:
        glUniformMatrix3x4fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;

    // Matrix mat4x2, mat4x3, mat4
    case GL_FLOAT_MAT4x2:
        glUniformMatrix4x2fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_MAT4x3:
        glUniformMatrix4x3fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_MAT4:
        glUniformMatrix4fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;

    // Invalid uniform type.
    default:
        std::cerr << "invalid uniform type: " << type << "\n";
        return false;
    }
    return true;
}

///
/// @brief Return all active uniforms in a shader program object.
/// Call glGetProgramiv with parameter GL_ACTIVE_UNIFORMS to get the number of
/// active uniform variables in the shader program object.
/// Call glGetProgramiv with parameter GL_ACTIVE_UNIFORM_MAX_LENGTH to get the
/// length of the longest active uniform name in the shader program.
/// Call glGetActiveUniform for each active uniform variable to query its name,
/// type and length.
/// Call glGetUniformLocation to get the location of the uniform variable name
/// in the shader program object. This function returns -1 if name is not an
/// active uniform variable. Possible data types are (cf. glGetActiveUniform):
/// GL_FLOAT, GL_FLOAT_VEC{2,3,4}
/// GL_DOUBLE, GL_DOUBLE_VEC{2,3,4}
///
/// GL_INT, GL_INT_VEC{2,3,4}
/// GL_UNSIGNED_INT, GL_UNSIGNED_INT_VEC{2,3,4}
///
/// GL_FLOAT_MAT{2,2x3,2x4}
/// GL_FLOAT_MAT{3x2,3,3x4}
/// GL_FLOAT_MAT{4x2,4x3,4}
///
/// GL_DOUBLE_MAT{2,2x3,2x4}
/// GL_DOUBLE_MAT{3x2,3,3x4}
/// GL_DOUBLE_MAT{4x2,4x3,4}
///
/// GL_SAMPLER_{1D,2D,3D}
/// GL_INT_SAMPLER_{1D,2D,3D}
/// GL_UNSIGNED_INT_SAMPLER_{1D,2D,3D}
///
std::vector<ActiveUniform> GetActiveUniforms(const GLuint &program)
{
    std::vector<ActiveUniform> uniforms;
    if (program == 0) {
        return uniforms;
    }

    GLint n_uniforms = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &n_uniforms);
    if (n_uniforms == 0) {
        return uniforms;
    }

    GLint max_length;
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);

    GLint count;
    GLenum type;
    std::vector<GLchar> name(max_length);
    for (GLuint i = 0; i < static_cast<GLuint>(n_uniforms); ++i) {
        glGetActiveUniform(
            program,
            i,
            static_cast<GLsizei>(max_length),
            nullptr,  // don't return num of chars written
            &count,
            &type,
            name.data());

        GLint location = glGetUniformLocation(program, name.data());
        assert(location != -1);

        uniforms.push_back({std::string(name.data()), location, count, type});
    }

    return uniforms;
}

} // Graphics
