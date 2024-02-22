//
// triangle.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef SAMPLES_GRAPHICS_TRIANGLE_H_
#define SAMPLES_GRAPHICS_TRIANGLE_H_

#include "core/graphics/graphics.h"

struct Triangle {
    GLuint m_program;           // shader program object
    GLuint m_vao;               // vertex array object
    GLuint m_vbo;               // vertex buffer object

    void Initialize();
    void Cleanup();
    void Update();
    void Render();
};

#endif // SAMPLES_GRAPHICS_TRIANGLE_H_
