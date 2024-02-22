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

#include "core/graphics/graphics.h"
#include "model.h"

Model gModel;

///
/// @brief Renderer callback functions.
///
void Graphics::OnResize(int width, int height)
{
    glViewport(0, 0, width, height);
}

void Graphics::OnKeyboard(int code, int scancode, int action, int mods)
{
    if (code == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        Graphics::Close();
    }

    if (code == GLFW_KEY_1 && action == GLFW_RELEASE) {
        gModel.current_mesh(0);
    }

    if (code == GLFW_KEY_2 && action == GLFW_RELEASE) {
        gModel.current_mesh(1);
    }

    if (code == GLFW_KEY_X && action == GLFW_RELEASE) {
        gModel.rotate(Model::Rotate::X);
    }

    if (code == GLFW_KEY_Y && action == GLFW_RELEASE) {
        gModel.rotate(Model::Rotate::Y);
    }

    if (code == GLFW_KEY_Z && action == GLFW_RELEASE) {
        gModel.rotate(Model::Rotate::Z);
    }

    if (code == GLFW_KEY_UP && action == GLFW_REPEAT) {
        gModel.scale(1.05f);
    }

    if (code == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
        gModel.scale(1.0 / 1.05f);
    }
}

void Graphics::OnMouseMove(double xpos, double ypos)
{}

void Graphics::OnMouseButton(int button, int action, int mods)
{}

void Graphics::OnInitialize()
{
    gModel.Initialize();
}

void Graphics::OnTerminate()
{
    gModel.Cleanup();
}

void Graphics::OnUpdate()
{
    static const uint32_t kMaxFrames = 360;
    static uint32_t FrameCount = 0;
    if (++FrameCount >= kMaxFrames) {
        Graphics::Close();
    }
    gModel.Update();
}

void Graphics::OnRender()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gModel.Render();
}

///
/// @brief main application client.
///
int main(int argc, char const *argv[])
{
    Graphics::RenderDesc desc =  {};
    desc.WindowTitle = "09-mesh-model";
    desc.WindowWidth = 800;
    desc.WindowHeight = 800;
    desc.GLVersionMajor = 3;
    desc.GLVersionMinor = 3;
    desc.PollTimeout = 0.01;

    try {
        Graphics::RenderLoop(desc);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
