#include "vertex_declaration.h"
#include "elog.h"
void VertexDeclaration_Dest(Stack _dec_stk)
{
    uint32 i = 0;
    for (; i < Stack_count(_dec_stk); i++)
    {
        var* var = Stack_find(_dec_stk, i);
        if (var)
        {
            VertexElement element;
            var_to_vtx_ele(element, *var);

            if (!VertexElement_is_null(element))
            {
                VertexElement_delete(element);
            }
        }
    }
    Stack_Dest(_dec_stk);
}

VertexDeclaration VertexDeclaration_new()
{
    VertexDeclaration ret;
    ret.self = Stack_new(Uint32);
    return ret;
}

void _VertexDeclaration_delete(VertexDeclaration _dec)
{
    VertexDeclaration_Dest(_dec.self);
    Mfree(_dec.self);
}

void VertexDeclaration_add_element(VertexDeclaration _dec, VertexElement _ele)
{
    uint32 i = 0;
    for (; i < Stack_count(_dec.self); i++)
    {
        var* var = Stack_find(_dec.self, i);
        VertexElement element;
        var_to_vtx_ele(element, *var);
        if (VertexElement_get_semantic(element) == VertexElement_get_semantic(_ele))
        {
            return;
        }
    }
    var v;
    vtx_ele_to_var(v, _ele);
    Stack_push(_dec.self, v);
    return;
}

uint32 VertexDeclaration_count(VertexDeclaration _dec)
{
    return Stack_count(_dec.self);
}

VertexElement VertexDeclaration_find(VertexDeclaration _dec, uint32 _i)
{
    var* v = Stack_find(_dec.self, _i);
    if (v)
    {
        VertexElement ret;
        var_to_vtx_ele(ret, *v);
        return ret;
    }
    else
    {
        return VertexElement_null();
    }
}

bool VertexDeclaration_test(VertexDeclaration _dec)
{
    bool find_pos_sem_flag = false;
    for (uint32 i = 0; i < VertexDeclaration_count(_dec); i++)
    {
        VertexElement ele = VertexDeclaration_find(_dec, i);
        if (VertexElement_get_semantic(ele) == Position)
        {
            find_pos_sem_flag = true;
            break;
        }
    }
    return find_pos_sem_flag;
}

bool VertexDeclaration_test_element_semantic(VertexDeclaration _dec, element_semantic _sem)
{
    bool find_pos_sem_flag = false;
    for (uint32 i = 0; i < VertexDeclaration_count(_dec); i++)
    {
        VertexElement ele = VertexDeclaration_find(_dec, i);
        if (VertexElement_get_semantic(ele) == _sem)
        {
            find_pos_sem_flag = true;
            break;
        }
    }
    return find_pos_sem_flag;
}
