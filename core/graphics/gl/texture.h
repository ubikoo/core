//
// texture.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_GL_TEXTURE_H_
#define GRAPHICS_GL_TEXTURE_H_

#include "core/graphics/common.h"

namespace Graphics {

/// -----------------------------------------------------------------------------
/// @brief Create a 1d texture with specified size and internal format.
///
struct Texture1dCreateInfo {
    GLsizei width;
    GLint internalformat;
    GLenum pixelformat;
    GLenum pixeltype;
    GLvoid *pixels;
};
GLuint CreateTexture1d(const Texture1dCreateInfo &info);

/// @brief Create a 2d texture with specified size and internal format.
struct Texture2dCreateInfo {
    GLsizei width;
    GLsizei height;
    GLint internalformat;
    GLenum pixelformat;
    GLenum pixeltype;
    GLvoid *pixels;
};
GLuint CreateTexture2d(const Texture2dCreateInfo &info);

/// @brief Create a 3d texture with specified size and internal format.
struct Texture3dCreateInfo {
    GLsizei width;
    GLsizei height;
    GLsizei depth;
    GLint internalformat;
    GLenum pixelformat;
    GLenum pixeltype;
    GLvoid *pixels;
};
GLuint CreateTexture3d(const Texture3dCreateInfo &info);

/// @brief Delete a texture object.
void DestroyTexture(const GLuint &texture);

/// @brief Bind the texture to the target at the specified texture unit.
void BindTexture(GLenum target, GLenum texunit, GLuint texture);

/// -----------------------------------------------------------------------------
/// @brief Create a 1-dimensional texture with a buffer store attached to it.
///
struct TextureBuffer {
    GLint internalformat;
    GLuint texture;
    GLuint buffer;
};

struct TextureBufferCreateInfo {
    GLint internalformat;       // texture internal format
    GLsizeiptr buffersize;      // texture buffer size
    GLenum bufferusage;         // texture buffer usage
};

TextureBuffer CreateTextureBuffer(const TextureBufferCreateInfo &info);
void DestroyTextureBuffer(const TextureBuffer &texbuffer);

/// @brief Bind the texture at the specified texture unit and attach the buffer.
void BindTextureBuffer(GLenum texunit, const TextureBuffer &texbuffer);

/// -----------------------------------------------------------------------------
/// @brief Generate texture mipmaps.
///
void SetTextureMipmap(
    GLenum target,
    GLint base_level = 0,
    GLint max_level = 1000,
    GLboolean generate = true);

/// @brief Set texture minifying and magnifying functions.
void SetTextureFilter(
    GLenum target,
    GLint filter_min = GL_NEAREST,
    GLint filter_mag = GL_NEAREST);

/// @brief Set wrap parameter for the texture coordinates.
void SetTextureWrap(
    GLenum target,
    GLint wrap_s = GL_REPEAT,
    GLint wrap_t = GL_REPEAT,
    GLint wrap_r = GL_REPEAT);

} // Graphics

#endif // GRAPHICS_GL_TEXTURE_H_
