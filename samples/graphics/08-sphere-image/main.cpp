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
static const std::string kImageFilename = "../assets/equirectangular.png";
static const size_t kMeshNodes = 1024;

struct Sphere {
    math::mat4f mModelView;
    Graphics::Mesh mMesh;
    Graphics::Image mImage;
    Graphics::Texture mTexture;
    Graphics::Pipeline mPipeline;

    void Initialize();
    void Render();
};
Sphere gSphere;

void Sphere::Initialize()
{
    // Initialize mvp matrix and create a mesh over a rectangle.
    {
        mModelView = math::mat4f::eye;
        mMesh = Graphics::CreateSphere(
            "Sphere",                   // vertex attributes prefix
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
        mImage = Graphics::LoadImage(kImageFilename, true, 4);

        Graphics::TextureCreateInfo info = {};
        info.target = GL_TEXTURE_2D;
        info.width = mImage->mWidth;
        info.height = mImage->mHeight;
        info.internalFormat = GL_RGBA8;
        info.pixelFormat = mImage->mFormat;
        info.pixelType = GL_UNSIGNED_BYTE;
        info.pixels = &mImage->mBitmap[0];
        info.generateMipmap = GL_TRUE;
        info.minFilter = GL_LINEAR;
        info.magFilter = GL_LINEAR;
        info.wrapS = GL_CLAMP_TO_EDGE;
        info.wrapT = GL_CLAMP_TO_EDGE;
        mTexture = Graphics::CreateTexture(info);
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
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/sphere.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/sphere.frag")};

        mPipeline = Graphics::CreatePipeline(info);
        mPipeline->Bind();
        mMesh->Bind();
        mPipeline->SetAttribute(mMesh->mAttributes);
        mPipeline->Unbind();
    }
}

void Sphere::Render()
{
    auto viewport = Graphics::GetViewport();

    // Update the sphere.
    {
        // Update the ModelView matrix
        float time = (float) glfwGetTime();

        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, (float) (0.5*M_PI));

        math::vec4f dir_y = math::dot(m, math::vec4f{0.0, 1.0, 0.0, 1.0});
        m = math::rotate(m, math::vec3f{dir_y.x, dir_y.y, dir_y.z}, 0.2f*time);

        math::vec4f dir_z = math::dot(m, math::vec4f{0.0, 0.0, 1.0, 1.0});
        m = math::rotate(m, math::vec3f{dir_z.x, dir_z.y, dir_z.z}, 0.8f*time);

        float ratio = viewport.width / viewport.height;
        math::mat4f p = math::orthographic(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
        mModelView = math::dot(p, m);
    }

    // Render the sphere.
    {
        GLenum texunit = 0;
        mPipeline->Use();
        mPipeline->SetUniform("uWidth",  GL_FLOAT, &viewport.width);
        mPipeline->SetUniform("uHeight", GL_FLOAT, &viewport.height);
        mPipeline->SetUniformMatrix("uMvp", GL_FLOAT_MAT4, true,
            mModelView.data);
        mPipeline->SetUniform("uTexSampler", GL_SAMPLER_2D, &texunit);
        mTexture->Bind(texunit);
        mPipeline->Clear();
        mMesh->Draw();
    }
}

/// -----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
    Graphics::Settings settings = {};
    settings.WindowTitle = "08-sphere-image";
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
    gSphere.Initialize();
    while (!Graphics::ShouldClose()) {
        gSphere.Render();
        Graphics::Present();
    }
    Graphics::Terminate();

    return EXIT_SUCCESS;
}
