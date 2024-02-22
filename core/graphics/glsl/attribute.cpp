//
// attribute.cpp
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
#include "attribute.h"

namespace Graphics {

/// -----------------------------------------------------------------------------
/// @brief Implementation of the OpenGL offset macro:
///     #define BUFFER_OFFSET(offset) ((void *)(offset))
/// @see OpenGL Programming Guide: The Official Guide to Learning OpenGL:
/// "While there a long history of OpenGL lore on why one might do this, we use
/// this macro to make the point that we’re specifying an offset into a buffer
/// object, rather than a pointer to a block of memory as glVertexAttribPointer’s
/// prototype would suggest.",
///
static inline GLvoid *BufferOffset(GLsizeiptr offset)
{
    return reinterpret_cast<GLvoid *>(offset);
}

/// -----------------------------------------------------------------------------
/// @brief Specify the data format of a vertex attribute with the given name in
/// the shader program object. The interpretation is stored for buffer object
/// currently bound to GL_ARRAY_BUFFER. The attribute is disabled by default and
/// must be enabled using glEnableVertexAttribArray before glVertexAttribPointer.
///
/// @note Vertex buffer data is of type vec{1,2,3,4}{d,f,i,ui} (cf DataType).
///
/// @note glVertexAttribPointer converts all values to floating point.
/// If normalized is GL_TRUE, values stored in an integer format are mapped to
/// [-1,1] (signed) or [0,1] (unsigned). Accepted data types are: GL_BYTE and
/// GL_UNSIGNED_BYTE, GL_SHORT and GL_UNSIGNED_SHORT, GL_INT and GL_UNSIGNED_INT,
/// GL_HALF_FLOAT and GL_FLOAT, GL_DOUBLE and GL_FIXED.
///
/// @note glVertexAttribIPointer always leaves values as integers. Only integer
/// types are accepted: GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT,
/// GL_INT, GL_UNSIGNED_INT.
///
/// @note glVertexAttribLPointer accepts only double type GL_DOUBLE.
///
/// @note Base and local offset represent an arbitrary set of vertex attribute
/// within the same buffer. For example, given two arrays of vertex attribute
/// with the following representation:
///     array 0 is [RGB XYZ, RGB XYZ, RGB XYZ, ...] with N vertices
///     array 1 is [RGB RGB RGB..., XYZ XYZ XYZ...] with M vertices
/// They can share the same buffer object data store, with the following XYZ
/// attribute offsets:
///     array 0 baseoffset = (0) bytes, i.e. beginning of the buffer.
///     array 0 localoffset = (3x8) bytes, i.e. after first RGB tuple.
///     array 1 baseoffset = (6x8xN) bytes, i.e. after array 0 data.
///     array 1 localoffset = (3x8xM) bytes, i.e. after M RGB tuples.
///
/// @see https://www.khronos.org/opengl/wiki/GLAPI/glVertexAttribPointer
///      https://www.khronos.org/opengl/wiki/GLAPI/glEnableVertexAttribArray
///      https://stackoverflow.com/questions/28014864
///      https://stackoverflow.com/questions/12427880
///      https://stackoverflow.com/questions/8220484
///
void SetVertexAttribute(const GLuint &program,
    const VertexAttributeDescription &attribute)
{
    GLint location = glGetAttribLocation(program, attribute.name.c_str());
    if (location == -1) {
        throw std::runtime_error("invalid attribute: " + attribute.name);
    }

    // Enable access to the generic vertex attribute at the specified location.
    glEnableVertexAttribArray(location);

    // Get the number of components and GL data type of the vertex attribute. The
    // vertex attribute access in the shader and in the data store is defined by
    // the attribute internaltype.
    GLint vertexsize = DataType::Length(attribute.type);
    GLenum vertextype = DataType::Type(attribute.type);

    switch (attribute.internaltype) {
    case GL_FLOAT:
        glVertexAttribPointer(
            location,
            vertexsize,
            vertextype,
            attribute.normalized,
            attribute.stride,
            BufferOffset(attribute.offset));
        break;

    case GL_INT:
        glVertexAttribIPointer(
            location,
            vertexsize,
            vertextype,
            attribute.stride,
            BufferOffset(attribute.offset));
        break;

    case GL_DOUBLE:
        glVertexAttribLPointer(
            location,
            vertexsize,
            vertextype,
            attribute.stride,
            BufferOffset(attribute.offset));
        break;

    default:
        throw std::runtime_error("invalid internaltype: " + attribute.name);
    }

    // Set the rate at which the attribute advances during instanced rendering.
    glVertexAttribDivisor(location, attribute.divisor);
}

void SetVertexAttributes(const GLuint &program,
    const std::vector<VertexAttributeDescription> &attributes)
{
    for (const auto &it : attributes) {
        SetVertexAttribute(program, it);
    }
}

///
/// @brief Reset the access to the vertex attribute with the given name in
/// the shader program object.
///
void UnsetVertexAttribute(const GLuint &program,
    const VertexAttributeDescription &attribute)
{
    GLint location = glGetAttribLocation(program, attribute.name.c_str());
    if (location == -1) {
        throw std::runtime_error("invalid attribute: " + attribute.name);
    }

    // Disable access to the generic vertex attribute at the specified location.
    glDisableVertexAttribArray(location);

    // Reset the rate at which the attribute advances during instanced rendering.
    glVertexAttribDivisor(location, 0);
}

void UnsetVertexAttributes(const GLuint &program,
    const std::vector<VertexAttributeDescription> &attributes)
{
    for (const auto &it : attributes) {
        UnsetVertexAttribute(program, it);
    }
}

/// -----------------------------------------------------------------------------
/// @brief Retrieve all active attributes in a shader program object.
/// Call glGetProgramiv with parameter GL_ACTIVE_ATTRIBUTES to get the number of
/// active attribute variables in the shader program object.
/// Call glGetProgramiv with parameter GL_ACTIVE_ATTRIBUTE_MAX_LENGTH to get the
/// length of the longest active attribute name in the shader program.
/// Call glGetActiveAttrib for each active attribute variable to query its name,
/// type and size.
/// Call glGetAttribLocation to get the location of the attribute variable name
/// in the shader program object. This function returns -1 if name is not an
/// active attribute variable. Possible data types are (cf. glGetActiveAttrib):
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
std::vector<ActiveAttribute> GetActiveAttributes(const GLuint &program)
{
    std::vector<ActiveAttribute> attributes;
    if (program == 0) {
        return attributes;
    }

    GLint n_attributes = 0;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &n_attributes);
    if (n_attributes == 0) {
        return attributes;
    }

    GLint max_length;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);

    GLint count;
    GLenum type;
    std::vector<GLchar> name(max_length);
    for (GLuint i = 0; i < static_cast<GLuint>(n_attributes); ++i) {
        glGetActiveAttrib(
            program,
            i,
            static_cast<GLsizei>(max_length),
            nullptr,  // don't return num of chars written
            &count,
            &type,
            name.data());

        GLint location = glGetAttribLocation(program, name.data());
        assert(location != -1);

        attributes.push_back({std::string(name.data()), location, count, type});
    }

    return attributes;
}

} // Graphics
