/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：pass.h
* 文件描述：pass对象，一个完整pass包含顶点着色器和像素着色器两部分
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef PASS_H
#define PASS_H
#include "common.h"
#include "etypes.h"
#include "tree.h"
#include "shader.h"
#include "texture2d.h"
#include "matrix4x4.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "sketch.h"

typedef struct _pass* Pass;
class RendererBase;
API_EXPORT Pass Pass_new(void);
API_EXPORT void Pass_delete(Pass _self);
API_EXPORT void Pass_set_vertex_shader(Pass _self, Shader _sdr);
API_EXPORT void Pass_set_pixel_shader(Pass _self, Shader _sdr);

API_EXPORT void _Pass_render(Pass _self, VertexBuffer _vbf, IndexBuffer _ibf, euint32 _num_idxs, e_mesh_mode _mode, const char* _file, euint _line);
#define Pass_render(s, v, i, n, m) _Pass_render(s, v, i, n, m, __FILE__, __LINE__)
API_EXPORT void _Pass_render_plane(Pass _self, RenderablePlane _pln, const char* _file, euint _line);
#define Pass_render_plane(s, p) _Pass_render_plane(s, p, __FILE__, __LINE__)
API_EXPORT void Pass_set_uniform_param_tex2d(Pass _self, const char* _name, Texture2DPtr _tex);
API_EXPORT void Pass_set_uniform_param_texcube(Pass _self, const char* _name, TextureCube _tex);
API_EXPORT void Pass_set_uniform_param_float(Pass _self, const char* _name, EFloat _ft);
API_EXPORT void Pass_set_uniform_param_float2(Pass _self, const char* _name, EFloat2 _ft2);
API_EXPORT void Pass_set_uniform_param_float3(Pass _self, const char* _name, EFloat3 _ft3);
API_EXPORT void Pass_set_uniform_param_float4(Pass _self, const char* _name, EFloat4 _ft4);
API_EXPORT void Pass_set_uniform_param_mat4x4(Pass _self, const char* _name, matrix4x4* _mat);
API_EXPORT void Pass_set_uniform_param_int(Pass _self, const char* _name, int _n);

API_EXPORT void Pass_set_uniform_param_ints(Pass _self, const char* _name, int* _ns, euint32 _cnt);
API_EXPORT void Pass_set_uniform_param_floats(Pass _self, const char* _name, EFloat* _fts, euint32 _cnt);
API_EXPORT void Pass_set_uniform_param_float2s(Pass _self, const char* _name, EFloat2* _ft2s, euint32 _cnt);
API_EXPORT void Pass_set_uniform_param_float3s(Pass _self, const char* _name, EFloat3* _ft3s, euint32 _cnt);
API_EXPORT void Pass_set_uniform_param_float4s(Pass _self, const char* _name, EFloat4* _ft4s, euint32 _cnt);
API_EXPORT void Pass_set_uniform_param_mat4x4s(Pass _self, const char* _name, matrix4x4** _mats, euint32 _cnt);

API_EXPORT void Pass_buy_vertex_param_source_tree(Pass _self, Tree _vtx_param_src_tree);
API_EXPORT void Pass_buy_pixel_param_source_tree(Pass _self, Tree _pxl_param_src_tree);
API_EXPORT void Pass_auto_set_uniform_params(Pass _self, RendererBase* _rdr, bool _write_log);
#endif
