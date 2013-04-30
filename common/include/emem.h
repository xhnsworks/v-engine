#ifndef EMEM_H
#define EMEM_H
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "etypes.h"
#define _MEM_BLOCK_HEAD_SIZE_ sizeof(euint) * 8

/// 必须确保所有分配出来的内存都是经过初始化的
API_EXPORT void TestInit();
API_EXPORT void MInit();

API_EXPORT vptr _Malloc(euint _size, const char* _file, uint32 _line);
API_EXPORT vptr _SMalloc(euint _size, const char* _file, uint32 _line);
API_EXPORT void _Mfree(vptr _ptr, const char* _file, uint32 _line);

void* TestAlloc(euint _size);
void TestFree(void* _ptr);

#define Malloc(s) _Malloc(s, __FILE__, __LINE__)
#define SMalloc(s) _SMalloc(s, __FILE__, __LINE__)
#define Mfree(p) _Mfree(p, __FILE__, __LINE__)

API_EXPORT void Mlog();
API_EXPORT bool MCheck();

API_EXPORT vptr Ealloc(euint _size);
API_EXPORT vptr SEalloc(euint _size);
API_EXPORT void Efree(vptr _ptr);
API_EXPORT const char* Minfo(vptr _ptr, euint* _line);
API_EXPORT euint Msize(vptr _ptr);

typedef struct _mem_pool_node_
{
    struct _mem_pool_node* self;
} MemPoolNode;
API_EXPORT MemPoolNode MemPoolNode_new(euint _chk_size, euint _num_chks);
API_EXPORT void MemPoolNode_delete(MemPoolNode _self);
API_EXPORT void* MemPoolNode_alloc(MemPoolNode _self, bool _is_safe_alloc);
///API_EXPORT void* MemPoolNode_salloc(MemPoolNode _self);
API_EXPORT bool MemPoolNode_free(MemPoolNode _self, void* _ptr);
API_EXPORT bool MemPoolNode_empty(MemPoolNode _self);

typedef struct mem_pool_
{
    struct _mem_pool* self;
} MemPool;
API_EXPORT MemPool MemPool_new(euint _chk_size);
API_EXPORT void MemPool_delete(MemPool _self);
API_EXPORT void* MemPool_alloc(MemPool _self, Iterator* _iter, bool _is_safe_alloc);
API_EXPORT void* MemPool_salloc(MemPool _self, Iterator* _iter);
API_EXPORT void MemPool_free(MemPool _self, Iterator _iter, void* _ptr);
API_EXPORT euint MemPool_chunk_size(MemPool _self);

typedef struct mem_allocator_
{
	struct _mem_allocator* self;
} MemAllocator;

API_EXPORT MemAllocator MemAllocator_new();
API_EXPORT void MemAllocator_delete(MemAllocator _self);
API_EXPORT void* MemAllocator_alloc(MemAllocator _self, euint _size, bool _is_safe_alloc);
API_EXPORT void* MemAllocator_salloc(MemAllocator _self, euint _size);
API_EXPORT void MemAllocator_free(MemAllocator _self, void* _ptr);
API_EXPORT void MemAllocator_log(MemAllocator _self);
API_EXPORT bool MemAllocator_check(MemAllocator _self);
#endif
