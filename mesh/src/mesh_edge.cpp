#include "mesh_edge.h"
#include "mesh_base.h"
#include "emem.h"
#include "array.h"

#include "elog.h"
int _mesh_edge_compare(vptr a, vptr b)
{
    MeshEdge e0 = (MeshEdge)a;
    MeshEdge e1 = (MeshEdge)b;

    elog("##e0 %d %d e1 %d %d", e0->vtx0, e0->vtx1, e1->vtx0, e1->vtx1);

    if      (e0->vtx0 < e1->vtx0)
        return -1;
    else if (e0->vtx0 > e1->vtx0)
        return 1;
    else
    {
        if      (e0->vtx1 < e1->vtx1)
            return -1;
        else if (e0->vtx1 > e1->vtx1)
            return 1;
        else
            return 0;
    }
}

Tree MeshEdgeTriangleTree_new()
{
    Tree ret = Tree_new(Vptr, Vptr, (MALLOC)Ealloc, (MFREE)Efree);
    Tree_set_key_compare_proc(ret, _mesh_edge_compare);
    return ret;
}

void MeshEdgeTriangleTree_delete(Tree self)
{
    Iterator iter = Tree_begin(self);
    while (iter)
    {
        var data = Tree_get_value(iter);
		MeshTriangleArray tri_array = (MeshTriangleArray)data.vptr_var;
        array_delete(tri_array);
        iter = Tree_next(iter);
    }
    Tree_Dest(self);
}

void MeshEdgeTriangleTree_insert(Tree self, MeshEdge e, MeshTriangle t)
{
    var key, data;
    MeshTriangleArray tri_array = NULL;
    key.vptr_var = e;
    Iterator iter = Tree_force_find(self, key);
    data = Tree_get_value(iter);
    if (!data.vptr_var) { tri_array = array_new(MeshTriangle, 5, NULL); }
	else                { tri_array = (MeshTriangleArray)data.vptr_var; }
    for (uint i = 0; i < array_n(tri_array); i++)
        if (t == tri_array[i])
            return;
    apush(tri_array, t);
    data.vptr_var = tri_array;
    Tree_set_value(iter, data);
}

MeshTriangleArray MeshEdgeTriangleTree_find_triangles(Tree self, MeshEdge e)
{
    var key, data;
    MeshTriangleArray tri_array = NULL;
    key.vptr_var = e;
    Iterator iter = Tree_force_find(self, key);
    data = Tree_get_value(iter);
    if (!data.vptr_var) { tri_array = array_new(MeshTriangle, 5, NULL); }
    else                { tri_array = (MeshTriangleArray)data.vptr_var; }
    return tri_array;
}

void MeshEdge_get_vertexs(MeshEdge self, uint32* vtx0, uint32* vtx1)
{
    *vtx0 = self->vtx0;
    *vtx1 = self->vtx1;
}
