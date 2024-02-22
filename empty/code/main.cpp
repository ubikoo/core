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
#include <array>

#include "core/math/math.h"
#include "core/compute/compute.h"
#include "core/graphics/graphics.h"

Graphics::Camera gCamera;

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
    gCamera.Keyboard(code, scancode, action, mods);
}

void Graphics::OnMouseMove(double xpos, double ypos)
{
    gCamera.MouseMove(xpos, ypos);
}

void Graphics::OnMouseButton(int button, int action, int mods)
{
    gCamera.MouseButton(button, action, mods);
}

void Graphics::OnInitialize()
{
    // Initialize camera.
    {
        std::array<GLint,4> viewport = {};
        glGetIntegerv(GL_VIEWPORT, &viewport[0]);
        float width = static_cast<float>(viewport[2]);
        float height = static_cast<float>(viewport[3]);

        gCamera = Graphics::CreateCamera(
            math::vec3f{0.0f, 0.0f,  0.0f},     // eye-position
            math::vec3f{0.0f, 0.0f, -1.0f},     // view-direction
            math::vec3f{0.0f, 1.0f,  0.0f},     // up-direction
            0.5f * M_PI,                        // y-fov
            width / height,                     // aspect
            0.01f,                              // z-near
            10.0f);                             // z-far
    }

    // Query information about all available platforms.
    {
        auto platform_ids = Compute::GetPlatformIDs();
        std::cout << "Number of Platforms " << platform_ids.size() << "\n\n";
        for (auto &platform : platform_ids) {
            auto info = Compute::GetPlatformInfo(platform);
            std::cout << Compute::ToString(info) << "\n";

            auto device_ids = Compute::GetDeviceIDs(platform);
            std::cout << "Number of Devices " << device_ids.size() << "\n\n";
            size_t count = 0;
            for (auto &device :device_ids) {
                auto info = Compute::GetDeviceInfo(device);
                std::cout << "Device # " << count++ << "\n"
                        << Compute::ToString(info) << "\n";
            }
        }
    }
}

void Graphics::OnTerminate()
{}

void Graphics::OnUpdate()
{
    static const uint32_t kMaxFrames = 360;
    static uint32_t FrameCount = 0;
    if (++FrameCount >= kMaxFrames) {
        Graphics::Close();
    }
}

void Graphics::OnRender()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

///
/// @brief Renderer callback functions.
///
int main(int argc, char const *argv[])
{
    Graphics::RenderDesc desc = {};
    desc.WindowTitle = "app";
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
