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
/// @brief Camera keyboard and mouse events.
///
void Camera::Keyboard(int code, int scancode, int action, int mods)
{
    if (IsEnabled()) {
        switch (code) {
        case GLFW_KEY_W:
            Move(mMoveSpeed);
            break;
        case GLFW_KEY_S:
            Move(-mMoveSpeed);
            break;
        case GLFW_KEY_A:
            Strafe(-mMoveSpeed);
            break;
        case GLFW_KEY_D:
            Strafe( mMoveSpeed);
            break;
        case GLFW_KEY_Q:
            Lift(-mMoveSpeed);
            break;
        case GLFW_KEY_E:
            Lift( mMoveSpeed);
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
        Yaw((sMousePosition.x - xpos) * mRotateSpeed);
        Pitch((sMousePosition.y - ypos) * mRotateSpeed);
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
    mPosition += mLook * step;
}

///
/// @brief Strafe the camera right and left along its right vector.
///
void Camera::Strafe(float step)
{
    math::vec3f right = math::normalize(math::cross(mLook, mUp));
    mPosition += right * step;
}

///
/// @brief Lift the camera along its upward vector.
///
void Camera::Lift(float step)
{
    math::vec3f right = math::normalize(math::cross(mLook, mUp));
    math::vec3f upward = math::normalize(math::cross(right, mLook));
    mPosition += upward * step;
}

///
/// @brief Rotate camera up and down around its right vector.
///
void Camera::Pitch(float angle)
{
    math::vec3f right = math::normalize(math::cross(mLook, mUp));
    Rotate(math::rotate(right, angle));
}

///
/// @brief Rotate camera left and right around its up vector.
///
void Camera::Yaw(float angle)
{
    math::vec3f right = math::normalize(math::cross(mLook, mUp));
    math::vec3f upward = math::normalize(math::cross(right, mLook));
    Rotate(math::rotate(upward, angle));
}

///
/// @brief Rotate the camera look-direction vector.
///
void Camera::Rotate(const math::mat4f &rot)
{
    math::vec4f d = {mLook.x, mLook.y, mLook.z, 0.0f};
    d = math::dot(rot, d);
    mLook = math::vec3f{d.x, d.y, d.z};
}

///
/// Zoom the camera field of view.
///
void Camera::Zoom(float scale)
{
    static const float kMaxFovy = 0.999f*M_PI;
    static const float kMinFovy = 0.001f*M_PI;
    mFovy *= scale;
    mFovy = std::min(std::max(mFovy, kMinFovy), kMaxFovy);
}

///
/// @brief Return the camera view transform.
///
math::mat4f Camera::View()
{
    return math::lookat(mPosition, mPosition + mLook, mUp);
}

///
/// @brief Return the camera projection transform.
///
math::mat4f Camera::Proj()
{
    return math::perspective(mFovy, mAspect, mZnear, mZfar);
}

///
/// @brief Create a camera with a specified local coordinate system and frustum.
///
Camera CreateCamera(const CameraCreateInfo &info)
{
    Camera camera = {};
    camera.mPosition = info.position;
    camera.mLook = math::normalize(info.ctr - info.position);
    camera.mUp = info.up;
    camera.mFovy = info.fovy;
    camera.mAspect = info.aspect;
    camera.mZnear = info.znear;
    camera.mZfar = info.zfar;
    camera.mMoveSpeed = info.moveSpeed;
    camera.mRotateSpeed = info.rotateSpeed;
    camera.mEnabled = false;
    return camera;
}

} // Graphics
