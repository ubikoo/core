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
#include <float.h>
#include "core/graphics/graphics.h"

/// -----------------------------------------------------------------------------
static const std::vector<std::string> kFilenames = {
    "../assets/bunny.obj",
    "../assets/cumulus.obj"
};
static const float kScaleFactor = 1.05f;

struct Model {
    enum RotateMode : uint8_t { X = 0, Y, Z };
    uint8_t mRotate;
    float mScale;
    math::mat4f mModelView;
    uint8_t mCurrentMeshId;
    std::vector<Graphics::Mesh> mMesh;
    std::vector<Graphics::Pipeline> mPipeline;

    static void OnKeyboard(int code, int scancode, int action, int mods);

    void Initialize();
    void Render();
};
Model gModel;

void Model::OnKeyboard(int code, int scancode, int action, int mods)
{
    if (code == GLFW_KEY_1 && action == GLFW_RELEASE) {
        gModel.mCurrentMeshId = 0;
    }

    if (code == GLFW_KEY_2 && action == GLFW_RELEASE) {
        gModel.mCurrentMeshId = 1;
    }

    if (code == GLFW_KEY_X && action == GLFW_RELEASE) {
        gModel.mRotate = Model::RotateMode::X;
    }

    if (code == GLFW_KEY_Y && action == GLFW_RELEASE) {
        gModel.mRotate = Model::RotateMode::Y;
    }

    if (code == GLFW_KEY_Z && action == GLFW_RELEASE) {
        gModel.mRotate = Model::RotateMode::Z;
    }

    if (code == GLFW_KEY_UP && action == GLFW_REPEAT) {
        gModel.mScale *= kScaleFactor;
    }

    if (code == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
        gModel.mScale /= kScaleFactor;
    }
}

void Model::Initialize()
{
    // Initialize model parameters.
    {
        mRotate = Model::RotateMode::X;
        mScale = 1.0f;
        mModelView = math::mat4f::eye;
        mCurrentMeshId = 0;
    }

    // Load the model meshes.
    for (auto &filename : kFilenames) {
        Graphics::Mesh mesh = Graphics::LoadMesh("model", filename);

        // Reset mesh centre of mass position.
        math::vec3f com = {};
        for (auto &vertex : mesh->mVertices) {
            com.x += vertex.position[0];
            com.y += vertex.position[1];
            com.z += vertex.position[2];
        }
        com /= static_cast<float>(mesh->mVertices.size());
        for (auto &vertex : mesh->mVertices) {
            vertex.position[0] -= com.x;
            vertex.position[1] -= com.y;
            vertex.position[2] -= com.z;
        }

        // Compute the lower and upper ranges. With the newly computed ranges,
        // scale the model such that its largest size is unity.
        math::vec3f lo = { FLT_MAX,  FLT_MAX,  FLT_MAX};
        math::vec3f hi = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
        for (auto &vertex : mesh->mVertices) {
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
        for (auto &vertex : mesh->mVertices) {
            vertex.position[0] *= scale;
            vertex.position[1] *= scale;
            vertex.position[2] *= scale;
        }

        // Normalize mesh normals.
        for (auto &vertex : mesh->mVertices) {
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
        for (auto &vertex : mesh->mVertices) {
            vertex.texcoord[1] = 1.0f - vertex.texcoord[1];
        }

        // Upload mesh data onto the gpu and store it.
        mesh->Copy();
        mMesh.push_back(std::move(mesh));
    }

    // Create a rendering pipeline for each mesh.
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
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/model.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/model.frag")};

        for (auto &mesh : mMesh) {
            mPipeline.push_back(Graphics::CreatePipeline(info));
            mPipeline.back()->Bind();
            mesh->Bind();
            mPipeline.back()->SetAttribute(mesh->mAttributes);
            mPipeline.back()->Unbind();
        }
    }
}

void Model::Render()
{
    // Update the model.
    {
        float ang = (float) glfwGetTime();
        float ang_x = (mRotate == Model::RotateMode::X) ? ang : 0.0f;
        float ang_y = (mRotate == Model::RotateMode::Y) ? ang : 0.0f;
        float ang_z = (mRotate == Model::RotateMode::Z) ? ang : 0.0f;

        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, ang_z);
        m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, ang_y);
        m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);
        m = math::scale(m, math::vec3f{mScale, mScale, mScale});

        auto viewport = Graphics::GetViewport();
        float ratio = viewport.width / viewport.height;
        math::mat4f p = math::orthographic(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
        mModelView = math::dot(p, m);
    }

    // Render the model.
    {
        size_t id = mCurrentMeshId;
        mPipeline[id]->Use();
        mPipeline[id]->SetUniformMatrix("u_mvp", GL_FLOAT_MAT4, true,
            mModelView.data);
        mPipeline[id]->Clear();
        mMesh[id]->Render();
    }
}

/// -----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
    Graphics::Settings settings = {};
    settings.WindowTitle = "09-mesh-model";
    settings.WindowWidth = 800;
    settings.WindowHeight = 800;
    settings.GLVersionMajor = 3;
    settings.GLVersionMinor = 3;
    settings.PollTimeout = 0.01;
    settings.MaxFrames = 600;
    settings.OnKeyboard = Model::OnKeyboard;
    settings.OnMouseMove = nullptr;
    settings.OnMouseButton = nullptr;

    Graphics::Initialize(settings);
    gModel.Initialize();
    while (!Graphics::ShouldClose()) {
        gModel.Render();
        Graphics::Present();
    }
    Graphics::Terminate();

    return EXIT_SUCCESS;
}
