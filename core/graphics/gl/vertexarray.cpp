//
// vertexarray.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <cassert>
#include "vertexarray.h"
#include "buffer.h"

namespace Graphics {

///
/// @brief Generate a new vertex array object and bind it to the context.
///
/// @par At draw call, for each active vertex attribute in the shader, do:
///  - glEnableVertexAttribArray to enable the attribute
///  - glBindBuffer(GL_ARRAY_BUFFER) to bind the buffer object that contains
///    the data for the attribute
///  - glVertexAttribPointer to define the location and format of the data
///    for the attribute in the buffer object bound to target GL_ARRAY_BUFFER
///
/// The steps are repeated for each vertex attribute of each drawable. A VAO
/// remembers the state needed to specify per-vertex attribute data to OpenGL.
/// The following functions change the state of a currently bound VAO:
///  - glVertexAttribPointer
///  - glEnableVertexAttribArray / glDisableVertexAttribArray
///  - glBindBuffer(GL_ELEMENT_ARRAY_BUFFER)
///
/// glBindBuffer(GL_ARRAY_BUFFER) is not part of a VAO. The association between
/// a buffer object and a vertex attribute happens when glVertexAttribPointer is
/// called. glVertexAttribPointer conflates two distinct operations:
///  - Where and how to fetch the data from memory
///  - What the data looks like
///
/// @par The usage pattern of a VAO is:
///  - At the beginning, for each drawable object:
///      - Bind the VAO
///      - Bind a Buffer Object with vertex indices to GL_ELEMENT_ARRAY_BUFFER.
///      - Bind the Buffer Object with vertex attributes to GL_ARRAY_BUFFER.
///      - Specify how OpenGL should interpret the vertex data (vertices,
///      normals, etc). The intepretation is stored in the VAO
///      - Unbind the VAO
///  - At render point, for each drawable object:
///      - Bind the VAO
///      - Call glDraw[*]
///      - Unbind the VAO
///
/// @see https://www.khronos.org/opengl/wiki/Vertex_Shader
///      https://www.khronos.org/opengl/wiki/Vertex_Specification
///      https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices
///      http://www.songho.ca/opengl/gl_vertexarray.html
///      https://stackoverflow.com/questions/37972229
///      https://stackoverflow.com/questions/34486197
///      https://stackoverflow.com/questions/7718976
///      https://stackoverflow.com/questions/13603956
///      https://stackoverflow.com/questions/37985684
///      https://stackoverflow.com/questions/46133849
///      https://stackoverflow.com/questions/21652546
///      https://gamedev.stackexchange.com/questions/99236
///      https://gamedev.stackexchange.com/questions/97063
///
GLuint CreateVertexArray()
{
    GLuint array;
    glGenVertexArrays(1, &array);
    glBindVertexArray(array);
    assert(glIsVertexArray(array));
    glBindVertexArray(0);
    return array;
}

///
/// @brief Delete a vertex array object.
///
void DestroyVertexArray(const GLuint &array)
{
    glDeleteVertexArrays(1, &array);
}

} // Graphics
