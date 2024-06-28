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

struct Panorama {
    math::mat4f mModelView;
    Graphics::Camera mCamera;
    Graphics::Image mImage;
    Graphics::Mesh mMesh;
    Graphics::Texture mTexture;
    Graphics::Pipeline mPipeline;

    static void OnKeyboard(int code, int scancode, int action, int mods);
    static void OnMouseMove(double xpos, double ypos);
    static void OnMouseButton(int button, int action, int mods);

    void Initialize();
    void Render();
};
Panorama gPanorama;

void Panorama::OnKeyboard(int code, int scancode, int action, int mods)
{
    gPanorama.mCamera.Keyboard(code, scancode, action, mods);
}

void Panorama::OnMouseMove(double xpos, double ypos)
{
    gPanorama.mCamera.MouseMove(xpos, ypos);
}

void Panorama::OnMouseButton(int button, int action, int mods)
{
    gPanorama.mCamera.MouseButton(button, action, mods);
}

void Panorama::Initialize()
{
    // Create panorama camera and initialize projection matrix.
    {
        mModelView = math::mat4f::eye;

        auto viewport = Graphics::GetViewport();
        Graphics::CameraCreateInfo info = {};
        info.position = {0.0f, 0.0f,  0.0f};
        info.ctr = {1.0f, 0.0f, 0.0f};
        info.up = {0.0f, 0.0f,  1.0f};
        info.fovy = 0.5f * M_PI;
        info.aspect = viewport.width / viewport.height;
        info.znear = 0.1f;
        info.zfar = 10.0f;
        info.moveSpeed = 0.1f;
        info.rotateSpeed = 0.001f * M_PI;
        mCamera = Graphics::CreateCamera(info);
    }

    // Load the image from the file, store it on a texture and create mesh.
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
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/panorama.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/panorama.frag")};

        mPipeline = Graphics::CreatePipeline(info);
        mPipeline->Bind();
        mMesh->Bind();
        mPipeline->SetAttribute(mMesh->mAttributes);
        mPipeline->Unbind();
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
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/panorama.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/panorama.frag")};

        mPipeline = Graphics::CreatePipeline(info);
        mPipeline->Bind();
        mMesh->Bind();
        mPipeline->SetAttribute(mMesh->mAttributes);
        mPipeline->Unbind();
    }
}

void Panorama::Render()
{
    // Update the panorama camera view.
    {
        math::mat4f proj = mCamera.Proj();
        math::mat4f view = mCamera.View();
        mModelView = math::dot(proj, view);
    }

    // Render the panorama camera view..
    {
        auto viewport = Graphics::GetViewport();
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
    settings.WindowTitle = "10-panorama";
    settings.WindowWidth = 800;
    settings.WindowHeight = 800;
    settings.GLVersionMajor = 3;
    settings.GLVersionMinor = 3;
    settings.PollTimeout = 0.1;
    settings.MaxFrames = 600;
    settings.OnKeyboard = Panorama::OnKeyboard;
    settings.OnMouseMove = Panorama::OnMouseMove;
    settings.OnMouseButton = Panorama::OnMouseButton;

    Graphics::Initialize(settings);
    gPanorama.Initialize();
    while (!Graphics::ShouldClose()) {
        gPanorama.Render();
        Graphics::Present();
    }
    Graphics::Terminate();

    return EXIT_SUCCESS;
}
