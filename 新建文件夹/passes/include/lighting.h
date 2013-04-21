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

typedef struct light_
{
    vptr self;
} Light;

typedef struct _point_light_render_data
{
    float x, y, z;
    float r, g, b, a;
    float ac;
} point_light_render_data;

typedef struct point_light_
{
    struct _point_light* self;
} PointLight;

extern light_type Light_get_type(Light _lt);
extern PointLight PointLight_new();
extern void PointLight_delete(PointLight _pl);
extern void PointLight_set(PointLight _pl, ShaderObject _pos, ShaderObject _col, ShaderObject _atte_coef);

extern ShaderObject PointLight_get_position_object(PointLight _pl);
extern ShaderObject PointLight_get_color_object(PointLight _pl);
extern ShaderObject PointLight_get_atte_coef_object(PointLight _pl);

extern void PointLight_set_position(PointLight _pl, float _x, float _y, float _z);
extern void PointLight_set_color(PointLight _pl, float _r, float _g, float _b, float _a);
extern void PointLight_set_atte_coef(PointLight _pl, float _ac);

extern EFloat3 PointLight_get_position(PointLight _pl);
extern EFloat4 PointLight_get_color(PointLight _pl);
extern EFloat PointLight_get_atte_coef(PointLight _pl);

extern point_light_render_data* PointLight_get_render_data(PointLight _pl);
extern void Light_delete(Light _lt);
#define to_light(l)        ({Light tmp = {(vptr)l.self}; tmp;})
#define to_point_light(l)  ({PointLight tmp = {(struct _point_light*)l.self}; tmp;})

#endif
