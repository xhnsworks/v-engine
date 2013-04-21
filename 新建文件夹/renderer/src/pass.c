#include "pass.h"
#include "recycle_bin.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "float3.h"
#include "float4.h"
#include "array.h"
#include "tree.h"
static uint32 g_max_texture_units = 0;

typedef struct _param
{
    char* name;
    param_type type;
    uint array_size;
    vptr param_ptr;
} param;

typedef struct _param* Param;
Param Param_new()
{
    Param ret = Malloc(sizeof(param));
    memset(ret, 0, sizeof(param));
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
    uint32 count = 0;
    while (_name[count])
    {
        count++;
    }
    _self->name = Malloc(count + 1);
    memcpy(_self->name, _name, count + 1);
}

uint _single_param_size(param_type _type)
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
    case Int_Param:
        return sizeof(int);
    default:
        return 0;
    }
}

void _export_to_memory(vptr _objs, param_type _type, uint32 _cnt, vptr _mem)
{
    char* mem = (char*)_mem;
    uint ele_size = _single_param_size(_type);
    switch (_type)
    {
    case Matrix4x4_Param:
    {
        Matrix4x4* mats = (Matrix4x4*)_objs;
        for (uint i = 0; i < _cnt; i++)
        {
            Matrix4x4 m = mats[i];
            Matrix4x4_export(m, (float*)mem);
            mem += ele_size;
        }
    }
    break;
    case Float32_Param:
    {
        EFloat* fts = (EFloat*)_objs;
        for (uint i = 0; i < _cnt; i++)
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
        for (uint i = 0; i < _cnt; i++)
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
        for (uint i = 0; i < _cnt; i++)
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
        for (uint i = 0; i < _cnt; i++)
        {
            EFloat4 f = ft4s[i];
            *((EFloat4*)mem) = f;
            mem += ele_size;
        }
    }
    break;
    case Texture2D_Param:
    {
        Texture2D* texs = (Texture2D*)_objs;
        for (uint i = 0; i < _cnt; i++)
        {
            Texture2D t = texs[i];
            *((Texture2D*)mem) = t;
            mem += ele_size;
        }
    }
    case Int_Param:
    {
        int* ns = (int*)_objs;
        for (uint i = 0; i < _cnt; i++)
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

void Param_set(Param _self, const char* _name, vptr _pams, param_type _type, uint32 _cnt)
{
    Param_set_name(_self, _name);
    _self->type = _type;
    _self->param_ptr = Malloc(_single_param_size(_type) * _cnt);
    _self->array_size = _cnt;
    _export_to_memory(_pams, _type, _cnt, _self->param_ptr);
}

typedef struct _pass
{
    uint32 id;
    Shader vertex_shader;
    Shader pixel_shader;
    Stack param_table;
    Tree param_tree;
    Tree attr_tree;
    uint32 linked;
} pass;

void Pass_clear_param(Pass _self);

void Pass_delete(Pass _self)
{
    Shader_delete(_self.self->vertex_shader);
    Shader_delete(_self.self->pixel_shader);
    if (_self.self->id)
        glDeleteProgram(_self.self->id);

    Pass_clear_param(_self);

    Stack_Dest(_self.self->param_table);
    Tree_Dest(_self.self->param_tree);
    Tree_Dest(_self.self->attr_tree);
    Mfree(_self.self->param_table);
    Mfree(_self.self);
}

Pass Pass_new(void)
{
    Pass ret;
    ret.self = Malloc(sizeof(pass));
    memset(ret.self, 0, sizeof(pass));
    ret.self->param_table = Stack_new(Vptr);
    ret.self->param_tree = Tree_new(String, Uint32, Ealloc, Efree);
    ret.self->attr_tree = Tree_new(String, Uint32, Ealloc, Efree);
    ret.self->linked = 0;
    return ret;
}

void Pass_push_param(Pass _self, Param _pam)
{
    var v;
    v.vptr_var = _pam;
    Stack_push(_self.self->param_table, v);
}

void Pass_apply_param(Pass _self)
{
    if (!g_max_texture_units)
    {
        GLint num_tex_units;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &num_tex_units);
        g_max_texture_units = (uint32)num_tex_units;
    }
    uint32 tid = 0;
    for (uint32 i = 0; i < Stack_count(_self.self->param_table); i++)
    {
        var* v = Stack_find(_self.self->param_table, i);
        if (v)
        {
            Param pam = v->vptr_var;
            uint32 p = UINT32_EXEC;
            var key;
            var data;
            key.str_var = (vptr)pam->name;
            if (Tree_find(_self.self->param_tree, key, &data))
            {
                p = data.uint32_var;
            }
            else
            {
                p = glGetUniformLocation(_self.self->id, pam->name);
                data.uint32_var = p;
                Tree_insert(_self.self->param_tree, key, data);
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
                glActiveTexture(GL_TEXTURE0 + tid);
                Texture2D_bind(*((Texture2D*)pam->param_ptr));
                glUniform1i(p, tid);

                tid++;
            }
            break;
            case Matrix4x4_Param:
                glUniformMatrix4fv(p, pam->array_size, GL_FALSE, (float*)pam->param_ptr);
                break;
            break;
            case Float32_Param:
                glUniform1fv(p, pam->array_size, (float*)pam->param_ptr);
                break;
            case Float32x2_Param:
                glUniform2fv(p, pam->array_size, (float*)pam->param_ptr);
                break;
            case Float32x3_Param:
                glUniform3fv(p, pam->array_size, (float*)pam->param_ptr);
                break;
            case Float32x4_Param:
                glUniform4fv(p, pam->array_size, (float*)pam->param_ptr);
                break;
            case Int_Param:
                glUniform1iv(p, pam->array_size, (int*)pam->param_ptr);
                break;
            default:
                break;
            }

        }
    }
}

void Pass_clear_param(Pass _self)
{
    while (Stack_count(_self.self->param_table))
    {
        var tmp;
        Stack_pop(_self.self->param_table, &tmp);
        Param pam = tmp.vptr_var;
        Param_delete(pam);
    }
}

void Pass_set_vertex_shader(Pass _self, Shader _sdr)
{
    if ( Shader_get_type(_sdr) == VertexShader && Shader_get_id(_sdr) && !to_ptr(_self.self->vertex_shader) )
    {
        if (_self.self->id)
            glAttachShader(_self.self->id, Shader_get_id(_sdr));
        else
        {
            _self.self->id = glCreateProgram();
            glAttachShader(_self.self->id, Shader_get_id(_sdr));
        }

        _self.self->vertex_shader = _sdr;
    }
}

void Pass_set_pixel_shader(Pass _self, Shader _sdr)
{
    if (Shader_get_type(_sdr) == PixelShader && Shader_get_id(_sdr) && !to_ptr(_self.self->pixel_shader) )
    {
        if (_self.self->id)
            glAttachShader(_self.self->id, Shader_get_id(_sdr));
        else
        {
            _self.self->id = glCreateProgram();
            glAttachShader(_self.self->id, Shader_get_id(_sdr));
        }

        _self.self->pixel_shader = _sdr;
    }
}

uint32 Pass_get_attr_loc(Pass _self, const char* _attr_sem)
{
    var key;
    var data;
    key.str_var = (vptr)_attr_sem;
    if (Tree_find(_self.self->attr_tree, key, &data))
    {
        return data.uint32_var;
    }
    else
    {
        uint32 ret = glGetAttribLocation(_self.self->id, _attr_sem);
        data.uint32_var = ret;
        Tree_insert(_self.self->attr_tree, key, data);
        return ret;
    }
}

void Pass_render(Pass _self, VertexBuffer _vbf, IndexBuffer _ibf, uint32 _num_idxs)
{
    if (to_ptr(_self))
    {
        if(!_self.self->linked)
        {
            glLinkProgram(_self.self->id);
            _self.self->linked = 1;
        }

        glUseProgram(_self.self->id);
    }

    VertexDeclaration dec = VertexBuffer_get_vertex_declaration(_vbf);
    uint32 pos_loc = UINT32_EXEC;
    uint32 nor_loc = UINT32_EXEC;
    uint32 col_loc = UINT32_EXEC;
    uint32 tex_loc = UINT32_EXEC;
    uint32 tgt_loc = UINT32_EXEC;
    uint32 bin_loc = UINT32_EXEC;
    for (uint32 i = 0; i < VertexDeclaration_count(dec); i++)
    {
        VertexElement element = VertexDeclaration_find(dec, i);
        if (!VertexElement_is_null(element))
        {
            switch (VertexElement_get_semantic(element))
            {
            case Position:
                ///pos_loc = glGetAttribLocation(_self.self->id, "Position");
                pos_loc = Pass_get_attr_loc(_self, "Position");
                glEnableVertexAttribArray(pos_loc);
                break;
            case Normal:
                ///nor_loc = glGetAttribLocation(_self.self->id, "Normal");
                nor_loc = Pass_get_attr_loc(_self, "Normal");
                glEnableVertexAttribArray(nor_loc);
                break;
            case Color:
                ///col_loc = glGetAttribLocation(_self.self->id, "Color");
                col_loc = Pass_get_attr_loc(_self, "Color");
                glEnableVertexAttribArray(col_loc);
                break;
            case TexCoord:
                ///tex_loc = glGetAttribLocation(_self.self->id, "Texcoord");
                tex_loc = Pass_get_attr_loc(_self, "Texcoord");
                glEnableVertexAttribArray(tex_loc);
                break;
            case Tangent:
                ///tgt_loc = glGetAttribLocation(_self.self->id, "Tangent");
                tgt_loc = Pass_get_attr_loc(_self, "Tangent");
                glEnableVertexAttribArray(tgt_loc);
                break;
            case Binormal:
                ///bin_loc = glGetAttribLocation(_self.self->id, "Binormal");
                bin_loc = Pass_get_attr_loc(_self, "Binormal");
                glEnableVertexAttribArray(bin_loc);
                break;
            default:
                break;
            }
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer_get_id(_vbf));
    for (uint32 i = 0; i < VertexDeclaration_count(dec); i++)
    {
        VertexElement element = VertexDeclaration_find(dec, i);
        if (!VertexElement_is_null(element))
        {
            switch (VertexElement_get_semantic(element))
            {
            case Position:
                switch (VertexElement_get_type(element))
                {
                case Float32_3:
                    glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, VertexBuffer_get_vertex_size(_vbf), (GLvoid*)((ref_ptr)VertexElement_get_offset(element)) );
                    break;
                default:
                    break;
                }
                break;
            case Normal:
                switch (VertexElement_get_type(element))
                {
                case Float32_3:
                    glVertexAttribPointer(nor_loc, 3, GL_FLOAT, GL_FALSE, VertexBuffer_get_vertex_size(_vbf), (GLvoid*)((ref_ptr)VertexElement_get_offset(element)) );
                    break;
                default:
                    break;
                }
                break;
            case Color:
                switch (VertexElement_get_type(element))
                {
                case Ubyte_4:
                    glVertexAttribPointer(col_loc, 4, GL_UNSIGNED_BYTE, GL_FALSE, VertexBuffer_get_vertex_size(_vbf), (GLvoid*)((ref_ptr)VertexElement_get_offset(element)) );
                    break;
                case Float32_4:
                    glVertexAttribPointer(col_loc, 4, GL_FLOAT, GL_FALSE, VertexBuffer_get_vertex_size(_vbf), (GLvoid*)((ref_ptr)VertexElement_get_offset(element)) );
                    break;
                default:
                    break;
                }
                break;
            case TexCoord:
                switch (VertexElement_get_type(element))
                {
                case Float32_2:
                    glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, VertexBuffer_get_vertex_size(_vbf), (GLvoid*)((ref_ptr)VertexElement_get_offset(element)) );
                    break;
                default:
                    break;
                }
                break;
            case Tangent:
                switch (VertexElement_get_type(element))
                {
                case Float32_3:
                    glVertexAttribPointer(tgt_loc, 3, GL_FLOAT, GL_FALSE, VertexBuffer_get_vertex_size(_vbf), (GLvoid*)((ref_ptr)VertexElement_get_offset(element)) );
                    break;
                default:
                    break;
                }
                break;
            case Binormal:
                switch (VertexElement_get_type(element))
                {
                case Float32_3:
                    glVertexAttribPointer(bin_loc, 3, GL_FLOAT, GL_FALSE, VertexBuffer_get_vertex_size(_vbf), (GLvoid*)((ref_ptr)VertexElement_get_offset(element)) );
                    break;
                default:
                    break;
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
    VertexBuffer_buffer_data(_vbf);
    IndexBuffer_buffer_data(_ibf);
    glDrawElements(GL_TRIANGLES, _num_idxs, GL_UNSIGNED_INT, 0);
    Texture2D_unbind();
    if (to_ptr(_self))
    {
        Pass_clear_param(_self);
    }

    for (uint32 i = 0; i < VertexDeclaration_count(dec); i++)
    {
        VertexElement element = VertexDeclaration_find(dec, i);
        {
            switch (VertexElement_get_semantic(element))
            {
            case Position:
                glDisableVertexAttribArray(pos_loc);
                break;
            case Normal:
                glDisableVertexAttribArray(nor_loc);
                break;
            case Color:
                glDisableVertexAttribArray(col_loc);
                break;
            case TexCoord:
                glDisableVertexAttribArray(tex_loc);
            default:
                break;
            }
        }
    }
}

void Pass_render_plane(Pass _self, RenderablePlane _pln)
{
    VertexBuffer vbf = RenderablePlane_get_vertex_buffer(_pln);
    IndexBuffer ibf = RenderablePlane_get_index_buffer(_pln);
    Pass_render(_self, vbf, ibf, 6);
}

void Pass_set_uniform_param_tex2d(Pass _self, const char* _name, Texture2D _tex)
{
    Param pam = Param_new();
    Param_set(pam, _name, &_tex, Texture2D_Param, 1);
    Pass_push_param(_self, pam);
}

void Pass_set_uniform_param_float(Pass _self, const char* _name, EFloat _ft)
{
    Param pam = Param_new();
    Param_set(pam, _name, &_ft, Float32_Param, 1);
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

void Pass_set_uniform_param_mat4x4(Pass _self, const char* _name, Matrix4x4 _mat)
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

void Pass_set_uniform_param_floats(Pass _self, const char* _name, EFloat* _fts, uint _cnt)
{
    Param pam = Param_new();
    Param_set(pam, _name, _fts, Float32_Param, _cnt);
    Pass_push_param(_self, pam);
}
void Pass_set_uniform_param_float3s(Pass _self, const char* _name, EFloat3* _ft3s, uint _cnt)
{
    Param pam = Param_new();
    Param_set(pam, _name, _ft3s, Float32x3_Param, _cnt);
    Pass_push_param(_self, pam);
}
void Pass_set_uniform_param_float4s(Pass _self, const char* _name, EFloat4* _ft4s, uint _cnt)
{
    Param pam = Param_new();
    Param_set(pam, _name, _ft4s, Float32x4_Param, _cnt);
    Pass_push_param(_self, pam);
}
extern void Pass_set_uniform_param_mat4x4s(Pass _self, const char* _name, Matrix4x4* _mats, uint _cnt)
{
    Param pam = Param_new();
    Param_set(pam, _name, _mats, Matrix4x4_Param, _cnt);
    Pass_push_param(_self, pam);
}
