//
// mesh.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef GRAPHICS_MESH_H_
#define GRAPHICS_MESH_H_

#include <string>
#include <vector>
#include "core/graphics/common.h"

namespace Graphics {

///
/// @brief Mesh maintains a shared-vertex representation of a mesh. It consists
/// of a collection of vertices and a list of indices representing the faces in
/// the mesh. Vertices have position, normal, color and texture coordinates.
/// Edges are implicit in the sequence of vertex indices composing the face.
///
/// @par The mesh data structure maintains the mesh data:
///  - attributes: positions, normals, color and texture coordinates.
///  - connectivity: adjacency relationships between vertices.
///
/// The data structure may optionally support a set of functions on the
/// data - rendering, geometry queries, and manipulation (remove or add a vertex,
/// vertex split, edge collapse, etc).
///
/// @par Shared-vertex representations are compact and allow constant time
/// complexity O(1) lookups of the vertices of a given face.
///
/// Indexed rendering requires a list of vertices of each face to be transmitted
/// to the GPU. The vertices are stored in a vertex buffer object holding the
/// position/color/normal structures.
///
/// Changes in shape, but not geometry, can be dynamically updated by simply
/// resending the vertex data without updating the face connectivity.
///
/// Shared-vertex representations are and encode little information
/// regarding the structure of the mesh - traversals and mesh manipulation
/// are inefficient.
///
/// @par Half-edge representations model explicitly the vertices, edges, and
/// faces of the mesh with additional adjacency information. Mesh manipulation
/// is more efficient - edge collapsing, mesh traversal, adjacency queries:
///  - which faces use this vertex?
///  - which edges use this vertex?
///  - which faces border this edge?
///  - which edges border this face?
///  - which faces are adjacent to this face?
///
/// @see http://paulbourke.net/dataformats/
///      https://www.redblobgames.com/x/1722-b-rep-triangle-meshes
///      https://en.wikipedia.org/wiki/Polygon_mesh
///      https://www.flipcode.com/archives/The_Half-Edge_Data_Structure.shtml
///      https://towardsdatascience.com/mesh-data-structure-d8b1a61d749e
///      https://martindevans.me/game-development/2016/03/30/
///          Procedural-Generation-For-Dummies-Half-Edge-Geometry
///
struct Mesh {
    // Vertex holds the geometry and attributes of a vertex in the mesh.
    struct Vertex {
        GLfloat position[3];
        GLfloat normal[3];
        GLfloat color[3];
        GLfloat texcoord[2];
    };

    // Index is a type alias to GLuint, used by GL_ELEMENT_ARRAY_BUFFER.
    typedef GLuint Index;

    // Mesh member variables.
    std::string name;                   // mesh name
    std::vector<Vertex> vertices;       // vertex list
    std::vector<Index> indices;         // face indexed list
    std::vector<VertexAttributeDescription> attributes; // vertex attributes
    GLuint vbo;                         // vertex buffer object
    GLuint ebo;                         // element buffer object
};

// Create an indexed grid with (n1 * n2) vertices.
std::vector<Mesh::Index> CreateGrid(const size_t n1, const size_t n2);

// Create a mesh.
Mesh CreateMesh(
    const std::string &name,
    const std::vector<Mesh::Vertex> &vertices,
    const std::vector<Mesh::Index> &indices);

// Destroy a mesh.
void DestroyMesh(Mesh &mesh);

// Upload mesh vertex data onto the gpu.
void UpdateMesh(const Mesh &mesh);

// Render the mesh.
void RenderMesh(const Mesh &mesh);

// Create a plane mesh with (n1 * n2) vertices.
Mesh CreatePlane(
    const std::string &name,
    size_t n1,
    size_t n2,
    GLfloat xlo,
    GLfloat xhi,
    GLfloat ylo,
    GLfloat yhi);

// Create a sphere mesh with (n1 * n2) vertices.
Mesh CreateSphere(
    const std::string &name,
    size_t n1,
    size_t n2,
    GLfloat radius,
    GLfloat theta_lo,
    GLfloat theta_hi,
    GLfloat phi_lo,
    GLfloat phi_hi);

// Load the model meshes from a specified filename.
Mesh LoadMesh(const std::string &name, const std::string &filename);

} // Graphics

#endif // GRAPHICS_MESH_H_
