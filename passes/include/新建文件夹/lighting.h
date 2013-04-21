#ifndef LIGHTING_H
#define LIGHTING_H
#include "common.h"
#include "etypes.h"
#include "shader.h"
#include "pass.h"
#include "shader_node.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"
#include "vertex_declaration.h"
#include "camera.h"

typedef vptr Light;

typedef struct _point_light_render_data
{
    EFloat3 position;
    EFloat4 color;
    float ac;
} point_light_render_data;

typedef struct _point_light* PointLight;

API_EXPORT light_type Light_get_type(Light _lt);
API_EXPORT PointLight PointLight_new();
API_EXPORT void PointLight_delete(PointLight _pl);
API_EXPORT void PointLight_set(PointLight _pl, ShaderObject _pos, ShaderObject _col, ShaderObject _atte_coef);

API_EXPORT ShaderObject PointLight_get_position_object(PointLight _pl);
API_EXPORT ShaderObject PointLight_get_color_object(PointLight _pl);
API_EXPORT ShaderObject PointLight_get_atte_coef_object(PointLight _pl);

API_EXPORT void PointLight_set_position_eft3(PointLight _pl, EFloat3 ft3);
API_EXPORT void PointLight_set_position(PointLight _pl, float _x, float _y, float _z);
API_EXPORT void PointLight_set_color(PointLight _pl, float _r, float _g, float _b, float _a);
API_EXPORT void PointLight_set_atte_coef(PointLight _pl, float _ac);

API_EXPORT EFloat3 PointLight_get_position(PointLight _pl);
API_EXPORT EFloat4 PointLight_get_color(PointLight _pl);
API_EXPORT EFloat PointLight_get_atte_coef(PointLight _pl);

API_EXPORT point_light_render_data* PointLight_get_render_data(PointLight _pl);
API_EXPORT void Light_delete(Light _lt);
#define to_light(l)        ({Light tmp = {(vptr)l}; tmp;})
#define to_point_light(l)  ({PointLight tmp = {(struct _point_light*)l}; tmp;})
typedef struct _light_base_2* LightBase2;
API_EXPORT void LightBase2_delete(LightBase2 _self);
API_EXPORT EFloat3 LightBase2_get_pos(LightBase2 _self);
API_EXPORT EFloat3 LightBase2_get_dir(LightBase2 _self);
API_EXPORT EFloat4 LightBase2_get_color(LightBase2 _self);
API_EXPORT float LightBase2_get_atte_coef(LightBase2 _self);
API_EXPORT int LightBase2_get_num_shadow_emitters(LightBase2 _self);
API_EXPORT Matrix4x4 LightBase2_get_proj_matrix(LightBase2 _self, uint i);
API_EXPORT Matrix4x4 LightBase2_get_world_matrix(LightBase2 _self, uint i);
API_EXPORT Texture2D LightBase2_get_shadow_texture(LightBase2 _self, uint i);
API_EXPORT void LightBase2_render_shadow_texture(LightBase2 _self, Tree _rdr_tree);
API_EXPORT EFloat3 LightBase2_get_shadow_dir(LightBase2 _self, uint i);
API_EXPORT Camera LightBase2_get_camera(LightBase2 _self, uint i);

typedef struct _direction_light* DirLight;

API_EXPORT DirLight DirLight_new();
API_EXPORT void DirLight_delete(DirLight _self);
API_EXPORT void DirLight_shadow_renderer_update(DirLight _self);
#endif
