//
// mesh.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

/// @brief tinyobjloader implementation
#define TINYOBJLOADER_IMPLEMENTATION
#include <external/tinyobjloader/tiny_obj_loader.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "mesh.h"
#include "helpers.h"

namespace Graphics {

///
/// @brief Copy mesh vertex data onto the gpu.
///
void MeshObject::Copy() const
{
    GLsizeiptr size = mVertices.size() * sizeof(MeshObject::Vertex);
    mVbo->Copy(0, size, mVertices.data());
}

///
/// @brief Bind/unbind the mesh vertex and element buffer objects.
///
void MeshObject::Bind() const
{
    mVbo->Bind();
    mEbo->Bind();
}

void MeshObject::Unbind() const
{
    mVbo->Unbind();
    mEbo->Unbind();
}

///
/// @brief Draw the mesh.
///
void MeshObject::Draw() const
{
    GLsizei numElements = mIndices.size();
    glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, (GLvoid *) 0);
}

///
/// @brief Draw count instances of the mesh.
///
void MeshObject::DrawInstanced(GLsizei count) const
{
    GLsizei numElements = mIndices.size();
    glDrawElementsInstanced(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, (GLvoid *) 0, count);
}


///
/// @brief A mesh grid with (n1 * n2) vertices along the first and second
/// dimensions is defined as an orientable manifold with topology:
///     (i, j + 1) ------  (i + 1, j + 1)
///               | \   |
///               |  \  |
///               |   \ |
///        (i, j) ------ (i + 1, j)
///
/// where (0 <= i < n1 - 1) and (0 <= j < n2 - 1), resulting:
///      (n1 - 1) * (n2 - 1) quads in a lattice with (n1 * n2) vertices,
///      2 triangles per quad, or 2 * (n1 - 1) * (n2 - 1) lattice triangles,
///      3 indices per triangle, or 6 * (n1 - 1) * (n2 - 1) triangle indices.
///
/// The grid represents an orientable manifold with topology:
///
/// The first triangle is represented by indices {k0, k0 + 1, k1} and the second
/// triangle is represented by  {k1 + 1, k1, k0 + 1}.
///
std::vector<MeshObject::Index> CreateGrid(const size_t n1, const size_t n2)
{
    ThrowIfNot(n1 > 1 && n2 > 1);

    std::vector<MeshObject::Index> indices(6 * (n1 - 1) * (n2 - 1));
    size_t k = 0;
    for (size_t j = 0; j < n2 - 1; ++j) {
        size_t k0 = j * n1;     // first row
        size_t k1 = k0 + n1;    // second row
        for (size_t i = 0; i < n1 - 1; ++i, ++k0, ++k1) {
            // First triangle (upward hypotenuse)
            indices[k++] = k0;
            indices[k++] = k0 + 1;
            indices[k++] = k1;

            // Second triangle (downward hypotenuse)
            indices[k++] = k1 + 1;
            indices[k++] = k1;
            indices[k++] = k0 + 1;
        }
    }
    return indices;
}

///
/// @brief Create a mesh with a given name bound to a shader program object
/// from a list of vertices and indices.
///
/// The mesh maintains two vertex buffer objects - vbo and ebo - holding vertex
/// attributes and vertex indices respectively. The shader program object needs
/// to have the following vertex attributes active:
///  {position, normal, color, texcoord}
///
/// Every mesh has a name prefixing the vertex attributes. For example, if the
/// mesh name is ball, the names of the vertex attribute variables are:
///   {ball_position, ball_normal, ball_color, ball_texcoord}
///
Mesh CreateMesh(
    const std::string &name,
    const std::vector<MeshObject::Vertex> &vertices,
    const std::vector<MeshObject::Index> &indices)
{
    // Create a new mesh and given name from a list of vertices and indices.
    MeshObject *mesh = new MeshObject;
    mesh->mName = name;
    mesh->mVertices = vertices;
    mesh->mIndices = indices;

    // Specify vertex attributes description.
    {
        std::string positionName = std::string("in") + mesh->mName + std::string("Position");
        std::string normalName = std::string("in") + mesh->mName + std::string("Normal");
        std::string colorName = std::string("in") + mesh->mName + std::string("Color");
        std::string texcoordName = std::string("in") + mesh->mName + std::string("Texcoord");

        GLsizei stride = sizeof(MeshObject::Vertex);

        GLsizeiptr positionOffset = offsetof(MeshObject::Vertex, position);
        GLsizeiptr normalOffset = offsetof(MeshObject::Vertex, normal);
        GLsizeiptr colorOffset = offsetof(MeshObject::Vertex, color);
        GLsizeiptr texcoordOffset = offsetof(MeshObject::Vertex, texcoord);

        mesh->mAttributes = std::vector<AttributeDescription>{
            {positionName, GL_FLOAT, GL_FLOAT_VEC3, stride, positionOffset, false, 0},
            {normalName,   GL_FLOAT, GL_FLOAT_VEC3, stride, normalOffset,   false, 0},
            {colorName,    GL_FLOAT, GL_FLOAT_VEC3, stride, colorOffset,    false, 0},
            {texcoordName, GL_FLOAT, GL_FLOAT_VEC2, stride, texcoordOffset, false, 0},
        };
    }

    // Create buffer storage for vertex data with layout {(xyz,nor,rgb,uv),...}.
    {
        BufferCreateInfo info = {};
        info.target = GL_ARRAY_BUFFER;
        info.size = mesh->mVertices.size() * sizeof(MeshObject::Vertex);
        info.usage = GL_STATIC_DRAW;

        mesh->mVbo = CreateBuffer(info);
        mesh->mVbo->Copy(0, info.size , mesh->mVertices.data());
    }

    // Create a buffer storage for face indices with layout {(i0,i1,i2),...}.
    {
        BufferCreateInfo info = {};
        info.target = GL_ELEMENT_ARRAY_BUFFER;
        info.size = mesh->mIndices.size() * sizeof(MeshObject::Index);
        info.usage = GL_STATIC_DRAW;

        mesh->mEbo = CreateBuffer(info);
        mesh->mEbo->Copy(0, info.size , mesh->mIndices.data());
    }

    return Mesh(mesh);
}

///
/// @brief Create a plane represented by (n1 * n2) vertices on a rectangle
/// region in the xy-plane, bounded by lower (xlo, ylo) and upper (xhi, yhi)
/// positions.
///
/// The vertex coordinates (x, y) are linearly interpolated between (xlo, ylo)
/// and (xhi, yhi) positions along the n1 and n2 lattice directions.
/// Vertex normals point in the (0,0,1) direction.
/// Vertex colors map the texture coordinates to a (r,g)-colormap.
/// Vertex texture coordinates are linearly interpolated between (0,0) and (1,1).
///
Mesh CreatePlane(
    const std::string &name,
    size_t n1,
    size_t n2,
    GLfloat xlo,
    GLfloat xhi,
    GLfloat ylo,
    GLfloat yhi)
{
    ThrowIfNot(n1 > 1 && n2 > 1);
    ThrowIfNot(xlo < xhi && ylo < yhi);

    // Create mesh vertices.
    std::vector<MeshObject::Vertex> vertices(n1 * n2);

    GLfloat dx = (xhi - xlo) / (GLfloat) (n1 - 1);
    GLfloat dy = (yhi - ylo) / (GLfloat) (n2 - 1);

    GLfloat du = 1.0f / (GLfloat) (n1 - 1);
    GLfloat dv = 1.0f / (GLfloat) (n2 - 1);

    for (size_t j = 0; j < n2; ++j) {
        for (size_t i = 0; i < n1; ++i) {
            size_t k = i + j * n1;

            // Vertex positions are in the xy-plane by default.
            vertices[k].position[0] = xlo + (GLfloat) i * dx;
            vertices[k].position[1] = ylo + (GLfloat) j * dy;
            vertices[k].position[2] = 0.0;

            // Vertex normals point in the z-direction by default.
            vertices[k].normal[0] = 0.0f;
            vertices[k].normal[1] = 0.0f;
            vertices[k].normal[2] = 1.0f;

            // Encode the vertex colors with their uv-coordinates.
            vertices[k].color[0] = (GLfloat) i * du;
            vertices[k].color[1] = (GLfloat) j * dv;
            vertices[k].color[2] = 0.0f;

            // Vertex uv-coordinates lie in the unit square.
            vertices[k].texcoord[0] = (GLfloat) i * du;
            vertices[k].texcoord[1] = (GLfloat) j * dv;
        }
    }

    // Create mesh indexed index set.
    std::vector<MeshObject::Index> indices = CreateGrid(n1, n2);

    // Create mesh.
    return CreateMesh(name, vertices, indices);
}

///
/// @brief Create a mesh with (n1 * n2) vertices, mapping vertex positions onto
/// a region in a sphere with a specified radius and bounded by lower
/// (thetaLo, phiLo) and upper (thetaHi, phiHi) spherical coordinates.
///
/// Vertex coordinates are linearly interpolated between (thetaLo, phiLo) and
/// (thetaHi, phiHi) points along n1 and n2 lattice directions.
/// Vertex normals point in outward radial direction as defined by its spherical
/// coordinates.
/// Vertex colors map the texture coordinates to a (r,g)-colormap.
/// Vertex texture coordinates are linearly interpolated between (0,0) and (1,1).
///
Mesh CreateSphere(
    const std::string &name,
    size_t n1,
    size_t n2,
    GLfloat radius,
    GLfloat thetaLo,
    GLfloat thetaHi,
    GLfloat phiLo,
    GLfloat phiHi)
{
    ThrowIfNot(n1 > 1 && n2 > 1);
    ThrowIfNot(radius > 0.0);
    ThrowIfNot(thetaLo < thetaHi);
    ThrowIfNot(phiLo < phiHi);

    // Create mesh vertices.
    std::vector<MeshObject::Vertex> vertices(n1 * n2);

    GLfloat dtheta = (thetaHi - thetaLo) / (GLfloat) (n2 - 1);
    GLfloat dphi = (phiHi - phiLo) / (GLfloat) (n1 - 1);

    GLfloat du = 1.0f / (GLfloat) (n1 - 1);
    GLfloat dv = 1.0f / (GLfloat) (n2 - 1);

    for (size_t j = 0; j < n2; ++j) {
        for (size_t i = 0; i < n1; ++i) {
            size_t k = i + j * n1;

            // Compute the spherical coordinates
            GLfloat theta = thetaHi - (GLfloat) j * dtheta;
            GLfloat phi = phiLo + (GLfloat) i * dphi;

            // Vertex positions are just the normal scaled by the radius.
            vertices[k].position[0] = radius * std::sin(theta) * std::cos(phi);
            vertices[k].position[1] = radius * std::sin(theta) * std::sin(phi);
            vertices[k].position[2] = radius * std::cos(theta);

            // Vertex normals point in the z-direction by default.
            vertices[k].normal[0] = std::sin(theta) * std::cos(phi);
            vertices[k].normal[1] = std::sin(theta) * std::sin(phi);
            vertices[k].normal[2] = std::cos(theta);

            // Encode the vertex colors with their uv-coordinates.
            vertices[k].color[0] = (GLfloat) i * du;
            vertices[k].color[1] = (GLfloat) j * dv;
            vertices[k].color[2] = 0.0f;

            // Vertex uv-coordinates lie in the unit square.
            vertices[k].texcoord[0] = (GLfloat) i * du;
            vertices[k].texcoord[1] = (GLfloat) j * dv;
        }
    }

    // Create mesh indexed index set.
    std::vector<MeshObject::Index> indices = CreateGrid(n1, n2);

    // Create mesh.
    return CreateMesh(name, vertices, indices);
}

///
/// @brief Load the vertex and index data from a obj file. If successful,
/// retrieve all vertices and faces from the loaded data.
///
Mesh LoadMesh(const std::string &name, const std::string &filename)
{
    // Load model data from obj file. If triangulate flag is true, the number of
    // vertices will be 3 for all indices in the mesh.
    tinyobj::ObjReaderConfig reader_config;
    reader_config.triangulate = true;
    reader_config.triangulation_method = "simple";
    reader_config.vertex_color = true;
    reader_config.mtl_search_path = "";

    tinyobj::ObjReader reader;
    bool ret = reader.ParseFromFile(filename, reader_config);

    if (!reader.Error().empty()) {
        std::cerr << "Load reader error: " << reader.Error() << "\n";
    }

    if (!reader.Warning().empty()) {
        std::cerr << "Load reader warning: " << reader.Warning() << "\n";
    }

    ThrowIfNot(ret);

    // Retrieve all vertices and indices from model data. Iterate over all the
    // shapes and combine all of the faces iinto a single mesh data structure.
    // The Hashmap indexmap maintains a collection of vertex indices keyed by
    // the vertex attributes.
    // Everytime a vertex is read from the data, query the index map and check
    // if it already contains a vertex with the exact same position, normal,
    // color and texcoord. If not, add a new index to the map, keyed by the
    // attributes of the new vertex.
    // After that, get the vertex unique index from the map and store it in the
    // mesh indices array.
    // The magic constant 0x9e3779b9 is the integral part of the Golden Ratio's
    // fractional part (sqrt(5)-1)/2, multiplied by 2^32.
    auto hash_combine = [] (size_t seed, size_t hash) -> size_t {
        hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return (seed ^ hash);
    };

    auto hash = [&hash_combine] (const MeshObject::Vertex &vertex) -> size_t {
        std::hash<GLfloat> hasher;
        size_t seed = 0;

        seed = hash_combine(seed, hasher(vertex.position[0]));
        seed = hash_combine(seed, hasher(vertex.position[1]));
        seed = hash_combine(seed, hasher(vertex.position[2]));

        seed = hash_combine(seed, hasher(vertex.normal[0]));
        seed = hash_combine(seed, hasher(vertex.normal[1]));
        seed = hash_combine(seed, hasher(vertex.normal[2]));

        seed = hash_combine(seed, hasher(vertex.color[0]));
        seed = hash_combine(seed, hasher(vertex.color[1]));
        seed = hash_combine(seed, hasher(vertex.color[2]));

        seed = hash_combine(seed, hasher(vertex.texcoord[0]));
        seed = hash_combine(seed, hasher(vertex.texcoord[1]));

        return seed;
    };

    auto comp = [] (const MeshObject::Vertex &lhs, const MeshObject::Vertex &rhs) -> bool {
        return (lhs.position[0] == rhs.position[0] &&
                lhs.position[1] == rhs.position[1] &&
                lhs.position[2] == rhs.position[2] &&
                lhs.normal[0]   == rhs.normal[0]   &&
                lhs.normal[1]   == rhs.normal[1]   &&
                lhs.normal[2]   == rhs.normal[2]   &&
                lhs.color[0]    == rhs.color[0]    &&
                lhs.color[1]    == rhs.color[1]    &&
                lhs.color[2]    == rhs.color[2]    &&
                lhs.texcoord[0] == rhs.texcoord[0] &&
                lhs.texcoord[1] == rhs.texcoord[1]);
    };

    std::unordered_map<
        MeshObject::Vertex,
        GLuint,
        decltype(hash),
        decltype(comp)> indexmap(3, hash, comp);

    const tinyobj::attrib_t &attrib = reader.GetAttrib();
    const std::vector<tinyobj::shape_t> &shapes = reader.GetShapes();

    std::vector<MeshObject::Vertex> vertices;
    std::vector<MeshObject::Index> indices;

    for (const auto &shape : shapes) {
        for (const auto &index : shape.mesh.indices) {
            MeshObject::Vertex vertex{};

            // Vertex position and color data.
            {
                size_t vertex_offset = 3 * index.vertex_index;

                vertex.position[0] = attrib.vertices[vertex_offset + 0];
                vertex.position[1] = attrib.vertices[vertex_offset + 1];
                vertex.position[2] = attrib.vertices[vertex_offset + 2];

                vertex.color[0] = attrib.colors[vertex_offset + 0];
                vertex.color[1] = attrib.colors[vertex_offset + 1];
                vertex.color[2] = attrib.colors[vertex_offset + 2];
            }

            // If normal_index is negative, data contains no normals.
            if (index.normal_index >= 0) {
                size_t normal_offset = 3 * index.normal_index;

                vertex.normal[0] = attrib.normals[normal_offset + 0];
                vertex.normal[1] = attrib.normals[normal_offset + 1];
                vertex.normal[2] = attrib.normals[normal_offset + 2];
            }

            // If texcoord_index is negative, data contains no texcoords.
            if (index.texcoord_index >= 0) {
                size_t texcoord_offset = 2 * index.texcoord_index;

                vertex.texcoord[0] = attrib.texcoords[texcoord_offset + 0];
                vertex.texcoord[1] = attrib.texcoords[texcoord_offset + 1];
            }

            // Add the new index keyed by the vertex attributes if the map does
            // not contain the loaded vertex.
            if (indexmap.count(vertex) == 0) {
                indexmap[vertex] = static_cast<GLuint>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(indexmap[vertex]);
        }
    }

    // Create mesh.
    return CreateMesh(name, vertices, indices);
}

} // namespace Graphics
