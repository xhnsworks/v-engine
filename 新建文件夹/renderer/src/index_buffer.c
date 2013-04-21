#include "index_buffer.h"
#include <GL/glew.h>
#include <GL/gl.h>

#define MAX_INDEX_BUFFER_SIZE 128 * 1024 * 3
typedef struct _index_buffer
{
    uint32 id;
    char* index_buffer;
    uint32 index_size;
    uint32 index_buffer_size;
    uint32 index_buffer_tail;
    uint32 dirty_flag;
} index_buffer;

void IndexBuffer_Dest(struct _index_buffer* _buf)
{
    printf("index buffer dest\n");
    Mfree( (vptr)((index_buffer*)_buf)->index_buffer );
}
IndexBuffer _IndexBuffer_new(const char* _file, uint _line)
{
    index_buffer* buffer = _Malloc(sizeof(index_buffer), _file, _line);
    buffer->index_size = sizeof(uint32);
    buffer->index_buffer = Malloc(sizeof(uint32) * MAX_INDEX_BUFFER_SIZE);
    buffer->index_buffer_size = sizeof(uint32) * MAX_INDEX_BUFFER_SIZE;
    buffer->index_buffer_tail = 0;
    buffer->dirty_flag = 1;
    glGenBuffers( 1, &buffer->id );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer->id );
    IndexBuffer ret;
    ret.self = buffer;
    return ret;
}
void _IndexBuffer_delete(IndexBuffer _buf, const char* _file, uint _line)
{
    IndexBuffer_Dest(_buf.self);
    Mfree(_buf.self);
}
uint32* IndexBuffer_get_buffer(IndexBuffer _buf)
{
    return (uint32*)_buf.self->index_buffer;
}

uint32 IndexBuffer_get_id(IndexBuffer _buf)
{
    return _buf.self->id;
}

uint32 IndexBuffer_get_buffer_size(IndexBuffer _buf)
{
    return _buf.self->index_buffer_size;
}

uint32 IndexBuffer_attach_mesh(IndexBuffer _buf, Mesh _mesh)
{
    uint32 tail = _buf.self->index_buffer_tail;
    uint32* idx = Mesh_get_index(_mesh);
    uint32* ibf = (uint32*)((ref_ptr)_buf.self->index_buffer + (ref_ptr)(tail * sizeof(uint32) * 3));
    uint32 i = 0;
    for (; i < Mesh_get_face_count(_mesh); i++)
    {
        ibf[i * 3] = idx[i * 3];
        ibf[i * 3 + 1] = idx[i * 3 + 1];
        ibf[i * 3 + 2] = idx[i * 3 + 2];
    }
    _buf.self->index_buffer_tail = Mesh_get_face_count(_mesh);
    _buf.self->dirty_flag = 1;
    return tail;
}

void IndexBuffer_buffer_data(IndexBuffer _buf)
{
    if (_buf.self->dirty_flag)
    {
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, IndexBuffer_get_buffer_size(_buf), IndexBuffer_get_buffer(_buf), GL_STATIC_DRAW );
        _buf.self->dirty_flag = 0;
    }
}
