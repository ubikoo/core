//
// texture.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <exception>
#include "texture.h"

namespace Graphics {

///
/// @brief Create a Texture with specified size and internal format.
/// Only these types of textures are represented here for simplicity:
///     GL_TEXTURE_1D: 1-dimensional textures with width, but no height or depth.
///     GL_TEXTURE_2D: 2-dimensional textures with width and height but no depth.
///     GL_TEXTURE_3D: 3-dimensional textures with width, height and depth.
///
/// Generate mipmaps and set the indices of the lowest GL_TEXTURE_BASE_LEVEL and
/// highest GL_TEXTURE_MAX_LEVEL mipmap levels.
/// A texture is not mipmap complete if it expects the existence of mipmaps and
/// none is supplied. Texture minifying function GL_TEXTURE_minFilter can be:
/// GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST,
/// GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR.
/// Texture magnifying function GL_TEXTURE_magFilter can be: GL_NEAREST or
/// GL_LINEAR.
///
/// The way texture coordinates are handled outside the range [0.0, 1.0] is
/// specified by GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, and GL_TEXTURE_WRAP_R.
///
/// @see https://www.khronos.org/opengl/wiki/Texture
///
Texture CreateTexture(const TextureCreateInfo &info)
{
    TextureObject *texture = new TextureObject;
    {
        // Generate a texture object and bind to the first texture unit.
        texture->mTarget = info.target;
        glGenTextures(1, &texture->mId);

        // Bind the texture at the first texture unit.
        texture->Bind(0);

        // Allocate texture data store.
        switch (info.target) {
        case GL_TEXTURE_1D:
            glTexImage1D(
                info.target,
                0,                      // level of detail - 0 is base image
                info.internalformat,    // internal format
                info.width,             // texture width
                0,                      // border parameter - must be 0 (legacy)
                info.pixelformat,       // format of pixel data
                info.pixeltype,         // type of pixel data
                info.pixels);           // pointer to pixel data
            break;

        case GL_TEXTURE_2D:
            glTexImage2D(
                info.target,
                0,                      // level of detail - 0 is base image
                info.internalformat,    // internal format
                info.width,             // texture width
                info.height,            // texture height
                0,                      // border parameter - must be 0 (legacy)
                info.pixelformat,       // format of pixel data
                info.pixeltype,         // type of pixel data(GLubyte)
                info.pixels);           // pointer to pixel data
            break;

        case GL_TEXTURE_3D:
            glTexImage3D(
                info.target,
                0,                      // level of detail - 0 is base image
                info.internalformat,    // internal format
                info.width,             // texture width
                info.height,            // texture height
                info.depth,             // texture depth
                0,                      // border parameter - must be 0 (legacy)
                info.pixelformat,       // format of pixel data
                info.pixeltype,         // type of pixel data(GLubyte)
                info.pixels);           // pointer to pixel data
            break;

        default:
            throw std::runtime_error("invalid texture target");
            break;
        }

        // Generate texture mipmaps and set the lowest and highest mipmap levels.
        if (info.generateMipmap) {
            glGenerateMipmap(info.target);
        }
        glTexParameteri(info.target, GL_TEXTURE_BASE_LEVEL, info.baseLevel);
        glTexParameteri(info.target, GL_TEXTURE_MAX_LEVEL, info.maxLevel);

        // Set texture minifying and magnifying functions.
        glTexParameteri(info.target, GL_TEXTURE_MIN_FILTER, info.minFilter);
        glTexParameteri(info.target, GL_TEXTURE_MAG_FILTER, info.magFilter);

        // Set wrap parameter for the texture coordinates.
        glTexParameteri(info.target, GL_TEXTURE_WRAP_S, info.wrapS);
        glTexParameteri(info.target, GL_TEXTURE_WRAP_T, info.wrapT);
        glTexParameteri(info.target, GL_TEXTURE_WRAP_R, info.wrapR);

        // Unbind the texture from the first texture unit.
        texture->Unbind(0);
    }
    return Texture(texture, TextureDeleter());
}

///
/// @brief Bind/Unbind the texture to its target at the specified texture unit.
///
void TextureObject::Bind(GLenum unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(mTarget, mId);
}

void TextureObject::Unbind(GLenum unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(mTarget, 0);
    glActiveTexture(GL_TEXTURE0);
}

} // Graphics
