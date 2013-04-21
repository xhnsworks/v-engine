#ifndef PASS_H
#define PASS_H
#include "common.h"
#include "etypes.h"
#include "shader.h"
#include "texture2d.h"
#include "matrix4x4.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "sketch.h"

struct _pass;
typedef struct pass_
{
    struct _pass* self;
} Pass;

extern Pass Pass_new(void);
extern void Pass_delete(Pass _self);
extern void Pass_set_vertex_shader(Pass _self, Shader _sdr);
extern void Pass_set_pixel_shader(Pass _self, Shader _sdr);

extern void Pass_render(Pass _self, VertexBuffer _vbf, IndexBuffer _ibf, uint32 _num_idxs);
extern void Pass_render_plane(Pass _self, RenderablePlane _pln);
extern void Pass_set_uniform_param_tex2d(Pass _self, const char* _name, Texture2D _tex);
extern void Pass_set_uniform_param_float(Pass _self, const char* _name, EFloat _ft);
extern void Pass_set_uniform_param_float3(Pass _self, const char* _name, EFloat3 _ft3);
extern void Pass_set_uniform_param_float4(Pass _self, const char* _name, EFloat4 _ft4);
extern void Pass_set_uniform_param_mat4x4(Pass _self, const char* _name, Matrix4x4 _mat);
extern void Pass_set_uniform_param_int(Pass _self, const char* _name, int _n);

extern void Pass_set_uniform_param_floats(Pass _self, const char* _name, EFloat* _fts, uint _cnt);
extern void Pass_set_uniform_param_float3s(Pass _self, const char* _name, EFloat3* _ft3s, uint _cnt);
extern void Pass_set_uniform_param_float4s(Pass _self, const char* _name, EFloat4* _ft4s, uint _cnt);
extern void Pass_set_uniform_param_mat4x4s(Pass _self, const char* _name, Matrix4x4* _mats, uint _cnt);
#endif
