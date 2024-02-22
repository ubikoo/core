//
// render.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <exception>
#include <algorithm>

#include "render.h"

namespace Graphics {

///
/// @brief Static render data.
///
static RenderDesc gRenderDesc = {};
static GLFWwindow *gWindow = nullptr;

///
/// @brief Static callback functions.
///
static void OnErrorCallback(int code, const char *desc)
{
    std::cerr << "GLFW error code " << code << ": " << desc << "\n";
}

static void OnResizeCallback(GLFWwindow *window, int width, int height)
{
    OnResize(width, height);
}

static void OnKeyboardCallback(GLFWwindow *window,
    int code, int scancode, int action, int mods)
{
    OnKeyboard(code, scancode, action, mods);
}

static void OnMouseMoveCallback(GLFWwindow *window, double xpos, double ypos)
{
    OnMouseMove(xpos, ypos);
}

static void OnMouseButtonCallback(GLFWwindow *window,
    int button, int action, int mods)
{
    OnMouseButton(button, action, mods);
}

///
/// @brief Main render loop.
///
void RenderLoop(const RenderDesc &desc)
{
    gRenderDesc = desc;
    Initialize();
    OnInitialize();
    while (!ShouldClose()) {
        OnUpdate();
        OnRender();
        Present();
    }
    OnTerminate();
    Terminate();
}

///
/// @brief Initialize GLFW library and create a window with a OpenGL context.
///
/// @note Only forward compatible core profile contexts for OpenGL versions 3.2
/// and later are supported. Set context hints GLFW_OPENGL_FORWARD_COMPAT and
/// GLFW_OPENGL_PROFILE before creation. A forward-compatible context is one
/// where all functionality deprecated in the requested version is removed.
///
/// @note After creating the window, make current the OpenGL context and load
/// the OpenGL function pointers using the context provided by the OpenGL loader
/// function. The watertight method is to call gladLoadGLLoader after each and
/// every context change.
///
void Initialize()
{
    // Ensure it is not initialized.
    assert(!gWindow);

    // Initialize the GLFW library and create a GLFWwindow.
    glfwSetErrorCallback(OnErrorCallback);

    if (glfwInit() != GLFW_TRUE) {
        throw std::runtime_error("failed to initialise GLFW library");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gRenderDesc.GLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gRenderDesc.GLVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    gWindow = glfwCreateWindow(
        gRenderDesc.WindowWidth,
        gRenderDesc.WindowHeight,
        gRenderDesc.WindowTitle.c_str(),
        nullptr,                // run in windowed mode
        nullptr);               // do not share the OpenGL context
    if (gWindow == nullptr) {
        glfwTerminate();
        throw std::runtime_error("failed to create GLFWwindow");
    }

    glfwMakeContextCurrent(gWindow);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("failed to create GLFWwindow");
    }
    glfwSwapInterval(1);

    // Enable window events.
    glfwSetFramebufferSizeCallback(gWindow, OnResizeCallback);
    glfwSetKeyCallback(gWindow, OnKeyboardCallback);
    glfwSetCursorPosCallback(gWindow, OnMouseMoveCallback);
    glfwSetMouseButtonCallback(gWindow, OnMouseButtonCallback);

    // Log OpenGL/GLFW information string.
    std::cout << "GLFW version: " << glfwGetVersionString() << "\n"
              << "OpenGL Renderer: " << glGetString(GL_RENDERER) << "\n"
              << "OpenGL Version: " << glGetString(GL_VERSION) << "\n\n";
}

///
/// @brief Terminate the GLFW library and destroy the window.
///
void Terminate()
{
    glfwDestroyWindow(gWindow);
    gWindow = nullptr;
    glfwTerminate();
}

///
/// @brief Signal the application to close.
///
void Close()
{
    glfwSetWindowShouldClose(gWindow, GLFW_TRUE);
}

///
/// @brief Is the application signaled to close.
///
bool ShouldClose()
{
    return glfwWindowShouldClose(gWindow);
}

///
/// @brief Swap buffers and poll events.
///
void Present()
{
    // Update title with current fame rate.
    static size_t FrameCount = 0;
    static double FrameTime = glfwGetTime();

    FrameCount++;
    double ElapsedTime = glfwGetTime() - FrameTime;
    if (ElapsedTime >= 1.0) {
        double fps = static_cast<double>(FrameCount) / ElapsedTime;

        std::ostringstream buffer;
        buffer << gRenderDesc.WindowTitle << ": " << FrameCount << " frames at "
               << std::setprecision(2) << (1000.0 / fps) << " ms/frame";
        glfwSetWindowTitle(gWindow, buffer.str().c_str());

        FrameCount = 0;
        FrameTime = glfwGetTime();
    }

    // Swap front and back buffers.
    glfwSwapBuffers(gWindow);

    // Poll events
    glfwWaitEventsTimeout(std::max(0.0, gRenderDesc.PollTimeout));
}

} // Graphics
