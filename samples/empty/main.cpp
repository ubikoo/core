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

/// -----------------------------------------------------------------------------
void Graphics::OnKeyboard(int code, int scancode, int action, int mods)
{
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
        Viewport viewport = Graphics::GetViewport();
        CameraCreateInfo info = {};
        info.position = {0.0f, 0.0f,  0.0f};
        info.ctr = {0.0f, 0.0f, -1.0f};
        info.up = {0.0f, 1.0f,  0.0f};
        info.fovy = 0.5f * M_PI;
        info.aspect = viewport.width / viewport.height;
        info.znear = 0.1f;
        info.zfar = 10.0f;
        info.moveSpeed = 1.0f;
        info.rotateSpeed = 0.01f * M_PI;
        gCamera = Graphics::CreateCamera(info);
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

void Graphics::OnMainLoop()
{
    // Clear the framebuffer.
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/// -----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
    Graphics::Settings settings = {};
    settings.WindowTitle = "empty";
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
