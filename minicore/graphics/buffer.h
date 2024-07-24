//
// buffer.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_BUFFER_H_
#define GRAPHICS_BUFFER_H_

#include <memory>
#include "common.h"

namespace Graphics {

///
/// @brief Interface to a buffer bound to a specified target, with size in bytes,
/// and usage.
///
struct BufferCreateInfo {
    GLenum target{0};
    GLsizeiptr size{1};
    GLenum usage{0};
};

struct BufferObject {
    GLenum mTarget{0};
    GLuint mId{0};

    void Bind() const;
    void Unbind() const;
    void Copy(GLintptr offset, GLsizeiptr size, const GLvoid *data) const;
};

struct BufferDeleter {
    void operator()(BufferObject *obj) {
        if (obj) { glDeleteBuffers(1, &obj->mId); }
        delete obj;
    }
};

using Buffer = std::unique_ptr<BufferObject, BufferDeleter>;
Buffer CreateBuffer(const BufferCreateInfo &info);

} // namespace Graphics

#endif // GRAPHICS_BUFFER_H_
