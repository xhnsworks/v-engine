#include "pch.h"
#include "material_instance.h"
#include "std_pass.h"
#include "lighting_pass.h"
#include "display_pass.h"

#include "array.h"
#include "estring.h"
#include "gl_utility.h"
#include "shader_buffer.h"
#include "renderer.h"
#include "pass_declaration.h"
#include "render_system.h"
#include "texture2d.h"

MaterialInstance MaterialInstance_new(const char* mat_name, const char* _col_tex_file, const char* _nor_tex_file, const char* _res_grp)
{
    MaterialInstance ret;
    ///ret = (MaterialInstance)SMalloc(sizeof(material_instance));
	ret = ENEW material_instance;

    Texture2DPtr null_tex;
    if (_col_tex_file && strcmp(_col_tex_file, "") != 0)
        ret->col_tex = RenderSystem_register_texture2d(_col_tex_file, _res_grp);
    else
        ret->col_tex = null_tex;
    if (_nor_tex_file && strcmp(_nor_tex_file, "") != 0)
        ret->nor_tex = RenderSystem_register_texture2d(_nor_tex_file, _res_grp);
    else
        ret->nor_tex = null_tex;

    if (mat_name)
        ret->mat_name = EString_new(mat_name);
    else
        ret->mat_name = EString_new("default_material");

    ///ret->material_id = Renderer_get_material_id(_renderer, mat_name);

    return ret;
}

material_instance::~material_instance()
{
    EString_delete(mat_name);
}

void MaterialInstance_delete(MaterialInstance _self)
{
    ///Pass_delete(SELF.std_pass);
    /**
    if (to_ptr(SELF.col_tex))
        Texture2D_delete(SELF.col_tex);
    if (to_ptr(SELF.nor_tex))
        Texture2D_delete(SELF.nor_tex);
        **/
    ///EString_delete(_self->mat_name);

    ///Mfree(_self);
	delete _self;
}
/**
Pass MaterialInstance_get_std_pass(MaterialInstance _self)
{
    return SELF.std_pass;
}
**/
Texture2DPtr MaterialInstance_get_color_texture(MaterialInstance _self)
{
    return _self->col_tex;
}

Texture2DPtr MaterialInstance_get_normal_texture(MaterialInstance _self)
{
    return _self->nor_tex;
}

const char* MaterialInstance_get_material_name(MaterialInstance _self)
{
    return _self->mat_name;
}

bool MaterialInstance_has_color_texture(MaterialInstance _self)
{
    return _self->col_tex.get() != NULL;
}
bool MaterialInstance_has_normal_texture(MaterialInstance _self)
{
    return _self->nor_tex.get() != NULL;
}

MaterialInstance MaterialInstance_clone(MaterialInstance _self)
{
    MaterialInstance ret;
    ///ret = (MaterialInstance)SMalloc(sizeof(material_instance));
	ret = ENEW material_instance;
    ///ret->material_id = _self->material_id;
    ret->mat_name = EString_new(_self->mat_name);
    ///ret.self->std_pass = SELF.std_pass;
    ///ret.self->vtx_dec = SELF.vtx_dec;
    ret->col_tex = _self->col_tex;
    ret->nor_tex = _self->nor_tex;
    return ret;
}
