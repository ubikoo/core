//
// datatype.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_GLSL_DATATYPE_H_
#define GRAPHICS_GLSL_DATATYPE_H_

#include <string>
#include <map>
#include "core/graphics/common.h"

namespace Graphics {

///
/// @brief Query the properties of an active variable in a shader program object.
///
struct DataType {
    struct Item {
        std::string name;       // name of the OpenGL enumeration data type
        GLuint length;          // length in units of the primitive data type
        GLuint size;            // size of the primitive data type in bytes
        GLenum type;            // enumerated type of the primitive data type
    };
    static const std::map<GLenum, Item> Map;

    // Does the map contain the key of the GL data type?
    static bool Contains(const GLenum datatype);

    // Return the name of the GL data type.
    static std::string Name(const GLenum datatype);

    // Return the length of the GL data type in units of the primitive
    // data type.
    static GLuint Length(const GLenum datatype);

    // Return the size of the underlying primitive data type in bytes.
    static GLuint Size(const GLenum datatype);

    // Return the enumerated type of the underlying primitive data type.
    static GLenum Type(const GLenum datatype);
};

} // Graphics

#endif // GRAPHICS_GLSL_DATATYPE_H_
