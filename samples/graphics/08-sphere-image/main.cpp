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
    math::mat4f ModelView;
    Graphics::Mesh Mesh;
    Graphics::Image Image;
    Graphics::Texture Texture;
    Graphics::Pipeline Pipeline;
};
Sphere gSphere;

/// -----------------------------------------------------------------------------
void OnInitialize()
{
    // Initialize mvp matrix and create a mesh over a rectangle.
    {
        gSphere.ModelView = math::mat4f::eye;
        gSphere.Mesh = Graphics::CreateSphere(
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
        gSphere.Image = Graphics::LoadImage(kImageFilename, true, 4);

        Graphics::TextureCreateInfo info = {};
        info.target = GL_TEXTURE_2D;
        info.width = gSphere.Image->mWidth;
        info.height = gSphere.Image->mHeight;
        info.internalFormat = GL_RGBA8;
        info.pixelFormat = gSphere.Image->mFormat;
        info.pixelType = GL_UNSIGNED_BYTE;
        info.pixels = &gSphere.Image->mBitmap[0];
        info.generateMipmap = GL_TRUE;
        info.minFilter = GL_LINEAR;
        info.magFilter = GL_LINEAR;
        info.wrapS = GL_CLAMP_TO_EDGE;
        info.wrapT = GL_CLAMP_TO_EDGE;
        gSphere.Texture = Graphics::CreateTexture(info);
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

        gSphere.Pipeline = Graphics::CreatePipeline(info);
        gSphere.Pipeline->Bind();
        gSphere.Mesh->Bind();
        gSphere.Pipeline->SetAttribute(gSphere.Mesh->mAttributes);
        gSphere.Pipeline->Unbind();
    }
}

void OnMainLoop()
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
        gSphere.ModelView = math::dot(p, m);
    }

    // Render the sphere.
    {
        GLenum texunit = 0;
        gSphere.Pipeline->Use();
        gSphere.Pipeline->SetUniform("u_width",  GL_FLOAT, &viewport.width);
        gSphere.Pipeline->SetUniform("u_height", GL_FLOAT, &viewport.height);
        gSphere.Pipeline->SetUniformMatrix(
            "u_mvp", GL_FLOAT_MAT4, true, gSphere.ModelView.data);
        gSphere.Pipeline->SetUniform("u_texsampler", GL_SAMPLER_2D, &texunit);
        gSphere.Texture->Bind(texunit);
        gSphere.Pipeline->Clear();
        gSphere.Mesh->Render();
    }
}

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
    settings.OnInitialize = OnInitialize;
    settings.OnMainLoop = OnMainLoop;

    try {
        Graphics::MainLoop(settings);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
