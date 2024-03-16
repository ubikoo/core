//
// vertexarray.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include "vertexarray.h"

namespace Graphics {

///
/// @brief Generate a new vertex array object. When calling a draw function, for
/// each active attribute of the vertex shader, specify where to fetch the input
/// data and what it looks like. This is done by repeating the steps below for
/// each vertex attribute:
///  - glEnableVertexAttribArray to enable the attribute.
///  - glBindBuffer(GL_ARRAY_BUFFER) to bind the buffer that contains the data.
///  - glVertexAttribPointer to define the buffer location and data format.
/// These functions change the state of the vertex array object. When calling
/// glVertexAttribPointer, the vertex array object keeps a reference of the buffer
/// currently bound, so there's no need to rebind the buffer again during a draw.
/// The usage pattern of is:
///  - At beginning, for each drawable object:
///      - Bind the Vertex Array Objet.
///      - Bind a Buffer Object with vertex indices to GL_ELEMENT_ARRAY_BUFFER.
///      - Bind the Buffer Object with vertex attributes to GL_ARRAY_BUFFER.
///      - Specify how OpenGL should interpret the vertex data (vertices, normals,
///         etc). The intepretation is stored in the VAO.
///      - Unbind the VAO
///  - At render point, for each drawable object:
///      - Bind the VAO
///      - Call glDraw[*]
///      - Unbind the VAO
///
/// @see https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices
///      https://gamedev.stackexchange.com/questions/99236
///      http://www.songho.ca/opengl/gl_vertexarray.html
///      https://stackoverflow.com/questions/21652546
///      https://stackoverflow.com/questions/13603956
///      https://stackoverflow.com/questions/37985684
///      https://stackoverflow.com/questions/46133849
///
VertexArray CreateVertexArray()
{
    VertexArrayObject *array = new VertexArrayObject;
    {
        glGenVertexArrays(1, &array->mId);
    }
    return VertexArray(array, VertexArrayDeleter());
}

///
/// @brief Bind/Unbind the vertex array object.
///
void VertexArrayObject::Bind() const
{
    glBindVertexArray(mId);
}

void VertexArrayObject::Unbind() const
{
    glBindVertexArray(0);
}

} // Graphics
