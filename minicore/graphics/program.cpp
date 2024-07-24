//
// program.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <vector>
#include <string>
#include <ios>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include "program.h"
#include "uniform.h"
#include "attribute.h"
#include "helpers.h"

namespace Graphics {

/// -----------------------------------------------------------------------------
/// @brief Create a new shader object. Compile the shader and query the shader
/// infolog to check the compile status. The following stages are recognised by
/// the shader: GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER and
/// GL_COMPUTE_SHADER (version>=4.3).
/// In order to be linked in a program object, the newly created shader must be
/// attached the the program object (cf Program).
/// It is permissible to attach a shader to a program object before source code
/// has been loaded into the shader or before the shader has been compiled.
/// It is permissible to attach multiple shader objects of the same type because
/// each may contain a portion of the complete shader.
/// It is also permissible to attach a shader to more than one program object.
/// If a shader is deleted while it is attached to a program object, it will be
/// flagged for deletion, but deletion occurs only when the shader is detached
/// from all program objects.
///
GLuint CreateShaderFromSource(const GLenum type, const std::string &source)
{
    // Create a shader object and compile.
    GLuint shader = glCreateShader(type);

    ThrowIf(source.empty());
    const GLchar *buffer = static_cast<const GLchar *>(source.c_str());
    glShaderSource(shader, 1, &buffer, nullptr);
    glCompileShader(shader);

    //
    // Query shader compilation status for errors. GL_COMPILE_STATUS returns
    // GL_TRUE if shader compilation was successful, and GL_FALSE otherwise.
    //
    GLint status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<GLchar> infolog(length + 1, '\0');

        glGetShaderInfoLog(shader, length, nullptr, infolog.data());
        std::cerr << "failed to compile shader:\n"
            << buffer << "\n\n" << infolog.data() << "\n\n";
        return 0;
    }

    return shader;
}

///
/// @brief Create a new shader object of a specified type from a file.
///
GLuint CreateShaderFromFile(const GLenum type, const std::string &filename)
{
    std::stringstream source(std::ios::out);
    std::ifstream file(filename);
    if (file) {
        source << file.rdbuf();
    }
    return CreateShaderFromSource(type, source.str());
}

///
/// @brief Create a shader program object from a set of shader stages. Only the
/// vertex stage is mandatory.
/// The sole purpose of the shader objects is to be linked to a shader program
/// object, after which time, they are detached from the program and deleted.
/// Each shader object is assumed to be uniquely attached to a single program.
///
///  1. Create a shader stage: glCreateShader, glShaderSource, glCompileShader
///  2. Create a new program: glCreateProgram
///  3. For each shader object: glAttachShader
///  4. Link program: glLinkProgram
///  5. For each shader object: glDetachShader, glDeleteShader
///
/// Vertex shader input attributes must have their locations specified in the
/// shader with:
///      layout(location = attribute_index) in Vec3 value;
/// glGetAttribLocation then queries the vertex attribute location at runtime.
/// Attributes that take up multiple slots will be given a sequential block in
/// the order starting with the given attribute. For example,
///      layout(location = 2) in Vec3 values[4];
/// will allocate four attribute indices 2, 3, 4, and 5.
///
/// Fragment shader outputs must have their location indices specified via:
///      layout(location = output_index) out Vec4 output_color;
///
/// Consider tagging variables with an index, such that they correspond by the
/// index instead of by name. For example, given a vertex shader providing:
///      layout(location = 0) out Vec4 out_color;
///      layout(location = 1) out Vec2 out_position;
///      layout(location = 2) out Vec3 out_normal;
///
/// The consuming shader can match the interface using different names:
///      layout(location = 0) out Vec4 inp_color;
///      layout(location = 1) out Vec2 inp_position;
///      layout(location = 2) out Vec3 inp_normal;
///
/// @see https://www.khronos.org/opengl/wiki/Shader
///      https://www.khronos.org/opengl/wiki/GLSL_:_common_mistakes
///      https://stackoverflow.com/questions/440144
///      https://stackoverflow.com/questions/9113154
///      https://stackoverflow.com/questions/4635913
///      https://gamedev.stackexchange.com/questions/22216
///      https://gamedev.stackexchange.com/questions/47910
///
GLuint CreateShaderProgram(const std::vector<GLuint> &shaders)
{
    // Create a new program object.
    GLuint program = glCreateProgram();

    // Attach the shader objects to the program and link.
    for (auto &it : shaders) {
        glAttachShader(program, it);
    }

    glLinkProgram(program);
    GLint status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        // Get the number of characters in the program information log.
        GLsizei length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::vector<GLchar> infolog(length + 1, '\0');

        // Get the information log for the specified program object.
        glGetProgramInfoLog(program, length, nullptr, infolog.data());
        throw std::runtime_error(infolog.data());
    }

    // Detach the shader objects from the program so they can be deleted.
    for (auto &it : shaders) {
        glDetachShader(program, it);
    }

    return program;
}

///
/// @brief Destroy a shader program object. Free the memory and invalidate the
/// identifier associated with the program object.
///
void DestroyShaderProgram(const GLuint &program)
{
    // Ignore if program name is 0.
    if (program == 0) {
        return;
    }

    // Bind the program and get the number of attached shaders.
    glUseProgram(program);

    GLint numShaders;
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &numShaders);
    if (numShaders > 0) {
        // Get the handles of each attached shader object.
        GLsizei maxCount = static_cast<GLsizei>(numShaders);
        std::vector<GLuint> shaders(maxCount);
        GLsizei count = 0;
        glGetAttachedShaders(program, maxCount, &count, shaders.data());

        // Detach and delete each shader object.
        for (GLsizei i = 0; i < count; ++i) {
            glDetachShader(program, shaders[i]);
            glDeleteShader(shaders[i]);
        }
    }

    // Delete the program.
    glDeleteProgram(program);
}

///
/// @brief Return an string containing the shader program object info:
/// 1. Link status.
/// 2. Number of attached shaders.
/// 3. Number of active uniform variables and the name, location,
///    count and type of each active uniform stored in the map.
/// 4. Number of active attribute variables and the name, location,
///    count and type of each active attribute stored in the map.
///
std::string GetShaderProgramInfo(const GLuint &program)
{
    // Return an empty vector if program is invalid
    if (!program) {
        std::cerr << "invalid shader program object\n";
        return std::string();
    }

    // Print shader program header information.
    GLint param;
    std::ostringstream ss;
    glGetProgramiv(program, GL_LINK_STATUS, &param);
    ss << "GL_LINK_STATUS = " << param << "\n";
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &param);
    ss << "GL_ATTACHED_SHADERS = " << param << "\n";

    // Print shader program active uniforms information map.
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &param);
    ss << "GL_ACTIVE_UNIFORMS = " << param << "\n";
    for (auto &it : GetActiveUniforms(program)) {
        ss << "(location = " << it.location << ") " << it.name
           << ", count " << it.count
           << ", type " << it.type
           << "\n";
    }

    // Print shader program active attributes information map.
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &param);
    ss << "GL_ACTIVE_ATTRIBUTES = " << param << "\n";
    for (auto &it : GetActiveAttributes(program)) {
        ss << "(location = " << it.location << ") " << it.name
           << ", count " << it.count
           << ", type " << it.type
           << "\n";
    }

    return ss.str();
}

/// -----------------------------------------------------------------------------
/// @brief Create a shader program object
///
Program CreateProgram(const ProgramCreateInfo &info)
{
    ProgramObject *program = new ProgramObject;
    {
        program->mId = CreateShaderProgram(info.shaders);
    }
    return Program(program, ProgramDeleter());
}

///
/// @brief Bind/Unbind the buffer object to its target.
///
void ProgramObject::Use() const
{
    glUseProgram(mId);
}


///
/// @brief Set the properites of the vertex attributes in the pipeline shader
/// program object.
///
void ProgramObject::SetAttribute(
    const std::vector<AttributeDescription> &attributes) const
{
    Graphics::SetAttribute(mId, attributes);
}

///
/// @brief Update the uniform with the specified name in the pipeline shader
/// program.
///
void ProgramObject::SetUniform(
    const std::string &name,
    const GLenum type,
    const void *data) const
{
    Graphics::SetUniform(mId, name, type, data);
}

///
/// @brief Update the uniform matrix with the specified name in the shader
/// program using the corresponding glUniform* function.
///
void ProgramObject::SetUniformMatrix(
    const std::string &name,
    const GLenum type,
    const GLboolean transpose,
    const void *data) const
{
    Graphics::SetUniformMatrix(mId, name, type, transpose, data);
}
} // namespace Graphics
