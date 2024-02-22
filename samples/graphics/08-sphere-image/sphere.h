//
// sphere.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef SAMPLES_GRAPHICS_SPHERE_H_
#define SAMPLES_GRAPHICS_SPHERE_H_

#include "core/graphics/graphics.h"

struct Sphere {
    math::mat4f m_mvp;              // modelviewprojection
    Graphics::Mesh m_mesh;          // mesh image and texture
    Graphics::Image m_image;
    GLuint m_texture;
    GLuint m_program;               // shader program object
    GLuint m_vao;                   // vertex array object

    void Initialize();
    void Cleanup();
    void Update();
    void Render();
};

#endif // SAMPLES_GRAPHICS_SPHERE_H_
