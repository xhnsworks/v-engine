#include <GL/glew.h>
#include <GL/gl.h>
#include "vertex_buffer.h"
#include "vertex_declaration.h"
#include "eassert.h"
typedef struct _vertex_buffer
{
    uint32 id;
    VertexDeclaration vertex_declaration;
    char* vertex_buffer;
    uint32 vertex_size;
    uint32 vertex_buffer_size;
    uint32 vertex_buffer_tail;
    uint32 dirty_flag;
} vertex_buffer;

void VertexBuffer_Dest(struct _vertex_buffer* _buf)
{
    printf("vertex buffer dest\n");
    ///VertexDeclaration_Dest( to_ptr(_buf->vertex_declaration) );
    Mfree(_buf->vertex_buffer);
}
VertexBuffer _VertexBuffer_new(VertexDeclaration _dec, const char* _file, uint _line)
{
    if (VertexDeclaration_count(_dec))
    {
        vertex_buffer* buffer = (vertex_buffer*)_Malloc(sizeof(vertex_buffer), _file, _line);
        memset(buffer, 0, sizeof(vertex_buffer));
        uint32 vertex_size = 0;
        uint32 i = 0;
        for (; i < VertexDeclaration_count(_dec); i++)
        {
            VertexElement element = VertexDeclaration_find(_dec, i);
            {
                switch (VertexElement_get_semantic(element))
                {
                case Position:
                case Normal:
                case Color:
                case TexCoord:
                case Tangent:
                case Binormal:
                    break;
                default:
                    break;
                }
                VertexElement_set_offset(element, vertex_size);
                vertex_size += get_element_size(VertexElement_get_type(element));
            }
        }
        buffer->vertex_buffer = (char*)Malloc(vertex_size * MAX_VERTEX_BUFFER_SIZE);
        buffer->vertex_size = vertex_size;
        buffer->vertex_buffer_size = vertex_size * MAX_VERTEX_BUFFER_SIZE;
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

void _VertexBuffer_delete(VertexBuffer _buf, const char* _file, uint _line)
{
    VertexBuffer_Dest(_buf.self);
    Mfree(_buf.self);
}

vptr VertexBuffer_access(VertexBuffer _buf, element_semantic _sem, unsigned int _i)
{
    vertex_buffer* buf = _buf.self;
    if (VertexDeclaration_count(buf->vertex_declaration))
    {
        uint32 i = 0;
        for (; i < VertexDeclaration_count(buf->vertex_declaration); i++)
        {
            VertexElement ele = VertexDeclaration_find(buf->vertex_declaration, i);
            if (!VertexElement_is_null(ele))
            {
                if (VertexElement_get_semantic(ele) == _sem)
                {
                    uint32 offset = buf->vertex_size * _i + VertexElement_get_offset(ele);
                    return (vptr)((ref_ptr)buf->vertex_buffer + (ref_ptr)offset);
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

VertexDeclaration VertexBuffer_get_vertex_declaration(VertexBuffer _buf)
{
    return _buf.self->vertex_declaration;
}

uint32 VertexBuffer_get_id(VertexBuffer _buf)
{
    return _buf.self->id;
}

uint32 VertexBuffer_get_vertex_size(VertexBuffer _buf)
{
    return _buf.self->vertex_size;
}

uint32 VertexBuffer_get_buffer_size(VertexBuffer _buf)
{
    return _buf.self->vertex_buffer_size;
}

uint32 VertexBuffer_get_buffer_tail(VertexBuffer _buf)
{
    return _buf.self->vertex_buffer_tail;
}

vptr VertexBuffer_get_buffer(VertexBuffer _buf)
{
    return _buf.self->vertex_buffer;
}

void _fill_vertex_data(VertexBuffer _buf, VertexElement _ele, uint32 _ptr, float* _data, uint32 _n)
{
    float* vbf = (float*)((ref_ptr)_buf.self->vertex_buffer + (ref_ptr)(_ptr * _buf.self->vertex_size) + (ref_ptr)VertexElement_get_offset(_ele));
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
            EAssert(0, "Vertex Element type is Float32_4, but _n not equal 4");
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
            EAssert(0, "Vertex Element type is Float32_3, but _n not equal 3");
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
            EAssert(0, "Vertex Element type is Float32_2, but _n not equal 2");
            break;
        }
    }
    break;
    default:
        EAssert(0, "Unknow Vertex Element type");
        break;
    }
}
uint32 VertexBuffer_attach_mesh(VertexBuffer _buf, Mesh _mesh)
{
    if (VertexDeclaration_count(_buf.self->vertex_declaration))
    {
        float* fpos = Mesh_get_position(_mesh);
        float* fnor = Mesh_get_normal(_mesh);
        float* ftex = Mesh_get_tex_coord(_mesh);
        float* ftgt = Mesh_get_tangent(_mesh);
        float* fbin = Mesh_get_binormal(_mesh);
        uint32 tail = _buf.self->vertex_buffer_tail;
        uint32 i = 0;
        for (; i < VertexDeclaration_count(_buf.self->vertex_declaration); i++)
        {
            VertexElement ele = VertexDeclaration_find(_buf.self->vertex_declaration, i);
            {
                uint32 j = 0;
                for (; j < Mesh_get_vertex_count(_mesh); j++)
                {
                    switch (VertexElement_get_semantic(ele))
                    {
                    case Position:
                        _fill_vertex_data(_buf, ele, tail + j, &fpos[j * 3], 3);
                        ///
                        ///elog("####pos %f %f %f", fpos[j * 3 + 0], fpos[j * 3 + 1], fpos[j * 3 + 2]);
                        ///
                        break;
                    case TexCoord:
                        _fill_vertex_data(_buf, ele, tail + j, &ftex[j * 2], 2);
                        break;
                    case Normal:
                        _fill_vertex_data(_buf, ele, tail + j, &fnor[j * 3], 3);
                        break;
                    case Binormal:
                        _fill_vertex_data(_buf, ele, tail + j, &fbin[j * 3], 3);
                        break;
                    case Tangent:
                        _fill_vertex_data(_buf, ele, tail + j, &ftgt[j * 3], 3);
                        break;
                    default:
                        EAssert(0, "Unknow vertex semantic");
                        break;
                    }
                }
            }
        }
        _buf.self->vertex_buffer_tail += Mesh_get_vertex_count(_mesh);
        _buf.self->dirty_flag = 1;
        return tail;
    }
    else
    {
        return 0xffffffff;
    }
}
void VertexBuffer_reset(VertexBuffer _buf)
{
    _buf.self->vertex_size = 0;
    _buf.self->vertex_buffer_size = 0;
    _buf.self->vertex_buffer_tail = 0;
    _buf.self->dirty_flag = 1;
}

void VertexBuffer_buffer_data(VertexBuffer _buf)
{
    if (_buf.self->dirty_flag)
    {
        glBufferData( GL_ARRAY_BUFFER, VertexBuffer_get_buffer_size(_buf), VertexBuffer_get_buffer(_buf),GL_STATIC_DRAW );
        _buf.self->dirty_flag = 0;
    }
}
