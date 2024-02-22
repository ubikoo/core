//
// texture.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <cassert>
#include "texture.h"
#include "buffer.h"

namespace Graphics {

/// -----------------------------------------------------------------------------
/// @brief Create a 1d texture with specified size and internal format.
/// @par GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, and GL_TEXTURE_WRAP_R control
/// how texture coordinates are handled outside the range [0.0, 1.0].
/// @par A texture is not mipmap complete if it expects the existence of mipmaps
/// and none is supplied. There are six minifying functions:
///  GL_NEAREST, GL_LINEAR:
///  GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST
///  GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
/// @par Only these types of textures are accounted for simplicity:
///  GL_TEXTURE_1D: 1-dimensional textures with width, but no height or depth.
///  GL_TEXTURE_2D: 2-dimensional textures with width and height but no depth.
///  GL_TEXTURE_3D: 3-dimensional textures with width, height and depth.
///  GL_TEXTURE_BUFFER: 1-dimensional texture with an attached buffer object.
///
/// @see https://www.khronos.org/opengl/wiki/Texture
///
GLuint CreateTexture1d(const Texture1dCreateInfo &info)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_1D, texture);
    assert(glIsTexture(texture));

    glTexImage1D(
        GL_TEXTURE_1D,
        0,                      // level of detail - 0 is base image
        info.internalformat,    // internal format
        info.width,             // texture width
        0,                      // border parameter - must be 0 (legacy)
        info.pixelformat,       // format of pixel data
        info.pixeltype,         // type of pixel data
        info.pixels);           // pointer to pixel data

    glBindTexture(GL_TEXTURE_1D, 0);
    return texture;
}

///
/// @brief Create a 2d texture with specified size and internal format.
///
GLuint CreateTexture2d(const Texture2dCreateInfo &info)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    assert(glIsTexture(texture));

    glTexImage2D(
        GL_TEXTURE_2D,
        0,                      // level of detail - 0 is base image
        info.internalformat,    // internal format
        info.width,             // texture width
        info.height,            // texture height
        0,                      // border parameter - must be 0 (legacy)
        info.pixelformat,       // format of pixel data
        info.pixeltype,         // type of pixel data(GLubyte)
        info.pixels);           // pointer to pixel data

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

///
/// @brief Create a 3d texture with specified size and internal format.
///
GLuint CreateTexture3d(const Texture3dCreateInfo &info)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_3D, texture);
    assert(glIsTexture(texture));

    glTexImage3D(
        GL_TEXTURE_3D,
        0,                      // level of detail - 0 is base image
        info.internalformat,    // internal format
        info.width,             // texture width
        info.height,            // texture height
        info.depth,             // texture depth
        0,                      // border parameter - must be 0 (legacy)
        info.pixelformat,       // format of pixel data
        info.pixeltype,         // type of pixel data(GLubyte)
        info.pixels);           // pointer to pixel data

    glBindTexture(GL_TEXTURE_3D, 0);
    return texture;
}

///
/// @brief Delete a texture object.
///
void DestroyTexture(const GLuint &texture)
{
    glDeleteTextures(1, &texture);
}

///
/// @brief Bind the texture to the target at the specified texture unit.
///
void BindTexture(GLenum target, GLenum texunit, GLuint texture)
{
    glActiveTexture(GL_TEXTURE0 + texunit);
    glBindTexture(target, texture);
}

/// -----------------------------------------------------------------------------
/// @brief Create a 1-dimensional texture with a buffer store attached to it.
/// Create a buffer data store with the specified size and generate a texture
/// bound to GL_TEXTURE_BUFFER. Attach the buffer to the generated texture.
/// If buffer argument is zero, any buffer object attached to the buffer texture
/// is detached. Otherwise it must be the name of an existing buffer object.
///
TextureBuffer CreateTextureBuffer(const TextureBufferCreateInfo &info)
{
    TextureBuffer texbuffer = {};
    texbuffer.internalformat = info.internalformat;

    BufferCreateInfo buffer_info = {};
    buffer_info.target = GL_TEXTURE_BUFFER;
    buffer_info.size = info.buffersize;
    buffer_info.usage = info.bufferusage;
    texbuffer.buffer = CreateBuffer(buffer_info);

    glGenTextures(1, &texbuffer.texture);
    glBindTexture(GL_TEXTURE_BUFFER, texbuffer.texture);
    assert(glIsTexture(texbuffer.texture));
    glTexBuffer(GL_TEXTURE_BUFFER, info.internalformat, texbuffer.buffer);
    glBindTexture(GL_TEXTURE_BUFFER, 0);

    return texbuffer;
}

///
/// @brief Delete a texture buffer object.
///
void DestroyTextureBuffer(const TextureBuffer &texbuffer)
{
    glDeleteBuffers(1, &texbuffer.buffer);
    glDeleteTextures(1, &texbuffer.texture);
}

///
/// @brief Bind the texture at the specified texture unit and attach the buffer.
///
void BindTextureBuffer(GLenum texunit, const TextureBuffer &texbuffer)
{
    glActiveTexture(GL_TEXTURE0 + texunit);
    glBindTexture(GL_TEXTURE_BUFFER, texbuffer.texture);
    glTexBuffer(GL_TEXTURE_BUFFER, texbuffer.internalformat, texbuffer.buffer);
}

/// -----------------------------------------------------------------------------
/// @brief Generate texture mipmaps and set the indices of the lowest
/// GL_TEXTURE_BASE_LEVEL and highest GL_TEXTURE_MAX_LEVEL mipmap levels.
/// Initially, GL_TEXTURE_BASE_LEVEL is 0 and GL_TEXTURE_MAX_LEVEL is 1000.
///
void SetTextureMipmap(
    const GLenum target,
    const GLint base_level,
    const GLint max_level,
    const GLboolean generate)
{
    if (generate) {
        glGenerateMipmap(target);
    }
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, base_level);
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, max_level);
}

///
/// @brief Set texture minifying and magnifying functions.
/// Minifying function GL_TEXTURE_MIN_FILTER can be either:
///      GL_NEAREST, GL_LINEAR,
///      GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST,
///      GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR.
/// Magnifying function GL_TEXTURE_MAG_FILTER can be either:
///      GL_NEAREST, or GL_LINEAR,
/// Initially, GL_TEXTURE_MIN_FILTER and GL_TEXTURE_MAG_FILTER are set to
/// GL_NEAREST.
///
void SetTextureFilter(
    const GLenum target,
    const GLint filter_min,
    const GLint filter_mag)
{
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter_min);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter_mag);
}

///
/// @brief Set wrap parameter for the texture coordinates GL_TEXTURE_WRAP_S,
/// GL_TEXTURE_WRAP_T and GL_TEXTURE_WRAP_R to either:
///  GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT, GL_REPEAT, or
///  GL_MIRROR_CLAMP_TO_EDGE.
/// Initially, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T and GL_TEXTURE_WRAP_R are
/// set to GL_REPEAT.
///
void SetTextureWrap(
    const GLenum target,
    const GLint wrap_s,
    const GLint wrap_t,
    const GLint wrap_r)
{
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, wrap_r);
}

} // Graphics
