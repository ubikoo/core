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
static const std::string kImageFilename = "../assets/baboon_512.png";
static const size_t kMeshNodes = 1024;

struct Quad {
    math::mat4f ModelView;
    Graphics::Mesh Mesh;
    Graphics::Image Image;
    Graphics::Texture Texture;
    Graphics::Pipeline Pipeline;
};
Quad gQuad;

/// -----------------------------------------------------------------------------
void Graphics::OnKeyboard(int code, int scancode, int action, int mods)
{}
void Graphics::OnMouseMove(double xpos, double ypos)
{}
void Graphics::OnMouseButton(int button, int action, int mods)
{}

void Graphics::OnInitialize()
{
    // Initialize mvp matrix and create a mesh over a rectangle.
    {
        gQuad.ModelView = math::mat4f::eye;
        gQuad.Mesh = Graphics::CreatePlane(
            "quad",                     // vertex attributes prefix
            kMeshNodes,                 // n1 vertices
            kMeshNodes,                 // n2 vertices
            -1.0,                       // xlo
            1.0,                        // xhi
            -1.0,                       // ylo
            1.0);                       // yhi
    }

    // Load the 2d-image from the specified filename.
    {
        gQuad.Image = Graphics::LoadImage(kImageFilename, true, 4);

        Graphics::TextureCreateInfo info = {};
        info.target = GL_TEXTURE_2D;
        info.width = gQuad.Image->mWidth;
        info.height = gQuad.Image->mHeight;
        info.internalFormat = GL_RGBA8;
        info.pixelFormat = gQuad.Image->mFormat;
        info.pixelType = GL_UNSIGNED_BYTE;
        info.pixels = &gQuad.Image->mBitmap[0];
        info.generateMipmap = GL_TRUE;
        info.minFilter = GL_LINEAR;
        info.magFilter = GL_LINEAR;
        info.wrapS = GL_CLAMP_TO_EDGE;
        info.wrapT = GL_CLAMP_TO_EDGE;
        gQuad.Texture = Graphics::CreateTexture(info);
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

        gQuad.Pipeline = Graphics::CreatePipeline(info);
        gQuad.Pipeline->Bind();
        gQuad.Mesh->Bind();
        gQuad.Pipeline->SetAttribute(gQuad.Mesh->mAttributes);
        gQuad.Pipeline->Unbind();
    }
}

void Graphics::OnTerminate()
{}

void Graphics::OnMainLoop()
{
    auto viewport = Graphics::GetViewport();

    // Update the ModelView matrix
    {
        float time = (float) glfwGetTime();

        float ang_x = 0.8 * time;
        float ang_y = 0.4 * time;
        float ang_z = 0.2 * time;

        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, ang_z);
        m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, ang_y);
        m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);

        float ratio = viewport.width / viewport.height;
        math::mat4f p = math::orthographic(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
        gQuad.ModelView = math::dot(p, m);
    }

    // Render the quad.
    {
        GLenum texunit = 0;

        gQuad.Pipeline->Use();
        gQuad.Pipeline->SetUniform("u_width",  GL_FLOAT, &viewport.width);
        gQuad.Pipeline->SetUniform("u_height", GL_FLOAT, &viewport.height);
        gQuad.Pipeline->SetUniformMatrix(
            "u_mvp", GL_FLOAT_MAT4, true, gQuad.ModelView.data);
        gQuad.Pipeline->SetUniform("u_texsampler", GL_SAMPLER_2D, &texunit);
        gQuad.Texture->Bind(texunit);
        gQuad.Pipeline->Clear();
        gQuad.Mesh->Render();
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

    try {
        Graphics::MainLoop(settings);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
