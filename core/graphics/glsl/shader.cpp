//
// shader.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <cassert>
#include <vector>
#include <string>
#include <ios>
#include <iostream>
#include <sstream>
#include <fstream>

#include "datatype.h"
#include "uniform.h"

namespace Graphics {

///
/// @brief Create a new shader object. Compile the shader and query the shader
/// infolog to check the compile status.
///
/// @note In order to be linked in a program object, the newly created shader
/// must be attached the the program object (cf Program).
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
    assert(glIsShader(shader));

    assert(!source.empty());
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
        GLint log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> infolog(log_length + 1, '\0');

        glGetShaderInfoLog(shader, log_length, nullptr, infolog.data());
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
    std::ifstream file(filename);
    if (file) {
        std::stringstream source(std::ios::out);
        source << file.rdbuf();
        return CreateShaderFromSource(type, source.str());
    }
    return 0;
}

///
/// @brief Delete a shader program object
///
void DestroyShader(const GLuint &shader)
{
    if (shader == 0) {
        return;
    }
    glDeleteShader(shader);
}

void DestroyShaders(const std::vector<GLuint> &shaders)
{
    for (auto &shader : shaders) {
        DestroyShader(shader);
    }
}

} // Graphics
