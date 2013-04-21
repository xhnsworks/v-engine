#include "vertex_element.h"
#include "elog.h"
#include "eassert.h"
typedef struct _vertex_element
{
    element_type type;
    element_semantic semantic;
    uint32 offset;
} vertex_element;

void VertexElement_Dest(VertexElement _ele)
{
    printf("vertex element dest\n");
}

VertexElement _VertexElement_new(element_type _typ, element_semantic _sem, const char* _file, uint _line)
{
    VertexElement ret;
    ret.self = _Malloc(sizeof(vertex_element), _file, _line);
    ret.self->type = _typ;
    ret.self->semantic = _sem;
    ret.self->offset = 0;
    return ret;
}

void _VertexElement_delete(VertexElement _ele, const char* _file, uint32 _line)
{
    VertexElement_Dest(_ele);
    Mfree(_ele.self);
#if BIT_WIDTH == 64
    snprintf(g_elog_buffer,126,"VertexElement_delete FILE %s LINE %d PTR %I64x", _file, _line, (ref_ptr)_ele.self);
#else
    snprintf(g_elog_buffer,126,"VertexElement_delete FILE %s LINE %d PTR %x", _file, _line, (ref_ptr)_ele.self);
#endif
    ELog_write();
}

uint32 get_element_size(element_type _type)
{
    switch (_type)
    {
    case Float32_2:
        return sizeof(float) * 2;
    case Float32_3:
        return sizeof(float) * 3;
    case Float32_4:
        return sizeof(float) * 4;
    case Ubyte_1:
        return 1;
    case Ubyte_2:
        return 2;
    case Ubyte_3:
        return 3;
    case Ubyte_4:
        return 4;
    default:
        EAssert(0, "Unknow element type");
        return 0;
    }
}

element_semantic VertexElement_get_semantic(VertexElement _ele)
{
    return _ele.self->semantic;
}

element_type VertexElement_get_type(VertexElement _ele)
{
    return _ele.self->type;
}

uint32 VertexElement_get_offset(VertexElement _ele)
{
    return _ele.self->offset;
}

void VertexElement_set_offset(VertexElement _ele, uint32 _offset)
{
    _ele.self->offset = _offset;
}

bool VertexElement_is_null(VertexElement _ele)
{
    if (_ele.self)
        return false;
    else
        return true;
}

VertexElement VertexElement_null()
{
    VertexElement ret;
    ret.self = NULL;
    return ret;
}
