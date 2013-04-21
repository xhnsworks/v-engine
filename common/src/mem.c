#include "mem.h"
#include "elog.h"
#include "eassert.h"

#define ALLOC_INFO_RESERVED 16
#define REFER_INFO_RESERVED 16

///#define USE_C_MALLOC

#include <malloc.h>

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
    uint line;
} refer_info;

typedef struct _totel_refer_info
{
    uint used_mem_size;
    uint unused_mem_size;
} totel_refer_info;

#ifdef USE_SSE
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>

static char align_char[32];
static __m128* zero_ptr = NULL;
#endif
_INLINE_ void meminit(void* _mem, uint _size)
{
#ifdef USE_SSE
	uint i = 0;
	/// 当USE_SSE被定义时_size一定是16的整数倍
	uint cnt = _size / 16;
	__m128* m128_ptr = NULL;
    /// 这里分支预测很容易预测正确的
    if (!zero_ptr)
    {
        ref_ptr offs = (ref_ptr)align_char % 16;
        zero_ptr = (__m128*)(align_char + offs);
        ///*zero_ptr = _mm_set1_ps(0.0f);
        memset(zero_ptr, 0, sizeof(__m128));
    }

    /// 当USE_SSE被定义时_mem一定是16字节对齐的
    m128_ptr = (__m128*)_mem;
    for (; i < cnt; i++)
        m128_ptr[i] = *zero_ptr;
#else
    memset(_mem, 0, _size);
#endif
}

_INLINE_ void mem_node_init(mem_node* _node)
{
    /// _node->data是未对齐的
    memset(_node->data, 0, ALLOC_INFO_RESERVED + REFER_INFO_RESERVED);
}

/// 分配尺寸对齐16字节
#define cale_alloc_size(s) ( s + (16 - (s % 16)) );
/// 从begin到end(包含end)之间的内存区为有效内存区
void* alloc_mem_list(uint _chk_size, uint _num_chks, uint* _real_chk_size, vptr* _begin, vptr* _end, mem_node** _head)
{
	char* ret = NULL;
	uint totel_size = 0;
    uint chk_size = 0;
	uint i = 0;

	chk_size = cale_alloc_size(_chk_size);
	totel_size = chk_size * _num_chks;

    *_real_chk_size = chk_size;
    ret = _aligned_malloc(totel_size, 16);
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
    _aligned_free(_mem_list);
}

typedef struct _mem_pool_node
{
    void* mem_list;
    uint real_chk_size;
    vptr begin;
    vptr end;
    mem_node* head;
} mem_pool_node;

mem_pool_node allco_mem_pool_node(uint _chk_size, uint _num_chks)
{
    mem_pool_node ret;
    ret.mem_list = alloc_mem_list(_chk_size, _num_chks, &ret.real_chk_size, &ret.begin, &ret.end, &ret.head);
    return ret;
}

totel_refer_info mem_pool_node_log(mem_pool_node* _node)
{
	char* i = _node->begin;
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

void* alloc(mem_pool_node* _node)
{
    mem_node* ret = _node->head;
    EAssert(is_from(_node, ret), "invalid memory node!");
    if(ret)
    {
        if (_node->head->next)
        {
            EAssert(is_from(_node, ret->next), "invalid memory node!");
            _node->head = ret->next;
        }
        else
        {
            _node->head = NULL;
        }
        meminit(ret, _node->real_chk_size);
    }
    return ret;
}

void dealloc(mem_pool_node* _node, void* _ptr)
{
    EAssert(is_from(_node, _ptr), "invalid memory node!");
    mem_node* n = _ptr;
    if (_node->head)
    {
        EAssert(is_from(_node, _node->head), "invalid memory node!");
        n->next = _node->head;
    }
    else
        n->next = NULL;
    _node->head = n;
}

MemPoolNode MemPoolNode_new(uint _chk_size, uint _num_chks)
{
    MemPoolNode ret;
    ret.self = malloc(sizeof(mem_pool_node));
    *ret.self = allco_mem_pool_node(_chk_size, _num_chks);
    return ret;
}

void MemPoolNode_delete(MemPoolNode _self)
{
    free_mem_list(_self.self->mem_list);
    free(_self.self);
}

void* MemPoolNode_alloc(MemPoolNode _self)
{
    return alloc(_self.self);
}
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
        node = node->next;
    }
    return true;
}

bool MemPoolNode_empty(MemPoolNode _self)
{
    return SELF.head == NULL;
}

typedef struct _mem_pool
{
    uint real_chk_size;
    uint num_chunk_per_mem_block;
    List mem_pool_chain;
} mem_pool;

#define PAGE_SIZE (4 * 1024)
MemPool MemPool_new(uint _chk_size)
{
    MemPool ret;
	uint num_chunk_per_mem_block = 0;
	uint c = 1;
	MemPoolNode node = {NULL};
	var v;

    ret.self = malloc(sizeof(mem_pool));
    ret.self->mem_pool_chain = List_new(Vptr, malloc, free);
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

    return ret;
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
        mem_pool_node* node = v.vptr_var;
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
        mem_pool_node* node = v.vptr_var;
        MemPoolNode n = {node};
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

void* MemPool_alloc(MemPool _self, Iterator* _iter)
{
    Iterator iter = List_begin(SELF.mem_pool_chain);

    var v = List_get_value(iter);
    MemPoolNode node = {v.vptr_var};
    void* ret = MemPoolNode_alloc(node);
    *_iter = iter;
    if (MemPoolNode_empty(node))
    {
        List_throw_back(SELF.mem_pool_chain, iter);
        iter = List_begin(SELF.mem_pool_chain);
        v = List_get_value(iter);
        node.self = v.vptr_var;
        if (MemPoolNode_empty(node))
        {
            node = MemPoolNode_new(SELF.real_chk_size, SELF.num_chunk_per_mem_block);
            v.vptr_var = node.self;
            List_push_front(SELF.mem_pool_chain, v);
            SELF.num_chunk_per_mem_block *= 2;
        }
    }

    return ret;
}

void MemPool_free(MemPool _self, Iterator _iter, void* _ptr)
{
    var v = List_get_value(_iter);
    MemPoolNode node = {v.vptr_var};
    MemPoolNode_free(node, _ptr);
    List_throw_front(SELF.mem_pool_chain, _iter);
}

uint MemPool_chunk_size(MemPool _self)
{
    return _self.self->real_chk_size;
}

#ifndef USE_C_MALLOC
#define MAX_MEM_POOLS 512
static MemPool g_mem_pools[MAX_MEM_POOLS];
static uint g_init_chunk_size = 32 + ALLOC_INFO_RESERVED + REFER_INFO_RESERVED;
#endif

void MInit()
{
#ifndef USE_C_MALLOC
	uint i = 0;
    uint chk_size = g_init_chunk_size;
    for (; i < MAX_MEM_POOLS; i++)
    {
        g_mem_pools[i] = MemPool_new( (i + 1) * chk_size );
    }
#endif
}

vptr _Malloc(uint _size, const char* _file, uint32 _line)
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

void _Mfree(vptr _ptr, const char* _file, uint32 _line)
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
	uint i = 0;
    totel_refer_info ret;
    ret.unused_mem_size = 0;
    ret.used_mem_size = 0;
    for (; i < MAX_MEM_POOLS; i++)
    {
        totel_refer_info info = MemPool_log(g_mem_pools[i]);
        ret.unused_mem_size += info.unused_mem_size;
        ret.used_mem_size += info.used_mem_size;
    }
    elog("totel used mem %d totel unused mem %d", ret.used_mem_size, ret.unused_mem_size);
#endif
}

bool MCheck()
{
#ifndef USE_C_MALLOC
	uint i = 0;
    for (; i < MAX_MEM_POOLS; i++)
    {
        if (!MemPool_check(g_mem_pools[i]))
            return false;
    }
    return true;
#endif
}

void* align_malloc_16(uint32 size)
{
    return _aligned_malloc(size, 16);
}

typedef struct _alloc_info
{
    MemPool mem_pool;
    Iterator iter;
} alloc_info;

vptr Ealloc(uint _size)
{
#ifdef USE_C_MALLOC
    vptr ret = align_malloc_16(_size);
    if (ret)
        memset(ret, 0, _size);
    return ret;
#else
    ///
	char* ret = NULL;
	alloc_info ainfo = {{NULL}, NULL};
	refer_info rinfo = {NULL, 0};
	uint i = 0;

	_size += (ALLOC_INFO_RESERVED + REFER_INFO_RESERVED);
	i = _size / g_init_chunk_size;

    if (i < MAX_MEM_POOLS)
    {
        MemPool mp = g_mem_pools[i];
        Iterator iter;
        ret = MemPool_alloc(mp, &iter);
        ainfo.mem_pool = mp;
        ainfo.iter = iter;
    }
    else
    {
        ret = _aligned_malloc(_size, 16);
        meminit(ret, _size);
    }
    ((alloc_info*)ret)[0] = ainfo;
    ret += ALLOC_INFO_RESERVED;
    ((refer_info*)ret)[0] = rinfo;
    ret += REFER_INFO_RESERVED;

    return ret;
#endif
}
void Efree(vptr _ptr)
{
#ifdef USE_C_MALLOC
    _aligned_free(_ptr);
    return;
#else
    ///
	refer_info* ip = NULL;
	char* ptr = _ptr;
	alloc_info info = {{NULL}, NULL};

    if (!_ptr)
        return;
    ptr -= (ALLOC_INFO_RESERVED + REFER_INFO_RESERVED);
    info = ((alloc_info*)ptr)[0];

    ip = ((refer_info*)((char*)_ptr - REFER_INFO_RESERVED));
    ip->file_name = NULL;
    ip->line = 0;

    if ( !to_ptr(info.mem_pool) )
    {
        _aligned_free(ptr);
    }
    else
    {
        MemPool_free(info.mem_pool, info.iter, ptr);
    }

    ///EAssert(MCheck(), "memory check fail!");
#endif
}
const char* Minfo(vptr _ptr, uint* _line)
{
#ifdef USE_C_MALLOC
    return NULL;
#else
    char* ptr = _ptr;
	refer_info info = {NULL, 0};

    if (!_ptr)
        return NULL;
    ptr -= (ALLOC_INFO_RESERVED);
    info = ((refer_info*)ptr)[0];
    *_line = info.line;
    return info.file_name;
#endif
}

uint Msize(vptr _ptr)
{
#ifdef USE_C_MALLOC
    return 0;
#else
    char* ptr = _ptr;
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
