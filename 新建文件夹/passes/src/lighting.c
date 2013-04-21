#include "lighting.h"

#include "pass.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"
#include "vertex_declaration.h"
#include "shader_node.h"
#include "array.h"
#include "pass_console.h"
#include "shader_buffer.h"
#include "estring.h"
#include "float3.h"
#include "float4.h"

typedef struct _light_base
{
    light_type type;
    char data[1];
} light_base;

typedef struct _point_light_data
{
    ShaderObject position;
    ShaderObject color;
    ShaderObject atte_coef;
    point_light_render_data render_data;
} point_light_data;

typedef struct _point_light
{
    light_type type;
    struct _point_light_data data;
} point_light;

light_type Light_get_type(Light _lt)
{
    light_base* lb = (light_base*)_lt.self;
    return lb->type;
}

PointLight PointLight_new()
{
    PointLight ret;
    ret.self = (point_light*)Malloc(sizeof(point_light));
    ret.self->type = Point;
    memset(&ret.self->data, 0, sizeof(point_light_data));
    return ret;
}

void PointLight_delete(PointLight _pl)
{
    ///ShaderObject_delete(_pl.self->data.position);
    ///ShaderObject_delete(_pl.self->data.color);
    Mfree(_pl.self);
}

void PointLight_set(PointLight _pl, ShaderObject _pos, ShaderObject _col, ShaderObject _atte_coef)
{
    _pl.self->data.position = _pos;
    _pl.self->data.color = _col;
    _pl.self->data.atte_coef = _atte_coef;
}

ShaderObject PointLight_get_position_object(PointLight _pl)
{
    return _pl.self->data.position;
}

ShaderObject PointLight_get_color_object(PointLight _pl)
{
    return _pl.self->data.color;
}

ShaderObject PointLight_get_atte_coef_object(PointLight _pl)
{
    return _pl.self->data.atte_coef;
}

void PointLight_set_position(PointLight _pl, float _x, float _y, float _z)
{
    _pl.self->data.render_data.x = _x;
    _pl.self->data.render_data.y = _y;
    _pl.self->data.render_data.z = _z;
}

void PointLight_set_color(PointLight _pl, float _r, float _g, float _b, float _a)
{
    _pl.self->data.render_data.r = _r;
    _pl.self->data.render_data.g = _g;
    _pl.self->data.render_data.b = _b;
    _pl.self->data.render_data.a = _a;
}

void PointLight_set_atte_coef(PointLight _pl, float _ac)
{
    _pl.self->data.render_data.ac = _ac;
}

EFloat3 PointLight_get_position(PointLight _pl)
{
    EFloat3 ret = {_pl.self->data.render_data.x, _pl.self->data.render_data.y, _pl.self->data.render_data.z};
    return ret;
}
EFloat4 PointLight_get_color(PointLight _pl)
{
    EFloat4 ret = {_pl.self->data.render_data.r, _pl.self->data.render_data.g, _pl.self->data.render_data.b, _pl.self->data.render_data.a};
    return ret;
}
EFloat PointLight_get_atte_coef(PointLight _pl)
{
    return _pl.self->data.render_data.ac;
}

point_light_render_data* PointLight_get_render_data(PointLight _pl)
{
    return &_pl.self->data.render_data;
}

void Light_delete(Light _lt)
{
    light_base* lb = (light_base*)_lt.self;
    switch (lb->type)
    {
        case Point:
        {
            PointLight pl = to_point_light(_lt);
            PointLight_delete(pl);
        }
        break;
        case Direction:
        case Spot:
        break;
    }
}

