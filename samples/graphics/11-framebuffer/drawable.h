//
// drawable.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef SAMPLES_GRAPHICS_DRAWABLE_H_
#define SAMPLES_GRAPHICS_DRAWABLE_H_

#include "core/graphics/graphics.h"

struct Drawable {
    // Sphere drawable
    struct {
        math::mat4f mvp;            // sphere mvp matrix
        Graphics::Mesh mesh;        // image and texture
        GLuint texture;             // sphere texture
        GLuint program;             // shader program object
        GLuint vao;                 // sphere vertex array object
    } m_sphere;

    // Quad drawable
    struct {
        math::mat4f mvp;            // quad mvp matrix
        Graphics::Mesh mesh;        // image and texture
        GLuint program;             // shader program object
        GLuint vao;                 // quad vertex array object
    } m_quad;

    // Fbo data
    struct {
        GLsizei width;              // fbo width
        GLsizei height;             // fbo height
        GLuint color_texture;       // fbo color attachment
        GLuint depth_texture;       // fbo depth attachment
        GLuint id;                  // framebuffer object id
    } m_fbo;

    void Initialize();
    void InitializeSphere();
    void InitializeQuad();
    void InitalizeFramebuffer();

    void Cleanup();
    void Update();
    void Render();
};

#endif // SAMPLES_GRAPHICS_DRAWABLE_H_
