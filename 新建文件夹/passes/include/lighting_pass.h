#ifndef LIGHTING_PASS_H
#define LIGHTING_PASS_H
#include "common.h"
#include "etypes.h"
#include "shader.h"
#include "pass.h"
#include "shader_node.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"
#include "vertex_declaration.h"
#include "lighting.h"
#define MAX_LIGHTS_PER_PASS 8
#define POINT_LIGHT_POSITIOIN_ARRAY "PointLightPositionArray"
#define POINT_LIGHT_COLOR_ARRAY     "PointLightColorArray"
#define POINT_LIGHT_ATTE_COEF_ARRAY "PointLightAtteCoefArray"
#define POINT_LIGHT_COUNT           "PointLightCount"
typedef struct light_slot_
{
    struct _light_slot* self;
} LightSlot;
extern LightSlot LightSlot_new();
extern void LightSlot_delete(LightSlot _ls);
extern void LightSlot_push_light(LightSlot _ls, Light _lt);
extern Light LightSlot_get_light(LightSlot _ls, uint32 _i);
extern uint32 LightSlot_get_count(LightSlot _ls);

extern void LightSlot_apply(LightSlot _ls, Pass _pas);
extern Pass create_lighting_pass(VertexDeclaration _dec, bool _has_lighting_map);
#endif
