//
// framebuffer.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <cassert>
#include <vector>

#include "framebuffer.h"
#include "renderbuffer.h"
#include "texture.h"

namespace Graphics {

/// -----------------------------------------------------------------------------
/// @brief Create a framebuffer object with width and height, using textures
/// as rendering surfaces for the color and depth attachments.
/// @note There must be at least one color render target attached to the
/// framebuffer. Depth buffer attachment is optional.
/// @see https://www.khronos.org/opengl/wiki/Image_Format
///      https://www.khronos.org/opengl/wiki/Renderbuffer_Object
///      https://www.khronos.org/opengl/wiki/Texture
///      https://paroj.github.io/gltut/Texturing/Tutorial%2014.html
///      https://docs.gl/gl4/glFramebufferRenderbuffer
///      https://docs.gl/gl4/glFramebufferTexture
///      http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture
///      http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping
///
GLuint CreateFramebuffer(
    const FramebufferCreateInfo &info,
    GLuint *color_textures,
    GLuint *depth_texture)
{
    //
    // Generate a new framebuffer object and bind it to the target point.
    //
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    assert(glIsFramebuffer(framebuffer));

    //
    // Attach n-color-renderable textures to the framebuffer.
    //
    {
        Texture2dCreateInfo texture_info = {};
        texture_info.width = info.width;
        texture_info.height = info.height;
        texture_info.internalformat = info.color_internalformat;
        texture_info.pixelformat = info.color_pixelformat;
        texture_info.pixeltype = info.color_pixeltype;
        texture_info.pixels = nullptr;

        for (GLsizei i = 0; i < info.n_color_attachments; ++i) {
            GLuint texture = CreateTexture2d(texture_info);

            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, info.filter_min);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, info.filter_mag);
            glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0 + i,
                GL_TEXTURE_2D,
                texture,
                0);
            glBindTexture(GL_TEXTURE_2D, 0);

            color_textures[i] = texture;
        }
    }

    //
    // Attach a depth-renderable texture to the framebuffer.
    //
    if (depth_texture != nullptr) {
        Texture2dCreateInfo texture_info = {};
        texture_info.width = info.width;
        texture_info.height = info.height;
        texture_info.internalformat = info.depth_internalformat;
        texture_info.pixelformat = info.depth_pixelformat;
        texture_info.pixeltype = info.depth_pixeltype;
        texture_info.pixels = nullptr;

        GLuint texture = CreateTexture2d(texture_info);

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, info.filter_min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, info.filter_mag);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D,
            texture,
            0);
        glBindTexture(GL_TEXTURE_2D, 0);

        *depth_texture = texture;
    }

    //
    // Specify which attached color buffers will be used for rendering.
    // A framebuffer object is not complete without a color attachment. If the
    // framebuffer does not have any color attachments, tell OpenGL to not render
    // any color data by setting glDrawBuffer and glReadbuffer to GL_NONE.
    //
    if (info.n_color_attachments > 0) {
        std::vector<GLenum> color_attachments;
        for (GLsizei i = 0; i < info.n_color_attachments; ++i) {
            color_attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
        }
        glDrawBuffers(color_attachments.size(), color_attachments.data());
    } else {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert(status == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return framebuffer;
}

/// -----------------------------------------------------------------------------
/// @brief Create a framebuffer object using renderbuffers as rendering surfaces
/// for the color and depth attachments.
///
GLuint CreateFramebufferRenderbuffer(
    const FramebufferRenderbufferCreateInfo &info,
    GLuint *color_renderbuffers,
    GLuint *depth_renderbuffer)
{
    //
    // Generate a new framebuffer object and bind it to the target point.
    //
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    assert(glIsFramebuffer(framebuffer));

    //
    // Attach n color-renderable renderbuffers to the framebuffer.
    //
    {
        RenderbufferCreateInfo renderbuffer_info = {};
        renderbuffer_info.width = info.width;
        renderbuffer_info.height = info.height;
        renderbuffer_info.internalformat = info.color_internalformat;

        for (GLsizei i = 0; i < info.n_color_attachments; ++i) {
            GLuint renderbuffer = CreateRenderbuffer(renderbuffer_info);
            glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
            glFramebufferRenderbuffer(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0 + i,
                GL_RENDERBUFFER,
                renderbuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            color_renderbuffers[i] = renderbuffer;
        }
    }

    //
    // Attach a depth-renderable renderbuffer to the framebuffer.
    //
    if (depth_renderbuffer != nullptr) {
        RenderbufferCreateInfo renderbuffer_info = {};
        renderbuffer_info.width = info.width;
        renderbuffer_info.height = info.height;
        renderbuffer_info.internalformat = info.depth_internalformat;

        GLuint renderbuffer = CreateRenderbuffer(renderbuffer_info);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER,
            renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        *depth_renderbuffer = renderbuffer;
    }

    //
    // Specify which attached color buffers will be used for rendering.
    // A framebuffer object is not complete without a color attachment. If the
    // framebuffer does not have any color attachments, tell OpenGL to not render
    // any color data by setting glDrawBuffer and glReadbuffer to GL_NONE.
    //
    if (info.n_color_attachments > 0) {
        std::vector<GLenum> color_attachments;
        for (GLsizei i = 0; i < info.n_color_attachments; ++i) {
            color_attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
        }
        glDrawBuffers(color_attachments.size(), color_attachments.data());
    } else {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert(status == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return framebuffer;
}

/// -----------------------------------------------------------------------------
/// @brief Delete a framebuffer object.
///
void DestroyFramebuffer(const GLuint &framebuffer)
{
    glDeleteFramebuffers(1, &framebuffer);
}

} // Graphics
