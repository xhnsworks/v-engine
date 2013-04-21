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
    for (uint i = 0; i < array_n(_self->shader_node_reference_table); i++)
    {
        ShaderNodeBase_delete(_self->shader_node_reference_table[i], __FILE__, __LINE__);
    }
    array_delete(_self->shader_node_reference_table);
}
CircuitBoard CircuitBoard_new()
{
    CircuitBoard ret = Malloc(sizeof(circuit_board));
    CircuitBoard_Init(ret);
    return ret;
}
void CircuitBoard_delete(CircuitBoard _self)
{
    CircuitBoard_Dest(_self);
    Mfree(_self);
}
ShaderNode _CircuitBoard_add_reference_node(CircuitBoard _self, ShaderNode _prototype_node, const char* _file, uint _line)
{
    ///ShaderNode csn = _ShaderNode_clone(_prototype_node, __FILE__, __LINE__);
    ShaderNodeBase snb = (ShaderNodeBase)_prototype_node;
    ShaderNode csn = snb->shader_node_clone_proc(_prototype_node, _file, _line);
    apush(_self->shader_node_reference_table, csn);
    return csn;
}
char* CircuitBoard_compile(CircuitBoard _self)
{
    int remainder = STRING_BUFFER_SIZE;
    int offs = 0;
    char mbuf[remainder];
    char* str = mbuf;
    remainder--;
    uint n = array_n(_self->shader_node_reference_table);
    for (uint i = 0; i < n; i++)
    {
        ShaderNode node = _self->shader_node_reference_table[i];
        ShaderNodeBase base = (ShaderNodeBase)node;
        const char* compiled = base->compile_proc(base);
        EAssert(compiled, "compile shader node fail!");
        offs = snprintf(str, remainder, "%s\n", compiled);
        str += offs;
        remainder -= offs;
        EString_delete(compiled);
    }
    return EString_new(mbuf);
}
CircuitBoard CircuitBoard_clone(CircuitBoard _self, const char* _file, uint _line)
{
    CircuitBoard ret = _Malloc(sizeof(circuit_board), _file, _line);
    uint n = array_n(_self->shader_node_reference_table);
    ret->shader_node_reference_table = array_new(ShaderNode, n, NULL);
    for (uint i = 0; i < n; i++)
    {
        ShaderNode sn = ShaderNode_clone(_self->shader_node_reference_table[i]);
        apush(ret->shader_node_reference_table, sn);
    }
    return ret;
}
