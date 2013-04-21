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
    int lighting_node_id;
    uint num_shadow_emitters;
} light_state;
typedef struct _light_state* LightState;

#define DirectionLightingNode     (MaxShaderNode + 1)
#define DiffuseSpecularOutputNode (MaxShaderNode + 2)
#define ShadowMapTestNode         (MaxShaderNode + 3)

API_EXPORT void lighting_template(LightState _lt_state, PxlSdrBuf _psb, SdrNdGen _sng, CircuitBoard _cb, bool _has_lighting_map);
API_EXPORT Pass create_lighting_pass_ex3(Renderer _rdr, VertexDecl _dec, LightState _lt_state, lighting_decl* _decls, bool _has_lighting_map);

API_EXPORT LightingModelRendererParamTable LightingModelRendererParamTable_new();
API_EXPORT void LightingModelRendererParamTable_delete(LightingModelRendererParamTable _self);
#endif
