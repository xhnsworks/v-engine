#ifndef MESH_H
#define MESH_H
#include "common.h"
#include "etypes.h"
#include "version_0004_data.h"
#include "version_0005_data.h"
#include "version_0006_data.h"
#include "version_0007_data.h"

#include "xhn_smart_ptr.hpp"
#include "emem.hpp"

class mesh : public RefObject
{
public:
    euint32 version;
    const char* file;
    euint line;
    struct version_0007_data data;
	mesh(const char* _file, euint _line);
	~mesh();
};
typedef mesh* Mesh;
_INLINE_ struct version_0004_data* _Mesh_get_data_0004(Mesh _mesh)
{
    return &_mesh->data.base_data.base_data.base_data;
}
_INLINE_ struct version_0005_data* _Mesh_get_data_0005(Mesh _mesh)
{
    return &_mesh->data.base_data.base_data;
}
_INLINE_ struct version_0006_data* _Mesh_get_data_0006(Mesh _mesh)
{
    return &_mesh->data.base_data;
}
_INLINE_ struct version_0007_data* _Mesh_get_data_0007(Mesh _mesh)
{
    return &_mesh->data;
}

typedef struct _mesh_edge
{
    euint32 vtx0;
    euint32 vtx1;
} mesh_edge;

/// 每个三角形包括三条边，每条边包括key和非key两个部分
/// 作为key的边是为了从Tree里查询的key，是小序号在前，大序号在后的顺序排列的
typedef struct _mesh_triangle
{
    mesh_edge edge0_key;
    mesh_edge edge0;
    mesh_edge edge1_key;
    mesh_edge edge1;
    mesh_edge edge2_key;
    mesh_edge edge2;
} mesh_triangle;

///API_EXPORT Mesh _Mesh_new(const char* _file, euint _line);
#define Mesh_new() ENEW mesh(__FILE__, __LINE__)
/**
API_EXPORT void Mesh_delete(Mesh _mesh);
**/
///API_EXPORT void Mesh_Dest(struct _mesh* _mesh);
API_EXPORT void Mesh_load_form_file(Mesh _mesh, const char* _file_name);
API_EXPORT float* Mesh_get_position(Mesh _mesh);
API_EXPORT float* Mesh_get_tex_coord(Mesh _mesh);
API_EXPORT float* Mesh_get_normal(Mesh _mesh);
API_EXPORT float* Mesh_get_tangent(Mesh _mesh);
API_EXPORT float* Mesh_get_binormal(Mesh _mesh);
API_EXPORT float* Mesh_get_edge_proj_weight(Mesh _mesh);
API_EXPORT float* Mesh_get_color(Mesh _mesh);
API_EXPORT euint32* Mesh_get_index(Mesh _mesh);
API_EXPORT euint32 Mesh_get_vertex_count(Mesh _mesh);
API_EXPORT euint32 Mesh_get_face_count(Mesh _mesh);
API_EXPORT e_mesh_mode Mesh_get_mode(Mesh _mesh);
API_EXPORT euint32 face_size(e_mesh_mode mesh_mode);
API_EXPORT void Mesh_build(Mesh _mesh,
                           float* pos_stream, float* uv_stream, float* nor_stream, euint32 num_vtxs,
                           euint32* idx_stream, euint32 num_faces,
                           e_mesh_mode mesh_mode);
API_EXPORT void Mesh_build2(Mesh _mesh,
                           float* pos_stream, float* uv_stream, float* nor_stream, float* col_stream,
                           euint32 num_vtxs,
                           euint32* idx_stream, euint32 num_faces,
                           e_mesh_mode mesh_mode);
API_EXPORT void Mesh_fill_epw(Mesh _mesh, float* _epw_stream);
API_EXPORT Mesh Mesh_build_volume(Mesh _mesh);
API_EXPORT void Mesh_print_mesh_count();

API_EXPORT void Mesh_apply_transform(Mesh _mesh, const matrix4x4* _trans);

typedef xhn::SmartPtr<mesh> MeshPtr;
#endif
