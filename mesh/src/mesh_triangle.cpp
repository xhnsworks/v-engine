#include "mesh_triangle.h"
#include "mesh_base.h"

static inline void _get_edge_key(MeshEdge e, MeshEdge key)
{
    if (e->vtx0 < e->vtx1)
    {
        key->vtx0 = e->vtx0;
        key->vtx1 = e->vtx1;
    }
    else
    {
        key->vtx0 = e->vtx1;
        key->vtx1 = e->vtx0;
    }
}

MeshTriangleArray MeshTriangleArray_new_from_mesh(Mesh m)
{
    if (_Mesh_get_data_0005(m)->mesh_mode != Triangular)
        return NULL;
    struct version_0004_data* data = _Mesh_get_data_0004(m);

    MeshTriangleArray ret = array_new(MeshTriangle, data->face_count, NULL);
    for (euint32 i = 0; i < data->face_count; i++)
    {
        MeshTriangle tri = (MeshTriangle)SMalloc(sizeof(mesh_triangle));
        tri->edge0.vtx0 = data->indexs[i * 3 + 0];
        tri->edge0.vtx1 = data->indexs[i * 3 + 1];

        tri->edge1.vtx0 = data->indexs[i * 3 + 1];
        tri->edge1.vtx1 = data->indexs[i * 3 + 2];

        tri->edge2.vtx0 = data->indexs[i * 3 + 2];
        tri->edge2.vtx1 = data->indexs[i * 3 + 0];

        _get_edge_key(&tri->edge0, &tri->edge0_key);
        _get_edge_key(&tri->edge1, &tri->edge1_key);
        _get_edge_key(&tri->edge2, &tri->edge2_key);

        apush(ret, tri);
    }
    return ret;
}

void MeshTriangleArray_delete(MeshTriangleArray self)
{
    euint count = array_n(self);
    for (euint i = 0; i < count; i++)
    {
        Mfree(self[i]);
    }
    array_delete(self);
}

Tree MeshTriangleArray_make_edge_triangle_tree(MeshTriangleArray self)
{
    Tree ret = MeshEdgeTriangleTree_new();
    MeshEdge edge0;
    MeshEdge edge1;
    MeshEdge edge2;
    euint count = array_n(self);
    for (euint i = 0; i < count; i++)
    {
        MeshTriangle tri = self[i];
        MeshTriangle_get_edges(tri, &edge0, &edge1, &edge2);
        MeshEdgeTriangleTree_insert(ret, edge0, tri);
        MeshEdgeTriangleTree_insert(ret, edge1, tri);
        MeshEdgeTriangleTree_insert(ret, edge2, tri);
    }
    return ret;
}

void MeshTriangle_get_edges(MeshTriangle self, MeshEdge* edge0, MeshEdge* edge1, MeshEdge* edge2)
{
    *edge0 = &self->edge0_key;
    *edge1 = &self->edge1_key;
    *edge2 = &self->edge2_key;
}
