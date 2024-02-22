//
// iobuffer.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef SAMPLES_GRAPHICS_IOBUFFER_H_
#define SAMPLES_GRAPHICS_IOBUFFER_H_

#include "core/graphics/graphics.h"

///
/// @brief IOBuffer holds a framebuffer object with a specified size, a single
/// color attachment and and optionally a depth attachment.
///
struct IOBuffer {
    GLint m_width;
    GLint m_height;
    GLuint m_color_texture;
    GLuint m_depth_texture;
    GLuint m_fbo;

    void bind();
    void unbind();

    static IOBuffer Create(const GLint width, const GLint height);
    static void Destroy(IOBuffer &iobuffer);
};

#endif // SAMPLES_GRAPHICS_IOBUFFER_H_
