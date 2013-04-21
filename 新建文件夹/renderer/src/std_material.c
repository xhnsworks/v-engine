#include "std_material.h"
#include "texture_console.h"
#include "array.h"
typedef struct _std_material
{
    VertexDeclaration vtx_dec;
    Pass std_pass;
    Texture2D col_tex;
    Texture2D nor_tex;
} std_material;

StdMaterial StdMaterial_new(VertexDeclaration _dec, const char* _col_tex_file, const char* _nor_tex_file)
{
    StdMaterial ret;
    ret.self = Malloc(sizeof(std_material));
    VertexDeclaration dec = _dec;

    bool has_col_map = false;
    bool has_nor_map = false;
    if (_col_tex_file && strcmp(_col_tex_file, "") != 0)
        has_col_map = true;
    if (_nor_tex_file && strcmp(_nor_tex_file, "") != 0)
        has_nor_map = true;

    std_pass_status status;
    status.has_color_map = has_col_map;
    status.has_normal_map = has_nor_map;
    status.num_render_pipes = 3;
    status.render_pipes[0].out = ColorOutput;
    status.render_pipes[1].out = NormalDepthOutput;
    status.render_pipes[2].out = PositionOutput;
    status.render_pipes[0].tgt = RenderTarget0;
    status.render_pipes[1].tgt = RenderTarget1;
    status.render_pipes[2].tgt = RenderTarget2;
    ret.self->std_pass = create_std_pass_from_dec(dec, &status);

    ret.self->vtx_dec = dec;
    Texture2D null_tex = {NULL};
    if (has_col_map)
        ret.self->col_tex = create_texture_from_file(_col_tex_file);
    else
        ret.self->col_tex = null_tex;

    if (has_nor_map)
        ret.self->nor_tex = create_texture_from_file(_nor_tex_file);
    else
        ret.self->nor_tex = null_tex;

    return ret;
}

void StdMaterial_delete(StdMaterial _self)
{
    Pass_delete(_self.self->std_pass);
    if (to_ptr(_self.self->col_tex))
        Texture2D_delete(_self.self->col_tex);
    if (to_ptr(_self.self->nor_tex))
        Texture2D_delete(_self.self->nor_tex);

    Mfree(_self.self);
}

Pass StdMaterial_get_std_pass(StdMaterial _self)
{
    return _self.self->std_pass;
}

Texture2D StdMaterial_get_color_texture(StdMaterial _self)
{
    return _self.self->col_tex;
}

Texture2D StdMaterial_get_normal_texture(StdMaterial _self)
{
    return _self.self->nor_tex;
}

bool StdMaterial_has_color_texture(StdMaterial _self)
{
    return to_ptr(_self.self->col_tex) != NULL;
}
bool StdMaterial_has_normal_texture(StdMaterial _self)
{
    return to_ptr(_self.self->nor_tex) != NULL;
}
