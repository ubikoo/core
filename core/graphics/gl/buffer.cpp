//
// buffer.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <cassert>
#include "buffer.h"

namespace Graphics {

///
/// @brief Create a buffer bound to a target, with size in bytes, and usage.
/// Buffer usage can be classified down to two parts, frequency and access type:
///  GL_STREAM_DRAW,  GL_STREAM_READ,  GL_STREAM_COPY
///  GL_STATIC_DRAW,  GL_STATIC_READ,  GL_STATIC_COPY
///  GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY
/// @par A buffer object contains an array of unformatted data allocated by GL
/// and nothing more. The meaning of the data is defined when the buffer is bound
/// to a target using glBindBuffer.
/// A buffer is only a vertex buffer holding vertex array attributes when bound
/// to a GL_ARRAY_BUFFER target.
/// The exact same buffer is interpreted as an element buffer object with vertex
/// indices if bound to a GL_ELEMENT_ARRAY_BUFFER target.
/// The exact same buffer is interpreted as a pixel buffer object if bound to
/// GL_PIXEL_PACK_BUFFER.
/// @see OpenGL buffer objects
///      https://www.khronos.org/opengl/wiki/Buffer_Object
///      http://www.songho.ca/opengl
///      https://stackoverflow.com/questions/748162
///      OpenGL direct state access
///      https://www.khronos.org/opengl/wiki/Direct_State_Access
///      https://stackoverflow.com/questions/33363490
///      OpenGL persistent buffer mapping and immutable storage
///      https://learnopengl.com/Advanced-OpenGL/Advanced-Data
///      https://gamedev.stackexchange.com/questions/87074
///
GLuint CreateBuffer(const BufferCreateInfo &info)
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(info.target, buffer);
    assert(glIsBuffer(buffer));

    glBufferData(info.target, info.size, NULL, info.usage);

    glBindBuffer(info.target, 0);
    return buffer;
}

///
/// @brief Delete a buffer object.
///
void DestroyBuffer(const GLuint &buffer)
{
    glDeleteBuffers(1, &buffer);
}

} // Graphics
