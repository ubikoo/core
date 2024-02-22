//
// framebuffer.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_GL_FRAMEBUFFER_H_
#define GRAPHICS_GL_FRAMEBUFFER_H_

#include <vector>
#include "core/graphics/common.h"

namespace Graphics {

/// -----------------------------------------------------------------------------
/// @brief Create a framebuffer object with width and height, using textures
/// as rendering surfaces for the color and depth attachments.
///
struct FramebufferCreateInfo {
    GLsizei width;
    GLsizei height;
    GLint filter_min;
    GLint filter_mag;
    GLsizei n_color_attachments;
    GLenum color_internalformat;
    GLenum color_pixelformat;
    GLenum color_pixeltype;
    GLenum depth_internalformat;
    GLenum depth_pixelformat;
    GLenum depth_pixeltype;
};
GLuint CreateFramebuffer(
    const FramebufferCreateInfo &info,
    GLuint *color_textures,
    GLuint *depth_texture);

/// -----------------------------------------------------------------------------
/// @brief Create a framebuffer object using renderbuffers as rendering surfaces.
///
struct FramebufferRenderbufferCreateInfo {
    GLsizei width;
    GLsizei height;
    GLsizei n_color_attachments;
    GLenum depth_internalformat;
    GLenum color_internalformat;
};
GLuint CreateFramebufferRenderbuffer(
    const FramebufferRenderbufferCreateInfo &info,
    GLuint *color_renderbuffers,
    GLuint *depth_renderbuffer);

/// -----------------------------------------------------------------------------
/// @brief Delete a framebuffer object.
///
void DestroyFramebuffer(const GLuint &framebuffer);

} // Graphics

#endif // GRAPHICS_GL_FRAMEBUFFER_H_
