#include "rwbuffer.h"
#include "mem.h"
/// |0x0000                 |0x0004                 |0x0008                  |0x000c
/// |0x00 |0x01 |0x02 |0x03 |0x04 |0x05 |0x06 |0x07 | 0x08 |0x09 |0x0a |0x0b |0x0c |0x0d |0x0e |0x0f
/// |top barrier                                    |bottom barrier          |
/// |real top barrier                                     real bottom barrier|

#define GetRealSize(type, size) size + ( sizeof(type) - (size % sizeof(type)) )

typedef struct _rw_buffer
{
    volatile uint* top_barrier;
    volatile uint* bottom_barrier;
    volatile uint* top_pointer;
    volatile uint* bottom_pointer;
} rw_buffer;

RWBuffer RWBuffer_new(uint buffer_size)
{
    RWBuffer ret;
    ret.self = (rw_buffer*)Malloc(sizeof(rw_buffer));

    uint real_size = GetRealSize(uint, buffer_size);
    uint number_uints = real_size / sizeof(uint);
    ret.self->top_barrier = (uint*)Malloc(real_size);
    ret.self->bottom_barrier = ret.self->top_barrier + number_uints;
    ret.self->top_pointer = ret.self->top_barrier;
    ret.self->bottom_pointer = ret.self->top_barrier;
    return ret;
}

void RWBuffer_delete(RWBuffer _self)
{
    if (_self.self->top_barrier)
    {
        Mfree((void*)_self.self->top_barrier);
        _self.self->top_barrier = NULL;
        _self.self->bottom_barrier = NULL;
    }
}

bool RWBuffer_Read(RWBuffer _self, uint* result, uint* read_size)
{
    register uint* registered_top_pointer = (uint*)_self.self->top_pointer;

    if (registered_top_pointer == _self.self->bottom_pointer)
        return false;

    uint* buffer_chunk_pointer = registered_top_pointer;
    *read_size = *buffer_chunk_pointer;
    buffer_chunk_pointer++;

    if (buffer_chunk_pointer > _self.self->bottom_barrier)
        buffer_chunk_pointer = (uint*)_self.self->top_barrier;

    uint number_uints = *read_size / sizeof(uint);
    uint* next = (uint*)*buffer_chunk_pointer;
    buffer_chunk_pointer++;

    if (buffer_chunk_pointer > _self.self->bottom_barrier)
        buffer_chunk_pointer = (uint*)_self.self->top_barrier;

    for (uint i = 0; i < number_uints; i++)
    {
        if (buffer_chunk_pointer > _self.self->bottom_barrier)
            buffer_chunk_pointer = (uint*)_self.self->top_barrier;

        result[i] = *buffer_chunk_pointer;
        buffer_chunk_pointer++;
    }

    _self.self->top_pointer = next;
    return true;
}

bool RWBuffer_Write(RWBuffer _self, const uint* from, const uint write_size)
{
    register uint* registered_top_pointer = (uint*)_self.self->top_pointer;
    register uint* registered_bottom_pointer = (uint*)_self.self->bottom_pointer;
    uint real_write_size = GetRealSize(uint, write_size);
    uint number_uints = real_write_size / sizeof(uint);
    uint* buffer_chunk_pointer = registered_bottom_pointer + 2;

    for (uint i = 0; i < number_uints; i++)
    {
        if (buffer_chunk_pointer > _self.self->bottom_barrier)
            buffer_chunk_pointer = (uint*)_self.self->top_barrier;

        if (buffer_chunk_pointer == registered_top_pointer)
            return false;

        *buffer_chunk_pointer = from[i];
        buffer_chunk_pointer++;
    }

    if (buffer_chunk_pointer > _self.self->bottom_barrier)
        buffer_chunk_pointer = (uint*)_self.self->top_barrier;

    *registered_bottom_pointer = real_write_size;
    registered_bottom_pointer++;
    if (registered_bottom_pointer > _self.self->bottom_barrier)
        registered_bottom_pointer = _self.self->top_barrier;
    *registered_bottom_pointer = (uint)buffer_chunk_pointer;

    _self.self->bottom_pointer = buffer_chunk_pointer;
    return true;
}
