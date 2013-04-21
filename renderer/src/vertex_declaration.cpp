#include "pch.h"
#include "vertex_declaration.h"
#include "elog.h"
#include "emem.h"
#include "render_system.h"
#include "math.h"
#include "estring.h"

static inline bool _is_digital(char c)
{
    return c >= '0' && c <= '9';
}
element_semantic _get_ele_sem(char c)
{
    switch (c)
    {
    case 'p':
        return Position;
    case 't':
        return TexCoord;
    case 'n':
        return Normal;
    case 'c':
        return Color;
    case 'T':
        return Tangent;
    case 'B':
        return Binormal;
    default:
        return EmptySemantic;
    }
}
element_type _get_ele_type(const char* str, char** next)
{
    uint32 ret = 0;
    switch (str[0])
    {
    case 'f':
        ret = FLOAT_TYPE;
        break;
    case 'i':
        ret = UINT_TYPE;
        break;
    case 'b':
        ret = UBYTE_TYPE;
        break;
    default:
        *next = NULL;
        return EmptyType;
    }
    const char* d = &str[1];
    int count = 0;
    while ( _is_digital(d[count]) )
    {
        count++;
    }
    if (!count)
    {
        *next = NULL;
        return EmptyType;
    }

    *next = (char*)&d[count];
    int num = 0;
    do
    {
        count--;
        num += ((d[count] - '0') * (int)powf(10.0f, (float)count));
    } while (count >= 0);

    ret |= num;
    switch (ret)
    {
    case Float32_1:
    case Float32_2:
    case Float32_3:
    case Float32_4:
    case Ubyte_1:
    case Ubyte_2:
    case Ubyte_3:
    case Ubyte_4:
    case Uint32_1:
    case Uint32_2:
        break;
    default:
        *next = NULL;
        return EmptyType;
    }

    return (element_type)ret;
}

uint _get_vtx_eles(char* str, vertex_element* eles, uint max_eles)
{
    if (!max_eles)
        max_eles = 1;
    uint count = 0;
    char* s = str;
    while (*s)
    {
        if (count > max_eles - 1)
            return count;

        element_semantic sem = _get_ele_sem(*s);
        if (sem == EmptySemantic)
            return 0;
        s++;
        element_type type = _get_ele_type(s, &s);
        if (type == EmptyType)
            return 0;
        eles[count].semantic = sem;
        eles[count].type = type;

        count++;
    }
    return count;
}

void VertexDecl_Dest(VertexElement* _eles)
{
    for (uint i = 0; i < array_n(_eles); i++)
    {
        VertexElement element = _eles[i];
        VertexElement_delete(element);
    }
    array_delete(_eles);
}

VertexDecl VertexDecl_new()
{
    VertexElement null_ele = {EmptyType, EmptySemantic};
	VertexDecl ret = (VertexDecl)SMalloc(sizeof(vertex_decl));
    ret->elements = array_new(VertexElement, 5, null_ele);
    ret->decl_string = NULL;
    return ret;
}

VertexDecl VertexDecl_new_from_string(const char* str)
{
    VertexElement null_ele = {EmptyType, EmptySemantic};
    vertex_element eles[16];
    uint n = _get_vtx_eles((char*)str, eles, 16);
    vertex_decl tmp;
    tmp.num_elements = n;
    tmp.elements = eles;
    tmp.decl_string = str;
    if (n)
        return RenderSystem_register_vertex_declaration(&tmp);
    else
        return NULL;
}

void _VertexDecl_delete(VertexDecl _dec)
{
    VertexDecl_Dest(_dec->elements);
    if (_dec->decl_string)
        EString_delete(_dec->decl_string);
    Mfree(_dec);
}

void VertexDecl_add_element(VertexDecl _dec, VertexElement _ele)
{
    for (uint i = 0; i < array_n(_dec->elements); i++)
    {
        VertexElement element = _dec->elements[i];
        if (VertexElement_get_semantic(element) == VertexElement_get_semantic(_ele))
        {
            return;
        }
    }
    apush(_dec->elements, _ele);
    _dec->num_elements = array_n(_dec->elements);
}

uint32 VertexDecl_count(VertexDecl _dec)
{
    return array_n(_dec->elements);
}

VertexElement VertexDecl_find(VertexDecl _dec, uint32 _i)
{
    return array_safe_get(_dec->elements, _i);
}

const char* VertexDecl_get_decl_string(VertexDecl _dec)
{
    return _dec->decl_string;
}

uint VertexDecl_get_vertex_size(VertexDecl _dec)
{
    uint ret = 0;
    for (uint i = 0; i < array_n(_dec->elements); i++)
    {
        VertexElement ele = _dec->elements[i];
        ret += get_element_size(VertexElement_get_type(ele));
    }
    return ret;
}

bool VertexDecl_test(VertexDecl _dec)
{
    bool find_pos_sem_flag = false;
    for (uint i = 0; i < array_n(_dec->elements); i++)
    {
        VertexElement ele = _dec->elements[i];
        if (VertexElement_get_semantic(ele) == Position)
        {
            find_pos_sem_flag = true;
            break;
        }
    }
    return find_pos_sem_flag;
}

bool VertexDecl_test_element_semantic(VertexDecl _dec, element_semantic _sem)
{
    bool find_pos_sem_flag = false;
    for (uint i = 0; i < array_n(_dec->elements); i++)
    {
        VertexElement ele = _dec->elements[i];
        if (VertexElement_get_semantic(ele) == _sem)
        {
            find_pos_sem_flag = true;
            break;
        }
    }
    return find_pos_sem_flag;
}

VertexDecl VertexDecl_clone(VertexDecl _dec)
{
    VertexDecl ret = VertexDecl_new();
    for (uint i = 0; i < _dec->num_elements; i++)
    {
        VertexElement ele = _dec->elements[i];
        VertexDecl_add_element(ret, ele);
    }
    ret->num_elements = _dec->num_elements;
    if (_dec->decl_string)
        ret->decl_string = EString_new(_dec->decl_string);
    return ret;
}

int VertexDecl_compare(VertexDecl d0, VertexDecl d1)
{
    uint num0 = d0->num_elements;
    uint num1 = d1->num_elements;
    if (num0 < num1)
    {
        for (uint i = 0; i < num0; i++)
        {
            if      ( VertexElement_compare(d0->elements[i], d1->elements[i]) < 0 )
                return -1;
            else if ( VertexElement_compare(d0->elements[i], d1->elements[i]) > 0 )
                return 1;
        }
        return -1;
    }
    else
    {
        for (uint i = 0; i < num1; i++)
        {
            if      ( VertexElement_compare(d0->elements[i], d1->elements[i]) < 0 )
                return -1;
            else if ( VertexElement_compare(d0->elements[i], d1->elements[i]) > 0 )
                return 1;
        }
        if (num0 != num1)
            return 1;
        else
            return 0;
    }
}
