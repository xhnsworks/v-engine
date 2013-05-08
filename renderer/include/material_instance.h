/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：material_instance.h
* 文件描述：材质实例
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef MATERIAL_INSTANCE_H
#define MATERIAL_INSTANCE_H
#include "common.h"
#include "etypes.h"
#include "vertex_declaration.h"
#include "matrix4x4.h"
#include "texture2d.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader_object.h"
#include "pixel_shader_buffer.h"
#include "pass.h"
#include "texture2d.h"

struct material_instance : public MemObject
{
	material_instance()
		: mat_name(NULL)
	{}
	~material_instance();
	///VertexDecl vtx_dec;
	///Pass std_pass;
	Texture2DPtr col_tex;
	Texture2DPtr nor_tex;
	const char* mat_name;
	///int material_id;
};
typedef struct material_instance* MaterialInstance;

API_EXPORT MaterialInstance MaterialInstance_new(const char* mat_name, const char* _col_tex_file, const char* _nor_tex_file, const char* _res_grp);
API_EXPORT void MaterialInstance_delete(MaterialInstance _self);
///API_EXPORT Pass MaterialInstance_get_std_pass(MaterialInstance _self);
API_EXPORT Texture2DPtr MaterialInstance_get_color_texture(MaterialInstance _self);
API_EXPORT Texture2DPtr MaterialInstance_get_normal_texture(MaterialInstance _self);
API_EXPORT const char* MaterialInstance_get_material_name(MaterialInstance _self);
API_EXPORT bool MaterialInstance_has_color_texture(MaterialInstance _self);
API_EXPORT bool MaterialInstance_has_normal_texture(MaterialInstance _self);
API_EXPORT MaterialInstance MaterialInstance_clone(MaterialInstance _self);

#endif
