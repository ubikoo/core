//
// attribute.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_ATTRIBUTE_H_
#define GRAPHICS_ATTRIBUTE_H_

#include <string>
#include <vector>
#include "common.h"

namespace Graphics {

///
/// @brief Set the properties of a vertex attribute with the given name in the
/// shader: location and data format in the currently bound vertex buffer object,
/// internal data format in the shader, attribute divisor, etc.
///
struct AttributeDescription {
    std::string name;       // attribute name in the vertex shader
    GLenum internaltype;    // attribute internal type in the vertex shader
    GLenum type;            // vertex attribute data type in the data store
    GLsizei stride;         // byte offset between consecutive vertex attributes
    GLsizeiptr offset;      // offset of the first component in the data store
    GLboolean normalized;   // should fixed point values be normalized?
    GLuint divisor;         // vertex attribute divisor per instance
};

void SetAttribute(const GLuint &program,
    const AttributeDescription &attribute);
void SetAttribute(const GLuint &program,
    const std::vector<AttributeDescription> &attributes);

void UnsetAttribute(const GLuint &program,
    const AttributeDescription &attribute);
void UnSetAttribute(const GLuint &program,
    const std::vector<AttributeDescription> &attributes);

///
/// @brief Retrieve all active attributes in a shader program object.
///
struct ActiveAttribute {
    std::string name;       // name in the shader program
    GLint location;         // location in the shader program
    GLsizei count;          // count in units of OpenGL type
    GLenum type;            // enumerated OpenGL type
};

std::vector<ActiveAttribute> GetActiveAttributes(const GLuint &program);

} // namespace Graphics

#endif // GRAPHICS_ATTRIBUTE_H_
