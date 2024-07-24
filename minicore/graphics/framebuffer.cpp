//
// framebuffer.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <vector>
#include "framebuffer.h"
#include "texture.h"
#include "helpers.h"

namespace Graphics {

///
/// @brief Create a framebuffer object with width and height, using textures as
/// rendering surfaces for the color and depth attachments. At least one colour
/// render target needs to be attached for the framebuffer to be complete. The
/// depth buffer is optional. If the buffer contains no colour attachments, then
/// specify no fragment shader output values are written to any colour buffer.
/// @see https://docs.gl/gl4/glFramebufferTexture
///      https://open.gl/framebuffers
///      http://www.opengl-tutorial.org/intermediate-tutorials
///
Framebuffer CreateFramebuffer(const FramebufferCreateInfo &info)
{
    FramebufferObject *framebuffer = new FramebufferObject;
    {
        // Generate a new framebuffer object.
        glGenFramebuffers(1, &framebuffer->mId);
        framebuffer->Bind();

        // Attach n-color-renderable textures to the framebuffer.
        {
            TextureCreateInfo texture_info = {};
            texture_info.target = GL_TEXTURE_2D;
            texture_info.width = info.width;
            texture_info.height = info.height;
            texture_info.internalFormat = info.colorInternalformat;
            texture_info.pixelFormat = info.colorPixelformat;
            texture_info.pixelType = info.colorPixeltype;
            texture_info.pixels = nullptr;
            texture_info.minFilter = info.minFilter;
            texture_info.magFilter = info.magFilter;

            for (GLsizei i = 0; i < info.numColorAttachments; ++i) {
                auto texture = CreateTexture(texture_info);
                texture->Bind(0);
                glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0 + i,
                    GL_TEXTURE_2D,
                    texture->mId,
                    0);
                texture->Unbind(0);
                framebuffer->mColorAttachments.emplace_back(std::move(texture));
            }
        }

        // Attach a depth-renderable texture to the framebuffer.
        if (info.useDepthAttachment) {
            TextureCreateInfo texture_info = {};
            texture_info.target = GL_TEXTURE_2D;
            texture_info.width = info.width;
            texture_info.height = info.height;
            texture_info.internalFormat = info.depthInternalformat;
            texture_info.pixelFormat = info.depthPixelformat;
            texture_info.pixelType = info.depthPixeltype;
            texture_info.pixels = nullptr;
            texture_info.minFilter = info.minFilter;
            texture_info.magFilter = info.magFilter;

            auto texture = CreateTexture(texture_info);
            texture->Bind(0);
            glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT,
                GL_TEXTURE_2D,
                texture->mId,
                0);
            texture->Unbind(0);
            framebuffer->mDepthAttachment = std::move(texture);
        }

        //
        // Specify which color buffers will be used for rendering. If no colour
        // attachments are present, tell OpenGL to not render any color data.
        //
        if (info.numColorAttachments > 0) {
            std::vector<GLenum> buffers;
            for (GLsizei i = 0; i < info.numColorAttachments; ++i) {
                buffers.push_back(GL_COLOR_ATTACHMENT0 + i);
            }
            glDrawBuffers(buffers.size(), buffers.data());
        } else {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

        // Check the framebuffer is complete.
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        Base::ThrowIfNot(status == GL_FRAMEBUFFER_COMPLETE);
        framebuffer->Unbind();
    }
    return Framebuffer(framebuffer, FramebufferDeleter());
}

///
/// @brief Bind/Unbind the framebuffer object to its target.
///
void FramebufferObject::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, mId);
}

void FramebufferObject::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace Graphics
