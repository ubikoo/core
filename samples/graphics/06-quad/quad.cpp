//
// quad.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <array>
#include "core/graphics/graphics.h"
#include "quad.h"


///
/// @brief Create a new quad.
///
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
    // interprets the vertex attributes..
    {
        Graphics::BufferCreateInfo vbo_info = {};
        vbo_info.target = GL_ARRAY_BUFFER;
        vbo_info.size = vertex_data_size;
        vbo_info.usage = GL_STATIC_DRAW;
        m_vbo = Graphics::CreateBuffer(vbo_info);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_data_size, &vertex_data[0]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // Create a buffer storage for the vertex indices.
    {
        Graphics::BufferCreateInfo ebo_info = {};
        ebo_info.target = GL_ELEMENT_ARRAY_BUFFER;
        ebo_info.size = index_data_size;
        ebo_info.usage = GL_STATIC_DRAW;
        m_ebo = Graphics::CreateBuffer(ebo_info);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, index_data_size,
            &index_data[0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    // Create the shader program object.
    {
        std::vector<GLuint> shaders{
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/quad.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/quad.frag")};
        m_program = Graphics::CreateProgram(shaders);
        Graphics::DestroyShaders(shaders);
        std::cout << Graphics::GetProgramInfoString(m_program) << "\n";
    }

    // Create vertex array object.
    {
        m_vao = Graphics::CreateVertexArray();
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

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
/// @brief Destroy the quad.
///
void Quad::Cleanup()
{
    Graphics::DestroyBuffer(m_ebo);
    Graphics::DestroyBuffer(m_vbo);
    Graphics::DestroyVertexArray(m_vao);
    Graphics::DestroyProgram(m_program);
}

///
/// @brief Update the quad.
///
void Quad::Update()
{
    // Update the modelviewprojection matrix
    float time = (float) glfwGetTime();
    float ang_x = 0.6 * time;
    float ang_y = 0.4 * time;
    float ang_z = 0.2 * time;
    math::mat4f m = math::mat4f::eye;
    m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, ang_z);
    m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, ang_y);
    m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);

    std::array<GLint,4> viewport = {};
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);
    float width = static_cast<float>(viewport[2]);
    float height = static_cast<float>(viewport[3]);
    float ratio = width / height;

    math::mat4f p = math::orthographic(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
    m_mvp = math::dot(p, m);
}

///
/// @brief Render the quad.
///
void Quad::Render()
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
        Graphics::SetUniformMatrix(m_program, "u_mvp", GL_FLOAT_MAT4, true, m_mvp.data);
    }

    // Draw the two triangles comprising the quad.
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid *) 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
