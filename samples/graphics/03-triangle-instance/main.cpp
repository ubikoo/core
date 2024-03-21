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
static const size_t kNumTriangles = 30;

struct Triangle {
    std::vector<math::mat4f, math::align_allocator<math::mat4f>> mModelView;
    Graphics::Buffer mVbo;               // vertex buffer object
    Graphics::Pipeline mPipeline;        // program rendering pipeline

    void Initialize();
    void Render();
};
Triangle gTriangle;

void Triangle::Initialize()
{
    // Vertex positions and color attributes with layout:
    // {(xyzw)_1, (xyzw)_2, ..., (rgba)_1, (rgba)_2}
    const std::vector<GLfloat> vertex_data = {
        // positions
        -0.5f, -0.5f, 0.0f, 1.0f,   // bottom left
         0.5f, -0.5f, 0.0f, 1.0f,   // bottom right
        -0.5f,  0.5f, 0.0f, 1.0f,   // top left
         0.5f,  0.5f, 0.0f, 1.0f,   // top right
        // colors
         0.0f,  0.0f, 1.0f, 1.0f,   // bottom left
         0.0f,  1.0f, 0.0f, 1.0f,   // bottom right
         1.0f,  0.0f, 0.0f, 1.0f,   // top left
         1.0f,  0.0f, 0.0f, 1.0f};  // top right
    const GLsizeiptr vertex_data_size = vertex_data.size() * sizeof(GLfloat);

    // Compute the triangle offset along each dimension.
    for (size_t i = 0; i < kNumTriangles; ++i) {
        mModelView.push_back(math::mat4f::eye);
    }

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

    // Create the triangle rendering pipeline.
    {
        Graphics::PipelineCreateInfo info = {};
        info.polygonMode = GL_FILL;
        info.enableCullFace = GL_TRUE;
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
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/triangle.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/triangle.frag")};

        mPipeline = Graphics::CreatePipeline(info);
        mPipeline->Bind();
        mVbo->Bind();
        GLsizei stride = 4 * sizeof(GLfloat);
        GLsizeiptr offset_pos = 0;
        GLsizeiptr offset_col = vertex_data_size / 2;
        std::vector<Graphics::AttributeDescription> attributes{
            {"a_pos", GL_FLOAT, GL_FLOAT_VEC4, stride, offset_pos, false, 0},
            {"a_col", GL_FLOAT, GL_FLOAT_VEC4, stride, offset_col, false, 0}};
        mPipeline->SetAttribute(attributes);
        mPipeline->Unbind();
    }
}

void Triangle::Render()
{
    auto viewport = Graphics::GetViewport();

    // Update triangle ModelView matrices.
    {
        float ratio = viewport.width / viewport.height;
        float length = 0.5f * (float) mModelView.size();
        math::mat4f projection = math::orthographic(
            -ratio * length, ratio * length,
            -length, length,
            -length, length);

        float time = (float) glfwGetTime();
        for (size_t i = 0; i < mModelView.size(); ++i) {
            float n_matrices = (float) mModelView.size();

            // rotate the matrix
            float ang_x = 0.8f * time * (float) (i+1) / n_matrices;
            float ang_y = 0.6f * time * (float) (i+1) / n_matrices;
            float ang_z = 0.4f * time * (float) (i+1) / n_matrices;

            math::mat4f &m = mModelView[i];
            m = math::mat4f::eye;
            m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, ang_z);
            m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, ang_y);
            m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);

            // translate the matrix
            float radius = std::cos(time) * n_matrices / M_PI;
            float u1 = 2.0f * M_PI * (float) i / n_matrices;
            float u2 = 2.0f * (float) i / n_matrices - 1.0;
            math::vec3f trans{radius * std::cos(u1), radius * std::sin(u1), u2};
            m = math::translate(m, trans);
            m = dot(projection, m);
        }
    }

    // Render the triangles.
    {
        mPipeline->Use();
        mPipeline->SetUniform("u_width", GL_FLOAT, &viewport.width);
        mPipeline->SetUniform("u_height", GL_FLOAT, &viewport.height);
        mPipeline->Clear();
        for (auto &m : mModelView) {
            mPipeline->SetUniformMatrix("u_mvp", GL_FLOAT_MAT4, true, m.data);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    }
}

/// -----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
    Graphics::Settings settings = {};
    settings.WindowTitle = "03-triangle-instance";
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

    gTriangle.Initialize();
    while (!Graphics::ShouldClose()) {
        gTriangle.Render();
        Graphics::Present();
    }

    Graphics::Terminate();

    return EXIT_SUCCESS;
}
