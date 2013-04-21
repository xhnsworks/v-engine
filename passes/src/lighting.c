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
    light_base* lb = (light_base*)_lt;
    return lb->type;
}

PointLight PointLight_new()
{
    PointLight ret;
    ret = (point_light*)Malloc(sizeof(point_light));
    ret->type = Point;
    return ret;
}

void PointLight_delete(PointLight _pl)
{
    Mfree(_pl);
}

void PointLight_set(PointLight _pl, ShaderObject _pos, ShaderObject _col, ShaderObject _atte_coef)
{
    _pl->data.position = _pos;
    _pl->data.color = _col;
    _pl->data.atte_coef = _atte_coef;
}

ShaderObject PointLight_get_position_object(PointLight _pl)
{
    return _pl->data.position;
}

ShaderObject PointLight_get_color_object(PointLight _pl)
{
    return _pl->data.color;
}

ShaderObject PointLight_get_atte_coef_object(PointLight _pl)
{
    return _pl->data.atte_coef;
}

void PointLight_set_position_eft3(PointLight _pl, EFloat3 ft3)
{
    _pl->data.render_data.position.x = ft3.x;
    _pl->data.render_data.position.y = ft3.y;
    _pl->data.render_data.position.z = ft3.z;
}

void PointLight_set_position(PointLight _pl, float _x, float _y, float _z)
{
    _pl->data.render_data.position.x = _x;
    _pl->data.render_data.position.y = _y;
    _pl->data.render_data.position.z = _z;
}

void PointLight_set_color(PointLight _pl, float _r, float _g, float _b, float _a)
{
    _pl->data.render_data.color.x = _r;
    _pl->data.render_data.color.y = _g;
    _pl->data.render_data.color.z = _b;
    _pl->data.render_data.color.w = _a;
}

void PointLight_set_atte_coef(PointLight _pl, float _ac)
{
    _pl->data.render_data.ac = _ac;
}

EFloat3 PointLight_get_position(PointLight _pl)
{
    ///EFloat3 ret = {_pl->data.render_data.x, _pl->data.render_data.y, _pl->data.render_data.z};
    ///return ret;
    return _pl->data.render_data.position;
}
EFloat4 PointLight_get_color(PointLight _pl)
{
    ///EFloat4 ret = {_pl->data.render_data.r, _pl->data.render_data.g, _pl->data.render_data.b, _pl->data.render_data.a};
    ///return ret;
    return _pl->data.render_data.color;
}
EFloat PointLight_get_atte_coef(PointLight _pl)
{
    return _pl->data.render_data.ac;
}

point_light_render_data* PointLight_get_render_data(PointLight _pl)
{
    return &_pl->data.render_data;
}

void Light_delete(Light _lt)
{
    light_base* lb = (light_base*)_lt;
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

