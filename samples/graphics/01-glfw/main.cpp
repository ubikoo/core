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

void OnKeyboard(int code, int scancode, int action, int mods)
{
    std::cout << __func__ << " "
        << "code " << code << " "
        << "scancode " << scancode << " "
        << "action " << action << " "
        << "mods " << mods << "\n";
}

void OnMouseMove(double xpos, double ypos)
{
    std::cout << __func__ << " "
        << "xpos " << xpos << " "
        << "ypos " << ypos << "\n";
}

void OnMouseButton(int button, int action, int mods)
{
    std::cout << __func__ << " "
        << "button " << button << " "
        << "action " << action << " "
        << "mods " << mods << "\n";
}

int main(int argc, char const *argv[])
{
    Graphics::Settings settings = {};
    settings.WindowTitle = "01-glfw";
    settings.WindowWidth = 800;
    settings.WindowHeight = 800;
    settings.GLVersionMajor = 3;
    settings.GLVersionMinor = 3;
    settings.PollTimeout = 0.01;
    settings.MaxFrames = 600;
    settings.OnKeyboard = OnKeyboard;
    settings.OnMouseMove = OnMouseMove;
    settings.OnMouseButton = OnMouseButton;
    Graphics::Initialize(settings);

    while (!Graphics::ShouldClose()) {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Graphics::Present();
    }

    Graphics::Terminate();

    return EXIT_SUCCESS;
}
