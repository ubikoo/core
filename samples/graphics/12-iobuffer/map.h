//
// map.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef SAMPLES_GRAPHICS_MAP_H_
#define SAMPLES_GRAPHICS_MAP_H_

#include <array>
#include "core/graphics/graphics.h"
#include "iobuffer.h"

struct Map {
    // Map begin pass.
    struct {
        Graphics::Mesh quad;
        GLuint texture;
        GLuint program;
        GLuint vao;
    } m_begin;

    // Map draw pass.
    struct {
        size_t read_ix;
        size_t write_ix;
        size_t iterations;
        std::array<IOBuffer, 2> buffer;
        Graphics::Mesh quad;
        GLuint program;
        GLuint vao;
    } m_draw;

    // Map end pass.
    struct {
        Graphics::Mesh quad;
        GLuint program;
        GLuint vao;
    } m_end;

    void Initialize();
    void Cleanup();
    void Update();
    void Render();
};

#endif // SAMPLES_GRAPHICS_MAP_H_
