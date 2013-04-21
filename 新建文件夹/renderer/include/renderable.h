#ifndef RENDERABLE_H
#define RENDERABLE_H
#include "common.h"
#include "etypes.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "mesh.h"
#include "matrix4x4.h"
#include "pass.h"
struct _renderable;
typedef struct renderable_
{
    struct _renderable* self;
} Renderable;
extern Renderable Renderable_new(void);
extern void Renderable_add_mesh(Renderable _rabl, Mesh _mesh);
extern VertexBuffer Renderable_get_vertex_buffer(Renderable _rabl);
extern IndexBuffer Renderable_get_index_buffer(Renderable _rabl);
extern Matrix4x4 Renderable_get_world_matrix(Renderable _rabl);
extern uint32 Renderable_get_vertex_count(Renderable _rabl);
extern uint32 Renderable_get_index_count(Renderable _rabl);
extern uint32 Renderable_get_draw_count(Renderable _rabl);
extern Pass Renderable_get_begin_pass(Renderable _rabl);
extern Pass Renderable_get_end_pass(Renderable _rabl);
#endif
