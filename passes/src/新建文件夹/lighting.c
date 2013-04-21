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
#include "sfloat3.h"

#include "renderer.h"

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

#define MAX_SHADOW_EMITTERS 6
typedef struct _shadow_emitter_param
{
    ///ShadowRenderer shadow_rdr;
    Camera light_cam;
} shadow_emitter_param;
typedef struct _light_base_2
{
    EFloat3 pos;
    EFloat3 dir;
    EFloat4 color;
    float atte_coef;
    uint num_shadow_emitters;
    shadow_emitter_param shadow_emitter_param_array[MAX_SHADOW_EMITTERS];
    void (*Delete)(struct _light_base_2*);
    void (*Update)(struct _light_base_2*);
} light_base_2;

void LightBase2_Init(LightBase2 lt)
{
    lt->pos.x = 0.0f;
    lt->pos.y = 0.0f;
    lt->pos.z = 0.0f;
    lt->dir.x = 0.0f;
    lt->dir.y = 0.0f;
    lt->dir.z = -1.0f;
    lt->color.x = 1.0f;
    lt->color.y = 1.0f;
    lt->color.z = 1.0f;
    lt->color.w = 1.0f;
    lt->atte_coef = 1.0f;
    lt->num_shadow_emitters = 0;
    memset(&lt->shadow_emitter_param_array, 0, sizeof(lt->shadow_emitter_param_array));
}
void LightBase2_delete(LightBase2 _self)
{
   _self->Delete(_self);
}
EFloat3 LightBase2_get_pos(LightBase2 _self)
{
    return _self->pos;
}
EFloat3 LightBase2_get_dir(LightBase2 _self)
{
    return _self->dir;
}
EFloat4 LightBase2_get_color(LightBase2 _self)
{
    return _self->color;
}
float LightBase2_get_atte_coef(LightBase2 _self)
{
    return _self->atte_coef;
}
int LightBase2_get_num_shadow_emitters(LightBase2 _self)
{
    return _self->num_shadow_emitters;
}
Matrix4x4 LightBase2_get_proj_matrix(LightBase2 _self, uint i)
{
    if (i < _self->num_shadow_emitters)
        return Camera_get_render_matrix(_self->shadow_emitter_param_array[i].light_cam);
    else
        return NULL;
}
Matrix4x4 LightBase2_get_world_matrix(LightBase2 _self, uint i)
{
    if (i < _self->num_shadow_emitters)
        return Camera_get_world_matrix(_self->shadow_emitter_param_array[i].light_cam);
    else
        return NULL;
}
Texture2D LightBase2_get_shadow_texture(LightBase2 _self, uint i)
{
    /**
    if (i < _self->num_shadow_emitters)
        return ShadowRenderer_get_depth_tex(_self->shadow_emitter_param_array[i].shadow_rdr);
    else
        return NULL;
        **/
    return NULL;
}
void LightBase2_render_shadow_texture(LightBase2 _self, Tree _rdr_tree)
{
    /**
    _self->Update(_self);
    for (uint i = 0; i < _self->num_shadow_emitters; i++)
    {
        ShadowRenderer_render_renderable_tree(_self->shadow_emitter_param_array[i].shadow_rdr, _rdr_tree);
    }
    **/
}
EFloat3 LightBase2_get_shadow_dir(LightBase2 _self, uint i)
{
    if (i < _self->num_shadow_emitters)
        return Camera_get_direction(_self->shadow_emitter_param_array[i].light_cam);
    else {
        EFloat3 zero = {0.0f, 0.0f, 0.0f};
        return zero;
    }
}
Camera LightBase2_get_camera(LightBase2 _self, uint i)
{
    _self->Update(_self);
    if (i < _self->num_shadow_emitters)
        return _self->shadow_emitter_param_array[i].light_cam;
    else {
        Camera null_cam = {NULL};
        return null_cam;
    }
}

typedef struct _direction_light
{
    light_base_2 base;
} direction_light;

DirLight DirLight_new()
{
    DirLight ret = Malloc(sizeof(direction_light));
    LightBase2_Init(ret);
    ret->base.num_shadow_emitters = 1;
    Camera light_cam = Camera_new(512, 512);
    ///ShadowRenderer shadow_rdr = ShadowRenderer_new(light_cam);
    ///ret->base.shadow_emitter_param_array[0].shadow_rdr = shadow_rdr;
    ret->base.shadow_emitter_param_array[0].light_cam = light_cam;
    ret->base.Delete = DirLight_delete;
    ret->base.Update = DirLight_shadow_renderer_update;
    return ret;
}
void DirLight_delete(DirLight _self)
{
    ///ShadowRenderer_delete(_self->base.shadow_emitter_param_array[0].shadow_rdr);
    Camera_delete(_self->base.shadow_emitter_param_array[0].light_cam);
    Mfree(_self);
}
void DirLight_shadow_renderer_update(DirLight _self)
{
    /**
    sfloat3 sdir = SFloat3_assign_from_efloat3(&_self->base.dir);
    sfloat3 spos = SFloat3_assign_from_efloat3(&_self->base.pos);
    sdir = SFloat3_mul_float(1.0f, sdir);
    sdir = SFloat3_mul_float(5.0f, sdir);
    spos = SFloat3_add(spos, sdir);
    EFloat3 pos = SFloat3_convert(spos);
    Camera_set_position(_self->base.shadow_emitter_param_array[0].light_cam, pos);
    Camera_set_direction(_self->base.shadow_emitter_param_array[0].light_cam, _self->base.dir);
    Matrix4x4 ortho_mat = Matrix4x4_new();
    ///Matrix4x4_set_one(ortho_mat);
    Matrix4x4_projection(ortho_mat, 1.0f, 1.0f, 0.1f, 20.0f);
    Camera_set_proj_matrix(_self->base.shadow_emitter_param_array[0].light_cam, ortho_mat);
    Matrix4x4_delete(ortho_mat);
    **/
    EFloat3 cam_pos = {0.0f, 0.0f, 5.0f};
    EFloat3 center_pos = {0.0f, 0.0f, 0.0f};
    Camera_set_position(_self->base.shadow_emitter_param_array[0].light_cam, cam_pos);
    Camera_look_at(_self->base.shadow_emitter_param_array[0].light_cam, center_pos);
}
