#ifndef RENDERABLE_H
#define RENDERABLE_H
#include "common.h"
#include "etypes.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "mesh.h"
#include "matrix4x4.h"
#include "pass.h"
typedef enum _renderable_proc_result
{
    proc_continue,
    proc_end,
}renderable_proc_result;

typedef renderable_proc_result (*fn_renderable_proc)(Matrix4x4, float*, float*, float*);
typedef struct _renderable* Renderable;
API_EXPORT void Renderable_add_mesh(Renderable _rabl, Mesh _mesh);
API_EXPORT Matrix4x4 Renderable_get_world_matrix(Renderable _rabl);
API_EXPORT void Renderable_clear(Renderable _rabl);
API_EXPORT void Renderable_prev_render(Renderable _rabl);
API_EXPORT void Renderable_depth_prev_render(Renderable _rabl);
///API_EXPORT void Renderable_shadow_prev_render(Renderable _rabl);

API_EXPORT void Renderable_process(Renderable _rabl, fn_renderable_proc _proc);
#endif
