#include "camera.h"
#include "float3.h"
#include "float4.h"
#include "matrix4x4.h"
#include "mem.h"
#include "pipeline.h"
typedef struct _camera
{
    float ratio;
    float far_plane;
    float near_plane;
    float width;
    float height;
    Matrix4x4 cam_rota_mat;
    Matrix4x4 cam_tran_mat;
    Matrix4x4 cam_proj_mat;
    Matrix4x4 render_mat;
    Matrix4x4 inv_cam_rota_mat;
    Matrix4x4 inv_cam_tran_mat;
    Matrix4x4 inv_cam_proj_mat;
    Matrix4x4 inv_render_mat;
} camera;

void Camera_Dest(struct _camera* _self)
{

    Matrix4x4_delete(_self->cam_rota_mat);
    Matrix4x4_delete(_self->cam_tran_mat);
    Matrix4x4_delete(_self->cam_proj_mat);
    Matrix4x4_delete(_self->render_mat);

    Matrix4x4_delete(_self->inv_cam_rota_mat);
    Matrix4x4_delete(_self->inv_cam_tran_mat);
    Matrix4x4_delete(_self->inv_cam_proj_mat);
    Matrix4x4_delete(_self->inv_render_mat);
    Mfree(_self);
}

Camera Camera_Init(struct _camera* _self, uint _width, uint _height)
{
    _self->ratio = (float)_width / (float)_height;
    _self->far_plane = 10.0;
    _self->near_plane = 0.5;
    _self->width = (float)_width / g_width;
    _self->height = (float)_height / g_width;
    Matrix4x4_set_one(_self->cam_rota_mat);

    Matrix4x4_set_one(_self->cam_rota_mat);

    Matrix4x4_assign(_self->inv_cam_rota_mat, _self->cam_rota_mat);
    Matrix4x4_inverse(_self->inv_cam_rota_mat);
    Matrix4x4_set_translate(_self->cam_tran_mat, 0.0, 0.0, 0.0);
    Matrix4x4_assign(_self->inv_cam_tran_mat, _self->cam_tran_mat);
    Matrix4x4_inverse(_self->inv_cam_tran_mat);

    Matrix4x4_projection(_self->cam_proj_mat, _self->width, _self->height, _self->near_plane, _self->far_plane);

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

    cam->inv_cam_tran_mat = Matrix4x4_new();
    cam->inv_cam_rota_mat = Matrix4x4_new();
    cam->inv_cam_proj_mat = Matrix4x4_new();
    cam->inv_render_mat = Matrix4x4_new();
    return Camera_Init(cam, _width, _height);
}

void Camera_delete(Camera _self)
{
    Camera_Dest(_self.self);
}

void Camera_updata(Camera _self)
{
    Matrix4x4_mul_matrix4(_self.self->cam_rota_mat, _self.self->cam_tran_mat, _self.self->render_mat);
    Matrix4x4_mul_matrix4(_self.self->render_mat, _self.self->cam_proj_mat, _self.self->render_mat);
    ///Matrix4x4_log(_self.self->render_mat);
    Matrix4x4_assign(_self.self->inv_render_mat, _self.self->render_mat);
    Matrix4x4_inverse(_self.self->inv_render_mat);
    Matrix4x4_turn(_self.self->render_mat);
    Matrix4x4_turn(_self.self->inv_render_mat);
}

Matrix4x4 Camera_get_render_matrix(Camera _self)
{
    return _self.self->render_mat;
}
Matrix4x4 Camera_get_inv_render_matrix(Camera _self)
{
    return _self.self->inv_render_mat;
}

void Camera_translate(Camera _self, float _x, float _y, float _z)
{
    Matrix4x4 tran_mat = Matrix4x4_new();
    Matrix4x4_set_translate(tran_mat, _x, _y, _z);
    Matrix4x4_mul_matrix4(_self.self->cam_tran_mat, tran_mat, _self.self->cam_tran_mat);

    Matrix4x4_assign(_self.self->inv_cam_tran_mat, _self.self->cam_tran_mat);
    Matrix4x4_inverse(_self.self->inv_cam_tran_mat);

    Matrix4x4_delete(tran_mat);
    Camera_updata(_self);
}

void Camera_rotate(Camera _self, EFloat3 _axis, float _rad)
{
    float3 axis = Float3(_axis.x, _axis.y, _axis.z);
    Matrix4x4 mat = Matrix4x4_new();
    Matrix4x4_from_axis_angle(mat, axis, _rad);
    Matrix4x4_mul_matrix4(_self.self->cam_rota_mat, mat, _self.self->cam_rota_mat);

    Matrix4x4_assign(_self.self->inv_cam_rota_mat, _self.self->cam_rota_mat);
    Matrix4x4_inverse(_self.self->inv_cam_rota_mat);

    Matrix4x4_delete(mat);
    Camera_updata(_self);
}
EFloat3 Camera_get_position(Camera _self)
{
    float3 tran = Matrix4x4_get_translate(_self.self->render_mat);
    EFloat3 ret = {tran->x, tran->y, tran->z};
    return ret;
}

EFloat3 Camera_get_direction(Camera _self)
{
    float4 dir = Float4(0.0f, 0.0f, 1.0f, 1.0f);
    dir = Matrix4x4_mul_float4(_self.self->cam_rota_mat, dir);
    EFloat3 ret = {dir->x, dir->y, dir->z};
    return ret;
}

Matrix4x4 Camera_get_rota_matrix(Camera _self)
{
    return _self.self->inv_cam_rota_mat;
}

Matrix4x4 Camera_get_tran_matrix(Camera _self)
{
    return _self.self->inv_cam_tran_mat;
}

float Camera_get_near_plane(Camera _self)
{
    return _self.self->near_plane;
}
float Camera_get_far_plane(Camera _self)
{
    return _self.self->far_plane;
}
float Camera_get_width(Camera _self)
{
    return _self.self->width;
}
float Camera_get_height(Camera _self)
{
    return _self.self->height;
}
