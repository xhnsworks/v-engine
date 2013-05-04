#include "pch.h"
#include "index_buffer.h"
#include "vertex_buffer.h"
#include "emem.h"
#include "renderable_base.h"

typedef struct _index_buffer
{
    euint32 id;
    char* index_buffer;
    euint32 face_size;
    euint32 index_buffer_size;
    euint32 index_buffer_tail;
    euint32 dirty_flag;
    e_mesh_mode mesh_mode;
} index_buffer;

void IndexBuffer_Dest(struct _index_buffer* _buf)
{
    glDeleteBuffers(1, &_buf->id);
    Mfree( (vptr)((index_buffer*)_buf)->index_buffer );
}
IndexBuffer IndexBuffer_new(e_mesh_mode mode)
{
    index_buffer* buffer = (index_buffer*)Malloc(sizeof(index_buffer));
    buffer->face_size = face_size(mode);
    buffer->index_buffer = (char*)SMalloc(DEFAULT_INDEX_BUFFER_SIZE);
    buffer->index_buffer_size = DEFAULT_INDEX_BUFFER_SIZE;
    buffer->index_buffer_tail = 0;
    buffer->dirty_flag = 1;
    buffer->mesh_mode = mode;
    glGenBuffers( 1, &buffer->id );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer->id );
    IndexBuffer ret;
    ret = buffer;
    return ret;
}
void IndexBuffer_delete(IndexBuffer _self)
{
    IndexBuffer_Dest(_self);
    Mfree(_self);
}

bool IndexBuffer_read(IndexBuffer _self, unsigned int _i, vptr _result)
{
    if (_i >= _self->index_buffer_tail)
    {
        return false;
    }
    memcpy(_result, _self->index_buffer + _self->face_size * _i, _self->face_size);
    return true;
}

void _IndexBuffer_grow_up(IndexBuffer _self, euint num_faces);
vptr IndexBuffer_insert(IndexBuffer _self, unsigned int _i)
{
    if (_i >= _self->index_buffer_tail)
    {
        _IndexBuffer_grow_up(_self, _i - _self->index_buffer_tail);
        _self->index_buffer_tail += (_i - _self->index_buffer_tail + 1);
    }
    return _self->index_buffer + _self->face_size * _i;
}

euint32 IndexBuffer_get_id(IndexBuffer _self)
{
    return _self->id;
}
/**
euint32 IndexBuffer_get_buffer_size(IndexBuffer _self)
{
    return _self->index_buffer_size;
}
**/
void _IndexBuffer_grow_up(IndexBuffer _self, euint num_faces)
{
    euint totel_idx_buf_size = (_self->index_buffer_tail + num_faces) * _self->face_size;
    if (totel_idx_buf_size > _self->index_buffer_size)
    {
        char* tmp = (char*)SMalloc(totel_idx_buf_size * 2);
        if (_self->index_buffer_tail)
            memcpy(tmp, _self->index_buffer, _self->index_buffer_tail * _self->face_size);
        Mfree(_self->index_buffer);
        _self->index_buffer = tmp;
        _self->index_buffer_size = totel_idx_buf_size * 2;
    }
}

euint32 IndexBuffer_attach_mesh(IndexBuffer _self, Mesh _mesh, VertexBuffer _prev_vtx_buf)
{
    euint num_faces = Mesh_get_face_count(_mesh);
    _IndexBuffer_grow_up(_self, num_faces);

    euint32 tail = _self->index_buffer_tail;
    euint32* idx = Mesh_get_index(_mesh);
    euint32* ibf = (euint32*)((ref_ptr)_self->index_buffer + (ref_ptr)(tail * _self->face_size));
    euint32 i = 0;
    if (_self->mesh_mode == Triangular)
    {
        euint start = VertexBuffer_get_buffer_tail(_prev_vtx_buf);
        for (; i < Mesh_get_face_count(_mesh); i++)
        {
            ibf[i * 3] = start + idx[i * 3];
            ibf[i * 3 + 1] = start + idx[i * 3 + 1];
            ibf[i * 3 + 2] = start + idx[i * 3 + 2];
        }
    }
    else
    {
        euint start = VertexBuffer_get_buffer_tail(_prev_vtx_buf);
        for (; i < Mesh_get_face_count(_mesh); i++)
        {
            ibf[i * 2] = start + idx[i * 2];
            ibf[i * 2 + 1] = start + idx[i * 2 + 1];
        }
    }
    _self->index_buffer_tail += Mesh_get_face_count(_mesh);
    _self->dirty_flag = 1;
    return _self->index_buffer_tail;
}

void IndexBuffer_buffer_data(IndexBuffer _self)
{
    if (_self->dirty_flag)
    {
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, _self->index_buffer_tail * _self->face_size, _self->index_buffer, GL_STATIC_DRAW );
        _self->dirty_flag = 0;
    }
}

euint IndexBuffer_get_num_faces(IndexBuffer _self)
{
    return _self->index_buffer_tail;
}

euint IndexBuffer_get_face_size(IndexBuffer _self)
{
    return _self->face_size;
}

e_mesh_mode IndexBuffer_get_mesh_mode(IndexBuffer _self)
{
    return _self->mesh_mode;
}

void IndexBuffer_reset(IndexBuffer _self)
{
    _self->index_buffer_tail = 0;
    _self->dirty_flag = 1;
}
