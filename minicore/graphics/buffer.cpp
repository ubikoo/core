//
// buffer.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "buffer.h"

namespace Graphics {

///
/// @brief Create a buffer bound to a target, with size in bytes, and usage.
/// Buffer usage is specified by the frequency and access type:
///  GL_STREAM_DRAW,  GL_STREAM_READ,  GL_STREAM_COPY
///  GL_STATIC_DRAW,  GL_STATIC_READ,  GL_STATIC_COPY
///  GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY
/// A buffer contains an array of unformatted data allocated by GL. The meaning of
/// its data is defined by the target to which the buffer is bound.
/// If bound to a GL_ARRAY_BUFFER target it is understood as a vertex buffer.
/// If bound to a GL_ELEMENT_ARRAY_BUFFER target its is seen as an index buffer.
/// The same buffer is seen as a pixel buffer if bound to GL_PIXEL_PACK_BUFFER.
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
Buffer CreateBuffer(const BufferCreateInfo &info)
{
    BufferObject *buffer = new BufferObject;
    {
        buffer->mTarget = info.target;
        glGenBuffers(1, &buffer->mId);
        buffer->Bind();
        glBufferData(info.target, info.size, NULL, info.usage);
        buffer->Unbind();
    }
    return Buffer(buffer, BufferDeleter());
}

///
/// @brief Bind/Unbind the buffer object to its target.
///
void BufferObject::Bind() const
{
    glBindBuffer(mTarget, mId);
}

void BufferObject::Unbind() const
{
    glBindBuffer(mTarget, 0);
}

///
/// @brief Copy the specified data into a subregion of the buffer data store.
///
void BufferObject::Copy(
    GLintptr offset,
    GLsizeiptr size,
    const GLvoid *data) const
{
    Bind();
    glBufferSubData(mTarget, offset, size, data);
    Unbind();
}

} // namespace Graphics
