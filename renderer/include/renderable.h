/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：rendeable.h
* 文件描述：可渲染物体对象
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef RENDERABLE_H
#define RENDERABLE_H
#include "common.h"
#include "etypes.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "mesh.h"
#include "matrix4x4.h"
#include "pass.h"
#include "vertex_declaration.h"
#include "material_instance.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "emem.hpp"
#include "xhn_list.hpp"
#include <list>
typedef enum _renderable_proc_result
{
    proc_continue,
    proc_end,
}renderable_proc_result;

class renderable : public MemObject
{
	friend class RendererBase;
public:
    VertexDecl vtx_dec;
    VertexBuffer vtx_buf;
    IndexBuffer idx_buf;
    ///List mesh_list;
	xhn::list<MeshPtr> mesh_list;
    Pass std_pass;
    Pass depth_pass;
    Pass point_depth_pass;
    Pass shadow_pass;
    ///euint32 tri_cnt;
    MaterialInstance material;
    ///int material_id;
    matrix4x4* world_matrix;
    Renderer* ower;
    bool std_pass_dirty_flag;
    bool depth_pass_dirty_flag;
    bool shadow_pass_dirty_flag;
private:
	renderable();
	~renderable();
};

typedef renderable_proc_result (*fn_renderable_proc)(matrix4x4*, float*, float*, float*);
typedef renderable* Renderable;
API_EXPORT void Renderable_add_mesh(Renderable _rabl, MeshPtr _mesh);
API_EXPORT matrix4x4* Renderable_get_world_matrix(Renderable _rabl);
API_EXPORT void Renderable_clear(Renderable _rabl);
API_EXPORT void Renderable_prev_render(Renderable _rabl);
API_EXPORT void Renderable_depth_prev_render(Renderable _rabl);
///API_EXPORT void Renderable_shadow_prev_render(Renderable _rabl);

API_EXPORT void Renderable_process(Renderable _rabl, fn_renderable_proc _proc);
#endif
