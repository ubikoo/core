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
    math::mat4f ModelView;
    Graphics::Camera Camera;
    Graphics::Image Image;
    Graphics::Mesh Mesh;
    Graphics::Texture Texture;
    Graphics::Pipeline Pipeline;
};
Panorama gPanorama;

/// -----------------------------------------------------------------------------
void Graphics::OnKeyboard(int code, int scancode, int action, int mods)
{
    gPanorama.Camera.Keyboard(code, scancode, action, mods);
}

void Graphics::OnMouseMove(double xpos, double ypos)
{
    gPanorama.Camera.MouseMove(xpos, ypos);
}

void Graphics::OnMouseButton(int button, int action, int mods)
{
    gPanorama.Camera.MouseButton(button, action, mods);
}

void Graphics::OnInitialize()
{
    // Create panorama camera and initialize projection matrix.
    {
        gPanorama.ModelView = math::mat4f::eye;

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
        gPanorama.Camera = Graphics::CreateCamera(info);
    }

    // Load the image from the file, store it on a texture and create mesh.
    {
        gPanorama.Image = Graphics::LoadImage(kImageFilename, true, 4);

        Graphics::TextureCreateInfo info = {};
        info.target = GL_TEXTURE_2D;
        info.width = gPanorama.Image->mWidth;
        info.height = gPanorama.Image->mHeight;
        info.internalformat = GL_RGBA8;
        info.pixelformat = gPanorama.Image->mFormat;
        info.pixeltype = GL_UNSIGNED_BYTE;
        info.pixels = &gPanorama.Image->mBitmap[0];
        info.generateMipmap = GL_TRUE;
        info.minFilter = GL_LINEAR;
        info.magFilter = GL_LINEAR;
        info.wrapS = GL_CLAMP_TO_EDGE;
        info.wrapT = GL_CLAMP_TO_EDGE;
        gPanorama.Texture = Graphics::CreateTexture(info);

        gPanorama.Mesh = Graphics::CreateSphere(
            "sphere",                   // vertex attributes prefix
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

        gPanorama.Pipeline = Graphics::CreatePipeline(info);
        gPanorama.Pipeline->Bind();
        gPanorama.Mesh->Bind();
        gPanorama.Pipeline->SetAttribute(gPanorama.Mesh->mAttributes);
        gPanorama.Pipeline->Unbind();
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

        gPanorama.Pipeline = Graphics::CreatePipeline(info);
        gPanorama.Pipeline->Bind();
        gPanorama.Mesh->Bind();
        gPanorama.Pipeline->SetAttribute(gPanorama.Mesh->mAttributes);
        gPanorama.Pipeline->Unbind();
    }
}

void Graphics::OnTerminate()
{}

void Graphics::OnMainLoop()
{
    // Update the panorama camera view.
    {
        math::mat4f proj = gPanorama.Camera.Proj();
        math::mat4f view = gPanorama.Camera.View();
        gPanorama.ModelView = math::dot(proj, view);
    }

    // Render the panorama camera view..
    {
        auto viewport = Graphics::GetViewport();
        GLenum texunit = 0;

        gPanorama.Pipeline->Use();
        gPanorama.Pipeline->SetUniform("u_width",  GL_FLOAT, &viewport.width);
        gPanorama.Pipeline->SetUniform("u_height", GL_FLOAT, &viewport.height);
        gPanorama.Pipeline->SetUniformMatrix(
            "u_mvp", GL_FLOAT_MAT4, true, gPanorama.ModelView.data);
        gPanorama.Pipeline->SetUniform("u_texsampler", GL_SAMPLER_2D, &texunit);
        gPanorama.Texture->Bind(texunit);
        gPanorama.Pipeline->Clear();
        gPanorama.Mesh->Render();
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

    try {
        Graphics::MainLoop(settings);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
