/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：lighting.h
* 文件描述：光照相关的对象
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
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
#include "emem.hpp"
typedef enum _sketch_type
{
    Sketch2D,
    Sketch2DBox,
} sketch_type;
struct _line_drawer;
typedef class light_base_2* LightBase2;
#define MAX_SHADOW_EMITTERS 6
typedef struct _shadow_emitter_param
{
	Camera light_cam;
} shadow_emitter_param;

class light_base_2 : public MemObject
{
protected:
	EFloat3 pos;
	EFloat3 dir;
	EFloat4 color;
	float inner_cos;
	float outer_cos;
	float atte_coef;
	float influence;
	uint num_shadow_emitters;
	sketch_type shadow_type;
	shadow_emitter_param shadow_emitter_param_array[MAX_SHADOW_EMITTERS];
	/**
	LightDelete Delete;
	LightUpdate Update;
	LightSetPosition SetPosition;
	LightLookAt LookAt;
	LightDrawShape DrawShape;
	**/

	virtual void Dest() = 0;
	virtual void update();
	void Init();
public:
	virtual void set_position(EFloat3 _pos);
	virtual void look_at(EFloat3 _pt);
	virtual void draw_shape(struct _line_drawer* _drawer);

	static void Delete(LightBase2 _self);
	inline const EFloat3& get_position() {
		return pos;
	}
	inline const EFloat3& get_direction() {
		return dir;
	}
	inline const EFloat4& get_color() {
		return color;
	}
	inline float get_atte_coef() {
		return atte_coef;
	}
	inline int get_num_shadow_emitters() {
		return num_shadow_emitters;
	}
	inline matrix4x4* get_proj_matrix(uint i) {
		if (i < num_shadow_emitters)
			return Camera_get_render_matrix(shadow_emitter_param_array[i].light_cam);
		else
			return NULL;
	}
	inline matrix4x4* get_world_matrix(uint i)
	{
		if (i < num_shadow_emitters)
			return Camera_get_world_matrix(shadow_emitter_param_array[i].light_cam);
		else
			return NULL;
	}
	inline Camera get_camera(uint i)
	{
		update();
		if (i < num_shadow_emitters) {
			return shadow_emitter_param_array[i].light_cam;
		}
		else {
			Camera null_cam = {NULL};
			return null_cam;
		}
	}
	inline EFloat3 get_shadow_dir(uint i)
	{
		update();
		if (i < num_shadow_emitters)
			return Camera_get_direction(shadow_emitter_param_array[i].light_cam);
		else {
			EFloat3 zero(0.0f, 0.0f, 0.0f);
			return zero;
		}
	}
	inline float get_inner_cos()
	{
		return inner_cos;
	}
	inline float get_outer_cos()
	{
		return outer_cos;
	}
	inline float get_influence()
	{
		return influence;
	}
	inline void set_light_color(EFloat4 _color)
	{
		color = _color;
	}
	inline sketch_type get_shadow_type()
	{
		return shadow_type;
	}
};

typedef class direction_light* DirLight;
typedef class spot_light* SpotLight;
typedef class point_light* PointLight;

class direction_light : public light_base_2
{
public:
	static DirLight New();
	virtual void Dest();
};

class spot_light : public light_base_2
{
public:
	static SpotLight New();
	virtual void Dest();
	virtual void update();
};
class point_light : public light_base_2
{
public:
	static PointLight New();
	virtual void Dest();
	virtual void update();
};
/**
API_EXPORT void LightBase2_delete(LightBase2 _self);
API_EXPORT EFloat3 LightBase2_get_pos(LightBase2 _self);
API_EXPORT EFloat3 LightBase2_get_dir(LightBase2 _self);
API_EXPORT EFloat4 LightBase2_get_color(LightBase2 _self);
API_EXPORT float LightBase2_get_atte_coef(LightBase2 _self);
API_EXPORT int LightBase2_get_num_shadow_emitters(LightBase2 _self);
API_EXPORT matrix4x4* LightBase2_get_proj_matrix(LightBase2 _self, uint i);
API_EXPORT matrix4x4* LightBase2_get_world_matrix(LightBase2 _self, uint i);
**/
/**
API_EXPORT EFloat3 LightBase2_get_shadow_dir(LightBase2 _self, uint i);
API_EXPORT Camera LightBase2_get_camera(LightBase2 _self, uint i);
API_EXPORT void LightBase2_set_position(LightBase2 _self, EFloat3 _pos);
API_EXPORT void LightBase2_look_at(LightBase2 _self, EFloat3 _pt);

API_EXPORT float LightBase2_get_inner_cos(LightBase2 _self);
API_EXPORT float LightBase2_get_outer_cos(LightBase2 _self);
API_EXPORT float LightBase2_get_influence(LightBase2 _self);

API_EXPORT void LightBase2_set_light_color(LightBase2 _self, EFloat4 color);

API_EXPORT void LightBase2_draw_shape(LightBase2 _self, struct _line_drawer* _drawer);

API_EXPORT sketch_type LightBase2_get_shadow_type(LightBase2 _self);



API_EXPORT DirLight DirLight_new();
API_EXPORT void DirLight_delete(DirLight _self);
///API_EXPORT void DirLight_shadow_renderer_update(DirLight _self);
///API_EXPORT void DirLight_set_position(DirLight _self, EFloat3 _pos);
///API_EXPORT void DirLight_look_at(DirLight _self, EFloat3 _pt);

API_EXPORT SpotLight SpotLight_new();
API_EXPORT void SpotLight_delete(SpotLight _self);
API_EXPORT void SpotLight_update(SpotLight _self);

API_EXPORT PointLight PointLight_new();
///API_EXPORT void SpotLight_shadow_renderer_update(SpotLight _self);
///API_EXPORT void SpotLight_set_position(SpotLight _self, EFloat3 _pos);
///API_EXPORT void SpotLight_look_at(SpotLight _self. EFloat3 _pt);
**/
#endif
