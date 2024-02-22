//
// panorama.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef SAMPLES_GRAPHICS_PANORAMA_H_
#define SAMPLES_GRAPHICS_PANORAMA_H_

#include "core/graphics/graphics.h"

struct Panorama {
    math::mat4f m_mvp;              // panorama camera and projection matrix
    Graphics::Camera m_camera;
    Graphics::Image m_image;        // panorana image, mesh and texture
    Graphics::Mesh m_mesh;
    GLuint m_texture;
    GLuint m_program;               // shader program object
    GLuint m_vao;                   // vertex array object

    void Keyboard(int code, int scancode, int action, int mods);
    void MouseMove(double xpos, double ypos);
    void MouseButton(int button, int action, int mods);
    void Initialize();
    void Cleanup();
    void Update();
    void Render();
};

#endif // SAMPLES_GRAPHICS_PANORAMA_H_
