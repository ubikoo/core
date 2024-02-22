//
// model.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <array>
#include <cfloat>
#include "core/graphics/graphics.h"
#include "model.h"

static const std::vector<std::string> kFilenames = {
    "../assets/bunny.obj",
    "../assets/cumulus.obj"
};

///
/// @brief Create a new model.
///
void Model::Initialize()
{
    // Initialize rotation, scale and model view projection matrix.
    {
        m_rotate = Rotate::X;
        m_scale = 1.0f;
        m_mvp = math::mat4f::eye;
    }

    // Load the model meshes.
    m_current_mesh = 0;
    for (auto &filename : kFilenames) {
        Graphics::Mesh mesh = Graphics::LoadMesh("model", filename);

        // Reset mesh centre of mass position.
        math::vec3f com = {};
        for (auto &vertex : mesh.vertices) {
            com.x += vertex.position[0];
            com.y += vertex.position[1];
            com.z += vertex.position[2];
        }
        com /= static_cast<float>(mesh.vertices.size());
        for (auto &vertex : mesh.vertices) {
            vertex.position[0] -= com.x;
            vertex.position[1] -= com.y;
            vertex.position[2] -= com.z;
        }

        //
        // Compute the lower and upper ranges. With the newly computed
        // ranges, scale the model such that its largest size is unity.
        //
        math::vec3f lo = { FLT_MAX,  FLT_MAX,  FLT_MAX};
        math::vec3f hi = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
        for (auto &vertex : mesh.vertices) {
            lo.x = std::min(lo.x, vertex.position[0]);
            lo.y = std::min(lo.y, vertex.position[1]);
            lo.z = std::min(lo.z, vertex.position[2]);

            hi.x = std::max(hi.x, vertex.position[0]);
            hi.y = std::max(hi.y, vertex.position[1]);
            hi.z = std::max(hi.z, vertex.position[2]);
        }

        math::vec3f range = math::abs(hi - lo);
        float max_range = std::max(range.x, std::max(range.y, range.z));
        float scale = max_range > 0.0f ? 1.0f / max_range : 1.0f;
        std::cout << filename << " " << scale << "\n";
        for (auto &vertex : mesh.vertices) {
            vertex.position[0] *= scale;
            vertex.position[1] *= scale;
            vertex.position[2] *= scale;
        }

        // Normalize mesh normals.
        for (auto &vertex : mesh.vertices) {
            math::vec3f normal = {
                vertex.normal[0],
                vertex.normal[1],
                vertex.normal[2]
            };
            normal = math::normalize(normal);
            vertex.normal[0] = normal.x;
            vertex.normal[1] = normal.y;
            vertex.normal[2] = normal.z;
        }

        // Flip mesh vertically.
        for (auto &vertex : mesh.vertices) {
            vertex.texcoord[1] = 1.0f - vertex.texcoord[1];
        }

        // Upload mesh data onto the gpu.
        Graphics::UpdateMesh(mesh);
        m_mesh.push_back(mesh);
    }

    // Create the shader program object.
    {
        std::vector<GLuint> shaders{
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/model.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/model.frag")};
        m_program = Graphics::CreateProgram(shaders);
        Graphics::DestroyShaders(shaders);
        std::cout << Graphics::GetProgramInfoString(m_program) << "\n";
    }

    // Create vertex array object.
    for (auto &mesh : m_mesh) {
        m_vao.push_back(Graphics::CreateVertexArray());
        glBindVertexArray(m_vao.back());
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
        SetVertexAttributes(m_program, mesh.attributes);
        glBindVertexArray(0);
    }
}

///
/// @brief Destroy the model.
///
void Model::Cleanup()
{
    for (auto &mesh : m_mesh) {
        Graphics::DestroyMesh(mesh);
    }
    for (auto &vao : m_vao) {
        Graphics::DestroyVertexArray(vao);
    }
    Graphics::DestroyProgram(m_program);
}

///
/// @brief Update the model.
///
void Model::Update()
{
    // Update the modelviewprojection matrix
    float ang = (float) glfwGetTime();
    float ang_x = (m_rotate == Rotate::X) ? ang : 0.0f;
    float ang_y = (m_rotate == Rotate::Y) ? ang : 0.0f;
    float ang_z = (m_rotate == Rotate::Z) ? ang : 0.0f;

    math::mat4f m = math::mat4f::eye;
    m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, ang_z);
    m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, ang_y);
    m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);
    m = math::scale(m, math::vec3f{m_scale, m_scale, m_scale});

    std::array<GLint,4> viewport = {};
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);
    float width = static_cast<float>(viewport[2]);
    float height = static_cast<float>(viewport[3]);
    float ratio = width / height;

    math::mat4f p = math::orthographic(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
    m_mvp = math::dot(p, m);
}

///
/// @brief Render the model.
///
void Model::Render()
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
    glBindVertexArray(m_vao[m_current_mesh]);
    Graphics::SetUniformMatrix(m_program, "u_mvp", GL_FLOAT_MAT4, true, m_mvp.data);
    Graphics::RenderMesh(m_mesh[m_current_mesh]);
    glBindVertexArray(0);
    glUseProgram(0);
}
