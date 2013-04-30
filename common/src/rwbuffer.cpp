#include "pch.h"
#include "rwbuffer.h"
#include "emem.h"
/// |0x0000                 |0x0004                 |0x0008                  |0x000c
/// |0x00 |0x01 |0x02 |0x03 |0x04 |0x05 |0x06 |0x07 | 0x08 |0x09 |0x0a |0x0b |0x0c |0x0d |0x0e |0x0f
/// |top barrier                                    |bottom barrier          |
/// |real top barrier                                     real bottom barrier|

#define GetRealSize(type, size) size + ( sizeof(type) - (size % sizeof(type)) )

struct rw_buffer
{
    volatile euint* top_barrier;
    volatile euint* bottom_barrier;
    volatile euint* top_pointer;
    volatile euint* bottom_pointer;
};

RWBuffer RWBuffer_new(euint buffer_size)
{
    RWBuffer ret;
    euint real_size = 0;
    euint number_uints = 0;
    ret = (rw_buffer*)SMalloc(sizeof(rw_buffer));

    real_size = GetRealSize(euint, buffer_size);
    number_uints = real_size / sizeof(euint);
    ret->top_barrier = (euint*)SMalloc(real_size);
    ret->bottom_barrier = ret->top_barrier + number_uints;
    ret->top_pointer = ret->top_barrier;
    ret->bottom_pointer = ret->top_barrier;
    return ret;
}

void RWBuffer_delete(RWBuffer _self)
{
    if (_self->top_barrier)
    {
        Mfree((void*)_self->top_barrier);
        _self->top_barrier = NULL;
        _self->bottom_barrier = NULL;
    }
}

bool RWBuffer_Read(RWBuffer _self, euint* result, euint* read_size)
{
    register euint* registered_top_pointer = (euint*)_self->top_pointer;
    euint* buffer_chunk_pointer = NULL;
    euint number_uints = 0;
    euint* next = NULL;
    euint i = 0;

    if (registered_top_pointer == _self->bottom_pointer)
        return false;

    buffer_chunk_pointer = registered_top_pointer;
    *read_size = *buffer_chunk_pointer;
    buffer_chunk_pointer++;

    if (buffer_chunk_pointer > _self->bottom_barrier)
        buffer_chunk_pointer = (euint*)_self->top_barrier;

    number_uints = *read_size / sizeof(euint);
    next = (euint*)*buffer_chunk_pointer;
    buffer_chunk_pointer++;

    if (buffer_chunk_pointer > _self->bottom_barrier)
        buffer_chunk_pointer = (euint*)_self->top_barrier;

    for (i = 0; i < number_uints; i++)
    {
        if (buffer_chunk_pointer > _self->bottom_barrier)
            buffer_chunk_pointer = (euint*)_self->top_barrier;

        result[i] = *buffer_chunk_pointer;
        buffer_chunk_pointer++;
    }

    _self->top_pointer = next;
    return true;
}

bool RWBuffer_Write(RWBuffer _self, const euint* from, const euint write_size)
{
    register euint* registered_top_pointer = (euint*)_self->top_pointer;
    register euint* registered_bottom_pointer = (euint*)_self->bottom_pointer;
    euint real_write_size = GetRealSize(euint, write_size);
    euint number_uints = real_write_size / sizeof(euint);
    euint* buffer_chunk_pointer = registered_bottom_pointer + 2;
    euint i = 0;

    for (i = 0; i < number_uints; i++)
    {
        if (buffer_chunk_pointer > _self->bottom_barrier)
            buffer_chunk_pointer = (euint*)_self->top_barrier;

        if (buffer_chunk_pointer == registered_top_pointer)
            return false;

        *buffer_chunk_pointer = from[i];
        buffer_chunk_pointer++;
    }

    if (buffer_chunk_pointer > _self->bottom_barrier)
        buffer_chunk_pointer = (euint*)_self->top_barrier;

    *registered_bottom_pointer = real_write_size;
    registered_bottom_pointer++;
    if (registered_bottom_pointer > _self->bottom_barrier)
        registered_bottom_pointer = (euint*)_self->top_barrier;
    *registered_bottom_pointer = (euint)buffer_chunk_pointer;

    _self->bottom_pointer = buffer_chunk_pointer;
    return true;
}
