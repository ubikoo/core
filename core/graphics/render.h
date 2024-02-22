//
// render.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_RENDER_H_
#define GRAPHICS_RENDER_H_

#include <string>
#include "core/graphics/common.h"

namespace Graphics {

///
/// @brief Graphics main render loop providing a basic interface to GLFW library.
///
struct RenderDesc {
    std::string WindowTitle;
    int WindowWidth;
    int WindowHeight;
    int GLVersionMajor;
    int GLVersionMinor;
    double PollTimeout;
};

void RenderLoop(const RenderDesc &desc);
void Initialize();
void Terminate();
void Close();
bool ShouldClose();
void Present();

void OnResize(int width, int height);
void OnKeyboard(int code, int scancode, int action, int mods);
void OnMouseMove(double xpos, double ypos);
void OnMouseButton(int button, int action, int mods);
void OnInitialize();
void OnTerminate();
void OnUpdate();
void OnRender();

} // Graphics

#endif // GRAPHICS_RENDER_H_
