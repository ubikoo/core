//
// pipeline.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <string>
#include <vector>
#include "program.h"
#include "uniform.h"
#include "attribute.h"
#include "vertexarray.h"
#include "pipeline.h"

namespace Graphics {

///
/// @brief Create a render pipeline object.
///
Pipeline CreatePipeline(const PipelineCreateInfo &info)
{
    PipelineObject *pipeline = new PipelineObject;
    {
        pipeline->mPolygonMode = info.polygonMode;
        pipeline->mEnableCullFace = info.enableCullFace;
        pipeline->mCullFaceMode = info.cullFaceMode;
        pipeline->mFrontFaceMode = info.frontFaceMode;
        pipeline->mEnableDepthTest = info.enableDepthTest;
        pipeline->mDepthFunc = info.depthFunc;
        pipeline->mClearMask = info.clearMask;
        pipeline->mClearDepth = info.clearDepth;
        pipeline->mClearColor = info.clearColor;
        pipeline->mLineWidth = info.lineWidth;
        pipeline->mPointSize = info.pointSize;
        pipeline->mProgram = CreateProgram({info.shaders});
        pipeline->mVao = CreateVertexArray();
    }
    return Pipeline(pipeline, PipelineDeleter());
}

///
/// @brief Use the pipeline as part of the current rendering state.
///
void PipelineObject::Use() const
{
    // Specify pipeline rasterizer state.
    glPolygonMode(GL_FRONT_AND_BACK, mPolygonMode);

    if (mEnableCullFace) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
    glCullFace(mCullFaceMode);
    glFrontFace(mFrontFaceMode);

    if (mEnableDepthTest) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    glDepthFunc(mDepthFunc);

    glLineWidth(mLineWidth);
    glPointSize(mPointSize);

    // Use the pipeline shader program object
    mProgram->Use();
    mVao->Bind();
}

///
/// @brief Bind/Unbind the pipeline vertex array object.
///
void PipelineObject::Bind() const
{
    mVao->Bind();
}

void PipelineObject::Unbind() const
{
    mVao->Unbind();
}

///
/// @brief Clear the pipeline color and depth buffers to their preset values.
///
void PipelineObject::Clear() const
{
    glClearColor(mClearColor[0],mClearColor[1],mClearColor[2],mClearColor[3]);
    glClearDepth(mClearDepth);
    glClear(mClearMask);
}

///
/// @brief Set the properites of the vertex attributes in the pipeline shader
/// program object.
///
void PipelineObject::SetAttribute(
    const std::vector<AttributeDescription> &attributes) const
{
    mProgram->SetAttribute(attributes);
}

///
/// @brief Update the uniform with the specified name in the pipeline shader
/// program.
///
void PipelineObject::SetUniform(
    const std::string &name,
    const GLenum type,
    const void *data) const
{
    mProgram->SetUniform(name, type, data);
}

///
/// @brief Update the uniform matrix with the specified name in the shader
/// program using the corresponding glUniform* function.
///
void PipelineObject::SetUniformMatrix(
    const std::string &name,
    const GLenum type,
    const GLboolean transpose,
    const void *data) const
{
    mProgram->SetUniformMatrix(name, type, transpose, data);
}

} // namespace Graphics
