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
    math::mat4f mModelView;
    Graphics::Buffer mVbo;
    Graphics::Buffer mEbo;
    Graphics::Pipeline mPipeline;

    void Initialize();
    void Render();
};
Quad gQuad;

void Quad::Initialize()
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

        mVbo = Graphics::CreateBuffer(info);
        mVbo->Copy(0, vertex_data_size, &vertex_data[0]);
    }

    // Create a buffer storage for the vertex indices.
    {
        Graphics::BufferCreateInfo info = {};
        info.target = GL_ELEMENT_ARRAY_BUFFER;
        info.size = index_data_size;
        info.usage = GL_STATIC_DRAW;

        mEbo = Graphics::CreateBuffer(info);
        mEbo->Copy(0, index_data_size, &index_data[0]);
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

        mPipeline = Graphics::CreatePipeline(info);
        mPipeline->Bind();
        mEbo->Bind();
        mVbo->Bind();
        GLsizei stride = 4 * sizeof(GLfloat);
        GLsizeiptr offset_pos = 0;
        GLsizeiptr offset_col = vertex_data_size / 2;
        std::vector<Graphics::AttributeDescription> attributes{
            {"inPos", GL_FLOAT, GL_FLOAT_VEC4, stride, offset_pos, false, 0},
            {"inCol", GL_FLOAT, GL_FLOAT_VEC4, stride, offset_col, false, 0},
        };
        mPipeline->SetAttribute(attributes);

        mPipeline->Unbind();
    }
}

void Quad::Render()
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
        mModelView = math::dot(p, m);
    }

    // Render the quad.
    {
        mPipeline->Use();
        mPipeline->SetUniform("uWidth",  GL_FLOAT, &viewport.width);
        mPipeline->SetUniform("uHeight", GL_FLOAT, &viewport.height);
        mPipeline->SetUniformMatrix("uMvp", GL_FLOAT_MAT4, true,
            mModelView.data);
        mPipeline->Clear();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid *) 0);
    }
}

/// -----------------------------------------------------------------------------
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
    settings.OnKeyboard = nullptr;
    settings.OnMouseMove = nullptr;
    settings.OnMouseButton = nullptr;

    Graphics::Initialize(settings);
    gQuad.Initialize();
    while (!Graphics::ShouldClose()) {
        gQuad.Render();
        Graphics::Present();
    }
    Graphics::Terminate();

    return EXIT_SUCCESS;
}
