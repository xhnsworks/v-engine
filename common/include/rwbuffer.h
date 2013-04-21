#ifndef RWBUFFER_H
#define RWBUFFER_H
#include "common.h"
#include "etypes.h"
API_EXPORT RWBuffer RWBuffer_new(uint buffer_size);
API_EXPORT void RWBuffer_delete(RWBuffer _self);
API_EXPORT bool RWBuffer_Read(RWBuffer _self, uint* result, uint* read_size);
API_EXPORT bool RWBuffer_Write(RWBuffer _self, const uint* from, const uint write_size);
#endif
