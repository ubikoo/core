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

struct Drawable {
    enum RotateMode : uint8_t { X = 0, Y, Z };
    uint8_t Rotate{RotateMode::X};

    struct {
        math::mat4f mvp;                // sphere mvp matrix
        Graphics::Mesh mesh;            // image and texture
        Graphics::Texture texture;      // sphere texture
        Graphics::Pipeline pipeline;    // rendering pipeline
    } mSphere;                          // sphere draw pass.


    struct {
        math::mat4f mvp;                // quad mvp matrix
        Graphics::Mesh mesh;            // image and texture
        Graphics::Pipeline pipeline;    // rendering pipeline
    } mQuad;                            // quad draw pass.

    GLsizei mWidth;                     // drawable width
    GLsizei mHeight;                    // drawable height
    Graphics::Framebuffer mFbo;         // drawable framebuffer

    static void OnKeyboard(int code, int scancode, int action, int mods);

    void Initialize();
    void InitializeSphere();
    void InitializeQuad();
    void InitalizeFramebuffer();
    void Update();
    void Render();
};
Drawable gDrawable;

void Drawable::OnKeyboard(int code, int scancode, int action, int mods)
{
    if (code == GLFW_KEY_X && action == GLFW_RELEASE) {
        gDrawable.Rotate = Drawable::RotateMode::X;
    }

    if (code == GLFW_KEY_Y && action == GLFW_RELEASE) {
        gDrawable.Rotate = Drawable::RotateMode::Y;
    }

    if (code == GLFW_KEY_Z && action == GLFW_RELEASE) {
        gDrawable.Rotate = Drawable::RotateMode::Z;
    }
}

void Drawable::Initialize()
{
    InitializeSphere();
    InitializeQuad();
    InitalizeFramebuffer();
}

void Drawable::InitializeSphere()
{
    mSphere.mvp = math::mat4f::eye;
    mSphere.mesh = Graphics::CreateSphere(
        "Sphere",                   // vertex attributes prefix
        kMeshNodes,                 // n1 vertices
        kMeshNodes,                 // n2 vertices
        1.0,                        // radius
        0.0,                        // theta_lo
        M_PI,                       // theta_hi
        -M_PI,                      // phi_lo
        M_PI);                      // phi_hi

    Graphics::Image image = Graphics::LoadImage(kImageFilename);
    mWidth = image->mWidth;
    mHeight = image->mHeight;

    Graphics::TextureCreateInfo textureInfo = {};
    textureInfo.target = GL_TEXTURE_2D;
    textureInfo.width = mWidth;
    textureInfo.height = mHeight;
    textureInfo.internalFormat = GL_RGBA8;
    textureInfo.pixelFormat = image->mFormat;
    textureInfo.pixelType = GL_UNSIGNED_BYTE;
    textureInfo.pixels = &image->mBitmap[0];
    textureInfo.generateMipmap = GL_TRUE;
    textureInfo.minFilter = GL_LINEAR;
    textureInfo.magFilter = GL_LINEAR;
    textureInfo.wrapS = GL_CLAMP_TO_EDGE;
    textureInfo.wrapT = GL_CLAMP_TO_EDGE;
    mSphere.texture = Graphics::CreateTexture(textureInfo);

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
        Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/sphere.vert"),
        Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/sphere.frag")};
    mSphere.pipeline = Graphics::CreatePipeline(pipelineInfo);
    mSphere.pipeline->Bind();
    mSphere.mesh->Bind();
    mSphere.pipeline->SetAttribute(mSphere.mesh->mAttributes);
    mSphere.pipeline->Unbind();
}

void Drawable::InitializeQuad()
{
    mQuad.mvp = math::mat4f::eye;
    mQuad.mesh = Graphics::CreatePlane(
        "Quad",                     // vertex attributes prefix
        kMeshNodes,                 // n1 vertices
        kMeshNodes,                 // n2 vertices
        -1.0,                       // xlo
        1.0,                        // xhi
        -1.0,                       // ylo
        1.0);                       // yhi

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
        Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/quad.vert"),
        Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/quad.frag")};
    mQuad.pipeline = Graphics::CreatePipeline(pipelineInfo);
    mQuad.pipeline->Bind();
    mQuad.mesh->Bind();
    mQuad.pipeline->SetAttribute(mQuad.mesh->mAttributes);
    mQuad.pipeline->Unbind();
}

void Drawable::InitalizeFramebuffer()
{
    Graphics::FramebufferCreateInfo fboInfo = {};
    fboInfo.width = mWidth;
    fboInfo.height = mHeight;
    fboInfo.minFilter = GL_LINEAR;
    fboInfo.magFilter = GL_LINEAR;
    fboInfo.numColorAttachments = 1;
    fboInfo.colorInternalformat = GL_RGBA;
    fboInfo.colorPixelformat = GL_RGBA;
    fboInfo.colorPixeltype = GL_UNSIGNED_BYTE;
    fboInfo.useDepthAttachment = GL_TRUE;
    fboInfo.depthInternalformat = GL_DEPTH_COMPONENT24;
    fboInfo.depthPixelformat = GL_DEPTH_COMPONENT;
    fboInfo.depthPixeltype = GL_UNSIGNED_BYTE;
    mFbo = Graphics::CreateFramebuffer(fboInfo);
}

void Drawable::Update()
{
    // Compute the orthographic projection matrix
    float time = (float) glfwGetTime();

    // Update the sphere ModelView matrix
    {
        float angX = 0.0f * time;
        float angY = 2.0f * time;
        float angZ = 0.0f * time;

        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, angX);

        math::vec4f dirY = math::dot(m, math::vec4f{0.0f,1.0f,0.0f,1.0f});
        m = math::rotate(m, math::vec3f{dirY.x, dirY.y, dirY.z}, angY);

        math::vec4f dirZ = math::dot(m, math::vec4f{0.0f,0.0f,1.0f,1.0f});
        m = math::rotate(m, math::vec3f{dirZ.x, dirZ.y, dirZ.z}, angZ);

        auto viewport = Graphics::GetViewport();
        float ratio = viewport.width / viewport.height;
        math::mat4f proj = math::orthographic(
            -ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
        mSphere.mvp = math::dot(proj, m);
    }

    // Update the quad ModelView matrix
    {
        float rate = 0.5f * time;
        float angX = gDrawable.Rotate == Drawable::RotateMode::X ? rate : 0.0f;
        float angY = gDrawable.Rotate == Drawable::RotateMode::Y ? rate : 0.0f;
        float angZ = gDrawable.Rotate == Drawable::RotateMode::Z ? rate : 0.0f;

        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, angZ);
        m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, angY);
        m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, angX);

        float ratio = (float) mWidth / mHeight;
        math::mat4f proj = math::orthographic(
            -ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
        mQuad.mvp = math::dot(proj, m);
    }
}

void Drawable::Render()
{
    // Render into the framebuffer rendertexture
    {
        mFbo->Bind();
        auto viewport = Graphics::GetViewport();
        Graphics::SetViewport({0, 0, (GLfloat)mWidth, (GLfloat)mHeight});

        GLenum texunit = 0;
        mSphere.pipeline->Use();
        mSphere.pipeline->SetUniformMatrix("uMvp", GL_FLOAT_MAT4, true,
            mSphere.mvp.data);
        mSphere.pipeline->SetUniform("uTexSampler", GL_SAMPLER_2D, &texunit);
        mSphere.texture->Bind(texunit);
        mSphere.pipeline->Clear();
        mSphere.mesh->Draw();

        Graphics::SetViewport(viewport);
        mFbo->Unbind();
    }

    // Render into the window framebuffer
    {
        auto viewport = Graphics::GetViewport();
        {
            GLfloat aspect = (GLfloat) mWidth / mHeight;
            GLfloat w = viewport.width;
            GLfloat h = viewport.height / aspect;
            GLfloat y = 0.5f * h;
            GLfloat x = 0.0f;
            Graphics::SetViewport({x, y, w, h});
        }

        GLenum texunit = 0;
        mQuad.pipeline->Use();
        mQuad.pipeline->SetUniformMatrix("uMvp", GL_FLOAT_MAT4, true,
            mQuad.mvp.data);
        mQuad.pipeline->SetUniform("uTexSampler", GL_SAMPLER_2D, &texunit);
        mFbo->mColorAttachments[0]->Bind(texunit);
        mQuad.pipeline->Clear();
        mQuad.mesh->Draw();
        Graphics::SetViewport(viewport);
    }
}

/// -----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
    Graphics::Settings settings = {};
    settings.WindowTitle = "11-framebuffer";
    settings.WindowWidth = 800;
    settings.WindowHeight = 800;
    settings.GLVersionMajor = 3;
    settings.GLVersionMinor = 3;
    settings.PollTimeout = 0.01;
    settings.MaxFrames = 600;
    settings.OnKeyboard = Drawable::OnKeyboard;
    settings.OnMouseMove = nullptr;
    settings.OnMouseButton = nullptr;

    Graphics::Initialize(settings);
    gDrawable.Initialize();
    while (!Graphics::ShouldClose()) {
        gDrawable.Update();
        gDrawable.Render();
        Graphics::Present();
    }
    Graphics::Terminate();

    return EXIT_SUCCESS;
}
