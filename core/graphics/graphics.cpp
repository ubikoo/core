//
// graphics.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <iostream>
#include <iomanip>
#include <sstream>
#include <exception>
#include <algorithm>
#include "graphics.h"
#include "helpers.h"

namespace Graphics {

///
/// @brief Static global data.
///
static Settings gSettings = {};
static GLFWwindow *gWindow = nullptr;

///
/// @brief Default error callback function.
///
static void OnErrorCallback(int code, const char *desc)
{
    std::cerr << "GLFW error code " << code << ": " << desc << "\n";
}

///
/// @brief Update the viewport with the new framebuffer size in pixels.
///
static void OnResizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

///
/// @brief Keyboard and mouse callback functions. Esc key closes the application
/// by default.
///
static void OnKeyboardCallback(GLFWwindow *window,
    int code, int scancode, int action, int mods)
{
    if (code == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        Close();
    }

    if (gSettings.OnKeyboard) {
        gSettings.OnKeyboard(code, scancode, action, mods);
    }
}

static void OnMouseMoveCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (gSettings.OnMouseMove) {
        gSettings.OnMouseMove(xpos, ypos);
    }
}

static void OnMouseButtonCallback(GLFWwindow *window,
    int button, int action, int mods)
{
    if (gSettings.OnMouseButton) {
        gSettings.OnMouseButton(button, action, mods);
    }
}

///
/// @brief Main render loop.
///
void MainLoop(const Settings &settings)
{
    gSettings = settings;
    Initialize();
    if (gSettings.OnInitialize) {
        gSettings.OnInitialize();
    }
    while (!ShouldClose()) {
        if (gSettings.OnMainLoop) {
            gSettings.OnMainLoop();
        }
        Present();
    }
    if (gSettings.OnTerminate) {
        gSettings.OnTerminate();
    }
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
    ThrowIf(gWindow);

    // Initialize the GLFW library and create a GLFWwindow.
    glfwSetErrorCallback(OnErrorCallback);

    if (glfwInit() != GLFW_TRUE) {
        throw std::runtime_error("failed to initialise GLFW library");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gSettings.GLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gSettings.GLVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    gWindow = glfwCreateWindow(
        gSettings.WindowWidth,
        gSettings.WindowHeight,
        gSettings.WindowTitle.c_str(),
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
    // Render these many frames.
    {
        static uint32_t NumFrames = 0;
        if (++NumFrames > gSettings.MaxFrames) {
            Close();
        }
    }

    // Update title with current fame rate.
    {
        static uint32_t FrameCount = 0;
        static double BeginTime = glfwGetTime();

        FrameCount++;
        double ElapsedTime = glfwGetTime() - BeginTime;
        if (ElapsedTime >= 1.0) {
            double fps = static_cast<double>(FrameCount) / ElapsedTime;
            std::ostringstream buffer;
            buffer << gSettings.WindowTitle << ": "
                << FrameCount << " frames at "
                << std::setprecision(2)
                << (1000.0 / fps) << " ms/frame";
            glfwSetWindowTitle(gWindow, buffer.str().c_str());

            FrameCount = 0;
            BeginTime = glfwGetTime();
        }
    }

    // Swap front/back buffers and poll events.
    glfwSwapBuffers(gWindow);
    glfwWaitEventsTimeout(std::max(0.0, gSettings.PollTimeout));
}

} // Graphics
