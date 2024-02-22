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
static const size_t kNumTriangles = 20;

///
/// @brief Create a new triangle.
///
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
        m_mvp.push_back(math::mat4f::eye);
    }

    // Create buffer storage for vertex position and color and specify how OpenGL
    // interprets the vertex attributes..
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

    // Triangle shader program object.
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
    std::array<GLint,4> viewport = {};
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);
    float width = static_cast<float>(viewport[2]);
    float height = static_cast<float>(viewport[3]);
    float ratio = width / height;
    float length = 0.5 * (float) m_mvp.size();

    math::mat4f project = math::orthographic(-ratio*length, ratio*length,
        -length, length, -length, length);

    // Update triangle modelviewprojection matrices
    float time = (float) glfwGetTime();
    for (size_t i = 0; i < m_mvp.size(); ++i) {
        math::mat4f &m = m_mvp[i];

        // rotate the matrix
        float ang_x = 0.6 * time * (float) (i+1) / (float) m_mvp.size();
        float ang_y = 0.4 * time * (float) (i+1) / (float) m_mvp.size();
        float ang_z = 0.2 * time * (float) (i+1) / (float) m_mvp.size();

        m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, ang_z);
        m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, ang_y);
        m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);

        // translate the matrix
        float radius = std::cos(0.5*time) * (float) m_mvp.size() / M_PI;
        float u1 = 2.0f * M_PI * (float) i / m_mvp.size();
        float u2 = 2.0f * (float) i / m_mvp.size() - 1.0;
        math::vec3f r_trans{radius * std::cos(u1), radius * std::sin(u1), u2};

        m = math::translate(m, r_trans);
        m = dot(project, m);
    }
}

///
/// @brief Render the
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
    {
        std::array<GLint,4> viewport = {};
        glGetIntegerv(GL_VIEWPORT, &viewport[0]);
        GLfloat width = viewport[2];
        GLfloat height = viewport[3];
        Graphics::SetUniform(m_program, "u_width", GL_FLOAT, &width);
        Graphics::SetUniform(m_program, "u_height", GL_FLOAT, &height);
    }
    for (auto &m : m_mvp) {
        Graphics::SetUniformMatrix(m_program, "u_mvp", GL_FLOAT_MAT4, true, m.data);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}
