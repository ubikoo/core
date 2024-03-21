//
// renderbuffer.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "renderbuffer.h"

namespace Graphics {

///
/// @brief Create a renderbuffer object.
///
Renderbuffer CreateRenderbuffer(const RenderbufferCreateInfo &info)
{
    RenderbufferObject *renderbuffer = new RenderbufferObject;
    {
        glGenRenderbuffers(1, &renderbuffer->mId);
        renderbuffer->Bind();
        glRenderbufferStorage(
            GL_RENDERBUFFER,
            info.internalFormat,
            info.width,
            info.height);
        renderbuffer->Unbind();
    }
    return Renderbuffer(renderbuffer, RenderbufferDeleter());
}

///
/// @brief Bind/Unbind the renderbuffer object.
///
void RenderbufferObject::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, mId);
}

void RenderbufferObject::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

} // Graphics
