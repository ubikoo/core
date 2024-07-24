//
// renderbuffer.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_RENDERBUFFER_H_
#define GRAPHICS_RENDERBUFFER_H_

#include <memory>
#include "common.h"

namespace Graphics {

///
/// @brief Interface to a renderbuffer with width and height, and using the
/// specified internal format.
///
struct RenderbufferCreateInfo {
    GLsizei width{1};
    GLsizei height{1};
    GLenum internalFormat{GL_RGBA};
};

struct RenderbufferObject {
    GLuint mId{0};

    void Bind() const;
    void Unbind() const;
};

struct RenderbufferDeleter {
    void operator()(RenderbufferObject *obj) {
        if (obj) { glDeleteRenderbuffers(1, &obj->mId); }
        delete obj;
    }
};

using Renderbuffer = std::unique_ptr<RenderbufferObject, RenderbufferDeleter>;
Renderbuffer CreateRenderbuffer(const RenderbufferCreateInfo &info);

} // namespace Graphics

#endif // GRAPHICS_RENDERBUFFER_H_
