//
// graphics.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

///
/// @brief Graphics provides an interface to OpenGL. It contains helper functions
/// to initialize the GLFW library and to run the the main rendering loop.
/// The interface is implemented in a data-oriented style for ease of use and to
/// avoid issues associated with usage of an object-oriented style to maintain GL
/// objects (RAII & move semantics). GL objects are one of the following types:
///  - Regular objects contain data and are to transmit data to the GPU:
///      Buffer, Renderbuffer, Texture, Query, Sampler
///  - Container objects contain no data, only references to other objects:
///      Framebuffer, Vertex Array, Transform Feedback, Program Pipeline
///  - Non-standard objects do not use conventions for other GL objects:
///      Shader and Program
/// @see https://www.khronos.org/wiki/OpenGL_Object
///      https://www.khronos.org/wiki/GLSL_Object
///      https://www.khronos.org/wiki/Common_Mistakes
///
#include <string>
#include "attribute.h"
#include "buffer.h"
#include "camera.h"
#include "common.h"
#include "framebuffer.h"
#include "helpers.h"
#include "image.h"
#include "mesh.h"
#include "pipeline.h"
#include "program.h"
#include "renderbuffer.h"
#include "texture.h"
#include "uniform.h"
#include "vertexarray.h"

namespace Graphics {

struct Settings {
    std::string WindowTitle{"Untitled"};
    int32_t WindowWidth{640};
    int32_t WindowHeight{640};
    int32_t GLVersionMajor{3};
    int32_t GLVersionMinor{3};
    double PollTimeout{0.1};
    uint32_t MaxFrames{static_cast<uint32_t>(-1)};
};

void MainLoop(const Settings &settings);
void Initialize();
void Terminate();
void Close();
bool ShouldClose();
void Present();

void OnKeyboard(int code, int scancode, int action, int mods);
void OnMouseMove(double xpos, double ypos);
void OnMouseButton(int button, int action, int mods);
void OnInitialize();
void OnTerminate();
void OnMainLoop();

} // Graphics

#endif // GRAPHICS_H_
