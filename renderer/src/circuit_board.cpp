#include "pch.h"
#include "circuit_board.h"
#include "shader_node_base.h"
#include "estring.h"
#include "eassert.h"
#include "elog.h"
void CircuitBoard_Init(CircuitBoard _self)
{
    _self->shader_node_reference_table = array_new(ShaderNode, 20, NULL);
}
void CircuitBoard_Dest(CircuitBoard _self)
{
    for (euint32 i = 0; i < array_n(_self->shader_node_reference_table); i++)
    {
        ShaderNodeBase_delete((ShaderNodeBase)_self->shader_node_reference_table[i]);
    }
    array_delete(_self->shader_node_reference_table);
}
CircuitBoard CircuitBoard_new()
{
    CircuitBoard ret = (CircuitBoard)SMalloc(sizeof(s_circuit_board));
    CircuitBoard_Init(ret);
    return ret;
}
void CircuitBoard_delete(CircuitBoard _self)
{
    CircuitBoard_Dest(_self);
    Mfree(_self);
}
ShaderNode CircuitBoard_add_reference_node(CircuitBoard _self, ShaderNode _prototype_node)
{
    ///ShaderNode csn = _ShaderNode_clone(_prototype_node, __FILE__, __LINE__);
    ShaderNodeBase snb = (ShaderNodeBase)_prototype_node;
    ShaderNode csn = (ShaderNode)snb->clone_proc((ShaderNodeBase)_prototype_node);
    apush(_self->shader_node_reference_table, csn);
    return csn;
}
char* CircuitBoard_compile(CircuitBoard _self)
{
    int offs = 0;
    char mbuf[STRING_BUFFER_SIZE];
	int remainder = STRING_BUFFER_SIZE;
    char* str = mbuf;
	str[0] = 0x00;
    remainder--;
    euint32 n = array_n(_self->shader_node_reference_table);
    for (euint32 i = 0; i < n; i++)
    {
        ShaderNode node = _self->shader_node_reference_table[i];
        ShaderNodeBase base = (ShaderNodeBase)node;
        const char* compiled = base->compile_proc(base);
        EAssert(compiled, "%s", "compile shader node fail!");
        offs = snprintf(str, remainder, "%s\n", compiled);
        str += offs;
        remainder -= offs;
        EString_delete(compiled);
    }
    return (char*)EString_new(mbuf);
}
CircuitBoard CircuitBoard_clone(CircuitBoard _self)
{
    CircuitBoard ret = (CircuitBoard)Malloc(sizeof(s_circuit_board));
    euint32 n = array_n(_self->shader_node_reference_table);
    ret->shader_node_reference_table = array_new(ShaderNode, n, NULL);
    for (euint32 i = 0; i < n; i++)
    {
        ShaderNode sn = ShaderNode_clone(_self->shader_node_reference_table[i]);
        apush(ret->shader_node_reference_table, sn);
    }
    return ret;
}
