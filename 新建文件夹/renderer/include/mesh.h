#ifndef MESH_H
#define MESH_H
#include <common.h>
#include <etypes.h>
struct _mesh;
typedef struct mesh_
{
    struct _mesh* self;
} Mesh;
extern Mesh Mesh_new();
extern void Mesh_delete(Mesh _mesh);
extern void Mesh_Dest(struct _mesh* _mesh);
extern void Mesh_load_form_file(Mesh _mesh, const char* _file_name);
extern float* Mesh_get_position(Mesh _mesh);
extern float* Mesh_get_tex_coord(Mesh _mesh);
extern float* Mesh_get_normal(Mesh _mesh);
extern float* Mesh_get_tangent(Mesh _mesh);
extern float* Mesh_get_binormal(Mesh _mesh);
extern uint32* Mesh_get_index(Mesh _mesh);
extern uint32 Mesh_get_vertex_count(Mesh _mesh);
extern uint32 Mesh_get_face_count(Mesh _mesh);
#endif
