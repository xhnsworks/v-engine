#include "pch.h"
#include "branch_node.h"
#include "shader_node_base.h"
#include "array.h"
#include "estring.h"

typedef struct _condition
{
    ShaderObject left_obj;
    compare_operation compare_op;
    ShaderObject right_obj;
} condition;

typedef struct _branch_block
{
    condition cond;
    CircuitBoard board;
} branch_block;
typedef struct _branch_node
{
    shader_node_base base;
    branch_block* branch_blocks;
    CircuitBoard else_block;
} branch_node;
#ifdef __cplusplus
void _compile_block_vc(int& remainder, int& offs, char*& str, CircuitBoard board)
{
	{
		offs = snprintf(str, remainder, "\n{\n");
		str += offs;
		remainder -= offs;
	}
	{
		char* tmp = CircuitBoard_compile(board);
		if (!tmp)
			return;
		offs = snprintf(str, remainder, "%s\n", tmp);
		str += offs;
		remainder -= offs;
		EString_delete(tmp);
	}
	{
		offs = snprintf(str, remainder, "}\n");
		str += offs;
		remainder -= offs;
	}
}
#endif
const char* BranchNode_compile(BranchNode _self)
{
    ///int remainder = STRING_BUFFER_SIZE;
    char mbuf[STRING_BUFFER_SIZE];
    int remainder = STRING_BUFFER_SIZE;
    char* str = mbuf;
    remainder--;
    int offs = 0;
    offs = snprintf(str, remainder, "\n");
    str += offs;
    remainder -= offs;
#ifndef __cplusplus
    void _compile_block(CircuitBoard board)
    {
        {
            offs = snprintf(str, remainder, "\n{\n");
            str += offs;
            remainder -= offs;
        }
        {
            char* tmp = CircuitBoard_compile(board);
            if (!tmp)
                return;
            offs = snprintf(str, remainder, "%s\n", tmp);
            str += offs;
            remainder -= offs;
            EString_delete(tmp);
        }
        {
            offs = snprintf(str, remainder, "}\n");
            str += offs;
            remainder -= offs;
        }
    }
#endif
    uint n = array_n(_self->branch_blocks);
    for (uint i = 0; i < n; i++)
    {
        condition cond = _self->branch_blocks[i].cond;
        if (!ShaderObject_equal(cond.left_obj, cond.right_obj))
        {
            const char* left_name = ShaderObject_get_name(cond.left_obj);
            const char* right_name = ShaderObject_get_name(cond.right_obj);
            uint left_array_size, left_array_index;
            uint right_array_size, right_array_index;
            shader_object_type left_type = ShaderObject_get_type(cond.left_obj, &left_array_size, &left_array_index);
            shader_object_type right_type = ShaderObject_get_type(cond.right_obj, &right_array_size, &right_array_index);
            return NULL;
        }
        if (i == 0)
        {
            offs = snprintf(str, remainder, "if      (");
            str += offs;
            remainder -= offs;
        }
        else
        {
            offs = snprintf(str, remainder, "else if (");
            str += offs;
            remainder -= offs;
        }
        switch (cond.compare_op)
        {
        case Equal:
            offs = snprintf(str, remainder, "%s == %s)", ShaderObject_get_name(cond.left_obj), ShaderObject_get_name(cond.right_obj));
            break;
        case Less:
            offs = snprintf(str, remainder, "%s < %s)", ShaderObject_get_name(cond.left_obj), ShaderObject_get_name(cond.right_obj));
            break;
        case Greater:
            offs = snprintf(str, remainder, "%s < %s)", ShaderObject_get_name(cond.left_obj), ShaderObject_get_name(cond.right_obj));
            break;
        case LessEqual:
            offs = snprintf(str, remainder, "%s <= %s)", ShaderObject_get_name(cond.left_obj), ShaderObject_get_name(cond.right_obj));
            break;
        case GreaterEqual:
            offs = snprintf(str, remainder, "%s >= %s)", ShaderObject_get_name(cond.left_obj), ShaderObject_get_name(cond.right_obj));
            break;
        case NotEqual:
            offs = snprintf(str, remainder, "%s != %s)", ShaderObject_get_name(cond.left_obj), ShaderObject_get_name(cond.right_obj));
            break;
        default:
            return NULL;
        }
        str += offs;
        remainder -= offs;
#ifndef __cplusplus
        _compile_block(_self->branch_blocks[i].board);
#else
		_compile_block_vc(remainder, offs, str, _self->branch_blocks[i].board);
#endif
    }
    if (_self->else_block)
    {
        offs = snprintf(str, remainder, "else");
        str += offs;
        remainder -= offs;
#ifndef __cplusplus
        _compile_block(_self->else_block);
#else
		_compile_block_vc(remainder, offs, str, _self->else_block);
#endif
    }
    *str = 0x00;
    const char* ret = EString_new(mbuf);
    return ret;
}
void BranchNode_Dest(BranchNode _self, const char* _file, uint _line)
{
    uint n = array_n(_self->branch_blocks);
    for (uint i = 0; i < n; i++)
    {
        ShaderObject_delete(_self->branch_blocks[i].cond.left_obj);
        ShaderObject_delete(_self->branch_blocks[i].cond.right_obj);
        CircuitBoard_delete(_self->branch_blocks[i].board);
    }
    array_delete(_self->branch_blocks);
    if (_self->else_block)
        CircuitBoard_delete(_self->else_block);
}
BranchNode _BranchNode_clone(BranchNode _bn, const char* _file, uint _line);
void BranchNode_Init(BranchNode _self, const char* _file, uint _line)
{
    _self->base.init_proc = (shader_node_init_proc)BranchNode_Init;
    _self->base.dest_proc = (shader_node_dest_proc)BranchNode_Dest;
    _self->base.clone_proc = (shader_node_clone_proc)_BranchNode_clone;
    _self->base.compile_proc = (shader_node_compile_proc)BranchNode_compile;
    branch_block null_brh_blk = {{NULL, EmptyCompare, NULL}, NULL};
    _self->branch_blocks = array_new(branch_block, 20, null_brh_blk);
    _self->else_block = NULL;
}

BranchNode _BranchNode_clone(BranchNode _bn, const char* _file, uint _line)
{
    BranchNode ret = (BranchNode)SMalloc(sizeof(branch_node));
    ret->base.init_proc = (shader_node_init_proc)BranchNode_Init;
    ret->base.dest_proc = (shader_node_dest_proc)BranchNode_Dest;
    ret->base.clone_proc = (shader_node_clone_proc)_BranchNode_clone;
    ret->base.compile_proc = (shader_node_compile_proc)BranchNode_compile;
    branch_block null_brh_blk = {{NULL, EmptyCompare, NULL}, NULL};
    uint n = array_n(_bn->branch_blocks);
    ret->branch_blocks = array_new(branch_block, n, null_brh_blk);
    for (uint i = 0; i < n; i++)
    {
        ShaderObject a = _bn->branch_blocks[i].cond.left_obj;
        ShaderObject b = _bn->branch_blocks[i].cond.right_obj;
        compare_operation op = _bn->branch_blocks[i].cond.compare_op;
        CircuitBoard board = CircuitBoard_clone(_bn->branch_blocks[i].board, _file, _line);

        branch_block block = {{ShaderObject_clone(a), op, ShaderObject_clone(b)}, board};
        apush(ret->branch_blocks, block);
    }
    if (_bn->else_block)
    {
        ret->else_block = CircuitBoard_clone(_bn->else_block, _file, _line);
    }
    else
    {
        ret->else_block = NULL;
    }
    return ret;
}

BranchNode BranchNode_new()
{
    BranchNode ret = (BranchNode)SMalloc(sizeof(branch_node));
    BranchNode_Init(ret, __FILE__, __LINE__);
    return ret;
}
void BranchNode_delete(BranchNode _self)
{
    ShaderNodeBase_delete((ShaderNodeBase)_self, __FILE__, __LINE__);
}

CircuitBoard BranchNode_add_branch(BranchNode _self, ShaderObject _a, compare_operation _op, ShaderObject _b)
{
    CircuitBoard board = CircuitBoard_new();
    branch_block block = {{ShaderObject_clone(_a), _op, ShaderObject_clone(_b)}, board};
    apush(_self->branch_blocks, block);
    return board;
}

CircuitBoard BranchNode_set_else_branch(BranchNode _self)
{
    if (_self->else_block)
        return NULL;
    CircuitBoard board = CircuitBoard_new();
    _self->else_block = board;
    return board;
}
