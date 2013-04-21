#ifndef MESH_TRIANGLE_H
#define MESH_TRIANGLE_H
#include "common.h"
#include "etypes.h"
#include "array.h"
#include "mesh.h"
#include "mesh_edge.h"

API_EXPORT MeshTriangleArray MeshTriangleArray_new_from_mesh(Mesh m);
API_EXPORT void MeshTriangleArray_delete(MeshTriangleArray self);

API_EXPORT Tree MeshTriangleArray_make_edge_triangle_tree(MeshTriangleArray self);

API_EXPORT void MeshTriangle_get_edges(MeshTriangle self, MeshEdge* edge0, MeshEdge* edge1, MeshEdge* edge2);

#endif
