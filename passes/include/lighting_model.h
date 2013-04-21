/*
* Copyright (c) 2013, �캣��
* All rights reserved.
*
* �ļ����ƣ�lighting_model.h
* �ļ�����������ģ��
*
* ��ǰ�汾��0.1
* ��    �ߣ��캣��
* ������ڣ�2013��1��27��
*/
#ifndef LIGHTING_MODEL_H
#define LIGHTING_MODEL_H
#include "common.h"
#include "etypes.h"
#include "tree.h"
#include "pixel_shader_buffer.h"
#include "shader_node_generator.h"
#include "pass.h"
#include "material_prototype.h"
typedef struct _lighting_model_renderer_param_table
{
    Tree param_table;
} lighting_model_renderer_param_table;
typedef struct _lighting_model_renderer_param_table* LightingModelRendererParamTable;

typedef struct _light_state
{
    light_type type;
    int lighting_node_id;
    uint num_shadow_emitters;
} light_state;
typedef struct _light_state* LightState;

#define DirectionLightingNode     (MaxShaderNode + 1)
#define DiffuseSpecularOutputNode (MaxShaderNode + 2)
#define Shadow2DMapTestNode       (MaxShaderNode + 3)
#define SpotLightingNode          (MaxShaderNode + 4)
#define WhiteScreenNode           (MaxShaderNode + 5)
#define PointLightingNode         (MaxShaderNode + 6)
#define ShadowCubeMapTestNode     (MaxShaderNode + 7)

API_EXPORT void lighting_template(LightState _lt_state, PxlSdrBuf _psb, SdrNdGen _sng, CircuitBoard _cb, bool _has_lighting_map);
API_EXPORT Pass create_lighting_pass_ex3(Renderer* _rdr, VertexDecl _dec, LightState _lt_state, lighting_decl* _decls, bool _has_lighting_map);

API_EXPORT LightingModelRendererParamTable LightingModelRendererParamTable_new();
API_EXPORT void LightingModelRendererParamTable_delete(LightingModelRendererParamTable _self);
#endif
