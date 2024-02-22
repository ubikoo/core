//
// sphere.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <array>
#include "core/graphics/graphics.h"
#include "sphere.h"

static const std::string kImageFilename = "../assets/equirectangular.png";
static const size_t kMeshNodes = 1024;

///
/// @brief Create a new sphere.
///
void Sphere::Initialize()
{
    // Initialize mvp matrix and create a mesh over a rectangle.
    {
        m_mvp = math::mat4f::eye;
        m_mesh = Graphics::CreateSphere(
            "sphere",                   // vertex attributes prefix
            kMeshNodes,                 // n1 vertices
            kMeshNodes,                 // n2 vertices
            1.0,                        // radius
            0.0,                        // theta_lo
            M_PI,                       // theta_hi
            -M_PI,                      // phi_lo
            M_PI);                      // phi_hi
    }

    // Load the 2d-image from the specified filename
    {
        m_image = Graphics::LoadImage(kImageFilename, true, 4);

        Graphics::Texture2dCreateInfo texture_info = {};
        texture_info.width = m_image.width;
        texture_info.height = m_image.height;
        texture_info.internalformat = GL_RGBA8;
        texture_info.pixelformat = m_image.format;
        texture_info.pixeltype = GL_UNSIGNED_BYTE;
        texture_info.pixels = &m_image.bitmap[0];
        m_texture = Graphics::CreateTexture2d(texture_info);

        glBindTexture(GL_TEXTURE_2D, m_texture);
        Graphics::SetTextureMipmap(GL_TEXTURE_2D);
        Graphics::SetTextureWrap(GL_TEXTURE_2D, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        Graphics::SetTextureFilter(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Create the shader program object.
    {
        std::vector<GLuint> shaders{
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/sphere.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/sphere.frag")};
        m_program = Graphics::CreateProgram(shaders);
        Graphics::DestroyShaders(shaders);
        std::cout << Graphics::GetProgramInfoString(m_program) << "\n";
    }

    // Create vertex array object.
    {
        m_vao = Graphics::CreateVertexArray();
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_mesh.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.ebo);
        SetVertexAttributes(m_program, m_mesh.attributes);
        glBindVertexArray(0);
    }
}

///
/// @brief Destroy the sphere.
///
void Sphere::Cleanup()
{
    Graphics::DestroyMesh(m_mesh);
    Graphics::DestroyTexture(m_texture);
    Graphics::DestroyVertexArray(m_vao);
    Graphics::DestroyProgram(m_program);
}

///
/// @brief Update the sphere.
///
void Sphere::Update()
{
    // Update the modelviewprojection matrix
    float time = (float) glfwGetTime();

    math::mat4f m = math::mat4f::eye;
    m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, (float) (0.5*M_PI));

    math::vec4f dir_y = math::dot(m, math::vec4f{0.0, 1.0, 0.0, 1.0});
    m = math::rotate(m, math::vec3f{dir_y.x, dir_y.y, dir_y.z}, 0.2f*time);

    math::vec4f dir_z = math::dot(m, math::vec4f{0.0, 0.0, 1.0, 1.0});
    m = math::rotate(m, math::vec3f{dir_z.x, dir_z.y, dir_z.z}, 0.8f*time);

    std::array<GLint,4> viewport = {};
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);
    float width = static_cast<float>(viewport[2]);
    float height = static_cast<float>(viewport[3]);
    float ratio = width / height;

    math::mat4f p = math::orthographic(-ratio, ratio, -1.0f, 1.0f, 0.1f, 1.0f);
    m_mvp = math::dot(p, m);
}

///
/// @brief Render the sphere.
///
void Sphere::Render()
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
        Graphics::SetUniformMatrix(m_program, "u_mvp", GL_FLOAT_MAT4, true,
            m_mvp.data);
    }
    GLenum texunit = 0;
    Graphics::SetUniform(m_program, "u_texsampler", GL_SAMPLER_2D, &texunit);
    Graphics::BindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit, m_texture);
    Graphics::RenderMesh(m_mesh);
    glBindVertexArray(0);
    glUseProgram(0);
}
