//
// vertexarray.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_VERTEXARRAY_H_
#define GRAPHICS_VERTEXARRAY_H_

#include <memory>
#include "common.h"

namespace Graphics {

///
/// @brief Interface to a vertex array object.
///
struct VertexArrayObject {
    GLuint mId{0};

    void Bind() const;
    void Unbind() const;
};

struct VertexArrayDeleter {
    void operator()(VertexArrayObject *obj) {
        if (obj) { glDeleteVertexArrays(1, &obj->mId); }
        delete obj;
    }
};

using VertexArray = std::unique_ptr<VertexArrayObject, VertexArrayDeleter>;
VertexArray CreateVertexArray();

} // namespace Graphics

#endif // GRAPHICS_VERTEXARRAY_H_
