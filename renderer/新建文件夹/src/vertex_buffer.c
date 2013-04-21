#include <GL/glew.h>
#include <GL/gl.h>
#include "vertex_buffer.h"
#include "vertex_declaration.h"
#include "eassert.h"
#include "mem.h"
#include "renderable_base.h"
typedef struct _vertex_buffer
{
    uint32 id;
    uint* vtx_ele_offs;
    VertexDecl vertex_declaration;
    char* vertex_buffer;
    uint32 vertex_size;
    uint32 vertex_buffer_size;
    uint32 vertex_buffer_tail;
    uint32 dirty_flag;
} vertex_buffer;

void VertexBuffer_Dest(struct _vertex_buffer* _buf)
{
    glDeleteBuffers(1, &_buf->id);
    array_delete(_buf->vtx_ele_offs);
    Mfree(_buf->vertex_buffer);
}
VertexBuffer _VertexBuffer_new(VertexDecl _dec, const char* _file, uint _line)
{
    if (VertexDecl_count(_dec))
    {
        vertex_buffer* buffer = (vertex_buffer*)_Malloc(sizeof(vertex_buffer), _file, _line);
        buffer->vtx_ele_offs = array_new(uint, VertexDecl_count(_dec), 0xffffffff);
        uint32 vertex_size = 0;
        uint32 i = 0;
        for (; i < VertexDecl_count(_dec); i++)
        {
            VertexElement element = VertexDecl_find(_dec, i);
            {
                switch (VertexElement_get_semantic(element))
                {
                case Position:
                case Normal:
                case Color:
                case TexCoord:
                case Tangent:
                case Binormal:
                default:
                    break;
                }
                buffer->vtx_ele_offs[i] = vertex_size;
                vertex_size += get_element_size(VertexElement_get_type(element));
            }
        }
        buffer->vertex_buffer = (char*)_Malloc(vertex_size * DEFAULT_VERTEX_BUFFER_SIZE, _file, _line);
        buffer->vertex_size = vertex_size;
        buffer->vertex_buffer_size = vertex_size * DEFAULT_VERTEX_BUFFER_SIZE;
        buffer->vertex_buffer_tail = 0;
        buffer->vertex_declaration = _dec;
        buffer->dirty_flag = 1;
        glGenBuffers( 1, &buffer->id );
        glBindBuffer( GL_ARRAY_BUFFER, buffer->id );
        VertexBuffer ret = {buffer};
        return ret;
    }
    else
    {
        VertexBuffer ret = {NULL};
        return ret;
    }
}

void _VertexBuffer_grow_up(VertexBuffer _self, uint num_vtxs)
{
    uint totel_vtx_buf_size = (_self->vertex_buffer_tail + num_vtxs) * _self->vertex_size;
    if (totel_vtx_buf_size > _self->vertex_buffer_size)
    {
        char* tmp = Malloc(totel_vtx_buf_size * 2);
        if (_self->vertex_buffer_tail)
            memcpy(tmp, _self->vertex_buffer, _self->vertex_buffer_tail * _self->vertex_size);
        Mfree(_self->vertex_buffer);
        _self->vertex_buffer = tmp;
        _self->vertex_buffer_size = totel_vtx_buf_size * 2;
    }
}

void _VertexBuffer_delete(VertexBuffer _self, const char* _file, uint _line)
{
    VertexBuffer_Dest(_self);
    Mfree(_self);
}

bool VertexBuffer_read(VertexBuffer _self, element_semantic _sem, unsigned int _i, vptr _result)
{
    uint* offs = _self->vtx_ele_offs;
    if (_i >= _self->vertex_buffer_tail)
    {
        return false;
    }
    if (VertexDecl_count(_self->vertex_declaration))
    {
        uint32 i = 0;
        for (; i < VertexDecl_count(_self->vertex_declaration); i++)
        {
            VertexElement ele = VertexDecl_find(_self->vertex_declaration, i);
            if (!VertexElement_is_null(ele))
            {
                if (VertexElement_get_semantic(ele) == _sem)
                {
                    uint32 offset = _self->vertex_size * _i + offs[i];
                    vptr src = (vptr)((ref_ptr)_self->vertex_buffer + (ref_ptr)offset);
                    memcpy(_result, src, VertexElement_get_size(ele));
                    return true;
                }
            }
        }
        return false;
    }
    else
    {
        return false;
    }
}

vptr VertexBuffer_insert(VertexBuffer _self, element_semantic _sem, unsigned int _i)
{
    uint* offs = _self->vtx_ele_offs;
    if (_i >= _self->vertex_buffer_tail)
    {
        _VertexBuffer_grow_up(_self, _i - _self->vertex_buffer_tail);
        _self->vertex_buffer_tail += (_i - _self->vertex_buffer_tail + 1);
    }
    if (VertexDecl_count(_self->vertex_declaration))
    {
        uint32 i = 0;
        for (; i < VertexDecl_count(_self->vertex_declaration); i++)
        {
            VertexElement ele = VertexDecl_find(_self->vertex_declaration, i);
            if (!VertexElement_is_null(ele))
            {
                if (VertexElement_get_semantic(ele) == _sem)
                {
                    uint32 offset = _self->vertex_size * _i + offs[i];
                    return (vptr)((ref_ptr)_self->vertex_buffer + (ref_ptr)offset);
                }
            }
        }
        return NULL;
    }
    else
    {
        return NULL;
    }
}

VertexDecl VertexBuffer_get_vertex_declaration(VertexBuffer _self)
{
    return _self->vertex_declaration;
}

uint32 VertexBuffer_get_id(VertexBuffer _self)
{
    return _self->id;
}

uint32 VertexBuffer_get_vertex_size(VertexBuffer _self)
{
    return _self->vertex_size;
}

uint32 VertexBuffer_get_buffer_size(VertexBuffer _self)
{
    return _self->vertex_buffer_tail * _self->vertex_size;
}

uint32 VertexBuffer_get_buffer_tail(VertexBuffer _self)
{
    return _self->vertex_buffer_tail;
}

void _fill_vertex_data(VertexBuffer _self, uint _offs, VertexElement _ele, uint32 _ptr, float* _data, uint32 _n)
{
    float* vbf = (float*)((ref_ptr)_self->vertex_buffer + (ref_ptr)(_ptr * _self->vertex_size) + (ref_ptr)_offs);
    switch (VertexElement_get_type(_ele))
    {
    case Float32_4:
    {
        switch (_n)
        {
        case 4:
            vbf[0] = _data[0];
            vbf[1] = _data[1];
            vbf[2] = _data[2];
            vbf[3] = _data[3];
            break;
        default:
            EAssert(0, "%s", "Vertex Element type is Float32_4, but _n not equal 4");
            break;
        }
    }
    break;
    case Float32_3:
    {
        switch (_n)
        {
        case 3:
            vbf[0] = _data[0];
            vbf[1] = _data[1];
            vbf[2] = _data[2];
            break;
        default:
            EAssert(0, "%s", "Vertex Element type is Float32_3, but _n not equal 3");
            break;
        }
    }
    break;
    case Float32_2:
    {
        switch (_n)
        {
        case 2:
            vbf[0] = _data[0];
            vbf[1] = _data[1];
            break;
        default:
            EAssert(0, "%s", "Vertex Element type is Float32_2, but _n not equal 2");
            break;
        }
    }
    break;
    case Float32_1:
    {
        switch (_n)
        {
        case 1:
            vbf[0] = _data[0];
            break;
        default:
            EAssert(0, "%s", "Vertex Element type is Float32_1, but _n not equal 1");
            break;
        }
    }
    break;
    default:
        EAssert(0, "%s", "Unknow Vertex Element type");
        break;
    }
}

uint32 VertexBuffer_attach_mesh(VertexBuffer _self, Mesh _mesh)
{
    uint* offs = _self->vtx_ele_offs;
    uint num_vtxs = Mesh_get_vertex_count(_mesh);
    _VertexBuffer_grow_up(_self, num_vtxs);
    if (VertexDecl_count(_self->vertex_declaration))
    {
        float* fpos = Mesh_get_position(_mesh);
        float* fnor = Mesh_get_normal(_mesh);
        float* ftex = Mesh_get_tex_coord(_mesh);
        float* ftgt = Mesh_get_tangent(_mesh);
        float* fbin = Mesh_get_binormal(_mesh);
        float* fepw = Mesh_get_edge_proj_weight(_mesh);
        float* fcol = Mesh_get_color(_mesh);
        uint32 tail = _self->vertex_buffer_tail;
        uint32 i = 0;
        for (; i < VertexDecl_count(_self->vertex_declaration); i++)
        {
            VertexElement ele = VertexDecl_find(_self->vertex_declaration, i);
            {
                uint32 j = 0;
                for (; j < Mesh_get_vertex_count(_mesh); j++)
                {
                    switch (VertexElement_get_semantic(ele))
                    {
                    case Position:
                        if (fpos)
                            _fill_vertex_data(_self, offs[i], ele, tail + j, &fpos[j * 3], 3);
                        break;
                    case TexCoord:
                        if (ftex)
                            _fill_vertex_data(_self, offs[i], ele, tail + j, &ftex[j * 2], 2);
                        break;
                    case Normal:
                        if (fnor)
                            _fill_vertex_data(_self, offs[i], ele, tail + j, &fnor[j * 3], 3);
                        break;
                    case Binormal:
                        if (fbin)
                            _fill_vertex_data(_self, offs[i], ele, tail + j, &fbin[j * 3], 3);
                        break;
                    case Tangent:
                        if (ftgt)
                            _fill_vertex_data(_self, offs[i], ele, tail + j, &ftgt[j * 3], 3);
                        break;
                    case Color:
                        if (fcol)
                            _fill_vertex_data(_self, offs[i], ele, tail + j, &fcol[j * 4], 4);
                        break;
                    default:
                        EAssert(0, "%s", "Unknow vertex semantic");
                        break;
                    }
                }
            }
        }
        _self->vertex_buffer_tail += Mesh_get_vertex_count(_mesh);
        _self->dirty_flag = 1;
        return _self->vertex_buffer_tail;
    }
    else
    {
        return 0xffffffff;
    }
}
void VertexBuffer_reset(VertexBuffer _self)
{
    ///_self->vertex_size = 0;
    _self->vertex_buffer_tail = 0;
    _self->dirty_flag = 1;
}

void VertexBuffer_buffer_data(VertexBuffer _self)
{
    if (_self->dirty_flag)
    {
        glBufferData( GL_ARRAY_BUFFER, VertexBuffer_get_buffer_size(_self), _self->vertex_buffer,GL_STATIC_DRAW );
        _self->dirty_flag = 0;
    }
}

uint* VertexBuffer_get_ele_offs(VertexBuffer _self)
{
    return _self->vtx_ele_offs;
}
