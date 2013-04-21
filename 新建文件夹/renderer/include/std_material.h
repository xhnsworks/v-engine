#ifndef STD_MATERIAL_H
#define STD_MATERIAL_H
#include "common.h"
#include "etypes.h"
#include "vertex_declaration.h"
#include "matrix4x4.h"
#include "texture2d.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader_object.h"
#include "std_pass.h"
#include "lighting_pass.h"
#include "display_pass.h"
struct _std_material;
typedef struct std_material_
{
    struct _std_material* self;
} StdMaterial;

extern StdMaterial StdMaterial_new(VertexDeclaration _dec, const char* _col_tex_file, const char* _nor_tex_file);
extern void StdMaterial_delete(StdMaterial _self);
extern Pass StdMaterial_get_std_pass(StdMaterial _self);
extern Texture2D StdMaterial_get_color_texture(StdMaterial _self);
extern Texture2D StdMaterial_get_normal_texture(StdMaterial _self);
extern bool StdMaterial_has_color_texture(StdMaterial _self);
extern bool StdMaterial_has_normal_texture(StdMaterial _self);
#endif
