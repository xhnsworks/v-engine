#ifndef RENDERABLE_BASE_H
#define RENDERABLE_BASE_H
#include "common.h"
#include "etypes.h"
#include "vertex_declaration.h"
#include "material_instance.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "renderer.h"
#include "matrix4x4.h"
#include "pass.h"
#include "list.h"
typedef struct _renderable
{
    VertexDecl vtx_dec;
    VertexBuffer vtx_buf;
    IndexBuffer idx_buf;
    List mesh_list;
    Pass std_pass;
    Pass depth_pass;
    Pass shadow_pass;
    ///uint32 tri_cnt;
    MaterialInstance material;
    ///int material_id;
    Matrix4x4 world_matrix;
    Renderer ower;
    bool std_pass_dirty_flag;
    bool depth_pass_dirty_flag;
    bool shadow_pass_dirty_flag;
} renderable;
#endif
