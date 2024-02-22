//
// model.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef SAMPLES_GRAPHICS_MODEL_H_
#define SAMPLES_GRAPHICS_MODEL_H_

#include <vector>
#include "core/graphics/graphics.h"

struct Model {
    enum Rotate : uint8_t { X = 0, Y, Z };
    uint8_t m_rotate;
    float m_scale;
    math::mat4f m_mvp;                      // modelviewprojection
    uint8_t m_current_mesh;                 // current mesh index
    std::vector<Graphics::Mesh> m_mesh;     // model meshes
    GLuint m_program;                       // shader program object
    std::vector<GLuint> m_vao;              // vertex array object

    void current_mesh(const uint8_t v) { m_current_mesh = v; }
    void rotate(const uint8_t v) { m_rotate = v; }
    void scale(const float v) { m_scale *= v; }

    void Initialize();
    void Cleanup();
    void Update();
    void Render();
};

#endif // SAMPLES_GRAPHICS_MODEL_H_
