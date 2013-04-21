#include "renderable.h"
#include "array.h"
#include "matrix4x4.h"
#include "pass.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"
#include "vertex_declaration.h"
#include "shader_node.h"
#include "pass_console.h"
#include "shader_buffer.h"
#include "renderable_base.h"
#include "mesh_base.h"

void Renderable_add_mesh(Renderable _rabl, Mesh _mesh)
{
    VertexBuffer_attach_mesh(_rabl->vtx_buf, _mesh);
    IndexBuffer_attach_mesh(_rabl->idx_buf, _mesh);
    var data;
    data.vptr_var = _mesh;
    List_push_back(_rabl->mesh_list, data);
    _mesh->ref_count++;
}

void Renderable_process(Renderable _rabl, fn_renderable_proc _proc)
{
    uint face_count = IndexBuffer_get_num_faces(_rabl->idx_buf);
    for (uint i = 0; i < face_count; i++)
    {
        uint idxs[3];
        IndexBuffer_read(_rabl->idx_buf, i, idxs);
        float pos0[3];
        float pos1[3];
        float pos2[3];
        VertexBuffer_read(_rabl->vtx_buf, Position, idxs[0], pos0);
        VertexBuffer_read(_rabl->vtx_buf, Position, idxs[1], pos1);
        VertexBuffer_read(_rabl->vtx_buf, Position, idxs[2], pos2);
        renderable_proc_result ret = _proc(_rabl->world_matrix, pos0, pos1, pos2);
        if (ret == proc_end)
            break;
    }
}
Matrix4x4 Renderable_get_world_matrix(Renderable _rabl)
{
    return _rabl->world_matrix;
}
void Renderable_clear(Renderable _rabl)
{
    ///VertexBuffer_reset(_rabl->vtx_buf);
    ///IndexBuffer_reset(_rabl->idx_buf);

    VertexDecl decl = VertexBuffer_get_vertex_declaration(_rabl->vtx_buf);
    VertexBuffer vbf = VertexBuffer_new(decl);
    VertexBuffer_delete(_rabl->vtx_buf);
    _rabl->vtx_buf = vbf;
    ///
    mesh_mode mode = IndexBuffer_get_mesh_mode(_rabl->idx_buf);
    IndexBuffer ibf = IndexBuffer_new(mode);
    IndexBuffer_delete(_rabl->idx_buf);
    _rabl->idx_buf = ibf;

    Iterator iter = List_begin(_rabl->mesh_list);
    while (iter)
    {
        var data = List_get_value(iter);
        Mesh mesh = data.vptr_var;
        Mesh_delete(mesh);
        iter = List_next(iter);
    }
    List_Dest(_rabl->mesh_list);
    _rabl->mesh_list = List_new(Vptr, Ealloc, Efree);
}

void Renderable_prev_render(Renderable _rabl)
{
    if (_rabl->std_pass_dirty_flag)
    {
        uint usage_tex_flags = 0;

        if (_rabl->material->col_tex)
            usage_tex_flags |= COLOR_MAP_MASK;
        if (_rabl->material->nor_tex)
            usage_tex_flags |= NORMAL_MAP_MASK;

        pass_decl p_dec;
        p_dec.dec = _rabl->vtx_dec;
        p_dec.usage_tex_flags = usage_tex_flags;
        ///p_dec.use_mat_id = true;

        _rabl->std_pass = RenderSystem_register_std_pass(&p_dec);
        _rabl->std_pass_dirty_flag = false;
    }
}

void Renderable_depth_prev_render(Renderable _rabl)
{
    if (_rabl->depth_pass_dirty_flag)
    {
        uint usage_tex_flags = 0;

        pass_decl p_dec;
        p_dec.dec = _rabl->vtx_dec;
        p_dec.usage_tex_flags = 0;
        ///p_dec.use_mat_id = false;

        _rabl->depth_pass = RenderSystem_register_depth_pass(&p_dec);
        _rabl->depth_pass_dirty_flag = false;
    }
}
/**
void Renderable_shadow_prev_render(Renderable _rabl)
{
    if (_rabl->shadow_pass_dirty_flag)
    {
        _rabl->shadow_pass = RenderSystem_register_shadow_pass(_rabl->vtx_dec);
        _rabl->shadow_pass_dirty_flag = false;
    }
}
**/
