#include "pch.h"
#include "emem.h"
#include "elog.h"
#include "eassert.h"
#include "pthread.h"

#define ALLOC_INFO_RESERVED 16
#define REFER_INFO_RESERVED 16

///#define USE_C_MALLOC
#ifndef __APPLE__
#include <malloc.h>
#else
#include <sys/malloc.h>
#endif
///==================================================================================///
#include "list.h"

typedef struct _mem_node
{
    vptr next;
    char data[1];
} mem_node;

typedef struct _refer_info
{
    const char* file_name;
    euint line;
} refer_info;

typedef struct _totel_refer_info
{
    euint used_mem_size;
    euint unused_mem_size;
} totel_refer_info;

#ifdef USE_SSE
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>

static char align_char[32];
static __m128* zero_ptr = NULL;
#endif
_INLINE_ void meminit(void* _mem, euint _size)
{
#ifdef USE_SSE
	euint i = 0;

	euint cnt = _size / 16;
	__m128* m128_ptr = NULL;
    
    if (!zero_ptr)
    {
        ref_ptr offs = (ref_ptr)align_char % 16;
		offs = 16 - offs;
        zero_ptr = (__m128*)(align_char + offs);
        ///*zero_ptr = _mm_set1_ps(0.0f);
        memset(zero_ptr, 0, sizeof(__m128));
    }

    m128_ptr = (__m128*)_mem;
    for (; i < cnt; i++)
        m128_ptr[i] = *zero_ptr;
#else
    memset(_mem, 0, _size);
#endif
}

_INLINE_ void mem_node_init(mem_node* _node)
{
    memset(_node->data, 0, ALLOC_INFO_RESERVED + REFER_INFO_RESERVED);
}

#define cale_alloc_size(s) ( s + (16 - (s % 16)) );

void* alloc_mem_list(euint _chk_size, euint _num_chks, euint* _real_chk_size, vptr* _begin, vptr* _end, mem_node** _head)
{
	char* ret = NULL;
	euint totel_size = 0;
    euint chk_size = 0;
	euint i = 0;

	chk_size = cale_alloc_size(_chk_size);
	totel_size = chk_size * _num_chks;

    *_real_chk_size = chk_size;
#ifndef __APPLE__
    ret = (char*)__mingw_aligned_malloc(totel_size, 16);
#else
    ret = (char*)malloc(totel_size);
#endif
    *_head = (mem_node*)ret;
    *_begin = ret;

    for (; i < _num_chks; i++)
    {
        mem_node* node = (mem_node*)&ret[i * chk_size];
        mem_node_init(node);

        if (i < _num_chks - 1)
            node->next = &ret[ (i + 1) * chk_size ];
        else
        {
            node->next = NULL;
            *_end = node;
        }
    }
    return ret;
}
void free_mem_list(void* _mem_list)
{
#ifndef __APPLE__
    __mingw_aligned_free(_mem_list);
#else
    free(_mem_list);
#endif
}

typedef struct _mem_pool_node
{
    void* mem_list;
    euint real_chk_size;
    vptr begin;
    vptr end;
    mem_node* head;
} mem_pool_node;

mem_pool_node allco_mem_pool_node(euint _chk_size, euint _num_chks)
{
    mem_pool_node ret;
    ret.mem_list = alloc_mem_list(_chk_size, _num_chks, &ret.real_chk_size, &ret.begin, &ret.end, &ret.head);
    return ret;
}

totel_refer_info mem_pool_node_log(mem_pool_node* _node)
{
	char* i = (char*)_node->begin;
    totel_refer_info ret;
    ret.unused_mem_size = 0;
    ret.used_mem_size = 0;
    for (; (vptr)i <= _node->end; i+=_node->real_chk_size)
    {
        refer_info* info = (refer_info*)(i + ALLOC_INFO_RESERVED);
        char* ptr = i + ALLOC_INFO_RESERVED + REFER_INFO_RESERVED;
        if (info->file_name)
        {
            ret.used_mem_size += _node->real_chk_size;
            elog("##ptr %x file %s, line %d", (ref_ptr)ptr, info->file_name, info->line);
        }
        else
        {
            ret.unused_mem_size += _node->real_chk_size;
        }
    }
    return ret;
}

bool is_from(mem_pool_node* _node, void* _ptr)
{
    bool test0 = (_ptr >= _node->begin && _ptr <= _node->end);
    bool test1 = ( ((ref_ptr)_ptr - (ref_ptr)_node->begin) % _node->real_chk_size == 0 );

    ///printf("##test0 %d test1 %d##is from [begin %x end %x], ptr %x\n", test0, test1, _node->begin, _node->end, _ptr);
    return (test0 && test1);
}

void* alloc(mem_pool_node* _node, bool _is_safe_alloc)
{
	mem_node* ret = _node->head;
	EAssert(is_from(_node, ret), "invalid memory node!");
	if(ret)
	{
		if (_node->head->next)
		{
			EAssert(is_from(_node, ret->next), "invalid memory node!");
			_node->head = (mem_node*)ret->next;
		}
		else
		{
			_node->head = NULL;
		}
		if (_is_safe_alloc)
		    meminit(ret, _node->real_chk_size);
	}
	return ret;
}

void dealloc(mem_pool_node* _node, void* _ptr)
{
    mem_node* n = NULL;
    EAssert(is_from(_node, _ptr), "invalid memory node!");
    n = (mem_node*)_ptr;
    if (_node->head)
    {
        EAssert(is_from(_node, _node->head), "invalid memory node!");
        n->next = _node->head;
    }
    else
        n->next = NULL;
    _node->head = n;
}

MemPoolNode MemPoolNode_new(euint _chk_size, euint _num_chks)
{
    MemPoolNode ret;
    ret.self = (struct _mem_pool_node*)malloc(sizeof(mem_pool_node));
    *ret.self = allco_mem_pool_node(_chk_size, _num_chks);
    return ret;
}

void MemPoolNode_delete(MemPoolNode _self)
{
    free_mem_list(_self.self->mem_list);
    free(_self.self);
}

void* MemPoolNode_alloc(MemPoolNode _self, bool _is_safe_alloc)
{
    return alloc(_self.self, _is_safe_alloc);
}
/**
void* MemPoolNode_salloc(MemPoolNode _self)
{
    return alloc(_self.self, true);
}
**/
bool MemPoolNode_free(MemPoolNode _self, void* _ptr)
{
    if (is_from(_self.self, _ptr))
    {
        dealloc(_self.self, _ptr);
        return true;
    }
    return false;
}

bool MemPoolNode_check(MemPoolNode _self)
{
    mem_node* node = _self.self->head;
    while (node)
    {
        if (!is_from(_self.self, node))
            return false;
        node = (mem_node*)node->next;
    }
    return true;
}

bool MemPoolNode_empty(MemPoolNode _self)
{
    return SELF.head == NULL;
}

typedef struct _mem_pool
{
    euint real_chk_size;
    euint num_chunk_per_mem_block;
    List mem_pool_chain;
	pthread_rwlock_t lock;
} mem_pool;

#define PAGE_SIZE (4 * 1024)
MemPool MemPool_new(euint _chk_size)
{
    MemPool ret;
	euint num_chunk_per_mem_block = 0;
	euint c = 1;
	MemPoolNode node = {NULL};
	var v;

    ret.self = (struct _mem_pool*)malloc(sizeof(mem_pool));
    ret.self->mem_pool_chain = List_new(Vptr,  (MALLOC)malloc, (MFREE)free);
    ret.self->real_chk_size = cale_alloc_size(_chk_size);

    while (!num_chunk_per_mem_block)
    {
        num_chunk_per_mem_block = (PAGE_SIZE * c) / cale_alloc_size(_chk_size);
        c++;
    }
    ret.self->num_chunk_per_mem_block = num_chunk_per_mem_block;

    node = MemPoolNode_new(_chk_size, ret.self->num_chunk_per_mem_block);

    v.vptr_var = node.self;
    List_push_back(ret.self->mem_pool_chain, v);

    ret.self->num_chunk_per_mem_block *= 2;

	pthread_rwlock_init(&ret.self->lock, NULL);
    return ret;
}

void MemPool_delete(MemPool _self)
{
    Iterator iter = List_begin(SELF.mem_pool_chain);
	while (iter)
	{
		var data = List_get_value(iter);
		MemPoolNode node = {(mem_pool_node*)data.vptr_var};
		MemPoolNode_delete(node);
		iter = List_next(iter);
	}
	pthread_rwlock_destroy(&_self.self->lock);
	free(_self.self);
}

totel_refer_info MemPool_log(MemPool _self)
{
	Iterator iter = List_begin(SELF.mem_pool_chain);
    totel_refer_info ret;
    ret.unused_mem_size = 0;
    ret.used_mem_size = 0;

    while (iter)
    {
        var v = List_get_value(iter);
        mem_pool_node* node = (mem_pool_node*)v.vptr_var;
        totel_refer_info info = mem_pool_node_log(node);
        ret.unused_mem_size += info.unused_mem_size;
        ret.used_mem_size += info.used_mem_size;
        iter = List_next(iter);
    }
    return ret;
}

bool MemPool_check(MemPool _self)
{
    Iterator iter = List_begin(SELF.mem_pool_chain);

    while (iter)
    {
        var v = List_get_value(iter);
        mem_pool_node* node = (mem_pool_node*)v.vptr_var;
        MemPoolNode n = {node};
		if (n.self->mem_list == (void*)0xffffffff)
			return false;
        if (!MemPoolNode_check(n))
            return false;
        iter = List_next(iter);
    }
    return true;
}

typedef struct _mem_desc
{
    MemPool mem_pool;
    Iterator iter;
} mem_desc;

void* MemPool_alloc(MemPool _self, Iterator* _iter, bool _is_safe_alloc)
{
	pthread_rwlock_wrlock(&_self.self->lock);
	Iterator iter = List_begin(SELF.mem_pool_chain);

	var v = List_get_value(iter);
	MemPoolNode node = {(struct _mem_pool_node*)v.vptr_var};
	void* ret = MemPoolNode_alloc(node, _is_safe_alloc);
	*_iter = iter;
	if (MemPoolNode_empty(node))
	{
		List_throw_back(SELF.mem_pool_chain, iter);
		iter = List_begin(SELF.mem_pool_chain);
		v = List_get_value(iter);
		node.self = (struct _mem_pool_node*)v.vptr_var;
		if (MemPoolNode_empty(node))
		{
			node = MemPoolNode_new(SELF.real_chk_size, SELF.num_chunk_per_mem_block);
			v.vptr_var = node.self;
			List_push_front(SELF.mem_pool_chain, v);
			SELF.num_chunk_per_mem_block *= 2;
		}
	}
    pthread_rwlock_unlock(&_self.self->lock);
	return ret;
}

void MemPool_free(MemPool _self, Iterator _iter, void* _ptr)
{
	pthread_rwlock_wrlock(&_self.self->lock);
    var v = List_get_value(_iter);
    MemPoolNode node = {(struct _mem_pool_node*)v.vptr_var};
    MemPoolNode_free(node, _ptr);
    List_throw_front(SELF.mem_pool_chain, _iter);
	pthread_rwlock_unlock(&_self.self->lock);
}

euint MemPool_chunk_size(MemPool _self)
{
    return _self.self->real_chk_size;
}

#define MAX_MEM_POOLS      512
#define DEFAULT_CHUNK_SIZE 32

typedef struct _mem_allocator
{
    MemPool mem_pools[MAX_MEM_POOLS];
	pthread_rwlock_t lock;
} mem_allocator;

typedef struct _alloc_info
{
	MemPool mem_pool;
	Iterator iter;
} alloc_info;

void* align_malloc_16(euint size)
{
#ifndef __APPLE__
	return __mingw_aligned_malloc(size, 16);
#else
    return malloc(size);
#endif
}

static MemAllocator g_MemAllocator = {NULL};
MemAllocator MemAllocator_new()
{
    MemAllocator ret;
	ret.self = (struct _mem_allocator*)malloc(sizeof(mem_allocator));
	memset(ret.self->mem_pools, 0, sizeof(ret.self->mem_pools));
	pthread_rwlock_init(&ret.self->lock, NULL);
	return ret;
}
void MemAllocator_delete(MemAllocator _self)
{
	int i = 0;
	for (; i < MAX_MEM_POOLS; i++)
	{
		if (SELF.mem_pools[i].self) 
		{
			MemPool_delete(SELF.mem_pools[i]);
		}
	}
	pthread_rwlock_destroy(&_self.self->lock);
	free(_self.self);
}
void* MemAllocator_alloc(MemAllocator _self, euint _size, bool _is_safe_alloc)
{
	char* ret = NULL;
	alloc_info ainfo = {{NULL}, NULL};
	refer_info rinfo = {NULL, 0};
	euint i = 0;

	_size += (ALLOC_INFO_RESERVED + REFER_INFO_RESERVED);
	i = _size / DEFAULT_CHUNK_SIZE;

	if (i < MAX_MEM_POOLS)
	{
		MemPool mp = SELF.mem_pools[i];
		pthread_rwlock_wrlock(&_self.self->lock);
		if (!mp.self)
		{
			mp = MemPool_new( (i + 1) * DEFAULT_CHUNK_SIZE + ALLOC_INFO_RESERVED + REFER_INFO_RESERVED );
			SELF.mem_pools[i] = mp;
		}
		pthread_rwlock_unlock(&_self.self->lock);
		Iterator iter;
        ret = (char*)MemPool_alloc(mp, &iter, _is_safe_alloc);
		ainfo.mem_pool = mp;
		ainfo.iter = iter;
	}
	else
	{
#ifndef __APPLE__
		ret = (char*)__mingw_aligned_malloc(_size, 16);
#else
        ret = (char*)malloc(_size);
#endif
		if (_is_safe_alloc)
		    meminit(ret, _size);
	}
	((alloc_info*)ret)[0] = ainfo;
	ret += ALLOC_INFO_RESERVED;
	((refer_info*)ret)[0] = rinfo;
	ret += REFER_INFO_RESERVED;

	return ret;
}
void* MemAllocator_salloc(MemAllocator _self, euint _size)
{
	return MemAllocator_alloc(_self, _size, true);
}
void MemAllocator_free(MemAllocator _self, void* _ptr)
{
	refer_info* ip = NULL;
	char* ptr = (char*)_ptr;
	alloc_info info = {{NULL}, NULL};

	if (!_ptr) {
		return;
	}
	ptr -= (ALLOC_INFO_RESERVED + REFER_INFO_RESERVED);
	info = ((alloc_info*)ptr)[0];

	ip = ((refer_info*)((char*)_ptr - REFER_INFO_RESERVED));
	ip->file_name = NULL;
	ip->line = 0;

	if ( !to_ptr(info.mem_pool) )
	{
#ifndef __APPLE__
		__mingw_aligned_free(ptr);
#else
        free(ptr);
#endif
	}
	else
	{
		MemPool_free(info.mem_pool, info.iter, ptr);
	}
}
void MemAllocator_log(MemAllocator _self)
{
	euint i = 0;
	totel_refer_info ret;
	ret.unused_mem_size = 0;
	ret.used_mem_size = 0;
	for (; i < MAX_MEM_POOLS; i++)
	{
		if (SELF.mem_pools[i].self) 
		{
			totel_refer_info info = MemPool_log(SELF.mem_pools[i]);
			ret.unused_mem_size += info.unused_mem_size;
			ret.used_mem_size += info.used_mem_size;
		}
	}
	elog("totel used mem %d totel unused mem %d", ret.used_mem_size, ret.unused_mem_size);
}
bool MemAllocator_check(MemAllocator _self)
{
	euint i = 0;
	for (; i < MAX_MEM_POOLS; i++)
	{
		if (SELF.mem_pools[i].self && !MemPool_check(SELF.mem_pools[i]))
			return false;
	}
	return true;
}

void MInit()
{
#ifndef USE_C_MALLOC
	g_MemAllocator = MemAllocator_new();
#endif
}

vptr _Malloc(euint _size, const char* _file, euint32 _line)
{
	vptr ret = Ealloc(_size);
#ifndef USE_C_MALLOC
	refer_info info;
	info.file_name = _file;
	info.line = _line;
	((refer_info*)((char*)ret - REFER_INFO_RESERVED))[0] = info;
#endif
	return ret;
}

vptr _SMalloc(euint _size, const char* _file, euint32 _line)
{
    vptr ret = SEalloc(_size);
#ifndef USE_C_MALLOC
    refer_info info;
    info.file_name = _file;
    info.line = _line;
    ((refer_info*)((char*)ret - REFER_INFO_RESERVED))[0] = info;
#endif
    return ret;
}

void _Mfree(vptr _ptr, const char* _file, euint32 _line)
{
    Efree(_ptr);
#ifdef USE_C_MALLOC
    ///EAssert(_heapchk(), "free memory error!");
#endif
}

bool Mtest(vptr _ptr)
{
    return true;
}

void Massert(vptr _ptr, const char* _info)
{
}

void Mlog()
{
#ifndef USE_C_MALLOC
	/**
	euint i = 0;
    totel_refer_info ret;
    ret.unused_mem_size = 0;
    ret.used_mem_size = 0;
    for (; i < MAX_MEM_POOLS; i++)
    {
		if (g_mem_pools[i].self) 
		{
			totel_refer_info info = MemPool_log(g_mem_pools[i]);
			ret.unused_mem_size += info.unused_mem_size;
			ret.used_mem_size += info.used_mem_size;
		}
    }
    elog("totel used mem %d totel unused mem %d", ret.used_mem_size, ret.unused_mem_size);
	**/
	MemAllocator_log(g_MemAllocator);
#endif
}

bool MCheck()
{
#ifndef USE_C_MALLOC
	/**
	euint i = 0;
    for (; i < MAX_MEM_POOLS; i++)
    {
        if (g_mem_pools[i].self && !MemPool_check(g_mem_pools[i]))
            return false;
    }
    return true;
	**/
	return MemAllocator_check(g_MemAllocator);
#endif
}
vptr Ealloc(euint _size)
{
#ifdef USE_C_MALLOC
	vptr ret = align_malloc_16(_size);
	return ret;
#else
	return MemAllocator_alloc(g_MemAllocator, _size, false);
#endif
}
vptr SEalloc(euint _size)
{
#ifdef USE_C_MALLOC
    vptr ret = align_malloc_16(_size);
    if (ret)
        memset(ret, 0, _size);
    return ret;
#else
	return MemAllocator_salloc(g_MemAllocator, _size);
#endif
}
void Efree(vptr _ptr)
{
#ifdef USE_C_MALLOC
    _aligned_free(_ptr);
    return;
#else
	MemAllocator_free(g_MemAllocator, _ptr);
#endif
}
const char* Minfo(vptr _ptr, euint* _line)
{
#ifdef USE_C_MALLOC
    return NULL;
#else
    char* ptr = (char*)_ptr;
	refer_info info = {NULL, 0};

    if (!_ptr)
        return NULL;
    ptr -= (ALLOC_INFO_RESERVED);
    info = ((refer_info*)ptr)[0];
    *_line = info.line;
    return info.file_name;
#endif
}

euint Msize(vptr _ptr)
{
#ifdef USE_C_MALLOC
    return 0;
#else
    char* ptr = (char*)_ptr;
    alloc_info info = {{NULL}, NULL};

    if (!_ptr)
        return 0;
    ptr -= (ALLOC_INFO_RESERVED + REFER_INFO_RESERVED);
    info = ((alloc_info*)ptr)[0];
    if (to_ptr(info.mem_pool))
        return MemPool_chunk_size(info.mem_pool);
    else
        return 0;
#endif
}
