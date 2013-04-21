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

API_EXPORT MaterialInstance MaterialInstance_new(const char* mat_name, const char* _col_tex_file, const char* _nor_tex_file);
API_EXPORT void MaterialInstance_delete(MaterialInstance _self);
///API_EXPORT Pass MaterialInstance_get_std_pass(MaterialInstance _self);
API_EXPORT Texture2D MaterialInstance_get_color_texture(MaterialInstance _self);
API_EXPORT Texture2D MaterialInstance_get_normal_texture(MaterialInstance _self);
API_EXPORT const char* MaterialInstance_get_material_name(MaterialInstance _self);
API_EXPORT bool MaterialInstance_has_color_texture(MaterialInstance _self);
API_EXPORT bool MaterialInstance_has_normal_texture(MaterialInstance _self);
API_EXPORT MaterialInstance MaterialInstance_clone(MaterialInstance _self);
#endif
