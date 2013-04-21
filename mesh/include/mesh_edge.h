#ifndef MESH_EDGE_H
#define MESH_EDGE_H
#include "common.h"
#include "etypes.h"
#include "mesh.h"
#include "tree.h"
API_EXPORT Tree MeshEdgeTriangleTree_new();
API_EXPORT void MeshEdgeTriangleTree_delete(Tree self);

API_EXPORT void MeshEdgeTriangleTree_insert(Tree self, MeshEdge e, MeshTriangle t);
API_EXPORT MeshTriangleArray MeshEdgeTriangleTree_find_triangles(Tree self, MeshEdge e);

API_EXPORT void MeshEdge_get_vertexs(MeshEdge self, uint32* vtx0, uint32* vtx1);
#endif
