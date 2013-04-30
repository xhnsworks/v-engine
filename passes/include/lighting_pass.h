/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：lighting_pass.h
* 文件描述：光照pass
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef LIGHTING_PASS_H
#define LIGHTING_PASS_H
#include "common.h"
#include "etypes.h"
#include "tree.h"
#include "shader.h"
#include "pass.h"
#include "shader_node.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"
#include "vertex_declaration.h"
#include "lighting.h"
#include "material_prototype.h"
#include "shader_node_generator.h"
#define MAX_LIGHTS_PER_PASS 1
#define POINT_LIGHT_POSITIOIN "PointLightPosition"
#define POINT_LIGHT_COLOR     "PointLightColor"
#define POINT_LIGHT_ATTE_COEF "PointLightAtteCoef"

API_EXPORT ShaderNode create_position_decode_node();
API_EXPORT ShaderNode create_normal_decode_node();
API_EXPORT ShaderNode create_to_camera_space_node();
API_EXPORT ShaderNode create_point_array_lighting_node();
API_EXPORT ShaderNode create_lighting_accumulator_node();
API_EXPORT ShaderNode create_lighting_output_node();
API_EXPORT ShaderNode create_lighting_vertex_shader_node(VertexDecl _dec);

///API_EXPORT void default_point_lighting_proc(PxlSdrBuf _pxl_sdr_buf, SdrNdGen _sdr_node_gtr, bool _has_lighting_map);
///API_EXPORT Pass create_lighting_pass_ex(SdrNdGen _sng, LightingProc _proc, VertexDecl _dec, bool _has_lighting_map);
///API_EXPORT Pass create_lighting_pass_ex2(Renderer _rdr, VertexDecl _dec, lighting_decl* _decls, bool _has_lighting_map);

typedef struct _param_entry
{
    const char* name;
    esint32 source;
} param_entry;
typedef struct _param_table
{
    Tree param_entries;
} ParamTable;

API_EXPORT ParamTable ParamTable_new();
API_EXPORT void ParamTable_delete(ParamTable _self);
API_EXPORT void ParamTable_add_entry(ParamTable _self, const char* _name, esint32 _src);
API_EXPORT void ParamTable_get_all_entrys(ParamTable _self, param_entry* _result);
#endif
