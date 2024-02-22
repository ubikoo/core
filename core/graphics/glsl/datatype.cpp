//
// datatype.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <string>
#include <vector>
#include <map>

#include "datatype.h"

namespace Graphics {

///
/// @brief Collection of key-value pairs of GL data items keyed by the enumerated
/// GL type. Each item holds the properties of a given GL type:
///      name:       name of the OpenGL enumerated data type
///      length:     length in units of the primitive data type
///      size:       size of the primitive data type in bytes
///      type:       enumerated value of the primitive data type
/// Basic data types are fundamental types and non-basic types are aggregates.
/// For simplicity, only a subset of GLSL fundamental data types are used here.
/// Vector
///      GL_DOUBLE, GL_DOUBLE_VEC[2,3,4]
///      GL_FLOAT, GL_FLOAT_VEC[2,3,4]
///      GL_INT, GL_INT_VEC[2,3,4]
///      GL_UNSIGNED_INT, GL_UNSIGNED_INT_VEC[2,3,4]
/// Matrix
///      GL_DOUBLE, GL_DOUBLE_VEC[2,3,4]
///      GL_FLOAT, GL_FLOAT_VEC[2,3,4]
/// Sampler
///      GL_SAMPLER_[1,2,3]D,
///      GL_SAMPLER_BUFFER, GL_SAMPLER_2D_RECT
///      GL_INT_SAMPLER_[1,2,3]D,
///      GL_INT_SAMPLER_BUFFER, GL_INT_SAMPLER_2D_RECT
///      GL_UNSIGNED_INT_SAMPLER_[1,2,3]D,
///      GL_UNSIGNED_INT_SAMPLER_BUFFER, GL_UNSIGNED_INT_SAMPLER_2D_RECT
///
/// @see https://www.khronos.org/opengl/wiki/Data_Type_(GLSL)
///      https://www.khronos.org/opengl/wiki/Sampler_(GLSL)
///
const std::map<GLenum, DataType::Item> DataType::Map = {
    // ---- Vector double, dvec2, dvec3, dvec4 -----------------------------
    {GL_DOUBLE,             {"GL_DOUBLE",       1, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_VEC2,        {"GL_DOUBLE_VEC2",  2, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_VEC3,        {"GL_DOUBLE_VEC3",  3, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_VEC4,        {"GL_DOUBLE_VEC4",  4, sizeof(GLdouble), GL_DOUBLE}},

    // ---- Vector float, vec2, vec3, vec4 ---------------------------------
    {GL_FLOAT,              {"GL_FLOAT",        1, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_VEC2,         {"GL_FLOAT_VEC2",   2, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_VEC3,         {"GL_FLOAT_VEC3",   3, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_VEC4,         {"GL_FLOAT_VEC4",   4, sizeof(GLfloat), GL_FLOAT}},

    // ---- Vector int, ivec2, ivec3, ivec4 --------------------------------
    {GL_INT,                {"GL_INT",          1, sizeof(GLint), GL_INT}},
    {GL_INT_VEC2,           {"GL_INT_VEC2",     2, sizeof(GLint), GL_INT}},
    {GL_INT_VEC3,           {"GL_INT_VEC3",     3, sizeof(GLint), GL_INT}},
    {GL_INT_VEC4,           {"GL_INT_VEC4",     4, sizeof(GLint), GL_INT}},

    // ---- Vector unsigned int, uvec2, uvec3, uvec4 -----------------------
    {GL_UNSIGNED_INT,       {"GL_UNSIGNED_INT",      1, sizeof(GLuint), GL_UNSIGNED_INT}},
    {GL_UNSIGNED_INT_VEC2,  {"GL_UNSIGNED_INT_VEC2", 2, sizeof(GLuint), GL_UNSIGNED_INT}},
    {GL_UNSIGNED_INT_VEC3,  {"GL_UNSIGNED_INT_VEC3", 3, sizeof(GLuint), GL_UNSIGNED_INT}},
    {GL_UNSIGNED_INT_VEC4,  {"GL_UNSIGNED_INT_VEC4", 4, sizeof(GLuint), GL_UNSIGNED_INT}},

    // ---- Matrix dmat2, dmat2x3, dmat2x4 ---------------------------------
    {GL_DOUBLE_MAT2,        {"GL_DOUBLE_MAT2",       4, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT2x3,      {"GL_DOUBLE_MAT2x3",     6, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT2x4,      {"GL_DOUBLE_MAT2x4",     8, sizeof(GLdouble), GL_DOUBLE}},

    // ---- Matrix dmat3x2, dmat3, dmat3x4 ---------------------------------
    {GL_DOUBLE_MAT3x2,      {"GL_DOUBLE_MAT3x2",     6, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT3,        {"GL_DOUBLE_MAT3",       9, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT3x4,      {"GL_DOUBLE_MAT3x4",    12, sizeof(GLdouble), GL_DOUBLE}},

    // ---- Matrix dmat4x2, dmat4x3, dmat4 ---------------------------------
    {GL_DOUBLE_MAT4x2,      {"GL_DOUBLE_MAT4x2",     8, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT4x3,      {"GL_DOUBLE_MAT4x3",    12, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT4,        {"GL_DOUBLE_MAT4",      16, sizeof(GLdouble), GL_DOUBLE}},

    // ---- Matrix mat2, mat2x3, mat2x4 ------------------------------------
    {GL_FLOAT_MAT2,         {"GL_FLOAT_MAT2",       4, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT2x3,       {"GL_FLOAT_MAT2x3",     6, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT2x4,       {"GL_FLOAT_MAT2x4",     8, sizeof(GLfloat), GL_FLOAT}},

    // ---- ---- Matrix mat3x2, mat3, mat3x4 -------------------------------
    {GL_FLOAT_MAT3x2,       {"GL_FLOAT_MAT3x2",      6, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT3,         {"GL_FLOAT_MAT3",        9, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT3x4,       {"GL_FLOAT_MAT3x4",     12, sizeof(GLfloat), GL_FLOAT}},

    // ---- Matrix mat4x2, mat4x3, mat4 ------------------------------------
    {GL_FLOAT_MAT4x2,       {"GL_FLOAT_MAT4x2",      8, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT4x3,       {"GL_FLOAT_MAT4x3",     12, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT4,         {"GL_FLOAT_MAT4",       16, sizeof(GLfloat), GL_FLOAT}},

    // ---- Sampler data types ---------------------------------------------
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
/// @brief Does the map contain the key of the GL data type?
///
bool DataType::Contains(const GLenum datatype)
{
    return (DataType::Map.find(datatype) != DataType::Map.end());
}

///
/// @brief Return the name of the GL data type.
///
std::string DataType::Name(const GLenum datatype)
{
    if (!Contains(datatype)) {
        return {};
    }
    return (DataType::Map.at(datatype)).name;
}

///
/// @brief Return the length of the GL data type in units of the primitive
/// data type.
///
GLuint DataType::Length(const GLenum datatype)
{
    if (!Contains(datatype)) {
        return 0;
    }
    return (DataType::Map.at(datatype)).length;
}

///
/// @brief Return the size of the underlying primitive data type in bytes.
///
GLuint DataType::Size(const GLenum datatype)
{
    if (!Contains(datatype)) {
        return 0;
    }
    return (DataType::Map.at(datatype)).size;
}

///
/// @brief Return the enumerated type of the underlying primitive data type.
///
GLenum DataType::Type(const GLenum datatype)
{
    if (!Contains(datatype)) {
        return 0;
    }
    return (DataType::Map.at(datatype)).type;
}

} // Graphics
