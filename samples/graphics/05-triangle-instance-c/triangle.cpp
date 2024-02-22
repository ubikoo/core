//
// triangle.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <array>
#include "core/graphics/graphics.h"
#include "triangle.h"

///
/// @brief Number of triangle instances.
///
static const size_t kNumCells = 100;

///
/// @brief Create a new triangle.
///
void Triangle::Initialize()
{
    // Vertex positions and color attributes with layout:
    // {(xyzw)_1, (xyzw)_2, ..., (rgba)_1, (rgba)_2}
    const GLfloat scale = 1.0 / static_cast<GLfloat>(kNumCells);
    std::vector<GLfloat> vertex_data = {
        // positions
        -1.0f*scale, -1.0f*scale, 0.0f*scale, 1.0f, // bottom left
         1.0f*scale, -1.0f*scale, 0.0f*scale, 1.0f, // bottom right
        -0.0f*scale,  1.0f*scale, 0.0f*scale, 1.0f, // top
        // colors
         0.0f,  0.0f, 1.0f, 1.0f,                   // bottom left
         0.0f,  1.0f, 0.0f, 1.0f,                   // bottom right
         1.0f,  0.0f, 0.0f, 1.0f};                  // top

    // Create a buffer storage for the vertex position and color and specify how
    // OpenGL interprets the vertex attributes.
    GLsizeiptr vertex_data_size = vertex_data.size() * sizeof(GLfloat);
    {
        Graphics::BufferCreateInfo info = {};
        info.target = GL_ARRAY_BUFFER;
        info.size = vertex_data_size;
        info.usage = GL_STATIC_DRAW;
        m_vbo = Graphics::CreateBuffer(info);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_data_size, &vertex_data[0]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // Compute the offset along each dimension.
    m_offset.data.clear();
    for (size_t i = 0; i < kNumCells; ++i) {
        for (size_t j = 0; j < kNumCells; ++j) {
            for (size_t k = 0; k < kNumCells; ++k) {
                float x = -1.0f + 2.0f * scale * static_cast<GLfloat>(i);
                float y = -1.0f + 2.0f * scale * static_cast<GLfloat>(j);
                float z = -1.0f + 2.0f * scale * static_cast<GLfloat>(k);
                m_offset.data.push_back(x);
                m_offset.data.push_back(y);
                m_offset.data.push_back(z);
            }
        }
    }

    // Create the buffer storage for the offsets.
    GLsizeiptr offset_data_size = m_offset.data.size() * sizeof(GLfloat);
    {
        Graphics::BufferCreateInfo info = {};
        info.target = GL_ARRAY_BUFFER;
        info.size = offset_data_size;
        info.usage = GL_STATIC_DRAW;
        m_offset.vbo = Graphics::CreateBuffer(info);

        glBindBuffer(GL_ARRAY_BUFFER, m_offset.vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, offset_data_size, &m_offset.data[0]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // Create the shader program object.
    {
        std::vector<GLuint> shaders{
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/triangle.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/triangle.frag")};
        m_program = Graphics::CreateProgram(shaders);
        Graphics::DestroyShaders(shaders);
        std::cout << Graphics::GetProgramInfoString(m_program) << "\n";
    }

    // Create vertex array object.
    {
        m_vao = Graphics::CreateVertexArray();
        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        GLsizei stride = 4 * sizeof(GLfloat);
        GLsizeiptr offset_pos = 0;
        GLsizeiptr offset_col = vertex_data_size / 2;
        std::vector<Graphics::VertexAttributeDescription> attributes{
            {"a_pos", GL_FLOAT, GL_FLOAT_VEC4, stride, offset_pos, false, 0},
            {"a_col", GL_FLOAT, GL_FLOAT_VEC4, stride, offset_col, false, 0},
        };
        SetVertexAttributes(m_program, attributes);

        glBindBuffer(GL_ARRAY_BUFFER, m_offset.vbo);
        Graphics::VertexAttributeDescription attribute{
            "a_offset", GL_FLOAT, GL_FLOAT_VEC3, sizeof(GLfloat), 0, false, 1};
        SetVertexAttribute(m_program, attribute);
        glBindVertexArray(0);
    }
}

///
/// @brief Destroy the triangle.
///
void Triangle::Cleanup()
{
    Graphics::DestroyBuffer(m_vbo);
    Graphics::DestroyVertexArray(m_vao);
    Graphics::DestroyProgram(m_program);
}

///
/// @brief Update the triangle.
///
void Triangle::Update()
{
    // Update the modelviewprojection matrix
    float time = (float) glfwGetTime();
    float ang_x = 0.3f * time;
    float ang_y = 0.2f * time;
    float ang_z = 0.1f * time;

    math::mat4f m = math::mat4f::eye;
    m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, ang_z);
    m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, ang_y);
    m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);
    m = math::scale(m, math::vec3f::ones);

    std::array<GLint,4> viewport = {};
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);
    float width = static_cast<float>(viewport[2]);
    float height = static_cast<float>(viewport[3]);
    float ratio = width / height;

    math::mat4f p = math::orthographic(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
    m_mvp = math::dot(p, m);
}

///
/// @brief Render the triangle.
///
void Triangle::Render()
{
    // Specify draw state modes.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Bind the shader program object and draw.
    glUseProgram(m_program);
    glBindVertexArray(m_vao);
    Graphics::SetUniformMatrix(m_program, "u_mvp", GL_FLOAT_MAT4, true, m_mvp.data);

    // Draw a triangle for each offset element in the array.
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, m_offset.data.size());
    glBindVertexArray(0);
    glUseProgram(0);
}