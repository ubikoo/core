//
// camera.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <algorithm>
#include "core/math/math.h"
#include "camera.h"

namespace Graphics {

///
/// @brief Camera constants
///
const float Camera::kMoveDistance = 0.1f;
const float Camera::kRotateAngle = 0.001f * M_PI;

///
/// @brief Camera keyboard and mouse events.
///
void Camera::Keyboard(int code, int scancode, int action, int mods)
{
    if (IsEnabled()) {
        switch (code) {
        case GLFW_KEY_W:
            Move(Camera::kMoveDistance * speed);
            break;
        case GLFW_KEY_S:
            Move(-Camera::kMoveDistance * speed);
            break;
        case GLFW_KEY_A:
            Strafe(-Camera::kMoveDistance * speed);
            break;
        case GLFW_KEY_D:
            Strafe( Camera::kMoveDistance * speed);
            break;
        case GLFW_KEY_Q:
            Lift(-Camera::kMoveDistance * speed);
            break;
        case GLFW_KEY_E:
            Lift( Camera::kMoveDistance * speed);
            break;
        default:
            break;
        }
    }
}

void Camera::MouseMove(double xpos, double ypos)
{
    static math::vec2f sMousePosition = {};
    if (IsEnabled()) {
        Yaw((sMousePosition.x - xpos) * Camera::kRotateAngle * speed);
        Pitch((sMousePosition.y - ypos) * Camera::kRotateAngle * speed);
    }
    sMousePosition = {static_cast<float>(xpos), static_cast<float>(ypos)};
}

void Camera::MouseButton(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        Enable();
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        Disable();
    }
}

///
/// @brief Move the camera forward and backward along its look vector.
///
void Camera::Move(float step)
{
    position += look * step;
}

///
/// @brief Strafe the camera right and left along its right vector.
///
void Camera::Strafe(float step)
{
    math::vec3f right = math::normalize(math::cross(look, up));
    position += right * step;
}

///
/// @brief Lift the camera along its upward vector.
///
void Camera::Lift(float step)
{
    math::vec3f right = math::normalize(math::cross(look, up));
    math::vec3f upward = math::normalize(math::cross(right, look));
    position += upward * step;
}

///
/// @brief Rotate camera up and down around its right vector.
///
void Camera::Pitch(float angle)
{
    math::vec3f right = math::normalize(math::cross(look, up));
    Rotate(math::rotate(right, angle));
}

///
/// @brief Rotate camera left and right around its up vector.
///
void Camera::Yaw(float angle)
{
    math::vec3f right = math::normalize(math::cross(look, up));
    math::vec3f upward = math::normalize(math::cross(right, look));
    Rotate(math::rotate(upward, angle));
}

///
/// @brief Rotate the camera look-direction vector.
///
void Camera::Rotate(const math::mat4f &rot)
{
    math::vec4f d = {look.x, look.y, look.z, 0.0f};
    d = math::dot(rot, d);
    look = math::vec3f{d.x, d.y, d.z};
}

///
/// Zoom the camera field of view.
///
void Camera::Zoom(float scale)
{
    static const float max_fovy = 0.999f*M_PI;
    static const float min_fovy = 0.001f*M_PI;
    fovy *= scale;
    fovy = std::min(std::max(fovy, min_fovy), max_fovy);
}

///
/// Set camera speed value.
///
void Camera::Speed(float value)
{
    speed = value;
}

///
/// @brief Return the camera view transform.
///
math::mat4f Camera::View()
{
    return math::lookat(position, position + look, up);
}

///
/// @brief Return the camera projection transform.
///
math::mat4f Camera::Proj()
{
    return math::perspective(fovy, aspect, znear, zfar);
}

///
/// @brief Create a camera with a specified local coordinate system and frustum.
///
Camera CreateCamera(
    const math::vec3f &position,
    const math::vec3f &ctr,
    const math::vec3f &up,
    const float fovy,
    const float aspect,
    const float znear,
    const float zfar)
{
    Camera camera = {};
    camera.position = position;
    camera.look = math::normalize(ctr - position);
    camera.up = up;
    camera.fovy = fovy;
    camera.aspect = aspect;
    camera.znear = znear;
    camera.zfar = zfar;
    camera.speed = 1.0f;
    camera.enabled = false;
    return camera;
}

} // Graphics
