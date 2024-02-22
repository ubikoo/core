//
// renderbuffer.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <cassert>
#include "renderbuffer.h"

namespace Graphics {

///
/// @brief Create a renderbuffer object with width and height using a data store
/// with the specified internal format.
/// @see https://www.khronos.org/opengl/wiki/Renderbuffer_Object
///
GLuint CreateRenderbuffer(const RenderbufferCreateInfo &info)
{
    GLuint renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    assert(glIsRenderbuffer(renderbuffer));

    glRenderbufferStorage(GL_RENDERBUFFER,
        info.internalformat, info.width, info.height);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    return renderbuffer;
}

///
/// @brief Delete a renderbuffer object.
///
void DestroyRenderbuffer(const GLuint &renderbuffer)
{
    glDeleteRenderbuffers(1, &renderbuffer);
}

} // Graphics
