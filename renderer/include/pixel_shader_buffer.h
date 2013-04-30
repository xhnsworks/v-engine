#ifndef PIXEL_SHADER_BUFFER_H
#define PIXEL_SHADER_BUFFER_H
#include "common.h"
#include "etypes.h"
/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：pixel_shader_buffer.h
* 文件描述：像素着色器的缓冲对象，用于生成像素着色器源字符串
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#include "shader_object.h"
#include "shader_buffer.h"
struct _pixel_shader_buffer;
/**
typedef struct pixel_shader_buffer_
{
    struct _pixel_shader_buffer* self;
} PxlSdrBuf;
**/
class Renderer;
typedef struct _pixel_shader_buffer* PxlSdrBuf;
API_EXPORT PxlSdrBuf _PxlSdrBuf_new(const char* _file, euint _line);
API_EXPORT void _PxlSdrBuf_delete(PxlSdrBuf _psb, const char* _file, euint _line);
#define PxlSdrBuf_new() _PxlSdrBuf_new(__FILE__, __LINE__)
#define PxlSdrBuf_delete(p) _PxlSdrBuf_delete(p, __FILE__, __LINE__)
API_EXPORT void PxlSdrBuf_complete(PxlSdrBuf _psb);

struct _i_pxl_sdr_buf
{
    PxlSdrBuf (*__New)(const char* _file, euint _line);
    void (*__Delete)(PxlSdrBuf _vsb, const char* _file, euint _line);
    void (*complete)(PxlSdrBuf _vsb);

    ShaderObject (*add_varying)(ShaderBuffer _sb, param_type _type, const char* _vary, esint32 _src);
    ShaderObject (*_add_uniform)(ShaderBuffer _sb, param_type _type, const char* _unif, euint32 _array_size, esint32 _src,
                                                  const char* _file, euint _line);
    ShaderObject (*_add_uniform_from_renderer)(ShaderBuffer _self, Renderer* _rdr, esint32 _id, const char* _unif,
                                                                const char* _file, euint _line);
    ShaderObject (*new_object)(ShaderBuffer _sb, shader_object_type _type, const char* _name, euint32 _array_size);
    ShaderObject (*new_immediate_float_object)(ShaderBuffer _sb, float _ft);
    ShaderObject (*new_immediate_int_object)(ShaderBuffer _sb, int _i);
	ShaderObject (*sample_texture2d_rgba)(ShaderBuffer _sb, ShaderObject _tex, ShaderObject _uv);
	ShaderObject (*sample_texture2d_rgb)(ShaderBuffer _sb, ShaderObject _tex, ShaderObject _uv);
    void (*add_prototype_node)(ShaderBuffer _sb, ShaderNode _sn);
    ShaderNode (*_add_reference_node)(ShaderBuffer _sb, const char* _name, const char* _file, euint _line);
    void (*_add_branch_node)(ShaderBuffer _sb, BranchNode _bn, const char* _file, euint _line);
    ShaderObject (*find_object)(ShaderBuffer _sb, const char* _name);
    void (*print_shader_object)(ShaderBuffer _sb);
    void (*push_shader_object)(ShaderBuffer _sb, ShaderObject _so);
    void (*push_immediate_shader_object)(ShaderBuffer _sb, ShaderObject _so);
    Tree (*sell_param_source_object_tree)(ShaderBuffer _sb);
    const char* (*get_output)(ShaderBuffer _sb);
};

static struct _i_pxl_sdr_buf IPxlSdrBuf = {
    _PxlSdrBuf_new,
    _PxlSdrBuf_delete,
    PxlSdrBuf_complete,

    ShaderBuffer_add_varying,
    _ShaderBuffer_add_uniform,
    _ShaderBuffer_add_uniform_from_renderer,
    ShaderBuffer_new_object,
    ShaderBuffer_new_immediate_float_object,
    ShaderBuffer_new_immediate_int_object,
	ShaderBuffer_sample_texture2d_rgba,
	ShaderBuffer_sample_texture2d_rgb,
    ShaderBuffer_add_prototype_node,
    _ShaderBuffer_add_reference_node,
    _ShaderBuffer_add_branch_node,
    ShaderBuffer_find_object,
    ShaderBuffer_print_shader_object,
    ShaderBuffer_push_shader_object,
    ShaderBuffer_push_immediate_shader_object,
    ShaderBuffer_sell_param_source_object_tree,
    ShaderBuffer_get_output,
};
#endif
