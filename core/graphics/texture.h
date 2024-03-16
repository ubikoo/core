//
// texture.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_TEXTURE_H_
#define GRAPHICS_TEXTURE_H_

#include <memory>
#include "common.h"

namespace Graphics {

///
/// @brief Interface to 1d-, 2d- and 3d-textures with specified size and internal
/// format.
///
struct TextureCreateInfo {
    GLenum target{0};
    GLsizei width{1};
    GLsizei height{1};
    GLsizei depth{1};
    GLint internalformat{GL_RGBA8};
    GLenum pixelformat{GL_RGBA};
    GLenum pixeltype{GL_UNSIGNED_BYTE};
    GLvoid *pixels{NULL};
    GLboolean generateMipmap{GL_FALSE};
    GLint baseLevel{0};
    GLint maxLevel{1000};
    GLint minFilter{GL_NEAREST};
    GLint magFilter{GL_NEAREST};
    GLint wrapS{GL_REPEAT};
    GLint wrapT{GL_REPEAT};
    GLint wrapR{GL_REPEAT};
};

struct TextureObject {
    GLenum mTarget{0};
    GLuint mId{0};

    void Bind(GLenum unit) const;
    void Unbind(GLenum unit) const;
};

struct TextureDeleter {
    void operator()(TextureObject *obj) {
        if (obj) { glDeleteTextures(1, &obj->mId); }
        delete obj;
    }
};

using Texture = std::unique_ptr<TextureObject, TextureDeleter>;
Texture CreateTexture(const TextureCreateInfo &info);

} // Graphics

#endif // GRAPHICS_TEXTURE_H_
