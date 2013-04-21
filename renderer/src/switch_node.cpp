#include "pch.h"
#include "switch_node.h"
#include "shader_node_base.h"
#include "array.h"
#include "estring.h"
#include "tree.h"
#include "eassert.h"
#include "elog.h"

typedef struct _switch_node
{
    shader_node_base base;
    ShaderObject switch_obj;
    Tree switch_tree;
} switch_node;

void SwitchNode_Dest(SwitchNode _self, const char* _file, uint _line)
{
    Iterator iter = Tree_begin(_self->switch_tree);
    while (iter)
    {
        var key = Tree_get_key(iter);
        var data = Tree_get_value(iter);
        sint* tags = (sint*)key.vptr_var;
        array_delete(tags);
        CircuitBoard board = (CircuitBoard)data.vptr_var;
        CircuitBoard_delete(board);
        iter = Tree_next(iter);
    }
    Tree_Dest(_self->switch_tree);
    if (to_ptr(_self->switch_obj))
        ShaderObject_delete(_self->switch_obj);
}
char* SwitchNode_compile(SwitchNode _self)
{
    if (!to_ptr(_self->switch_obj))
        return NULL;
    string_buffer_new(STRING_BUFFER_SIZE);
    sbuf_printf("switch ( %s )\n{\n", ShaderObject_get_name(_self->switch_obj));
    Iterator iter = Tree_begin(_self->switch_tree);
    while (iter)
    {
        var key = Tree_get_key(iter);
        var data = Tree_get_value(iter);
        sint* tags = (sint*)key.vptr_var;
        uint n = array_n(tags);
        for (uint i = 0; i < n; i++)
        {
            sbuf_printf("case %d:\n", tags[i]);
        }
        CircuitBoard cb = (CircuitBoard)data.vptr_var;
        sbuf_printf("%s", "{\n");
        char* compiled = CircuitBoard_compile(cb);
        sbuf_printf("%s\n", compiled);
        EString_delete(compiled);
        sbuf_printf("%s", "}\nbreak;\n");
        iter = Tree_next(iter);
    }
    sbuf_printf("%s", "}\n");
    return (char*)EString_new(get_string_buffer);
}
void SwitchNode_Init(SwitchNode _self, const char* _file, uint _line);
SwitchNode SwitchNode_clone(SwitchNode _self, const char* _file, uint _line)
{
    SwitchNode ret = (SwitchNode)SMalloc(sizeof(switch_node));
    ret->base.init_proc = (shader_node_init_proc)SwitchNode_Init;
    ret->base.dest_proc = (shader_node_dest_proc)SwitchNode_Dest;
    ret->base.clone_proc = (shader_node_clone_proc)SwitchNode_clone;
    ret->base.compile_proc = (shader_node_compile_proc)SwitchNode_compile;

    ret->switch_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    Iterator iter = Tree_begin(_self->switch_tree);
    while (iter)
    {
        var key = Tree_get_key(iter);
        var data = Tree_get_value(iter);

        sint* src_tags = (sint*)key.vptr_var;
        uint n = array_n(src_tags);
        sint* dst_tags = array_new(sint, n, -1);
        for (uint i = 0; i < n; i++)
        {
            ///dst_tags[i] = src_tags[i];
            apush(dst_tags, src_tags[i]);
        }

        CircuitBoard src_bd = (CircuitBoard)data.vptr_var;
        CircuitBoard dst_bd = CircuitBoard_clone(src_bd, _file, _line);

        key.vptr_var = dst_tags;
        data.vptr_var = dst_bd;
        Tree_insert(ret->switch_tree, key, data);

        iter = Tree_next(iter);
    }
    if (to_ptr(_self->switch_obj))
        ret->switch_obj = ShaderObject_clone(_self->switch_obj);
    else
        ret->switch_obj.self = NULL;

    return ret;
}
void SwitchNode_Init(SwitchNode _self, const char* _file, uint _line)
{
    _self->base.init_proc = (shader_node_init_proc)SwitchNode_Init;
    _self->base.dest_proc = (shader_node_dest_proc)SwitchNode_Dest;
    _self->base.clone_proc = (shader_node_clone_proc)SwitchNode_clone;
    _self->base.compile_proc = (shader_node_compile_proc)SwitchNode_compile;

    _self->switch_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    _self->switch_obj.self = NULL;
}

SwitchNode _SwitchNode_new(const char* _file, uint _line)
{
    SwitchNode ret = (SwitchNode)SMalloc(sizeof(switch_node));
    SwitchNode_Init(ret, _file, _line);
    return ret;
}
void _SwitchNode_delete(SwitchNode _self, const char* _file, uint _line)
{
    ShaderNodeBase_delete((ShaderNodeBase)_self, _file, _line);
}
CircuitBoard SwitchNode_add_branch_block(SwitchNode _self, sint* _tags)
{
    uint n = array_n(_tags);
    sint* tags = array_new(sint, n, -1);
    for (uint i = 0; i < n; i++)
    {
        apush(tags, _tags[i]);
    }
    CircuitBoard ret = CircuitBoard_new();
    var key, data;
    key.vptr_var = tags;
    data.vptr_var = ret;
    Tree_insert(_self->switch_tree, key, data);
    return ret;
}
void SwitchNode_set_switch_object(SwitchNode _self, ShaderObject _so)
{
    if (to_ptr(_self->switch_obj))
        return;
    uint array_size, array_index;
    shader_object_type type = ShaderObject_get_type(_so, &array_size, &array_index);
    EAssert(type == Int_Obj, "shader object %s is not int!", ShaderObject_get_name(_so));
    EAssert(array_size == 1 || array_index != INVALID_ARRAY_INDEX, "shader object %s is array!", ShaderObject_get_name(_so));
    _self->switch_obj = ShaderObject_clone(_so);
}
