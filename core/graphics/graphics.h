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
/// @brief Helper functions providing an interface to OpenGL. The interface is
/// implemented in a data-oriented style for ease of use and to avoid issues
/// associated with usage of an object-oriented style to maintain OpenGL objects:
/// RAII, move semantics, etc.
///  - Regular objects contain data and are to transmit data to the GPU
///      Buffer, Renderbuffer, Texture, Query, Sampler
///  - Container objects contain no data, only references to other objects
///      Framebuffer, Vertex Array, Transform Feedback, Program Pipeline
///  - Non-standard objects do not use conventions for other GL objects
///      Shader and Program
/// @see https://www.khronos.org/wiki/OpenGL_Object
///      https://www.khronos.org/wiki/GLSL_Object
///      https://www.khronos.org/wiki/Common_Mistakes
///

#include "gl/error.h"
#include "gl/buffer.h"
#include "gl/framebuffer.h"
#include "gl/renderbuffer.h"
#include "gl/texture.h"
#include "gl/vertexarray.h"

#include "glsl/datatype.h"
#include "glsl/attribute.h"
#include "glsl/program.h"
#include "glsl/shader.h"
#include "glsl/uniform.h"

#include "common.h"
#include "camera.h"
#include "image.h"
#include "mesh.h"
#include "render.h"

#endif // GRAPHICS_H_
