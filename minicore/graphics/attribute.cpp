//
// attribute.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <string>
#include <vector>
#include <unordered_map>
#include "attribute.h"
#include "helpers.h"

namespace Graphics {

///
/// @brief Implementation of the OpenGL offset macro:
///     #define BUFFER_OFFSET(offset) ((void *)(offset))
/// BufferOffset is used to make the point that we’re specifying an offset into
/// a buffer object, rather than a pointer to a block of memory as it could be
/// implied from glVertexAttribPointer’s prototype."
///
static inline GLvoid *BufferOffset(GLsizeiptr offset)
{
    return reinterpret_cast<GLvoid *>(offset);
}

///
/// @brief Collection of key-value pairs of data types keyed by their GL
/// type enumeration. Each item holds the properties of a given GL type:
///      name:       name of the OpenGL enumerated data type
///      length:     length in units of the primitive data type
///      size:       size of the primitive data type in bytes
///      type:       enumerated value of the primitive data type
/// Basic data types are fundamental types and non-basic types are aggregates.
/// For simplicity, only a subset of GLSL fundamental data types are used here.
/// Vector:
///     GL_DOUBLE, GL_DOUBLE_VEC[2,3,4]
///     GL_FLOAT, GL_FLOAT_VEC[2,3,4]
///     GL_INT, GL_INT_VEC[2,3,4]
///     GL_UNSIGNED_INT, GL_UNSIGNED_INT_VEC[2,3,4]
/// Matrix:
///     GL_DOUBLE, GL_DOUBLE_VEC[2,3,4]
///     GL_FLOAT, GL_FLOAT_VEC[2,3,4]
/// Sampler:
///     GL_SAMPLER_[1,2,3]D,
///     GL_SAMPLER_BUFFER, GL_SAMPLER_2D_RECT
///     GL_INT_SAMPLER_[1,2,3]D,
///     GL_INT_SAMPLER_BUFFER, GL_INT_SAMPLER_2D_RECT
///     GL_UNSIGNED_INT_SAMPLER_[1,2,3]D,
///     GL_UNSIGNED_INT_SAMPLER_BUFFER, GL_UNSIGNED_INT_SAMPLER_2D_RECT
///
/// @see https://www.khronos.org/opengl/wiki/Data_Type_(GLSL)
///      https://www.khronos.org/opengl/wiki/Sampler_(GLSL)
///
struct DataTypeDesc {
    struct Item {
        std::string name;       // name of the OpenGL enumeration data type
        GLuint length;          // length in units of the primitive data type
        GLuint size;            // size of the primitive data type in bytes
        GLenum type;            // enumerated type of the primitive data type
    };
    static const std::unordered_map<GLenum, Item> Map;
    static bool Contains(const GLenum datatype);
    static std::string Name(const GLenum datatype);
    static GLuint Length(const GLenum datatype);
    static GLuint Size(const GLenum datatype);
    static GLenum Type(const GLenum datatype);
};

const std::unordered_map<GLenum, DataTypeDesc::Item> DataTypeDesc::Map = {
    // ---- Vector double, dvec2, dvec3, dvec4 ----------------------------------
    {GL_DOUBLE,             {"GL_DOUBLE",           1, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_VEC2,        {"GL_DOUBLE_VEC2",      2, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_VEC3,        {"GL_DOUBLE_VEC3",      3, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_VEC4,        {"GL_DOUBLE_VEC4",      4, sizeof(GLdouble), GL_DOUBLE}},

    // ---- Vector float, Vec2, Vec3, Vec4 --------------------------------------
    {GL_FLOAT,              {"GL_FLOAT",            1, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_VEC2,         {"GL_FLOAT_VEC2",       2, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_VEC3,         {"GL_FLOAT_VEC3",       3, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_VEC4,         {"GL_FLOAT_VEC4",       4, sizeof(GLfloat), GL_FLOAT}},

    // ---- Vector int, ivec2, ivec3, ivec4 -------------------------------------
    {GL_INT,                {"GL_INT",              1, sizeof(GLint), GL_INT}},
    {GL_INT_VEC2,           {"GL_INT_VEC2",         2, sizeof(GLint), GL_INT}},
    {GL_INT_VEC3,           {"GL_INT_VEC3",         3, sizeof(GLint), GL_INT}},
    {GL_INT_VEC4,           {"GL_INT_VEC4",         4, sizeof(GLint), GL_INT}},

    // ---- Vector unsigned int, uvec2, uvec3, uvec4 ----------------------------
    {GL_UNSIGNED_INT,       {"GL_UNSIGNED_INT",      1, sizeof(GLuint), GL_UNSIGNED_INT}},
    {GL_UNSIGNED_INT_VEC2,  {"GL_UNSIGNED_INT_VEC2", 2, sizeof(GLuint), GL_UNSIGNED_INT}},
    {GL_UNSIGNED_INT_VEC3,  {"GL_UNSIGNED_INT_VEC3", 3, sizeof(GLuint), GL_UNSIGNED_INT}},
    {GL_UNSIGNED_INT_VEC4,  {"GL_UNSIGNED_INT_VEC4", 4, sizeof(GLuint), GL_UNSIGNED_INT}},

    // ---- Matrix dmat2, dmat2x3, dmat2x4 --------------------------------------
    {GL_DOUBLE_MAT2,        {"GL_DOUBLE_MAT2",       4, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT2x3,      {"GL_DOUBLE_MAT2x3",     6, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT2x4,      {"GL_DOUBLE_MAT2x4",     8, sizeof(GLdouble), GL_DOUBLE}},

    // ---- Matrix dmat3x2, dmat3, dmat3x4 --------------------------------------
    {GL_DOUBLE_MAT3x2,      {"GL_DOUBLE_MAT3x2",     6, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT3,        {"GL_DOUBLE_MAT3",       9, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT3x4,      {"GL_DOUBLE_MAT3x4",    12, sizeof(GLdouble), GL_DOUBLE}},

    // ---- Matrix dmat4x2, dmat4x3, dmat4 --------------------------------------
    {GL_DOUBLE_MAT4x2,      {"GL_DOUBLE_MAT4x2",     8, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT4x3,      {"GL_DOUBLE_MAT4x3",    12, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT4,        {"GL_DOUBLE_MAT4",      16, sizeof(GLdouble), GL_DOUBLE}},

    // ---- Matrix Mat2, mat2x3, mat2x4 -----------------------------------------
    {GL_FLOAT_MAT2,         {"GL_FLOAT_MAT2",       4, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT2x3,       {"GL_FLOAT_MAT2x3",     6, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT2x4,       {"GL_FLOAT_MAT2x4",     8, sizeof(GLfloat), GL_FLOAT}},

    // ---- ---- Matrix mat3x2, Mat3, mat3x4 ------------------------------------
    {GL_FLOAT_MAT3x2,       {"GL_FLOAT_MAT3x2",      6, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT3,         {"GL_FLOAT_MAT3",        9, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT3x4,       {"GL_FLOAT_MAT3x4",     12, sizeof(GLfloat), GL_FLOAT}},

    // ---- Matrix mat4x2, mat4x3, Mat4 -----------------------------------------
    {GL_FLOAT_MAT4x2,       {"GL_FLOAT_MAT4x2",      8, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT4x3,       {"GL_FLOAT_MAT4x3",     12, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT4,         {"GL_FLOAT_MAT4",       16, sizeof(GLfloat), GL_FLOAT}},

    // ---- Sampler data types --------------------------------------------------
    {GL_SAMPLER_1D,         {"GL_SAMPLER_1D",           1, sizeof(GLint), GL_INT}},
    {GL_SAMPLER_2D,         {"GL_SAMPLER_2D",           1, sizeof(GLint), GL_INT}},
    {GL_SAMPLER_3D,         {"GL_SAMPLER_3D",           1, sizeof(GLint), GL_INT}},
    {GL_SAMPLER_BUFFER,     {"GL_SAMPLER_BUFFER",       1, sizeof(GLint), GL_INT}},
    {GL_SAMPLER_2D_RECT,    {"GL_SAMPLER_2D_RECT",      1, sizeof(GLint), GL_INT}},

    {GL_INT_SAMPLER_1D,     {"GL_INT_SAMPLER_1D",       1, sizeof(GLint), GL_INT}},
    {GL_INT_SAMPLER_2D,     {"GL_INT_SAMPLER_2D",       1, sizeof(GLint), GL_INT}},
    {GL_INT_SAMPLER_3D,     {"GL_INT_SAMPLER_3D",       1, sizeof(GLint), GL_INT}},
    {GL_INT_SAMPLER_BUFFER, {"GL_INT_SAMPLER_BUFFER",   1, sizeof(GLint), GL_INT}},
    {GL_INT_SAMPLER_2D_RECT,{"GL_INT_SAMPLER_2D_RECT",  1, sizeof(GLint), GL_INT}},

    {GL_UNSIGNED_INT_SAMPLER_1D,     {"GL_UNSIGNED_INT_SAMPLER_1D",      1, sizeof(GLint), GL_INT}},
    {GL_UNSIGNED_INT_SAMPLER_2D,     {"GL_UNSIGNED_INT_SAMPLER_2D",      1, sizeof(GLint), GL_INT}},
    {GL_UNSIGNED_INT_SAMPLER_3D,     {"GL_UNSIGNED_INT_SAMPLER_3D",      1, sizeof(GLint), GL_INT}},
    {GL_UNSIGNED_INT_SAMPLER_BUFFER, {"GL_UNSIGNED_INT_SAMPLER_BUFFER",  1, sizeof(GLint), GL_INT}},
    {GL_UNSIGNED_INT_SAMPLER_2D_RECT,{"GL_UNSIGNED_INT_SAMPLER_2D_RECT", 1, sizeof(GLint), GL_INT}},
};

///
/// @brief Does the map contain the key of the attribute data type?
///
bool DataTypeDesc::Contains(const GLenum datatype)
{
    return (DataTypeDesc::Map.find(datatype) != DataTypeDesc::Map.end());
}

///
/// @brief Return the name of the attribute data type.
///
std::string DataTypeDesc::Name(const GLenum datatype)
{
    if (!Contains(datatype)) {
        return {};
    }
    return (DataTypeDesc::Map.at(datatype)).name;
}

///
/// @brief Return the length of the attribute data type in units of the
/// underlying primitive data type.
///
GLuint DataTypeDesc::Length(const GLenum datatype)
{
    if (!Contains(datatype)) {
        return 0;
    }
    return (DataTypeDesc::Map.at(datatype)).length;
}

///
/// @brief Return the size of the underlying primitive data type in bytes.
///
GLuint DataTypeDesc::Size(const GLenum datatype)
{
    if (!Contains(datatype)) {
        return 0;
    }
    return (DataTypeDesc::Map.at(datatype)).size;
}

///
/// @brief Return the enumerated type of the underlying primitive data type.
///
GLenum DataTypeDesc::Type(const GLenum datatype)
{
    if (!Contains(datatype)) {
        return 0;
    }
    return (DataTypeDesc::Map.at(datatype)).type;
}

///
/// @brief Set the properties of a vertex attribute with the given name in the
/// shader - location and data format in the vertex buffer object, internal data
/// format in the shader, attribute divisor, etc.
///
/// Vertex attributes are disabled by default and must be enabled using
/// glEnableVertexAttribArray before glVertexAttribPointer.
///
/// Vertex buffer data is of type vec{1,2,3,4}{d,f,i,ui} (cf DataTypeDesc).
/// glVertexAttribPointer converts all values to floating point. If normalized is
/// GL_TRUE, values stored in an integer format are mapped to [-1,1] (signed) or
/// [0,1] (unsigned). Accepted data types are: GL_BYTE and GL_UNSIGNED_BYTE,
/// GL_SHORT and GL_UNSIGNED_SHORT, GL_INT and GL_UNSIGNED_INT,/ GL_HALF_FLOAT
/// and GL_FLOAT, GL_DOUBLE and GL_FIXED.
/// glVertexAttribIPointer always leaves values as integers. Only integer types
/// are accepted: GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT,
/// GL_UNSIGNED_INT.
/// glVertexAttribLPointer accepts only double data type GL_DOUBLE.
///
/// Base and local offset represent an arbitrary set of vertex attribute within
/// the same buffer. For example, given two arrays of vertex attribute with the
/// following representation:
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
void SetAttribute(const GLuint &program, const AttributeDescription &attribute)
{
    GLint location = glGetAttribLocation(program, attribute.name.c_str());
    if (location == -1) {
        throw std::runtime_error("invalid attribute: " + attribute.name);
    }

    // Enable access to the generic vertex attribute at the specified location.
    glEnableVertexAttribArray(location);

    // Get the number of components and data type of the vertex attribute. The
    // vertex attribute access in the shader and in the data store is defined by
    // the attribute internaltype.
    GLint vertexsize = DataTypeDesc::Length(attribute.type);
    GLenum vertextype = DataTypeDesc::Type(attribute.type);

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

void SetAttribute(const GLuint &program,
    const std::vector<AttributeDescription> &attributes)
{
    for (const auto &it : attributes) {
        SetAttribute(program, it);
    }
}

///
/// @brief Reset the access to the vertex attribute with the given name in
/// the shader program object.
///
void UnsetAttribute(const GLuint &program,
    const AttributeDescription &attribute)
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

void UnSetAttribute(const GLuint &program,
    const std::vector<AttributeDescription> &attributes)
{
    for (const auto &it : attributes) {
        UnsetAttribute(program, it);
    }
}

///
/// @brief Retrieve all active attributes in a shader program object.
///
std::vector<ActiveAttribute> GetActiveAttributes(const GLuint &program)
{
    std::vector<ActiveAttribute> attributes;
    if (program == 0) {
        return attributes;
    }

    GLint numAttributes = 0;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
    if (numAttributes == 0) {
        return attributes;
    }

    GLint maxLength;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

    GLint count;
    GLenum type;
    std::vector<GLchar> name(maxLength);
    for (GLuint i = 0; i < static_cast<GLuint>(numAttributes); ++i) {
        glGetActiveAttrib(
            program,
            i,
            static_cast<GLsizei>(maxLength),
            nullptr,  // don't return num of chars written
            &count,
            &type,
            name.data());

        GLint location = glGetAttribLocation(program, name.data());
        ThrowIf(location == -1);

        attributes.push_back({std::string(name.data()), location, count, type});
    }

    return attributes;
}

} // namespace Graphics
