#include "camera.h"
#include "float3.h"
#include "float4.h"
#include "sfloat3.h"
#include "sfloat4.h"
#include "matrix4x4.h"
#include "mem.h"
#include "renderer.h"
typedef struct _camera
{
    float ratio;
    float far_plane;
    float near_plane;
    float width;
    float height;
    uint pixel_width;
    uint pixel_height;
    Matrix4x4 cam_rota_mat;
    Matrix4x4 cam_tran_mat;
    Matrix4x4 cam_proj_mat;
    Matrix4x4 render_mat;
    Matrix4x4 inv_render_mat;
    ///Matrix4x4 inv_cam_rota_mat;
    ///Matrix4x4 inv_cam_tran_mat;

    Matrix4x4 cam_world_mat;
    Matrix4x4 inv_cam_world_mat;

    Matrix4x4 unturned_cam_world_mat;
    Matrix4x4 unturned_inv_cam_world_mat;

    ///Matrix4x4 turned_render_mat;
    Matrix4x4 inv_cam_proj_mat;
    ///Matrix4x4 inv_render_mat;
} camera;

void Camera_Dest(struct _camera* _self)
{

    Matrix4x4_delete(_self->cam_rota_mat);
    Matrix4x4_delete(_self->cam_tran_mat);
    Matrix4x4_delete(_self->cam_proj_mat);
    Matrix4x4_delete(_self->render_mat);
    Matrix4x4_delete(_self->inv_render_mat);

    ///Matrix4x4_delete(_self->inv_cam_rota_mat);
    ///Matrix4x4_delete(_self->inv_cam_tran_mat);

    ///Matrix4x4_delete(_self->turned_render_mat);
    Matrix4x4_delete(_self->inv_cam_proj_mat);
    ///Matrix4x4_delete(_self->inv_render_mat);
    Matrix4x4_delete(_self->cam_world_mat);
    Matrix4x4_delete(_self->inv_cam_world_mat);

    Matrix4x4_delete(_self->unturned_cam_world_mat);
    Matrix4x4_delete(_self->unturned_inv_cam_world_mat);

    Mfree(_self);
}

Camera Camera_Init(struct _camera* _self, uint _width, uint _height)
{
    _self->ratio = (float)_width / (float)_height;
    _self->far_plane = 10.0;
    _self->near_plane = 0.5;
    ///_self->width = (float)_width / (float)g_width;
    ///_self->height = (float)_height / (float)g_width;
    _self->width = 1.0f;
    _self->height = (float)_height / (float)_width;
    _self->pixel_width = _width;
    _self->pixel_height = _height;

    Matrix4x4_set_one(_self->cam_rota_mat);
    ///Matrix4x4_assign(_self->inv_cam_rota_mat, _self->cam_rota_mat);
    ///Matrix4x4_inverse(_self->inv_cam_rota_mat);
    Matrix4x4_set_translate(_self->cam_tran_mat, 0.0, 0.0, 0.0);
    ///Matrix4x4_assign(_self->inv_cam_tran_mat, _self->cam_tran_mat);
    ///Matrix4x4_inverse(_self->inv_cam_tran_mat);

    if (_width && _height)
        Matrix4x4_projection(_self->cam_proj_mat, _self->width, _self->height, _self->near_plane, _self->far_plane);
    else
        Matrix4x4_set_one(_self->cam_proj_mat);

    Camera ret = {_self};
    Camera_updata(ret);

    return ret;
}

Camera Camera_new(uint _width, uint _height)
{
    camera* cam = Malloc(sizeof(camera));
    cam->cam_tran_mat = Matrix4x4_new();
    cam->cam_rota_mat = Matrix4x4_new();
    cam->cam_proj_mat = Matrix4x4_new();
    cam->render_mat = Matrix4x4_new();
    cam->inv_render_mat = Matrix4x4_new();

    ///cam->inv_cam_tran_mat = Matrix4x4_new();
    ///cam->inv_cam_rota_mat = Matrix4x4_new();

    cam->cam_world_mat = Matrix4x4_new();
    cam->inv_cam_world_mat = Matrix4x4_new();

    cam->unturned_cam_world_mat = Matrix4x4_new();
    cam->unturned_inv_cam_world_mat = Matrix4x4_new();

    ///cam->turned_render_mat = Matrix4x4_new();
    cam->inv_cam_proj_mat = Matrix4x4_new();
    ///cam->inv_render_mat = Matrix4x4_new();
    return Camera_Init(cam, _width, _height);
}

void Camera_delete(Camera _self)
{
    Camera_Dest(_self.self);
}

void Camera_updata(Camera _self)
{
    ///Matrix4x4_assign(SELF.inv_cam_rota_mat, SELF.cam_rota_mat);
    ///Matrix4x4_inverse(SELF.inv_cam_rota_mat);

    ///Matrix4x4_assign(SELF.inv_cam_tran_mat, SELF.cam_tran_mat);
    ///Matrix4x4_inverse(SELF.inv_cam_tran_mat);

    Matrix4x4_mul_matrix4(SELF.cam_tran_mat, SELF.cam_rota_mat, SELF.cam_world_mat);
    ///Matrix4x4_mul_matrix4(SELF.inv_cam_rota_mat, SELF.inv_cam_tran_mat, SELF.inv_cam_world_mat);
    Matrix4x4_assign(SELF.inv_cam_world_mat, SELF.cam_world_mat);
    Matrix4x4_inverse(SELF.inv_cam_world_mat);

    Matrix4x4_assign(SELF.unturned_cam_world_mat, SELF.cam_world_mat);
    Matrix4x4_assign(SELF.unturned_inv_cam_world_mat, SELF.inv_cam_world_mat);

    Matrix4x4_turn(SELF.cam_world_mat);
    Matrix4x4_turn(SELF.inv_cam_world_mat);

    Matrix4x4_assign(SELF.render_mat, SELF.cam_proj_mat);
    Matrix4x4_assign(SELF.inv_cam_proj_mat, SELF.cam_proj_mat);
    Matrix4x4_inverse(SELF.inv_cam_proj_mat);
    Matrix4x4_assign(SELF.inv_render_mat, SELF.inv_cam_proj_mat);
    Matrix4x4_turn(SELF.render_mat);
    Matrix4x4_turn(SELF.inv_render_mat);
}

void Camera_set_width(Camera _self, float _width)
{
    SELF.width = _width;
    Matrix4x4_projection(SELF.cam_proj_mat, SELF.width, SELF.height, SELF.near_plane, SELF.far_plane);
    Camera_updata(_self);
}

void Camera_set_height(Camera _self, float _height)
{
    SELF.height = _height;
    Matrix4x4_projection(SELF.cam_proj_mat, SELF.width, SELF.height, SELF.near_plane, SELF.far_plane);
    Camera_updata(_self);
}

void Camera_set_near(Camera _self, float _near)
{
    SELF.near_plane = _near;
    Matrix4x4_projection(SELF.cam_proj_mat, SELF.width, SELF.height, SELF.near_plane, SELF.far_plane);
    Camera_updata(_self);
}

void Camera_set_far(Camera _self, float _far)
{
    SELF.far_plane = _far;
    Matrix4x4_projection(SELF.cam_proj_mat, SELF.width, SELF.height, SELF.near_plane, SELF.far_plane);
    Camera_updata(_self);
}

void Camera_set(Camera _self, float _width, float _height, float _near, float _far)
{
    SELF.width = _width;
    SELF.height = _height;
    SELF.near_plane = _near;
    SELF.far_plane = _far;
    Matrix4x4_projection(SELF.cam_proj_mat, SELF.width, SELF.height, SELF.near_plane, SELF.far_plane);
    Camera_updata(_self);
}

void Camera_set_proj_matrix(Camera _self, Matrix4x4 _mat)
{
    Matrix4x4_assign(SELF.cam_proj_mat, _mat);
    Camera_updata(_self);
}

void Camera_set_rota_matrix(Camera _self, Matrix4x4 _mat)
{
    Matrix4x4_assign(SELF.cam_rota_mat, _mat);
    Camera_updata(_self);
}

Matrix4x4 Camera_get_render_matrix(Camera _self)
{
    return SELF.render_mat;
}

Matrix4x4 Camera_get_inv_render_matrix(Camera _self)
{
    return SELF.inv_render_mat;
}

Matrix4x4 Camera_get_world_matrix(Camera _self)
{
    return SELF.cam_world_mat;
}

Matrix4x4 Camera_get_inv_world_matrix(Camera _self)
{
    return SELF.inv_cam_world_mat;
}

void Camera_translate(Camera _self, float _x, float _y, float _z)
{
    Matrix4x4 tran_mat = Matrix4x4_new();
    Matrix4x4_set_translate(tran_mat, -_x, -_y, -_z);
    Matrix4x4_mul_matrix4(SELF.cam_tran_mat, tran_mat, SELF.cam_tran_mat);
    Matrix4x4_delete(tran_mat);
    Camera_updata(_self);
}

void Camera_rotate(Camera _self, EFloat3 _axis, float _rad)
{
    sfloat3 axis = SFloat3(_axis.x, _axis.y, _axis.z);
    Matrix4x4 mat = Matrix4x4_new();
    Matrix4x4_from_axis_angle(mat, axis, -_rad);
    Matrix4x4_mul_matrix4(SELF.cam_rota_mat, mat, SELF.cam_rota_mat);
    Matrix4x4_delete(mat);
    Camera_updata(_self);
}
EFloat3 Camera_get_position(Camera _self)
{
    sfloat4 pos = SFloat4(0.0f, 0.0f, 0.0f, 1.0f);
    pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);
    return SFloat3_convert(pos);
}

EFloat3 Camera_get_direction(Camera _self)
{
    sfloat4 pos = SFloat4(0.0f, 0.0f, 0.0f, 1.0f);
    sfloat4 dir = SFloat4(0.0f, 0.0f, 1.0f, 1.0f);
    pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);
    dir = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, dir);
    dir = SFloat4_sub(dir, pos);
    dir = SFloat4_normalize(dir);
    return SFloat3_convert(dir);
}

void Camera_look_at(Camera _self, EFloat3 _pt)
{
    sfloat3 tgt_dir = SFloat3_assign_from_efloat3(&_pt);
    sfloat4 pos = SFloat4(0.0f, 0.0f, 0.0f, 1.0f);
    pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);
    tgt_dir = SFloat3_sub(tgt_dir, SFloat4_xyz(pos));
    tgt_dir = SFloat3_normalize(tgt_dir);

    sfloat4 src_dir4 = SFloat4(0.0f, 0.0f, -1.0f, 1.0f);
    src_dir4 = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, src_dir4);
    sfloat3 src_dir = SFloat3_sub(SFloat4_xyz(src_dir4), SFloat4_xyz(pos));
    src_dir = SFloat3_normalize(src_dir);

    sfloat3 axis = SFloat3_cross(src_dir, tgt_dir);
    axis = SFloat3_normalize(axis);

    float dot = SFloat3_dot(src_dir, tgt_dir);
    float rad = acosf(dot);
    if (fabs(rad) > 0.0001f)
        Camera_rotate(_self, SFloat3_convert(axis), rad);
}

void Camera_set_position(Camera _self, EFloat3 _pt)
{
    sfloat3 tgt = SFloat3_assign_from_efloat3(&_pt);
    sfloat4 pos = SFloat4(0.0f, 0.0f, 0.0f, 1.0f);
    pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);
    EFloat3 disp = SFloat3_convert(SFloat3_sub(tgt, SFloat4_xyz(pos)));
    Camera_translate(_self, disp.x, disp.y, disp.z);
}
/**
Matrix4x4 Camera_get_rota_matrix(Camera _self)
{
    return SELF.inv_cam_rota_mat;
}

Matrix4x4 Camera_get_tran_matrix(Camera _self)
{
    return SELF.inv_cam_tran_mat;
}
**/
float Camera_get_near_plane(Camera _self)
{
    return SELF.near_plane;
}
float Camera_get_far_plane(Camera _self)
{
    return SELF.far_plane;
}
float Camera_get_width(Camera _self)
{
    return SELF.width;
}
float Camera_get_height(Camera _self)
{
    return SELF.height;
}
EFloat3 Camera_screen_position_to_world_position(Camera _self, float x, float y)
{
    sfloat4 pos;
    float half_pixel_width = (float)(SELF.pixel_width >> 1);
    float half_pixel_height = (float)(SELF.pixel_height >> 1);
    float pixel_width = (float)(SELF.pixel_width);
    float pixel_height = (float)(SELF.pixel_height);
    float width = SELF.width;
    float height = SELF.height;
    x = (x - half_pixel_width) / pixel_width;
    y = (y - half_pixel_height) / pixel_height;

    x *= -width;
    y *= height;

    sfloat4 z = SFloat4(0.0f, 0.0f, 0.0f, SELF.near_plane);
    z = Matrix4x4_mul_float4(SELF.inv_cam_proj_mat, z);
    pos = SFloat4(x , y, -SFloat4_get_z(&z), 1.0f);
    pos = Matrix4x4_mul_float4(SELF.unturned_inv_cam_world_mat, pos);
    return SFloat3_convert(SFloat4_xyz(pos));
}
