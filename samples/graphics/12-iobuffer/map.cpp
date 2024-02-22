//
// map.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <string>
#include <vector>
#include "core/graphics/graphics.h"
#include "iobuffer.h"
#include "map.h"


///
/// @brief Map constant parameters.
///
static const std::string kImageFilename = "../assets/monarch_512.png";
static const int kWidth = 1024;
static const int kHeight = 1024;

///
/// @brief Create a new map.
/// Begin by rendering the image to the map framebuffer.
/// Run the map shader program over a double framebuffer.
/// End by rendering the map framebuffer to the screen.
///
void Map::Initialize()
{
    // Map begin pass
    {
        // Create a mesh over a quad.
        m_begin.quad = Graphics::CreatePlane(
            "quad",             // vertex attributes prefix
            2,                  // n1 vertices
            2,                  // n2 vertices
            -1.0,               // xlo
             1.0,               // xhi
            -1.0,               // ylo
             1.0);              // yhi

        // Load the 2d-image from the specified filename.
        Graphics::Image image = Graphics::LoadImage(kImageFilename, true);

        Graphics::Texture2dCreateInfo texture_info = {};
        texture_info.width = image.width;
        texture_info.height = image.height;
        texture_info.internalformat = GL_RGBA8;
        texture_info.pixelformat = image.format;
        texture_info.pixeltype = GL_UNSIGNED_BYTE;
        texture_info.pixels = &image.bitmap[0];
        m_begin.texture = Graphics::CreateTexture2d(texture_info);

        glBindTexture(GL_TEXTURE_2D, m_begin.texture);
        Graphics::SetTextureMipmap(GL_TEXTURE_2D);
        Graphics::SetTextureWrap(GL_TEXTURE_2D, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        Graphics::SetTextureFilter(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Build the begin shader program.
        std::vector<GLuint> shaders{
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/map-begin.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/map-begin.frag")};
        m_begin.program = Graphics::CreateProgram(shaders);
        Graphics::DestroyShaders(shaders);
        std::cout << Graphics::GetProgramInfoString(m_begin.program) << "\n";

        // Create vertex array object.
        m_begin.vao = Graphics::CreateVertexArray();
        glBindVertexArray(m_begin.vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_begin.quad.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_begin.quad.ebo);
        SetVertexAttributes(m_begin.program, m_begin.quad.attributes);
        glBindVertexArray(0);
    }

    // Map run pass
    {
        // Create the map input/output framebuffer objects.
        m_draw.read_ix = 0;
        m_draw.write_ix = 1;
        m_draw.iterations = 0;
        m_draw.buffer[0] = IOBuffer::Create(kWidth, kHeight);
        m_draw.buffer[1] = IOBuffer::Create(kWidth, kHeight);

        // Create a mesh over a quad.
        m_draw.quad = Graphics::CreatePlane(
            "quad",             // vertex attributes prefix
            2,                  // n1 vertices
            2,                  // n2 vertices
            -1.0,               // xlo
             1.0,               // xhi
            -1.0,               // ylo
             1.0);              // yhi

        // Build the run shader program.
        std::vector<GLuint> shaders{
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/map-run.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/map-run.frag")};
        m_draw.program = Graphics::CreateProgram(shaders);
        Graphics::DestroyShaders(shaders);
        std::cout << Graphics::GetProgramInfoString(m_draw.program) << "\n";

        // Create vertex array object.
        m_draw.vao = Graphics::CreateVertexArray();
        glBindVertexArray(m_draw.vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_draw.quad.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_draw.quad.ebo);
        SetVertexAttributes(m_draw.program, m_draw.quad.attributes);
        glBindVertexArray(0);
    }

    // Map end pass
    {
        // Create a mesh over a quad.
        m_end.quad = Graphics::CreatePlane(
            "quad",             // vertex attributes prefix
            2,                  // n1 vertices
            2,                  // n2 vertices
            -1.0,               // xlo
             1.0,               // xhi
            -1.0,               // ylo
             1.0);              // yhi

        // Build the run shader program.
        std::vector<GLuint> shaders{
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/map-end.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/map-end.frag")};
        m_end.program = Graphics::CreateProgram(shaders);
        Graphics::DestroyShaders(shaders);
        std::cout << Graphics::GetProgramInfoString(m_end.program) << "\n";

        // Create vertex array object.
        m_end.vao = Graphics::CreateVertexArray();
        glBindVertexArray(m_end.vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_end.quad.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_end.quad.ebo);
        SetVertexAttributes(m_end.program, m_end.quad.attributes);
        glBindVertexArray(0);
    }
}

///
/// @brief Destroy the map.
///
void Map::Cleanup()
{
    // Map end shader
    Graphics::DestroyMesh(m_end.quad);
    Graphics::DestroyVertexArray(m_end.vao);
    Graphics::DestroyProgram(m_end.program);

    // Map run shader
    IOBuffer::Destroy(m_draw.buffer[0]);
    IOBuffer::Destroy(m_draw.buffer[1]);
    Graphics::DestroyMesh(m_draw.quad);
    Graphics::DestroyVertexArray(m_draw.vao);
    Graphics::DestroyProgram(m_draw.program);

    // Map begin shader
    Graphics::DestroyMesh(m_begin.quad);
    Graphics::DestroyTexture(m_begin.texture);
    Graphics::DestroyVertexArray(m_begin.vao);
    Graphics::DestroyProgram(m_begin.program);
}

///
/// @brief Update the map.
///
void Map::Update()
{}

///
/// @brief Render the map.
///
void Map::Render()
{
    // Specify draw state modes.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Map begin shader.
    {
        // Swap indices and bind the framebuffer for writing.
        std::swap(m_draw.read_ix, m_draw.write_ix);
        m_draw.buffer[m_draw.write_ix].bind();

        std::array<GLint, 4> viewport;
        glGetIntegerv(GL_VIEWPORT, &viewport[0]);
        glViewport(0, 0, kWidth, kHeight);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind the shader and draw.
        glUseProgram(m_begin.program);
        glBindVertexArray(m_begin.vao);
        GLenum texunit = 0;
        Graphics::SetUniform(m_begin.program, "u_texsampler", GL_SAMPLER_2D,
            &texunit);
        Graphics::BindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit,
            m_begin.texture);
        Graphics::RenderMesh(m_begin.quad);
        glBindVertexArray(0);
        glUseProgram(0);

        // Unbind the framebuffer
        m_draw.buffer[m_draw.write_ix].unbind();
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    }

    // Map run shader.
    for (size_t iter = 0; iter < m_draw.iterations; ++iter) {
        // Swap read/write buffer indices.
        std::swap(m_draw.read_ix, m_draw.write_ix);

        // Bind the framebuffer for writing
        m_draw.buffer[m_draw.write_ix].bind();

        std::array<GLint, 4> viewport;
        glGetIntegerv(GL_VIEWPORT, &viewport[0]);
        glViewport(0, 0, kWidth, kHeight);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind the shader and draw
        glUseProgram(m_draw.program);
        glBindVertexArray(m_draw.vao);
        GLenum texunit = 0;
        Graphics::SetUniform(m_draw.program, "u_texsampler",  GL_SAMPLER_2D,
            &texunit);
        Graphics::BindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit,
            m_draw.buffer[m_draw.read_ix].m_color_texture);
        Graphics::RenderMesh(m_begin.quad);
        glBindVertexArray(0);
        glUseProgram(0);

        // Unbind the framebuffer
        m_draw.buffer[m_draw.write_ix].unbind();
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    }

    // Map end shader.
    {
        // Swap read/write buffer indices.
        std::swap(m_draw.read_ix, m_draw.write_ix);

        // Bind the shader and draw.
        glUseProgram(m_end.program);
        glBindVertexArray(m_end.vao);
        GLenum texunit = 0;
        Graphics::SetUniform(m_end.program, "u_texsampler", GL_SAMPLER_2D,
            &texunit);
        Graphics::BindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit,
            m_draw.buffer[m_draw.read_ix].m_color_texture);
        Graphics::RenderMesh(m_begin.quad);
        glBindVertexArray(0);
        glUseProgram(0);
    }
}
