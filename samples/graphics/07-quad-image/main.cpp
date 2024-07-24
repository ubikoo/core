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
#include "minicore/graphics/graphics.h"

/// -----------------------------------------------------------------------------
static const std::string kImageFilename = "../assets/baboon_512.png";
static const size_t kMeshNodes = 1024;

struct Quad {
    Math::Mat4f mModelView;
    Graphics::Mesh mMesh;
    Graphics::Image mImage;
    Graphics::Texture mTexture;
    Graphics::Pipeline mPipeline;

    void Initialize();
    void Render();
};
Quad gQuad;

void Quad::Initialize()
{
    // Initialize mvp matrix and create a mesh over a rectangle.
    {
        mModelView = Math::Mat4f::Eye;
        mMesh = Graphics::CreatePlane(
            "Quad",                     // vertex attributes prefix
            kMeshNodes,                 // n1 vertices
            kMeshNodes,                 // n2 vertices
            -1.0,                       // xlo
            1.0,                        // xhi
            -1.0,                       // ylo
            1.0);                       // yhi
    }

    // Load the 2d-image from the specified filename.
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
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/quad.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/quad.frag")};

        mPipeline = Graphics::CreatePipeline(info);
        mPipeline->Bind();
        mMesh->Bind();
        mPipeline->SetAttribute(mMesh->mAttributes);
        mPipeline->Unbind();
    }
}

void Quad::Render()
{
    auto viewport = Graphics::GetViewport();

    // Update the ModelView matrix
    {
        float time = (float) glfwGetTime();

        float ang_x = 0.8 * time;
        float ang_y = 0.4 * time;
        float ang_z = 0.2 * time;

        Math::Mat4f m = Math::Mat4f::Eye;
        m = Math::Rotate(m, Math::Vec3f{0.0f, 0.0f, 1.0f}, ang_z);
        m = Math::Rotate(m, Math::Vec3f{0.0f, 1.0f, 0.0f}, ang_y);
        m = Math::Rotate(m, Math::Vec3f{1.0f, 0.0f, 0.0f}, ang_x);

        float ratio = viewport.width / viewport.height;
        Math::Mat4f p = Math::Orthographic(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
        mModelView = Math::Dot(p, m);
    }

    // Render the quad.
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
    settings.WindowTitle = "07-quad-image";
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
    gQuad.Initialize();
    while (!Graphics::ShouldClose()) {
        gQuad.Render();
        Graphics::Present();
    }
    Graphics::Terminate();

    return EXIT_SUCCESS;
}
