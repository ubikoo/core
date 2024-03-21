//
// main.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <iostream>
#include <exception>
#include <vector>
#include "core/graphics/graphics.h"

/// -----------------------------------------------------------------------------
struct Quad {
    math::mat4f ModelView;
    Graphics::Buffer Vbo;
    Graphics::Buffer Ebo;
    Graphics::Pipeline Pipeline;
};
Quad gQuad;

/// -----------------------------------------------------------------------------
void OnInitialize()
{
    // Vertex positions and color attributes with layout:
    // {(xyzw)_1, (xyzw)_2, ..., (rgba)_1, (rgba)_2}
    std::vector<GLfloat> vertex_data = {
        // positions
        -0.5f, -0.5f, 0.0f, 1.0f,   // bottom left
         0.5f, -0.5f, 0.0f, 1.0f,   // bottom right
        -0.5f,  0.5f, 0.0f, 1.0f,   // top left
         0.5f,  0.5f, 0.0f, 1.0f,   // top right
        // colors
         0.0f,  0.0f, 1.0f, 1.0f,   // bottom left
         0.0f,  1.0f, 0.0f, 1.0f,   // bottom right
         1.0f,  1.0f, 0.0f, 1.0f,   // top left
         1.0f,  0.0f, 0.0f, 1.0f};  // top right
    GLsizeiptr vertex_data_size = vertex_data.size() * sizeof(GLfloat);

    // Create the index data as a shared vertex representation.
    std::vector<GLuint> index_data = {
        0, 1, 2,                    // first triangle
        3, 2, 1};                   // second triangle
    GLsizeiptr index_data_size = index_data.size() * sizeof(GLuint);

    // Create buffer storage for vertex position and color and specify how OpenGL
    // interprets the vertex attributes.
    {
        Graphics::BufferCreateInfo info = {};
        info.target = GL_ARRAY_BUFFER;
        info.size = vertex_data_size;
        info.usage = GL_STATIC_DRAW;

        gQuad.Vbo = Graphics::CreateBuffer(info);
        gQuad.Vbo->Copy(0, vertex_data_size, &vertex_data[0]);
    }

    // Create a buffer storage for the vertex indices.
    {
        Graphics::BufferCreateInfo info = {};
        info.target = GL_ELEMENT_ARRAY_BUFFER;
        info.size = index_data_size;
        info.usage = GL_STATIC_DRAW;

        gQuad.Ebo = Graphics::CreateBuffer(info);
        gQuad.Ebo->Copy(0, index_data_size, &index_data[0]);
    }

    // Create the quad rendering pipeline.
    {
        Graphics::PipelineCreateInfo info = {};
        info.polygonMode = GL_FILL;
        info.enableCullFace = GL_FALSE;
        info.cullFaceMode = GL_BACK;
        info.frontFaceMode = GL_CCW;
        info.enableDepthTest = GL_TRUE;
        info.depthFunc = GL_LESS;
        info.clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
        info.clearColor = {0.5f, 0.5f, 0.5f, 1.0f};
        info.clearDepth = 1.0f;
        info.lineWidth = 1.0f;
        info.pointSize = 1.0f;
        info.shaders = {
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/quad.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/quad.frag")};

        gQuad.Pipeline = Graphics::CreatePipeline(info);
        gQuad.Pipeline->Bind();
        gQuad.Ebo->Bind();
        gQuad.Vbo->Bind();
        GLsizei stride = 4 * sizeof(GLfloat);
        GLsizeiptr offset_pos = 0;
        GLsizeiptr offset_col = vertex_data_size / 2;
        std::vector<Graphics::AttributeDescription> attributes{
            {"a_pos", GL_FLOAT, GL_FLOAT_VEC4, stride, offset_pos, false, 0},
            {"a_col", GL_FLOAT, GL_FLOAT_VEC4, stride, offset_col, false, 0},
        };
        gQuad.Pipeline->SetAttribute(attributes);

        gQuad.Pipeline->Unbind();
    }
}

void OnMainLoop()
{
    auto viewport = Graphics::GetViewport();

    // Update the ModelView matrix.
    {
        float time = (float) glfwGetTime();
        float ang_x = 0.6 * time;
        float ang_y = 0.4 * time;
        float ang_z = 0.2 * time;
        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, ang_z);
        m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, ang_y);
        m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);

        float ratio = viewport.width / viewport.height;
        math::mat4f p = math::orthographic(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
        gQuad.ModelView = math::dot(p, m);
    }

    // Render the quad.
    {
        gQuad.Pipeline->Use();
        gQuad.Pipeline->SetUniform("u_width",  GL_FLOAT, &viewport.width);
        gQuad.Pipeline->SetUniform("u_height", GL_FLOAT, &viewport.height);
        gQuad.Pipeline->SetUniformMatrix(
            "u_mvp", GL_FLOAT_MAT4, true, gQuad.ModelView.data);
        gQuad.Pipeline->Clear();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid *) 0);
    }
}

int main(int argc, char const *argv[])
{
    Graphics::Settings settings = {};
    settings.WindowTitle = "06-quad";
    settings.WindowWidth = 800;
    settings.WindowHeight = 800;
    settings.GLVersionMajor = 3;
    settings.GLVersionMinor = 3;
    settings.PollTimeout = 0.01;
    settings.MaxFrames = 600;
    settings.OnInitialize = OnInitialize;
    settings.OnMainLoop = OnMainLoop;

    try {
        Graphics::MainLoop(settings);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
