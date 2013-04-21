#include "pch.h"
#include "vertex_element.h"
#include "elog.h"
#include "eassert.h"
#include "emem.h"
#include "render_system.h"

void VertexElement_Dest(VertexElement _ele)
{
    printf("vertex element dest\n");
}

VertexElement _VertexElement_new(element_type _typ, element_semantic _sem, const char* _file, uint _line)
{
    vertex_element tmp = {_typ, _sem};
    VertexElement ret = tmp;
    return ret;
}

void _VertexElement_delete(VertexElement _ele, const char* _file, uint32 _line)
{
}

uint32 get_element_size(element_type _type)
{
    switch (_type)
    {
    case Float32_1:
        return sizeof(float) * 1;
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
        EAssert(0, "%s", "Unknow element type");
        return 0;
    }
}

uint32 VertexElement_get_size(VertexElement _ele)
{
    return get_element_size(_ele.type);
}

element_semantic VertexElement_get_semantic(VertexElement _ele)
{
    return _ele.semantic;
}

element_type VertexElement_get_type(VertexElement _ele)
{
    return _ele.type;
}

bool VertexElement_is_null(VertexElement _ele)
{
    return false;
}

VertexElement VertexElement_null()
{
    VertexElement ret = {EmptyType, EmptySemantic};
    return ret;
}

VertexElement VertexElement_clone(VertexElement _ele)
{
    return _ele;
}

int VertexElement_compare(VertexElement e0, VertexElement e1)
{

    if      (e0.type < e1.type)
        return -1;
    else if (e0.type > e1.type)
        return 1;
    else
    {
        if      (e0.semantic < e1.semantic)
            return -1;
        else if (e0.semantic > e1.semantic)
            return 1;
        else
            return 0;
    }
}
