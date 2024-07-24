//
// pipeline.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_PIPELINE_H_
#define GRAPHICS_PIPELINE_H_

#include <array>
#include <vector>
#include <memory>
#include "attribute.h"
#include "uniform.h"
#include "program.h"
#include "vertexarray.h"

namespace Graphics {

///
/// @brief Interface to a render pipeline object. The render pipeline maintains
/// a shader program object and associated vertex array object describing the
/// location and format of the program input vertex attributes. The pipeline also
/// maintains information about the rasterizer state and viewport to be used when
/// rendering.
///
struct PipelineCreateInfo {
    GLenum polygonMode{GL_FILL};
    GLboolean enableCullFace{GL_FALSE};
    GLenum cullFaceMode{GL_BACK};
    GLenum frontFaceMode{GL_CCW};
    GLboolean enableDepthTest{GL_FALSE};
    GLenum depthFunc{GL_LESS};
    GLbitfield clearMask{GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT};
    std::array<GLfloat,4> clearColor{0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat clearDepth{1.0};
    GLfloat lineWidth{1.0f};
    GLfloat pointSize{1.0f};
    std::vector<GLuint> shaders;
};

struct PipelineObject {
    GLenum mPolygonMode;
    GLboolean mEnableCullFace;
    GLenum mCullFaceMode;
    GLenum mFrontFaceMode;
    GLboolean mEnableDepthTest;
    GLenum mDepthFunc;
    GLbitfield mClearMask;
    std::array<GLfloat,4> mClearColor;
    GLfloat mClearDepth;
    GLfloat mLineWidth;
    GLfloat mPointSize;
    Program mProgram;
    VertexArray mVao;

    void Use() const;
    void Bind() const;
    void Unbind() const;
    void Clear() const;

    void SetAttribute(
        const std::vector<AttributeDescription> &attributes) const;
    void SetUniform(
        const std::string &name,
        const GLenum type,
        const void *data) const;
    void SetUniformMatrix(
        const std::string &name,
        const GLenum type,
        const GLboolean transpose,
        const void *data) const;
};

struct PipelineDeleter {
    void operator()(PipelineObject *obj) {
        delete obj;
    }
};

using Pipeline = std::unique_ptr<PipelineObject, PipelineDeleter>;
Pipeline CreatePipeline(const PipelineCreateInfo &info);

} // namespace Graphics

#endif // GRAPHICS_PIPELINE_H_
