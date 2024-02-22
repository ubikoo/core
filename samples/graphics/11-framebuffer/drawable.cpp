//
// drawable.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <array>
#include "core/graphics/graphics.h"
#include "drawable.h"

static const std::string kImageFilename = "../assets/equirectangular.png";
static const size_t kMeshNodes = 1024;

///
/// @brief Create a new drawable.
///
void Drawable::Initialize()
{
    InitializeSphere();
    InitializeQuad();
    InitalizeFramebuffer();
}

///
/// @brief Intialize the sphere draw pass.
///
void Drawable::InitializeSphere()
{
    // Initialize sphere projection matrix.
    m_sphere.mvp = math::mat4f::eye;

    // Create a mesh over a sphere.
    m_sphere.mesh = Graphics::CreateSphere(
        "sphere",                   // vertex attributes prefix
        kMeshNodes,                 // n1 vertices
        kMeshNodes,                 // n2 vertices
        1.0,                        // radius
        0.0,                        // theta_lo
        M_PI,                       // theta_hi
        -M_PI,                      // phi_lo
        M_PI);                      // phi_hi

    // Load the 2d-image from the specified filename.
    Graphics::Image image = Graphics::LoadImage(kImageFilename);

    Graphics::Texture2dCreateInfo texture_info = {};
    texture_info.width = image.width;
    texture_info.height = image.height;
    texture_info.internalformat = GL_RGBA8;
    texture_info.pixelformat = image.format;
    texture_info.pixeltype = GL_UNSIGNED_BYTE;
    texture_info.pixels = &image.bitmap[0];
    m_sphere.texture = Graphics::CreateTexture2d(texture_info);

    // Create the sphere shader program object.
    std::vector<GLuint> shaders{
        Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/sphere.vert"),
        Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/sphere.frag")};
    m_sphere.program = Graphics::CreateProgram(shaders);
    Graphics::DestroyShaders(shaders);
    std::cout << Graphics::GetProgramInfoString(m_sphere.program) << "\n";

    glBindTexture(GL_TEXTURE_2D, m_sphere.texture);
    Graphics::SetTextureMipmap(GL_TEXTURE_2D);
    Graphics::SetTextureWrap(GL_TEXTURE_2D, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    Graphics::SetTextureFilter(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create the sphere vertex array object.s
    m_sphere.vao = Graphics::CreateVertexArray();
    glBindVertexArray(m_sphere.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_sphere.mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphere.mesh.ebo);
    SetVertexAttributes(m_sphere.program, m_sphere.mesh.attributes);
    glBindVertexArray(0);
}

///
/// @brief Intialize the quad drawable.
///
void Drawable::InitializeQuad()
{
    // Initialize the quad projection matrix.
    m_quad.mvp = math::mat4f::eye;

    // Create a plane mesh over a quad.
    m_quad.mesh = Graphics::CreatePlane(
        "quad",                     // vertex attributes prefix
        kMeshNodes,                 // n1 vertices
        kMeshNodes,                 // n2 vertices
        -1.0,                       // xlo
        1.0,                        // xhi
        -1.0,                       // ylo
        1.0);                       // yhi

    // Create the quad shader program object.
    std::vector<GLuint> shaders{
        Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/quad.vert"),
        Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/quad.frag")};
    m_quad.program = Graphics::CreateProgram(shaders);
    Graphics::DestroyShaders(shaders);
    std::cout << Graphics::GetProgramInfoString(m_quad.program) << "\n";

    // Create the sphere vertex array object.s
    m_quad.vao = Graphics::CreateVertexArray();
    glBindVertexArray(m_quad.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_quad.mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quad.mesh.ebo);
    SetVertexAttributes(m_quad.program, m_quad.mesh.attributes);
    glBindVertexArray(0);
}

///
/// @brief Create a framebuffer with color and depth attachments.
///
void Drawable::InitalizeFramebuffer()
{
    // Set the fbo size equal to the sphere dimensions.
    glBindTexture(GL_TEXTURE_2D, m_sphere.texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &m_fbo.width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT,&m_fbo.height);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create the fbo with color and depth attachments.
    Graphics::FramebufferCreateInfo info = {};
    info.width = m_fbo.width;
    info.height = m_fbo.height;
    info.filter_min = GL_LINEAR;
    info.filter_mag = GL_LINEAR;
    info.n_color_attachments = 1;
    info.color_internalformat = GL_RGBA;
    info.color_pixelformat = GL_RGBA;
    info.color_pixeltype = GL_UNSIGNED_BYTE;
    info.depth_internalformat = GL_DEPTH_COMPONENT24;
    info.depth_pixelformat = GL_DEPTH_COMPONENT;
    info.depth_pixeltype = GL_UNSIGNED_BYTE;

    m_fbo.id = Graphics::CreateFramebuffer(info,
        &m_fbo.color_texture, &m_fbo.depth_texture);
}

///
/// @brief Destroy the drawable.
///
void Drawable::Cleanup()
{
    // Destroy sphere objects
    Graphics::DestroyMesh(m_sphere.mesh);
    Graphics::DestroyTexture(m_sphere.texture);
    Graphics::DestroyVertexArray(m_sphere.vao);
    Graphics::DestroyProgram(m_sphere.program);

    // Destroy quad objects
    Graphics::DestroyMesh(m_quad.mesh);
    Graphics::DestroyVertexArray(m_quad.vao);
    Graphics::DestroyProgram(m_quad.program);

    // Destroy fbo objects.
    Graphics::DestroyTexture(m_fbo.color_texture);
    Graphics::DestroyTexture(m_fbo.depth_texture);
    Graphics::DestroyFramebuffer(m_fbo.id);
}

///
/// @brief Update the drawable.
///
void Drawable::Update()
{
    // Compute the orthographic projection matrix
    float time = (float) glfwGetTime();

    // Update the sphere modelviewprojection matrix
    {
        float ang_x = 0.0f * time;
        float ang_y = 2.0f * time;
        float ang_z = 0.0f * time;

        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);

        math::vec4f dir_y = math::dot(m, math::vec4f{0.0f,1.0f,0.0f,1.0f});
        m = math::rotate(m, math::vec3f{dir_y.x, dir_y.y, dir_y.z}, ang_y);

        math::vec4f dir_z = math::dot(m, math::vec4f{0.0f,0.0f,1.0f,1.0f});
        m = math::rotate(m, math::vec3f{dir_z.x, dir_z.y, dir_z.z}, ang_z);

        std::array<GLint,4> viewport = {};
        glGetIntegerv(GL_VIEWPORT, &viewport[0]);
        float width = static_cast<float>(viewport[2]);
        float height = static_cast<float>(viewport[3]);
        float ratio = width / height;

        math::mat4f proj = math::orthographic(-ratio, ratio, -1.0f, 1.0f,
            -1.0f, 1.0f);
        m_sphere.mvp = math::dot(proj, m);
    }

    // Update the quad modelviewprojection matrix
    {
        float ang_x = 0.05f * time;
        float ang_y = 0.05f * time;
        float ang_z = 0.05f * time;

        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, ang_z);
        m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, ang_y);
        m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);

        float ratio = (float) m_fbo.width / m_fbo.height;
        math::mat4f proj = math::orthographic(
            -ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
        m_quad.mvp = math::dot(proj, m);
    }
}

///
/// @brief Render the drawable.
///
void Drawable::Render()
{
    // Specify draw state modes.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Render into the framebuffer rendertexture
    {
        // Bind the framebuffer
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo.id);
        std::array<GLint, 4> viewport;
        glGetIntegerv(GL_VIEWPORT, &viewport[0]);
        glViewport(0, 0, m_fbo.width, m_fbo.height);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind the sphere shader and draw.
        glUseProgram(m_sphere.program);
        glBindVertexArray(m_sphere.vao);
        // Graphics::SetUniform(sphere.program, "u_width", GL_FLOAT, &m_fbo.width);
        // Graphics::SetUniform(sphere.program, "u_height", GL_FLOAT, &m_fbo.height);
        Graphics::SetUniformMatrix(m_sphere.program, "u_mvp", GL_FLOAT_MAT4, true,
            m_sphere.mvp.data);
        GLenum texunit = 0;
        Graphics::SetUniform(m_sphere.program, "u_texsampler", GL_SAMPLER_2D,
            &texunit);
        Graphics::BindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit,
            m_sphere.texture);
        Graphics::RenderMesh(m_sphere.mesh);
        glBindVertexArray(0);
        glUseProgram(0);

        // Unbind the framebuffer
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    }

    //
    // Render into the window framebuffer
    //
    {
        // Bind the quad shader and draw.
        glUseProgram(m_quad.program);
        glBindVertexArray(m_quad.vao);
        // std::array<GLint,4> viewport = {};
        // glGetIntegerv(GL_VIEWPORT, &viewport[0]);
        // GLfloat width = viewport[2];
        // GLfloat height = viewport[3];
        // Graphics::SetUniform(quad.program, "u_width", GL_FLOAT, &width);
        // Graphics::SetUniform(quad.program, "u_height", GL_FLOAT, &height);
        Graphics::SetUniformMatrix(m_quad.program, "u_mvp", GL_FLOAT_MAT4, true,
            m_quad.mvp.data);
        GLenum texunit = 0;
        Graphics::SetUniform(m_quad.program, "u_texsampler", GL_SAMPLER_2D,
            &texunit);
        Graphics::BindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit,
            m_fbo.color_texture);
        Graphics::RenderMesh(m_quad.mesh);
        glBindVertexArray(0);
        glUseProgram(0);
    }
}
