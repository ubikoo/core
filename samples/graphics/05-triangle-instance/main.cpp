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
static const size_t kLatticeCells = 100;

struct Triangle {
    struct {
        std::vector<GLfloat, math::align_allocator<GLfloat>> data;
        Graphics::Buffer Vbo;
    } Offset;

    math::mat4f ModelView;
    Graphics::Buffer Vbo;
    Graphics::Pipeline Pipeline;
};
Triangle gTriangle;

/// -----------------------------------------------------------------------------
void Graphics::OnKeyboard(int code, int scancode, int action, int mods)
{}

void Graphics::OnMouseMove(double xpos, double ypos)
{}

void Graphics::OnMouseButton(int button, int action, int mods)
{}

void Graphics::OnInitialize()
{
    // Vertex positions and color attributes with layout:
    // {(xyzw)_1, (xyzw)_2, ..., (rgba)_1, (rgba)_2}
    const GLfloat scale = 1.0 / static_cast<GLfloat>(kLatticeCells);
    std::vector<GLfloat> vertex_data = {
        // positions
        -1.0f*scale, -1.0f*scale, 0.0f*scale, 1.0f, // bottom left
         1.0f*scale, -1.0f*scale, 0.0f*scale, 1.0f, // bottom right
        -0.0f*scale,  1.0f*scale, 0.0f*scale, 1.0f, // top
        // colors
         0.0f,  0.0f, 1.0f, 1.0f,                   // bottom left
         0.0f,  1.0f, 0.0f, 1.0f,                   // bottom right
         1.0f,  0.0f, 0.0f, 1.0f};                  // top
    GLsizeiptr vertex_data_size = vertex_data.size() * sizeof(GLfloat);

    // Create buffer storage for vertex position and color and specify how OpenGL
    // interprets the vertex attributes.
    {
        Graphics::BufferCreateInfo info = {};
        info.target = GL_ARRAY_BUFFER;
        info.size = vertex_data_size;
        info.usage = GL_STATIC_DRAW;

        gTriangle.Vbo = Graphics::CreateBuffer(info);
        gTriangle.Vbo->Copy(0, vertex_data_size, &vertex_data[0]);
    }

    // Compute the offset along each dimension and store it in a buffer.
    {
        gTriangle.Offset.data.clear();
        for (size_t i = 0; i < kLatticeCells; ++i) {
            for (size_t j = 0; j < kLatticeCells; ++j) {
                for (size_t k = 0; k < kLatticeCells; ++k) {
                    float x = -1.0f + 2.0f * scale * static_cast<GLfloat>(i);
                    float y = -1.0f + 2.0f * scale * static_cast<GLfloat>(j);
                    float z = -1.0f + 2.0f * scale * static_cast<GLfloat>(k);
                    gTriangle.Offset.data.push_back(x);
                    gTriangle.Offset.data.push_back(y);
                    gTriangle.Offset.data.push_back(z);
                }
            }
        }

        GLsizeiptr offset_data_size = gTriangle.Offset.data.size() * sizeof(GLfloat);

        Graphics::BufferCreateInfo info = {};
        info.target = GL_ARRAY_BUFFER;
        info.size = offset_data_size;
        info.usage = GL_STATIC_DRAW;

        gTriangle.Offset.Vbo = Graphics::CreateBuffer(info);
        gTriangle.Offset.Vbo->Copy(0, offset_data_size, &gTriangle.Offset.data[0]);
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

        gTriangle.Pipeline = Graphics::CreatePipeline(info);
        gTriangle.Pipeline->Bind();
        gTriangle.Vbo->Bind();
        GLsizei stride = 4 * sizeof(GLfloat);
        GLsizeiptr offset_pos = 0;
        GLsizeiptr offset_col = vertex_data_size / 2;
        std::vector<Graphics::AttributeDescription> vertex_attributes{
            {"a_pos", GL_FLOAT, GL_FLOAT_VEC4, stride, offset_pos, false, 0},
            {"a_col", GL_FLOAT, GL_FLOAT_VEC4, stride, offset_col, false, 0},
        };
        gTriangle.Pipeline->SetAttribute(vertex_attributes);

        gTriangle.Offset.Vbo->Bind();
        std::vector<Graphics::AttributeDescription> offset_attribute{
            {"a_offset", GL_FLOAT, GL_FLOAT_VEC3, 3 * sizeof(GLfloat), 0, false, 1},
        };
        gTriangle.Pipeline->SetAttribute(offset_attribute);
        gTriangle.Pipeline->Unbind();
    }
}

void Graphics::OnTerminate()
{}

void Graphics::OnMainLoop()
{
    // Update the ModelView matrix.
    {
        float time = (float) glfwGetTime();
        float ang_x = 0.6f * time;
        float ang_y = 0.4f * time;
        float ang_z = 0.2f * time;

        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, ang_z);
        m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, ang_y);
        m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);
        m = math::scale(m, math::vec3f::ones);

        auto viewport = Graphics::GetViewport();
        float ratio = viewport.width / viewport.height;
        math::mat4f p = math::orthographic(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
        gTriangle.ModelView = math::dot(p, m);
    }

    // Render the triangles. Draw a triangle associated with each offset.
    {
        gTriangle.Pipeline->Use();
        gTriangle.Pipeline->SetUniformMatrix(
            "u_mvp", GL_FLOAT_MAT4, true, gTriangle.ModelView.data);
        gTriangle.Pipeline->Clear();
        glDrawArraysInstanced(GL_TRIANGLES, 0, 3, gTriangle.Offset.data.size());
    }
}

/// -----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
    Graphics::Settings settings = {};
    settings.WindowTitle = "05-triangle-instance";
    settings.WindowWidth = 800;
    settings.WindowHeight = 800;
    settings.GLVersionMajor = 3;
    settings.GLVersionMinor = 3;
    settings.PollTimeout = 0.01;
    settings.MaxFrames = 600;

    try {
        Graphics::MainLoop(settings);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
