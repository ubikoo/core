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
static const std::string kImageFilename = "../assets/monarch_512.png";
static const int kWidth = 1024;
static const int kHeight = 1024;

struct Map {
    size_t mReadIx;
    size_t mWriteIx;
    size_t mNumIterations;
    std::array<Graphics::Framebuffer, 2> mIOBuffer;

    struct {
        Graphics::Mesh quad;
        Graphics::Texture texture;
        Graphics::Pipeline pipeline;
    } mBeginPass;

    struct {
        Graphics::Mesh quad;
        Graphics::Pipeline pipeline;
    } mRunPass;

    struct {
        Graphics::Mesh quad;
        Graphics::Pipeline pipeline;
    } mEndPass;

    static void OnKeyboard(int code, int scancode, int action, int mods);

    void Initialize();
    void Render();
};
Map gMap;

void Map::OnKeyboard(int code, int scancode, int action, int mods)
{
    if (code == GLFW_KEY_UP) {
        gMap.mNumIterations++;
        std::cout << "mNumIterations " << gMap.mNumIterations << "\n";
    }

    if (code == GLFW_KEY_DOWN) {
        if (gMap.mNumIterations > 0) {
            gMap.mNumIterations--;
            std::cout << "mNumIterations " << gMap.mNumIterations << "\n";
        }
    }
}

void Map::Initialize()
{
    // Initialize the map input/output framebuffers.
    {
        mReadIx = 0;
        mWriteIx = 1;
        mNumIterations = 0;

        Graphics::FramebufferCreateInfo fboInfo = {};
        fboInfo.width = kWidth;
        fboInfo.height = kHeight;
        fboInfo.minFilter = GL_NEAREST;
        fboInfo.magFilter = GL_NEAREST;
        fboInfo.numColorAttachments = 1;
        fboInfo.colorInternalformat = GL_RGB32F;
        fboInfo.colorPixelformat = GL_RGB;
        fboInfo.colorPixeltype = GL_FLOAT;
        fboInfo.useDepthAttachment = GL_FALSE;
        fboInfo.depthInternalformat = GL_DEPTH_COMPONENT32F;
        fboInfo.depthPixelformat = GL_DEPTH_COMPONENT;
        fboInfo.depthPixeltype = GL_FLOAT;
        mIOBuffer[0] = Graphics::CreateFramebuffer(fboInfo);
        mIOBuffer[1] = Graphics::CreateFramebuffer(fboInfo);
    }

    // Initialize the map begin pass.
    {
        mBeginPass.quad = Graphics::CreatePlane(
            "Quad",     // attributes prefix
            2,          // n1 vertices
            2,          // n2 vertices
            -1.0,       // xlo
             1.0,       // xhi
            -1.0,       // ylo
             1.0);      // yhi

        Graphics::Image image = Graphics::LoadImage(kImageFilename, true);
        Graphics::TextureCreateInfo textureInfo = {};
        textureInfo.target = GL_TEXTURE_2D;
        textureInfo.width = image->mWidth;
        textureInfo.height = image->mHeight;
        textureInfo.internalFormat = GL_RGBA8;
        textureInfo.pixelFormat = image->mFormat;
        textureInfo.pixelType = GL_UNSIGNED_BYTE;
        textureInfo.pixels = &image->mBitmap[0];
        textureInfo.generateMipmap = GL_TRUE;
        textureInfo.minFilter = GL_LINEAR;
        textureInfo.magFilter = GL_LINEAR;
        textureInfo.wrapS = GL_CLAMP_TO_EDGE;
        textureInfo.wrapT = GL_CLAMP_TO_EDGE;
        mBeginPass.texture = Graphics::CreateTexture(textureInfo);

        Graphics::PipelineCreateInfo pipelineInfo = {};
        pipelineInfo.polygonMode = GL_FILL;
        pipelineInfo.enableCullFace = GL_FALSE;
        pipelineInfo.cullFaceMode = GL_BACK;
        pipelineInfo.frontFaceMode = GL_CCW;
        pipelineInfo.enableDepthTest = GL_TRUE;
        pipelineInfo.depthFunc = GL_LESS;
        pipelineInfo.clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
        pipelineInfo.clearColor = {0.5f, 0.5f, 0.5f, 1.0f};
        pipelineInfo.clearDepth = 1.0f;
        pipelineInfo.lineWidth = 1.0f;
        pipelineInfo.pointSize = 1.0f;
        pipelineInfo.shaders = {
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/map-begin.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/map-begin.frag")};
        mBeginPass.pipeline = Graphics::CreatePipeline(pipelineInfo);
        mBeginPass.pipeline->Bind();
        mBeginPass.quad->Bind();
        mBeginPass.pipeline->SetAttribute(mBeginPass.quad->mAttributes);
        mBeginPass.pipeline->Unbind();
    }

    // Initialize the map run pass.
    {
        mRunPass.quad = Graphics::CreatePlane(
            "Quad",             // vertex attributes prefix
            2,                  // n1 vertices
            2,                  // n2 vertices
            -1.0,               // xlo
             1.0,               // xhi
            -1.0,               // ylo
             1.0);              // yhi

        Graphics::PipelineCreateInfo pipelineInfo = {};
        pipelineInfo.polygonMode = GL_FILL;
        pipelineInfo.enableCullFace = GL_FALSE;
        pipelineInfo.cullFaceMode = GL_BACK;
        pipelineInfo.frontFaceMode = GL_CCW;
        pipelineInfo.enableDepthTest = GL_TRUE;
        pipelineInfo.depthFunc = GL_LESS;
        pipelineInfo.clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
        pipelineInfo.clearColor = {0.5f, 0.5f, 0.5f, 1.0f};
        pipelineInfo.clearDepth = 1.0f;
        pipelineInfo.lineWidth = 1.0f;
        pipelineInfo.pointSize = 1.0f;
        pipelineInfo.shaders = {
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/map-run.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/map-run.frag")};
        mRunPass.pipeline = Graphics::CreatePipeline(pipelineInfo);
        mRunPass.pipeline->Bind();
        mRunPass.quad->Bind();
        mRunPass.pipeline->SetAttribute(mRunPass.quad->mAttributes);
        mRunPass.pipeline->Unbind();
    }

    // Initialize end render pass.
    {
        mEndPass.quad = Graphics::CreatePlane(
            "Quad",             // vertex attributes prefix
            2,                  // n1 vertices
            2,                  // n2 vertices
            -1.0,               // xlo
             1.0,               // xhi
            -1.0,               // ylo
             1.0);              // yhi

        Graphics::PipelineCreateInfo pipelineInfo = {};
        pipelineInfo.polygonMode = GL_FILL;
        pipelineInfo.enableCullFace = GL_FALSE;
        pipelineInfo.cullFaceMode = GL_BACK;
        pipelineInfo.frontFaceMode = GL_CCW;
        pipelineInfo.enableDepthTest = GL_TRUE;
        pipelineInfo.depthFunc = GL_LESS;
        pipelineInfo.clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
        pipelineInfo.clearColor = {0.5f, 0.5f, 0.5f, 1.0f};
        pipelineInfo.clearDepth = 1.0f;
        pipelineInfo.lineWidth = 1.0f;
        pipelineInfo.pointSize = 1.0f;
        pipelineInfo.shaders = {
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/map-end.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/map-end.frag")};
        mEndPass.pipeline = Graphics::CreatePipeline(pipelineInfo);
        mEndPass.pipeline->Bind();
        mEndPass.quad->Bind();
        mEndPass.pipeline->SetAttribute(mEndPass.quad->mAttributes);
        mEndPass.pipeline->Unbind();
    }
}

void Map::Render()
{
    // Begin by rendering the image to the map framebuffer.
    {
        std::swap(mReadIx, mWriteIx);
        mIOBuffer[mWriteIx]->Bind();

        auto viewport = Graphics::GetViewport();
        Graphics::SetViewport({0, 0, kWidth, kHeight});

        GLenum texunit = 0;
        mBeginPass.pipeline->Use();
        mBeginPass.pipeline->SetUniform("uTexSampler", GL_SAMPLER_2D, &texunit);
        mBeginPass.texture->Bind(texunit);
        mBeginPass.pipeline->Clear();
        mBeginPass.quad->Draw();

        mIOBuffer[mWriteIx]->Unbind();
        Graphics::SetViewport(viewport);
    }

    // Iterate the map shader program over a double framebuffer.
    for (size_t iter = 0; iter < mNumIterations; ++iter) {
        std::swap(mReadIx, mWriteIx);
        mIOBuffer[mWriteIx]->Bind();

        auto viewport = Graphics::GetViewport();
        Graphics::SetViewport({0, 0, kWidth, kHeight});

        GLenum texunit = 0;
        mRunPass.pipeline->Use();
        mRunPass.pipeline->SetUniform("uTexSampler", GL_SAMPLER_2D, &texunit);
        mIOBuffer[mReadIx]->mColorAttachments[0]->Bind(texunit);
        mRunPass.pipeline->Clear();
        mRunPass.quad->Draw();

        mIOBuffer[mWriteIx]->Unbind();
        Graphics::SetViewport(viewport);
    }

    // End by rendering the map framebuffer to the screen.
    {
        std::swap(mReadIx, mWriteIx);

        GLenum texunit = 0;
        mEndPass.pipeline->Use();
        mEndPass.pipeline->SetUniform("uTexSampler", GL_SAMPLER_2D, &texunit);
        mIOBuffer[mReadIx]->mColorAttachments[0]->Bind(texunit);
        mEndPass.pipeline->Clear();
        mEndPass.quad->Draw();
    }
}

/// -----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
    Graphics::Settings settings = {};
    settings.WindowTitle = "12-iobuffer";
    settings.WindowWidth = 800;
    settings.WindowHeight = 800;
    settings.GLVersionMajor = 3;
    settings.GLVersionMinor = 3;
    settings.PollTimeout = 0.01;
    settings.MaxFrames = 600;
    settings.OnKeyboard = Map::OnKeyboard;
    settings.OnMouseMove = nullptr;
    settings.OnMouseButton = nullptr;

    Graphics::Initialize(settings);
    gMap.Initialize();
    while (!Graphics::ShouldClose()) {
        gMap.Render();
        Graphics::Present();
    }
    Graphics::Terminate();

    return EXIT_SUCCESS;
}
