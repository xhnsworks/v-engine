#include "pch.h"
#include "pass.h"
///#include <GL/gl.h>
#include "array.h"
#include "tree.h"
#include "gl_utility.h"
#include "shader_object.h"
#include "eassert.h"
#include "renderer.h"

static euint32 g_max_texture_units = 0;

typedef struct _param
{
    char* name;
    param_type type;
    euint32 array_size;
    vptr param_ptr;
} param;

typedef struct _param* Param;
Param Param_new()
{
    Param ret = (Param)SMalloc(sizeof(param));
    ///memset(ret, 0, sizeof(param));
    return ret;
}

void Param_delete(Param _self)
{
    if (_self->name)
        Mfree(_self->name);
    if (_self->param_ptr)
        Mfree(_self->param_ptr);
    Mfree(_self);
}

void Param_set_name(Param _self, const char* _name)
{
    euint32 count = 0;
    while (_name[count])
    {
        count++;
    }
    _self->name = (char*)SMalloc(count + 1);
    memcpy(_self->name, _name, count + 1);
}

euint _single_param_size(param_type _type)
{
    switch (_type)
    {
    case Matrix4x4_Param:
        return sizeof(float) * 16;
    case Float32_Param:
        return sizeof(EFloat);
    case Float32x2_Param:
        return sizeof(EFloat2);
    case Float32x3_Param:
        return sizeof(EFloat3);
    case Float32x4_Param:
        return sizeof(EFloat4);
    case Texture2D_Param:
        return sizeof(Texture2D);
    case TextureCube_Param:
        return sizeof(TextureCube);
    case Int_Param:
        return sizeof(int);
    case Bool_Param:
        return sizeof(bool);
    default:
        return 0;
    }
}

void _export_to_memory(vptr _objs, param_type _type, euint32 _cnt, vptr _mem)
{
    char* mem = (char*)_mem;
    euint ele_size = _single_param_size(_type);
    switch (_type)
    {
    case Matrix4x4_Param:
    {
        matrix4x4** mats = (matrix4x4**)_objs;
        for (euint i = 0; i < _cnt; i++)
        {
            matrix4x4* m = mats[i];
            Matrix4x4_export(m, (float*)mem);
            mem += ele_size;
        }
    }
    break;
    case Float32_Param:
    {
        EFloat* fts = (EFloat*)_objs;
        for (euint i = 0; i < _cnt; i++)
        {
            EFloat f = fts[i];
            *((EFloat*)mem) = f;
            mem += ele_size;
        }
    }
    break;
    case Float32x2_Param:
    {
        EFloat2* ft2s = (EFloat2*)_objs;
        for (euint i = 0; i < _cnt; i++)
        {
            EFloat2 f = ft2s[i];
            *((EFloat2*)mem) = f;
            mem += ele_size;
        }
    }
    break;
    case Float32x3_Param:
    {
        EFloat3* ft3s = (EFloat3*)_objs;
        for (euint i = 0; i < _cnt; i++)
        {
            EFloat3 f = ft3s[i];
            *((EFloat3*)mem) = f;
            mem += ele_size;
        }
    }
    break;
    case Float32x4_Param:
    {
        EFloat4* ft4s = (EFloat4*)_objs;
        for (euint i = 0; i < _cnt; i++)
        {
            EFloat4 f = ft4s[i];
            *((EFloat4*)mem) = f;
            mem += ele_size;
        }
    }
    break;
    case Texture2D_Param:
    {
        Texture2D** texs = (Texture2D**)_objs;
        for (euint i = 0; i < _cnt; i++)
        {
            Texture2D* t = texs[i];
            *((Texture2D**)mem) = t;
            mem += ele_size;
        }
    }
    case TextureCube_Param:
    {
        TextureCube* texs = (TextureCube*)_objs;
        for (euint i = 0; i < _cnt; i++)
        {
            TextureCube t = texs[i];
            *((TextureCube*)mem) = t;
            mem += ele_size;
        }
    }
    case Int_Param:
    {
        int* ns = (int*)_objs;
        for (euint i = 0; i < _cnt; i++)
        {
            int n = ns[i];
            *((int*)mem) = n;
            mem += ele_size;
        }
    }
    break;
    default:
        break;
    }
}

void Param_set(Param _self, const char* _name, vptr _pams, param_type _type, euint32 _cnt)
{
    Param_set_name(_self, _name);
    _self->type = _type;
    _self->param_ptr = SMalloc(_single_param_size(_type) * _cnt);
    _self->array_size = _cnt;
    _export_to_memory(_pams, _type, _cnt, _self->param_ptr);
}

typedef struct _pass
{
    euint32 id;
    Shader vertex_shader;
    Shader pixel_shader;
    Stack param_table;
    Tree param_tree;
    Tree attr_tree;
    Tree vertex_param_source_tree;
    Tree pixel_param_source_tree;
    euint32 linked;
} pass;

void Pass_clear_param(Pass _self);

void Pass_delete(Pass _self)
{
    Shader_delete(_self->vertex_shader);
    Shader_delete(_self->pixel_shader);
    if (_self->id)
        glDeleteProgram(_self->id);

    Pass_clear_param(_self);

    Stack_Dest(_self->param_table);
    Tree_Dest(_self->param_tree);
    Tree_Dest(_self->attr_tree);
    Mfree(_self->param_table);
    Mfree(_self);

    if (_self->vertex_param_source_tree)
    {
        Iterator iter = Tree_begin(_self->vertex_param_source_tree);
        while (iter)
        {
            var data = Tree_get_value(iter);
            ShaderObject obj = {(struct _shader_object*)data.vptr_var};
            ShaderObject_delete(obj);
            iter = Tree_next(iter);
        }
        Tree_Dest(_self->vertex_param_source_tree);
    }

    if (_self->pixel_param_source_tree)
    {
        Iterator iter = Tree_begin(_self->pixel_param_source_tree);
        while (iter)
        {
            var data = Tree_get_value(iter);
            ShaderObject obj = {(struct _shader_object*)data.vptr_var};
            ShaderObject_delete(obj);
            iter = Tree_next(iter);
        }
        Tree_Dest(_self->pixel_param_source_tree);
    }
}

Pass Pass_new(void)
{
    Pass ret;
    ret = (Pass)SMalloc(sizeof(pass));
    memset(ret, 0, sizeof(pass));
    ret->param_table = Stack_new(Vptr);
    ret->param_tree = Tree_new(String, Uint32, (MALLOC)Ealloc, (MFREE)Efree);
    ret->attr_tree = Tree_new(String, Uint32, (MALLOC)Ealloc, (MFREE)Efree);
    ret->vertex_param_source_tree = NULL;
    ret->pixel_param_source_tree = NULL;
    ret->linked = 0;
    return ret;
}

void Pass_push_param(Pass _self, Param _pam)
{
    var v;
    v.vptr_var = _pam;
    Stack_push(_self->param_table, v);
}

void Pass_apply_param(Pass _self)
{
    if (!g_max_texture_units)
    {
        GLint num_tex_units;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &num_tex_units);
        g_max_texture_units = (euint32)num_tex_units;
    }
    ERROR_PROC;
    euint32 tid = 0;
    for (euint32 i = 0; i < Stack_count(_self->param_table); i++)
    {
        var* v = Stack_find(_self->param_table, i);
        if (v)
        {
            Param pam = (Param)v->vptr_var;
            euint32 p = UINT32_EXCE;
            var key;
            var data;
            key.str_var = (EString)pam->name;
            if (Tree_find(_self->param_tree, key, &data))
            {
                p = data.uint32_var;
            }
            else
            {
                p = glGetUniformLocation(_self->id, pam->name);
                data.uint32_var = p;
                Tree_insert(_self->param_tree, key, data);
            }

            switch (pam->type)
            {
            case Texture2D_Param:
            {
                if (tid > g_max_texture_units)
                {
                    ///
                }
                ///glActiveTextureARB(GL_TEXTURE0_ARB + tid);
                ERROR_PROC;
                glActiveTexture(GL_TEXTURE0 + tid);
                ///Texture2D_bind();
				Texture2D* tex = *((Texture2D**)pam->param_ptr);
				tex->Bind();
                glUniform1i(p, tid);
                ERROR_PROC;

                tid++;
            }
            break;
            case TextureCube_Param:
            {
                if (tid > g_max_texture_units)
                {
                    ///
                }
                ERROR_PROC;
                glActiveTexture(GL_TEXTURE0 + tid);
                TextureCube_bind(*((TextureCube*)pam->param_ptr));
                glUniform1i(p, tid);
                ERROR_PROC;

                tid++;
            }
            break;
            case Matrix4x4_Param:
                {
                    glUniformMatrix4fv(p, pam->array_size, GL_FALSE, (float*)pam->param_ptr);
                    ERROR_PROC;
                }
                break;
            case Float32_Param:
                {
                    glUniform1fv(p, pam->array_size, (float*)pam->param_ptr);
                    ERROR_PROC;
                }
                break;
            case Float32x2_Param:
                {
                    glUniform2fv(p, pam->array_size, (float*)pam->param_ptr);
                    ERROR_PROC;
                }
                break;
            case Float32x3_Param:
                {
                    glUniform3fv(p, pam->array_size, (float*)pam->param_ptr);
                    ERROR_PROC;
                }
                break;
            case Float32x4_Param:
                {
                    glUniform4fv(p, pam->array_size, (float*)pam->param_ptr);
                    ERROR_PROC;
                }
                break;
            case Int_Param:
                {
                    glUniform1iv(p, pam->array_size, (int*)pam->param_ptr);
                    ERROR_PROC;
                }
                break;
            default:
                break;
            }

        }
    }
    ERROR_PROC;
}

void Pass_clear_param(Pass _self)
{
    while (Stack_count(_self->param_table))
    {
        var tmp;
        Stack_pop(_self->param_table, &tmp);
        Param pam = (Param)tmp.vptr_var;
        Param_delete(pam);
    }
}

void Pass_set_vertex_shader(Pass _self, Shader _sdr)
{
    if ( Shader_get_type(_sdr) == VertexShader && Shader_get_id(_sdr) && !to_ptr(_self->vertex_shader) )
    {
        if (_self->id)
            glAttachShader(_self->id, Shader_get_id(_sdr));
        else
        {
            _self->id = glCreateProgram();
            glAttachShader(_self->id, Shader_get_id(_sdr));
        }

        _self->vertex_shader = _sdr;
    }
}

void Pass_set_pixel_shader(Pass _self, Shader _sdr)
{
    if (Shader_get_type(_sdr) == PixelShader && Shader_get_id(_sdr) && !to_ptr(_self->pixel_shader) )
    {
        if (_self->id)
            glAttachShader(_self->id, Shader_get_id(_sdr));
        else
        {
            _self->id = glCreateProgram();
            glAttachShader(_self->id, Shader_get_id(_sdr));
        }

        _self->pixel_shader = _sdr;
    }
}

euint32 Pass_get_attr_loc(Pass _self, const char* _attr_sem)
{
    var key;
    var data;
    key.str_var = (EString)_attr_sem;
    if (Tree_find(_self->attr_tree, key, &data))
    {
        return data.uint32_var;
    }
    else
    {
        euint32 ret = glGetAttribLocation(_self->id, _attr_sem);
        data.uint32_var = ret;
        Tree_insert(_self->attr_tree, key, data);
        return ret;
    }
}

void _Pass_render(Pass _self, VertexBuffer _vbf, IndexBuffer _ibf, euint32 _num_idxs, e_mesh_mode _mode, const char* _file, euint _line)
{
    if (to_ptr(_self))
    {
        if(!_self->linked)
        {
            glLinkProgram(_self->id);
            program_log(_self->id);
            ERROR_PROC;
            _self->linked = 1;
        }

        glUseProgram(_self->id);
        ERROR_PROC;
    }

    VertexDecl dec = VertexBuffer_get_vertex_declaration(_vbf);
    euint32* vtx_ele_offs = VertexBuffer_get_ele_offs(_vbf);
    euint32 pos_loc = UINT32_EXCE;
    euint32 nor_loc = UINT32_EXCE;
    euint32 col_loc = UINT32_EXCE;
    euint32 tex_loc = UINT32_EXCE;
    euint32 tgt_loc = UINT32_EXCE;
    euint32 bin_loc = UINT32_EXCE;
    ///euint32 epw_loc = UINT32_EXCE;
    for (euint32 i = 0; i < VertexDecl_count(dec); i++)
    {
        VertexElement element = VertexDecl_find(dec, i);
        if (!VertexElement_is_null(element))
        {
            switch (VertexElement_get_semantic(element))
            {
            case Position:
                pos_loc = Pass_get_attr_loc(_self, "Position");
                ERROR_PROC;
                if (pos_loc != UINT32_EXCE)
                {
                    glEnableVertexAttribArray(pos_loc);
                    ERROR_PROC;
                }
                break;
            case Normal:
                nor_loc = Pass_get_attr_loc(_self, "Normal");
                ERROR_PROC;
                if (nor_loc != UINT32_EXCE)
                {
                    glEnableVertexAttribArray(nor_loc);
                    ERROR_PROC;
                }
                break;
            case Color:
                col_loc = Pass_get_attr_loc(_self, "Color");
                ERROR_PROC;
                if (col_loc != UINT32_EXCE)
                {
                    glEnableVertexAttribArray(col_loc);
                    ERROR_PROC;
                }
                break;
            case TexCoord:
                tex_loc = Pass_get_attr_loc(_self, "TexCoord");
                ERROR_PROC;
                if (tex_loc != UINT32_EXCE)
                {
                    glEnableVertexAttribArray(tex_loc);
                    ERROR_PROC;
                }
                break;
            case Tangent:
                tgt_loc = Pass_get_attr_loc(_self, "Tangent");
                ERROR_PROC;
                if (tgt_loc != UINT32_EXCE)
                {
                    glEnableVertexAttribArray(tgt_loc);
                    ERROR_PROC;
                }
                break;
            case Binormal:
                bin_loc = Pass_get_attr_loc(_self, "Binormal");
                ERROR_PROC;
                if (bin_loc != UINT32_EXCE)
                {
                    glEnableVertexAttribArray(bin_loc);
                    ERROR_PROC;
                }
                break;
            default:
                break;
            }
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer_get_id(_vbf));
    for (euint32 i = 0; i < VertexDecl_count(dec); i++)
    {
        VertexElement element = VertexDecl_find(dec, i);
        if (!VertexElement_is_null(element))
        {
            switch (VertexElement_get_semantic(element))
            {
            case Position:
                if (pos_loc != UINT32_EXCE)
                {
                    switch (VertexElement_get_type(element))
                    {
                    case Float32_3:
                        glVertexAttribPointer(pos_loc,
                                              3,
                                              GL_FLOAT,
                                              GL_FALSE,
                                              VertexBuffer_get_vertex_size(_vbf),
                                              (GLvoid*)((ref_ptr)vtx_ele_offs[i]) );
                        ERROR_PROC;
                        break;
                    default:
                        break;
                    }
                }
                break;
            case Normal:
                if (nor_loc != UINT32_EXCE)
                {
                    switch (VertexElement_get_type(element))
                    {
                    case Float32_3:
                        glVertexAttribPointer(nor_loc,
                                              3,
                                              GL_FLOAT,
                                              GL_FALSE,
                                              VertexBuffer_get_vertex_size(_vbf),
                                              (GLvoid*)((ref_ptr)vtx_ele_offs[i]) );
                        ERROR_PROC;
                        break;
                    default:
                        break;
                    }
                }
                break;
            case Color:
                if (col_loc != UINT32_EXCE)
                {
                    switch (VertexElement_get_type(element))
                    {
                    case Ubyte_4:
                        glVertexAttribPointer(col_loc,
                                              4,
                                              GL_UNSIGNED_BYTE,
                                              GL_FALSE,
                                              VertexBuffer_get_vertex_size(_vbf),
                                              (GLvoid*)((ref_ptr)vtx_ele_offs[i]) );
                        ERROR_PROC;
                        break;
                    case Float32_4:
                        glVertexAttribPointer(col_loc,
                                              4,
                                              GL_FLOAT,
                                              GL_FALSE,
                                              VertexBuffer_get_vertex_size(_vbf),
                                              (GLvoid*)((ref_ptr)vtx_ele_offs[i]) );
                        ERROR_PROC;
                        break;
                    default:
                        break;
                    }
                }
                break;
            case TexCoord:
                if (tex_loc != UINT32_EXCE)
                {
                    switch (VertexElement_get_type(element))
                    {
                    case Float32_2:
                        glVertexAttribPointer(tex_loc,
                                              2,
                                              GL_FLOAT,
                                              GL_FALSE,
                                              VertexBuffer_get_vertex_size(_vbf),
                                              (GLvoid*)((ref_ptr)vtx_ele_offs[i]) );
                        ERROR_PROC;
                        break;
                    default:
                        break;
                    }
                }
                break;
            case Tangent:
                if (tgt_loc != UINT32_EXCE)
                {
                    switch (VertexElement_get_type(element))
                    {
                    case Float32_3:
                        glVertexAttribPointer(tgt_loc,
                                              3,
                                              GL_FLOAT,
                                              GL_FALSE,
                                              VertexBuffer_get_vertex_size(_vbf),
                                              (GLvoid*)((ref_ptr)vtx_ele_offs[i]) );
                        ERROR_PROC;
                        break;
                    default:
                        break;
                    }
                }
                break;
            case Binormal:
                if (bin_loc != UINT32_EXCE)
                {
                    switch (VertexElement_get_type(element))
                    {
                    case Float32_3:
                        glVertexAttribPointer(bin_loc,
                                              3,
                                              GL_FLOAT,
                                              GL_FALSE,
                                              VertexBuffer_get_vertex_size(_vbf),
                                              (GLvoid*)((ref_ptr)vtx_ele_offs[i]) );
                        ERROR_PROC;
                        break;
                    default:
                        break;
                    }
                }
                break;
            default:
                break;
            }
        }
    }
    /// draw
    if (to_ptr(_self))
    {
        Pass_apply_param(_self);
    }

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBuffer_get_id(_ibf) );
    ERROR_PROC;
    VertexBuffer_buffer_data(_vbf);
    IndexBuffer_buffer_data(_ibf);

	glDepthRange(0.0f, 1.0f);

    if (_mode == Triangular)
        glDrawElements(GL_TRIANGLES, _num_idxs, GL_UNSIGNED_INT, 0);
    else
        glDrawElements(GL_LINES, _num_idxs, GL_UNSIGNED_INT, 0);

    ERROR_PROC;
    ///Texture2D_unbind();
	Texture2D::Unbind();
    if (to_ptr(_self))
    {
        Pass_clear_param(_self);
    }

    for (euint32 i = 0; i < VertexDecl_count(dec); i++)
    {
        VertexElement element = VertexDecl_find(dec, i);
        {
            switch (VertexElement_get_semantic(element))
            {
            case Position:
                if (pos_loc != UINT32_EXCE)
                {
                    glDisableVertexAttribArray(pos_loc);
                    ERROR_PROC;
                }
                break;
            case Normal:
                if (nor_loc != UINT32_EXCE)
                {
                    glDisableVertexAttribArray(nor_loc);
                    ERROR_PROC;
                }
                break;
            case Color:
                if (col_loc != UINT32_EXCE)
                {
                    glDisableVertexAttribArray(col_loc);
                    ERROR_PROC;
                }
                break;
            case TexCoord:
                if (tex_loc != UINT32_EXCE)
                {
                    glDisableVertexAttribArray(tex_loc);
                    ERROR_PROC;
                }
                break;
            case Tangent:
                if (tgt_loc != UINT32_EXCE)
                {
                    glDisableVertexAttribArray(tgt_loc);
                    ERROR_PROC;
                }
                break;
            case Binormal:
                if (bin_loc != UINT32_EXCE)
                {
                    glDisableVertexAttribArray(bin_loc);
                    ERROR_PROC;
                }
                break;
            default:
                break;
            }
        }
    }
}

void _Pass_render_plane(Pass _self, RenderablePlane _pln, const char* _file, euint _line)
{
    VertexBuffer vbf = RenderablePlane_get_vertex_buffer(_pln);
    IndexBuffer ibf = RenderablePlane_get_index_buffer(_pln);
    _Pass_render(_self, vbf, ibf, 6, Triangular, _file, _line);
}

void Pass_set_uniform_param_tex2d(Pass _self, const char* _name, Texture2DPtr _tex)
{
    Param pam = Param_new();
	Texture2D* tex = _tex.get();
    Param_set(pam, _name, &tex, Texture2D_Param, 1);
    Pass_push_param(_self, pam);
}
void Pass_set_uniform_param_texcube(Pass _self, const char* _name, TextureCube _tex)
{
    Param pam = Param_new();
    Param_set(pam, _name, &_tex, TextureCube_Param, 1);
    Pass_push_param(_self, pam);
}
void Pass_set_uniform_param_float(Pass _self, const char* _name, EFloat _ft)
{
    Param pam = Param_new();
    Param_set(pam, _name, &_ft, Float32_Param, 1);
    Pass_push_param(_self, pam);
}

void Pass_set_uniform_param_float2(Pass _self, const char* _name, EFloat2 _ft2)
{
    Param pam = Param_new();
    Param_set(pam, _name, &_ft2, Float32x2_Param, 1);
    Pass_push_param(_self, pam);
}

void Pass_set_uniform_param_float3(Pass _self, const char* _name, EFloat3 _ft3)
{
    Param pam = Param_new();
    Param_set(pam, _name, &_ft3, Float32x3_Param, 1);
    Pass_push_param(_self, pam);
}

void Pass_set_uniform_param_float4(Pass _self, const char* _name, EFloat4 _ft4)
{
    Param pam = Param_new();
    Param_set(pam, _name, &_ft4, Float32x4_Param, 1);
    Pass_push_param(_self, pam);
}

void Pass_set_uniform_param_mat4x4(Pass _self, const char* _name, matrix4x4* _mat)
{
    Param pam = Param_new();
    Param_set(pam, _name, &_mat, Matrix4x4_Param, 1);
    Pass_push_param(_self, pam);
}

void Pass_set_uniform_param_int(Pass _self, const char* _name, int _n)
{
    Param pam = Param_new();
    Param_set(pam, _name, &_n, Int_Param, 1);
    Pass_push_param(_self, pam);
}

void Pass_set_uniform_param_ints(Pass _self, const char* _name, int* _ns, euint32 _cnt)
{
    Param pam = Param_new();
    Param_set(pam, _name, _ns, Int_Param, _cnt);
    Pass_push_param(_self, pam);
}
void Pass_set_uniform_param_floats(Pass _self, const char* _name, EFloat* _fts, euint32 _cnt)
{
    Param pam = Param_new();
    Param_set(pam, _name, _fts, Float32_Param, _cnt);
    Pass_push_param(_self, pam);
}
void Pass_set_uniform_param_float2s(Pass _self, const char* _name, EFloat2* _ft2s, euint32 _cnt)
{
    Param pam = Param_new();
    Param_set(pam, _name, _ft2s, Float32x2_Param, _cnt);
    Pass_push_param(_self, pam);
}
void Pass_set_uniform_param_float3s(Pass _self, const char* _name, EFloat3* _ft3s, euint32 _cnt)
{
    Param pam = Param_new();
    Param_set(pam, _name, _ft3s, Float32x3_Param, _cnt);
    Pass_push_param(_self, pam);
}
void Pass_set_uniform_param_float4s(Pass _self, const char* _name, EFloat4* _ft4s, euint32 _cnt)
{
    Param pam = Param_new();
    Param_set(pam, _name, _ft4s, Float32x4_Param, _cnt);
    Pass_push_param(_self, pam);
}
void Pass_set_uniform_param_mat4x4s(Pass _self, const char* _name, matrix4x4** _mats, euint32 _cnt)
{
    Param pam = Param_new();
    Param_set(pam, _name, _mats, Matrix4x4_Param, _cnt);
    Pass_push_param(_self, pam);
}

void Pass_buy_vertex_param_source_tree(Pass _self, Tree _vtx_param_src_tree)
{
    if (!_self->vertex_param_source_tree)
        _self->vertex_param_source_tree = _vtx_param_src_tree;
}
void Pass_buy_pixel_param_source_tree(Pass _self, Tree _pxl_param_src_tree)
{
    if (!_self->pixel_param_source_tree)
        _self->pixel_param_source_tree = _pxl_param_src_tree;
}

void _auto_set_uniform_params(Tree _param_source_tree, Pass _self, RendererBase* _rdr, bool _write_log)
{
	if (_param_source_tree)
	{
		Iterator iter = Tree_begin(_param_source_tree);
		while (iter)
		{
			var key = Tree_get_key(iter);
			var data = Tree_get_value(iter);
			param_source src = (param_source)key.uint32_var;
			if (_rdr->is_uniform_param_source(src))
			{
				renderer_param_value shader_object_value = _rdr->get_shader_object_value(src);
				ShaderObject obj = {(struct _shader_object*)data.vptr_var};
				euint32 array_size, array_index;
				shader_object_type type = ShaderObject_get_type(obj, &array_size, &array_index);
				EAssert(array_size > 0, "%s", "param array size less equal zero!");
				const char* name = ShaderObject_get_name(obj);
				switch (type)
				{
				case Int_Obj:
					if (array_size == 1)
					{
						int* p = (int*)shader_object_value.value;
						Pass_set_uniform_param_int(_self, name, *p);
					}
					else
					{
						int* p = (int*)shader_object_value.value;
						Pass_set_uniform_param_ints(_self, name, p, array_size);
						if (_write_log)
						{
							const char* src_str = _get_param_src_str(src);
							elog("##source %s", src_str);
							elog("%d", *p);
						}
					}
					break;
				case Float_Obj:
					if (array_size == 1)
					{
						EFloat* p = (EFloat*)shader_object_value.value;
						Pass_set_uniform_param_float(_self, name, *p);
						if (_write_log)
						{
							const char* src_str = _get_param_src_str(src);
							elog("##source %s", src_str);
							elog("%f", p->x);
						}
					}
					else
					{
						EFloat* p = (EFloat*)shader_object_value.value;
						Pass_set_uniform_param_floats(_self, name, p, array_size);
						if (_write_log)
						{
							const char* src_str = _get_param_src_str(src);
							elog("##source %s", src_str);
							elog("%f", p->x);
						}
					}
					break;
				case Float2_Obj:
					if (array_size == 1)
					{
						///EFloat2* p = shader_object_value;
						///Pass_set_uniform_param_float2(_self, name, *p);
						EFloat2* p = (EFloat2*)shader_object_value.value;
						Pass_set_uniform_param_float2(_self, name, *p);
						if (_write_log)
						{
							const char* src_str = _get_param_src_str(src);
							elog("##source %s", src_str);
							elog("%f %f", p->x, p->y);
						}
					}
					else
					{
						EFloat2* p = (EFloat2*)shader_object_value.value;
						Pass_set_uniform_param_float2s(_self, name, p, array_size);
						if (_write_log)
						{
							const char* src_str = _get_param_src_str(src);
							elog("##source %s", src_str);
							elog("%f %f", p->x, p->y);
						}
					}
					break;
				case Float3_Obj:
					if (array_size == 1)
					{
						EFloat3* p = (EFloat3*)shader_object_value.value;
						Pass_set_uniform_param_float3(_self, name, *p);
						if (_write_log)
						{
							const char* src_str = _get_param_src_str(src);
							elog("##source %s", src_str);
							elog("%f %f %f", p->x, p->y, p->z);
						}
					}
					else
					{
						EFloat3* p = (EFloat3*)shader_object_value.value;
						Pass_set_uniform_param_float3s(_self, name, p, array_size);
						if (_write_log)
						{
							const char* src_str = _get_param_src_str(src);
							elog("##source %s", src_str);
							elog("%f %f %f", p->x, p->y, p->z);
						}
					}
					break;
				case Float4_Obj:
					if (array_size == 1)
					{
						EFloat4* p = (EFloat4*)shader_object_value.value;
						Pass_set_uniform_param_float4(_self, name, *p);
					}
					else
					{
						EFloat4* p = (EFloat4*)shader_object_value.value;
						Pass_set_uniform_param_float4s(_self, name, p, array_size);
					}
					break;
				case Texture2D_Obj:
					if (array_size == 1)
					{
						Texture2D** p = (Texture2D**)shader_object_value.value;
						if (*p)
						{
							Pass_set_uniform_param_tex2d(_self, name, *p);
						}
					}
					else
					{
						///Texture2D* p = shader_object_value;
						///Pass_set_uniform_param_tex2ds(_self, name, p, array_size);
					}
					break;
				case TextureCube_Obj:
					if (array_size == 1)
					{
						TextureCube* p = (TextureCube*)shader_object_value.value;
						if (*p)
						{
							Pass_set_uniform_param_texcube(_self, name, *p);
						}
					}
					else
					{
						///Texture2D* p = shader_object_value;
						///Pass_set_uniform_param_tex2ds(_self, name, p, array_size);
					}
					break;
				case Texture1D_Obj:
					if (array_size == 1)
					{
						///
					}
					else
					{
						///
					}
					break;
				case Matrix4x4_Obj:
					if (array_size == 1)
					{
						matrix4x4* p = (matrix4x4*)shader_object_value.value;
						Pass_set_uniform_param_mat4x4(_self, name, p);
						if (_write_log)
						{
							const char* src_str = _get_param_src_str(src);
							elog("##source %s", src_str);
							Matrix4x4_log(p);
						}
					}
					else
					{
						/// 这里可能有bug
						matrix4x4** p = (matrix4x4**)shader_object_value.value;
						Pass_set_uniform_param_mat4x4s(_self, name, p, array_size);
						if (_write_log)
						{
							///const char* src_str = _get_param_src_str(src);
							///elog("##source %s", src_str);
							///Matrix4x4_log(p);
						}
					}
					break;
				case Bool_Obj:
					if (array_size == 1)
					{
						///
					}
					else
					{
						///
					}
					break;
				default:
					break;
				}
				shader_object_value_Dest(shader_object_value);
			}
			iter = Tree_next(iter);
		}
	}
}
void Pass_auto_set_uniform_params(Pass _self, RendererBase* _rdr, bool _write_log)
{
    ///_write_log = true;

    _auto_set_uniform_params(_self->vertex_param_source_tree, _self, _rdr, _write_log);
    _auto_set_uniform_params(_self->pixel_param_source_tree, _self, _rdr, _write_log);
}
