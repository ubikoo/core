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
#include "core/graphics/common.h"

namespace Graphics {

struct Camera {
    // Camera constants.
    static const float kMoveDistance;
    static const float kRotateAngle;

    // Camera coordinate system.
    math::vec3f position;     // camera position
    math::vec3f look;         // look-direction vector
    math::vec3f up;           // up-direction vector

    // Camera frustum properties.
    float fovy;             // y-direction field fo view
    float aspect;           // camera aspect ratio
    float znear;            // near z-plane position
    float zfar;             // far z-plane position

    // Camera status properties.
    float speed;            // Camera speed rate
    bool enabled;           // Is the camera enabled?

    // Camera keyboard and mouse events.
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

    void Speed(float value);

    void Enable() { enabled = true; }
    void Disable() { enabled = false; }
    bool IsEnabled() const { return enabled; }

    // Compute the camera transform matrices.
    math::mat4f View();
    math::mat4f Proj();
};

// Create a camera with a specified local coordinate system and frustum.
Camera CreateCamera(
    const math::vec3f &position,
    const math::vec3f &ctr,
    const math::vec3f &up,
    const float fovy,
    const float aspect,
    const float znear,
    const float zfar);

} // Graphics

#endif // GRAPHICS_CAMERA_H_
