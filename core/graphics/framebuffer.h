//
// framebuffer.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_FRAMEBUFFER_H_
#define GRAPHICS_FRAMEBUFFER_H_

#include <vector>
#include <memory>
#include "common.h"
#include "texture.h"

namespace Graphics {

///
/// @brief Create a framebuffer object with width and height, using textures as
/// rendering surfaces for the color and depth attachments.
///
struct FramebufferCreateInfo {
    GLsizei width{1};
    GLsizei height{1};
    GLint minFilter{GL_NEAREST};
    GLint magFilter{GL_NEAREST};
    GLsizei numColorAttachments{1};
    GLenum colorInternalformat{GL_RGBA8};
    GLenum colorPixelformat{GL_RGBA};
    GLenum colorPixeltype{GL_UNSIGNED_BYTE};
    GLboolean useDepthAttachment{GL_TRUE};
    GLenum depthInternalformat{GL_DEPTH_COMPONENT};
    GLenum depthPixelformat;
    GLenum depthPixeltype;
};

struct FramebufferObject {
    GLuint mId{0};
    std::vector<Texture> mColorAttachments;
    Texture mDepthAttachment;

    void Bind() const;
    void Unbind() const;
};

struct FramebufferDeleter {
    void operator()(FramebufferObject *obj) {
        if (obj) { glDeleteFramebuffers(1, &obj->mId); }
        delete obj;
    }
};

using Framebuffer = std::unique_ptr<FramebufferObject, FramebufferDeleter>;
Framebuffer CreateFramebuffer(const FramebufferCreateInfo &info);

} // Graphics

#endif // GRAPHICS_FRAMEBUFFER_H_
