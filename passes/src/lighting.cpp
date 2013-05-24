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
#include "sfloat3.h"

#include "renderer.h"
#include "camera.h"
#include "pixel_shader_buffer.h"
#include "math_base.h"
#include <math.h>

void light_base_2::Init()
{
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    dir.x = 0.0f;
    dir.y = 0.0f;
    dir.z = -1.0f;
    color.x = 1.0f;
    color.y = 1.0f;
    color.z = 1.0f;
    color.w = 1.0f;
    atte_coef = 1.0f;
    inner_cos = 1.0f;
    outer_cos = 0.0f;
    influence = 10.0f;
    num_shadow_emitters = 0;
    shadow_type = Sketch2D;
    memset(&shadow_emitter_param_array, 0, sizeof(shadow_emitter_param_array));
}
void light_base_2::Delete(LightBase2 _self)
{
    _self->Dest();
    delete _self;
}
void light_base_2::draw_shape(struct _line_drawer* _drawer)
{
    Camera_draw_shape(shadow_emitter_param_array[0].light_cam, _drawer);
}

void light_base_2::update()
{
    for (euint i = 0; i < num_shadow_emitters; i++) {
        sfloat3 cam_pos = SFloat3_assign_from_efloat3(&pos);
        sfloat3 cam_dir = SFloat3_assign_from_efloat3(&dir);
        cam_dir = SFloat3_mul_float(-10.0f, cam_dir);
        cam_pos = SFloat3_add(cam_dir, cam_pos);
        cam_dir = SFloat3_mul_float(-1.0, cam_dir);
        sfloat3 look_at_pos = SFloat3_add(cam_pos, cam_dir);

        Camera_set_position(shadow_emitter_param_array[0].light_cam, SFloat3_convert(cam_pos));
        Camera_look_at(shadow_emitter_param_array[0].light_cam, SFloat3_convert(look_at_pos));
    }
}
void light_base_2::set_position(EFloat3 _pos)
{
    pos = _pos;
}
void light_base_2::look_at(EFloat3 _pt)
{
    sfloat3 tgt_dir = SFloat3_assign_from_efloat3(&_pt);
    sfloat3 p = SFloat3_assign_from_efloat3(&pos);
    tgt_dir = SFloat3_sub(tgt_dir, p);
    tgt_dir = SFloat3_normalize(tgt_dir);

    dir = SFloat3_convert(tgt_dir);
}

DirLight direction_light::New()
{
	DirLight ret = ENEW direction_light;
    ret->Init();
    ret->num_shadow_emitters = 1;
    Camera light_cam = Camera_new(512, 512);

    matrix4x4* ortho_mat = Matrix4x4_new();
    Matrix4x4_orthogonal(ortho_mat, 10.0f, 10.0f, 0.1f, 100.0f);

	Camera_set(light_cam, true, 10.0f, 10.0f, 0.1f, 100.0f);
    Matrix4x4_delete(ortho_mat);

    ret->shadow_emitter_param_array[0].light_cam = light_cam;

    return ret;
}
void direction_light::Dest()
{
    Camera_delete(shadow_emitter_param_array[0].light_cam);
}

SpotLight spot_light::New()
{
    ///SpotLight ret = (SpotLight)SMalloc(sizeof(spot_light));
	SpotLight ret = ENEW spot_light;
    ret->Init();

    ret->num_shadow_emitters = 1;
    Camera light_cam = Camera_new(512, 512);

    float d = 30.0f;

    Camera_set_FOV(light_cam, d);

    ret->inner_cos = cosf(DegreesToRadian(20.0f));
    ret->outer_cos = cosf(DegreesToRadian(d));

    ///ret->base.pos.z = 3.0f;

    ret->shadow_emitter_param_array[0].light_cam = light_cam;
   
    return ret;
}
void spot_light::Dest()
{
    Camera_delete(shadow_emitter_param_array[0].light_cam);
}

void spot_light::update()
{
	light_base_2::update();
    Camera_set_radian(shadow_emitter_param_array[0].light_cam, acosf(outer_cos));
}

void point_light::Dest()
{
    for (int i = 0; i < 6; i++)
        Camera_delete(shadow_emitter_param_array[i].light_cam);
}
static EFloat3 s_point_light_look_at_table[] = {
    EFloat3(1.0, 0.0, 0.0),
    EFloat3(-1.0, 0.0, 0.0),
    EFloat3(0.0, 1.0, 0.0),
    EFloat3(0.0, -1.0, 0.0),
    EFloat3(0.0, 0.0, 1.0),
    EFloat3(0.0, 0.0, -1.0),
};
void point_light::update()
{
    for (int i = 0; i < 6; i++) {
        Camera cam = shadow_emitter_param_array[i].light_cam;
        Camera_set_position(cam, pos);
    }
}

PointLight point_light::New()
{
    ///PointLight ret = (PointLight)SMalloc(sizeof(point_light));
	PointLight ret = ENEW point_light;
    ret->Init();

    ret->num_shadow_emitters = 6;
    ret->shadow_type = Sketch2DBox;
    for (int i = 0; i < 6; i++)
    {
        Camera light_cam = Camera_new(512, 512);
        Camera_set_FOV(light_cam, 90.0);

        ret->shadow_emitter_param_array[i].light_cam = light_cam;
    }

    EFloat3 zero(0.0, 0.0, 0.0);
    for (int i = 0; i < 6; i++) {
        Camera cam = ret->shadow_emitter_param_array[i].light_cam;
        /// 当i为4的时候look at会有问题的
        if (i != 2 && i != 3 && i != 4)
        {
            Camera_set_position(cam, zero);
            Camera_look_at(cam, s_point_light_look_at_table[i]);
            Camera_set_position(cam, ret->pos);
        }
        else if (i == 2)
        {
            Camera_set_position(cam, zero);
            Camera_look_at(cam, s_point_light_look_at_table[i]);
            ///Camera_rotate(cam, s_point_light_look_at_table[i], E_PI);
            Camera_set_proj_scale(cam, -1.0, -1.0);
            Camera_set_position(cam, ret->pos);
        }
        else if (i == 3)
        {
            Camera_set_position(cam, zero);
            Camera_look_at(cam, s_point_light_look_at_table[i]);
            ///Camera_rotate(cam, s_point_light_look_at_table[i], E_PI);
            Camera_set_proj_scale(cam, -1.0, -1.0);
            Camera_set_position(cam, ret->pos);
        }
        else if (i == 4)
        {
            Camera_set_position(cam, zero);
            Camera_look_at(cam, s_point_light_look_at_table[0]);
            Camera_look_at(cam, s_point_light_look_at_table[4]);
            Camera_set_position(cam, ret->pos);
        }

        matrix4x4* cam_world_mat = Camera_get_world_matrix(cam);
        Matrix4x4_log(cam_world_mat);
    }
    elog("####");

    return ret;
}
