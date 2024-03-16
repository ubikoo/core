//
// camera.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_CAMERA_H_
#define GRAPHICS_CAMERA_H_

#include "core/math/math.h"
#include "common.h"

namespace Graphics {

struct Camera {
    // Camera coordinate system, frustum and movement properties.
    math::vec3f mPosition;      // camera position
    math::vec3f mLook;          // look-direction vector
    math::vec3f mUp;            // up-direction vector
    float mFovy;                // y-direction field fo view
    float mAspect;              // camera aspect ratio
    float mZnear;               // near z-plane position
    float mZfar;                // far z-plane position
    float mMoveSpeed;           // camera translation speed
    float mRotateSpeed;         // camera rotation speed

    // Is the camera enabled?
    bool mEnabled;

    // Keyboard and mouse events.
    void Keyboard(int code, int scancode, int action, int mods);
    void MouseMove(double xpos, double ypos);
    void MouseButton(int button, int action, int mods);

    // Transform the camera state.
    void Move(float step);
    void Strafe(float step);
    void Lift(float step);
    void Pitch(float angle);
    void Yaw(float angle);
    void Rotate(const math::mat4f &rot);
    void Zoom(float scale);

    void Enable() { mEnabled = true; }
    void Disable() { mEnabled = false; }
    bool IsEnabled() const { return mEnabled; }

    // Compute the camera transform matrices.
    math::mat4f View();
    math::mat4f Proj();
};

// Create a camera with a specified local coordinate system and frustum.
struct CameraCreateInfo {
    math::vec3f position{0.0f, 0.0f,  0.0f};    // eye-position
    math::vec3f ctr{0.0f, 0.0f, -1.0f};         // view-direction
    math::vec3f up{0.0f, 1.0f,  0.0f};          // up-direction
    float fovy{0.5f * M_PI};                    // y-fov
    float aspect{1.0};                          // aspect ratio
    float znear{0.1};                           // z-near
    float zfar{10.0};                           // z-far
    float moveSpeed{0.01f};                    // translation speed
    float rotateSpeed{0.01f * M_PI};           // rotation speed
};
Camera CreateCamera(const CameraCreateInfo &info);

} // Graphics

#endif // GRAPHICS_CAMERA_H_
