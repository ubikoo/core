//
// iobuffer.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "core/graphics/graphics.h"
#include "iobuffer.h"


///
/// @brief Create a IOBuffer with specified width and internal format.
///
IOBuffer IOBuffer::Create(const GLint width, const GLint height)
{
    Graphics::FramebufferCreateInfo info = {};
    info.width = width;
    info.height = height;
    info.filter_min = GL_NEAREST;
    info.filter_mag = GL_NEAREST;
    info.n_color_attachments = 1;
    info.color_internalformat = GL_RGB32F;
    info.color_pixelformat = GL_RGB;
    info.color_pixeltype = GL_FLOAT;
    info.depth_internalformat = GL_DEPTH_COMPONENT32F;
    info.depth_pixelformat = GL_DEPTH_COMPONENT;
    info.depth_pixeltype = GL_FLOAT;

    IOBuffer iobuffer;
    iobuffer.m_width = width;
    iobuffer.m_height = height;
    iobuffer.m_fbo = Graphics::CreateFramebuffer(info,
        &iobuffer.m_color_texture, &iobuffer.m_depth_texture);
    return iobuffer;
}

///
/// @brief Destroy the IOBuffer GL objects.
///
void IOBuffer::Destroy(IOBuffer &iobuffer)
{
    glDeleteTextures(1, &iobuffer.m_color_texture);
    glDeleteTextures(1, &iobuffer.m_depth_texture);
    glDeleteFramebuffers(1, &iobuffer.m_fbo);
}

///
/// @brief Bind the IObuffer for writing.
///
void IOBuffer::bind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

///
/// @brief Unbind the IObuffer for writing.
///
void IOBuffer::unbind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
