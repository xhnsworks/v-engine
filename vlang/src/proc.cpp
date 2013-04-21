#include "common.h"
#include "etypes.h"
#include "eassert.h"
#include "elog.h"
#include "emem.h"
#include "parser_env.h"
#include "symbol_stack.h"
#include "array.h"
#include "estring.h"

#include "float_op_float.h"
#include "float_op_int.h"
#include "int_op_float.h"
#include "int_op_int.h"

#include "function_call.h"

#include "single_op_single.h"
#include "jump.h"
#include "print.h"

#include <string.h>

void _StringOutput(const char* str)
{
    VPRINT("%s\n", str);
}
void PrintSymbolValue(ParserEnv* e, const char* str, SymbolValue value)
{
    VPRINT(str);
    const char* type_str = _get_type_str((ValueType)value.type);
    VPRINT("TYPE %s, ", type_str);
    if ((ValueType)value.type != UnknownValue)
        _value_string_output(e, value, _StringOutput);
    else
    {
        Iterator iter = value.data.tree_iter;
        var key = Tree_get_key(iter);
        const char* sym_str = (const char*)key.str_var;
        VPRINT("%s\n", sym_str);
    }
}

SymbolValue FloatTypeSym()
{
    SymbolValue ret;
    ret.type = FloatValue;
    ret.data.mem_addr.mem_type = MaxMem;
    ret.data.mem_addr.offs = -1;
    return ret;
}
SymbolValue IntTypeSym()
{
    SymbolValue ret;
    ret.type = IntValue;
    ret.data.mem_addr.mem_type = MaxMem;
    ret.data.mem_addr.offs = -1;
    return ret;
}

void BeginFuncDef(ParserEnv* e, SymbolValue _func_name)
{
    var key = Tree_get_key(_func_name.data.tree_iter);
    const char* func_name = (const char*)key.str_var;
    ///printf("new func %s\n", func_name);
    ParserEnv_new_function(e, func_name);
}

void CompleteFuncDef(ParserEnv* e)
{
    ParserEnv_complete_function(e);
}

void PrintSymbol(ParserEnv* e, SymbolValue _sym)
{
    const char* type_str = _get_type_str((ValueType)_sym.type);
    VPRINT("TYPE %s, VALUE ", type_str);
    if ((ValueType)_sym.type != UnknownValue)
        _value_string_output(e, _sym, _StringOutput);
    else
    {
        Iterator iter = _sym.data.tree_iter;
        var key = Tree_get_key(iter);
        const char* sym_str = (const char*)key.str_var;
        VPRINT("%s\n", sym_str);
    }
}
void AddParam(ParserEnv* e, SymbolValue _sym, SymbolValue valuetype)
{
    ///printf("ParamsDef BEGIN\n");
    ///printf("SYM: ");
    ///PrintSymbol(e, _sym);
    ///printf("VALUETYPE: ");
    ///PrintSymbol(e, valuetype);
    ///printf("\n");
    Iterator iter = _sym.data.tree_iter;
    var key = Tree_get_key(iter);
    var data = Tree_get_value(iter);
    SymbolValue* sv = (SymbolValue*)data.vptr_var;
    sv->type = valuetype.type;
    if (valuetype.type != StringValue)
    {
        sv->data.mem_addr.mem_type = VariMem;
        sv->data.mem_addr.offs = ValueAllocator_alloc(&e->vari_alloc, (ValueType)valuetype.type);
    }
    else
    {
        sv->data.mem_addr.mem_type = VariStrMem;
        sv->data.mem_addr.offs = ValueAllocator_alloc(&e->vari_str_alloc, (ValueType)valuetype.type);
    }
    FunctionClosures_push_vari(e->curt_func, (const char*)key.str_var, sv);
    ///printf("ParamsDef END\n");
    ///
}
void CompleteParamDef(ParserEnv* e)
{
    FunctionClosures_complete(e, e->curt_func);
}
void CompleteOutputDef(ParserEnv* e)
{
    FunctionClosures_complete(e, e->curt_func);
}

static inline uint _value_size(ParserEnv* e, SymbolValue src)
{
    switch (src.type)
    {
    case BoolValue:
    {
        return sizeof(bool);
    }
    case IntValue:
    {
        return sizeof(int);
    }
    case FloatValue:
    {
        return sizeof(float);
    }
    case Float2Value:
    {
        return sizeof(float) * 2;
    }
    case Float3Value:
    {
        return sizeof(float) * 3;
    }
    case Float4Value:
    {
        return sizeof(float) * 4;
    }
    case Matrix4x4Value:
    {
        return sizeof(float) * 16;
    }
    case StringValue:
    {
        char* str = e->mems[src.data.mem_addr.mem_type];
        uint len = strlen(str);
        return len + 1;
    }
    case Function:
    {
        return sizeof(FunctionClosures);
    }
    default:
        return 0;
    }
}
static inline SymbolValue _init_value_assign(ParserEnv* e, SymbolValue dst, SymbolValue src)
{
    if (dst.type != UnknownValue)
    {
        /// error
    }

    uint size = _value_size(e, src);
    if (!size)
    {
        /// error
    }
    dst.type = src.type;
    /// if type is Unknown then value is iterator
    Iterator iter = dst.data.tree_iter;
    var data = Tree_get_value(iter);
    SymbolValue* sv = (SymbolValue*)data.vptr_var;
    /// set value in symbol tree
    sv->type = src.type;
    sv->data.mem_addr.mem_type = VariMem;
    sv->data.mem_addr.offs = ValueAllocator_alloc(&e->vari_alloc, (ValueType)dst.type);

    dst.data.mem_addr.mem_type = VariMem;
    dst.data.mem_addr.offs = sv->data.mem_addr.offs;

    return dst;
}


static inline int _get_op_type(OperatorType op)
{
    switch (op)
    {
    case AddOp:
    case SubOp:
    case MulOp:
    case DivOp:
        return 2;
    case AssignOp:
        return 1;
    case LessOp:
    case GreaterOp:
    case EqualOp:
    case LessEqualOp:
    case GreaterEqualOp:
        return 3;
    default:
        return 0;
    }
}

CommandClosures SOP(ParserEnv* e, OperatorType op, SymbolValue dst, SymbolValue src, bool dst_is_func_arg, bool src_is_func_arg)
{
    CommandClosures cc = NULL;
    int op_type = _get_op_type(op);
    switch (op_type)
    {
    case 1:
        cc = SingleSymbolOperator1(e, op, dst, src);
        break;
    case 2:
        cc = SingleSymbolOperator2(e, op, dst, src);
        break;
    case 3:
        cc = SingleSymbolOperator3(e, op, dst, src);
        break;
    default:
        break;
    }
    if      (dst_is_func_arg && !src_is_func_arg)
    {
        apush(e->curt_func->func_arg_op_cmds_type_a_1, cc);
    }
    else if (!dst_is_func_arg && src_is_func_arg)
    {
        apush(e->curt_func->func_arg_op_cmds_type_b_1, cc);
    }
    else if (dst_is_func_arg && src_is_func_arg)
    {
        apush(e->curt_func->func_arg_op_cmds_type_c_1, cc);
    }
    FunctionClosures_add_command(e->curt_func, cc);
    return cc;
}

SymbolValue MultSymbolOperator(ParserEnv* e, OperatorType op, SymbolValue dst, SymbolValue src)
{
    if      ( (dst.type != ValueArray && dst.type != FuncArgValueArray) && (src.type != ValueArray && src.type != FuncArgValueArray) )
    {
        CommandClosures cc = SOP(e, op, dst, src, false, false);
        return CommandClosures_get_last_arg(cc);
    }
    else if ( (dst.type == ValueArray || dst.type == FuncArgValueArray) && (src.type != ValueArray && src.type != FuncArgValueArray) )
    {
        SymbolValue sv = array_safe_get(dst.data.value_array, 0);
        CommandClosures cc = SOP(e, op, sv, src, dst.type == FuncArgValueArray, false);
        return CommandClosures_get_last_arg(cc);
    }
    else if ( (dst.type != ValueArray && dst.type != FuncArgValueArray) && (src.type == ValueArray || src.type == FuncArgValueArray) )
    {
        SymbolValue sv = array_safe_get(src.data.value_array, 0);
        CommandClosures cc = SOP(e, op, dst, sv, false, src.type == FuncArgValueArray);
        return CommandClosures_get_last_arg(cc);
    }
    else if ( (dst.type == ValueArray || dst.type == FuncArgValueArray) && (src.type == ValueArray || src.type == FuncArgValueArray) )
    {
        uint dst_n = array_n(dst.data.value_array);
        uint src_n = array_n(src.data.value_array);
        uint n = dst_n;
        if (n < src_n)
            n = src_n;

        ///CommandClosures ccs[n];
		CommandClosures* ccs = (CommandClosures*)SMalloc(sizeof(CommandClosures) * n);
        for (uint i = 0; i < n; i++)
        {
            SymbolValue dst_sv = array_safe_get(dst.data.value_array, i);
            SymbolValue src_sv = array_safe_get(src.data.value_array, i);
            ccs[i] = SOP(e, op, dst_sv, src_sv, dst.type == FuncArgValueArray, src.type == FuncArgValueArray);
        }
        SymbolValue ret;
        ret.type = ValueArray;
        ///SymbolValue null_value;
        ///null_value.type = Unknown;
        ///memset(&null_value.data, 0, sizeof(null_value.data));
        ret.data.value_array = _symbol_value_array_new(n);
        for (uint i = 0; i < n; i++)
        {
            apush(ret.data.value_array, CommandClosures_get_last_arg(ccs[i]));
        }
		Mfree(ccs);
        return ret;
    }
    else
    {
        /// error
        SymbolValue null_value;
        null_value.type = UnknownValue;
        memset(&null_value.data, 0, sizeof(null_value.data));
        return null_value;
    }
}

SymbolValue NewValueArray(ParserEnv* e, SymbolValue v)
{
    ///printf("VALUETYPE: ");
    ///PrintSymbol(e, v);

    SymbolValue ret;
    ret.type = ValueArray;
    ///SymbolValue null_value;
    ///null_value.type = Unknown;
    ///memset(&null_value.data, 0, sizeof(null_value.data));
    ///ret.data.value_array = array_new(SymbolValue, 5, null_value);
    ret.data.value_array = _symbol_value_array_new(5);
    apush(ret.data.value_array, v);
    return ret;
}

SymbolValue ValueArrayPush(ParserEnv* e, SymbolValue dst, SymbolValue v)
{
    ///printf("VALUE: ");
    ///PrintSymbol(e, v);
    ///printf("DST: ");
    ///PrintSymbol(e, dst);

    apush(dst.data.value_array, v);
    return dst;
}

SymbolValue VarDef(ParserEnv* e, SymbolValue sym, SymbolValue var_type)
{
    ///printf("SYM: ");
    ///PrintSymbol(e, sym);
    ///printf("VALUETYPE: ");
    ///PrintSymbol(e, var_type);

    if (UnknownValue == sym.type)
    {
        Iterator iter = sym.data.tree_iter;
        var data = Tree_get_value(iter);
        SymbolValue* sv = (SymbolValue*)data.vptr_var;

        /// set value in symbol tree
        sv->type = var_type.type;
        sv->data.mem_addr.mem_type = VariMem;
        sv->data.mem_addr.offs = ValueAllocator_alloc(&e->vari_alloc, (ValueType)var_type.type);

        sym.type = var_type.type;
        sym.data.mem_addr.mem_type = VariMem;
        sym.data.mem_addr.offs = sv->data.mem_addr.offs;
        return sym;
    }
    else
    {
        parser_exception exce = s_exce_table[Symbol_is_defined_more_than_once];
        ParserEnv_push_exce(e, exce);

        sym.type = UnknownValue;
        memset(&sym.data, 0,sizeof(sym.data));
        return sym;
    }
}

SymbolValue AddProc(ParserEnv* e, SymbolValue a, SymbolValue b)
{
    SymbolValue ret = MultSymbolOperator(e, AddOp, a, b);
    return ret;
}
SymbolValue SubProc(ParserEnv* e, SymbolValue a, SymbolValue b)
{
    SymbolValue ret = MultSymbolOperator(e, SubOp, a, b);
    return ret;
}
SymbolValue MulProc(ParserEnv* e, SymbolValue a, SymbolValue b)
{
    SymbolValue ret = MultSymbolOperator(e, MulOp, a, b);
    return ret;
}
SymbolValue DivProc(ParserEnv* e, SymbolValue a, SymbolValue b)
{
    SymbolValue ret = MultSymbolOperator(e, DivOp, a, b);
    return ret;
}

SymbolValue AssignProc(ParserEnv* e, SymbolValue dst, SymbolValue src)
{
    MultSymbolOperator(e, AssignOp, dst, src);
    return dst;
}
SymbolValue InitAssignProc(ParserEnv* e, SymbolValue dst, SymbolValue src)
{
    dst = _init_value_assign(e, dst, src);

    CommandClosures cc = CommandClosures_new(FloatAssignFloatCommand);
    CommandClosures_add_arg(cc, src);
    CommandClosures_add_arg(cc, dst);
    FunctionClosures_add_command(e->curt_func, cc);

    return dst;
}
///
SymbolValue FuncCall(ParserEnv* e, SymbolValue func)
{
    ///printf("FuncCall\n");
    FunctionClosures fc = func.data.func;
    SymbolValue ret;
    ret.type = FuncArgValueArray;
    ///SymbolValue null_value;
    ///null_value.type = Unknown;
    ///memset(&null_value.data, 0, sizeof(null_value.data));
    ///null_value.data.mem_type = MaxMem;
    ///ret.data.value_array = array_new(SymbolValue, Tree_count(fc->output_param_tree), null_value);
    ///ret.data.value_array = _symbol_value_array_new(Tree_count(fc->output_param_tree));
    ///Iterator iter = Tree_begin(fc->output_param_tree);
    ///while (iter)
    uint n = array_n(fc->output_param_array);
    ret.data.value_array = _symbol_value_array_new(n);
    for (uint i = 0; i < n; i++)
    {
        ///var data = Tree_get_value(iter);
        ///SymbolValue* sv = (SymbolValue*)data.vptr_var;
        ///apush(ret.data.value_array, *sv);
        ///iter = Tree_next(iter);
        apush(ret.data.value_array, *fc->output_param_array[i]);
    }
    VPRINT("@@@@@@@\n");
    VPRINT("@@@@@@@\n");
    for (uint i = 0; i < array_n(ret.data.value_array); i++)
    {
        char mbuf[256];
        snprintf(mbuf, 255, "FUNC RET ARRAY %d: ", i);
        PrintSymbolValue(e, mbuf, ret.data.value_array[i]);
    }
    VPRINT("@@@@@@@\n");
    VPRINT("@@@@@@@\n");
    CommandClosures cc = CommandClosures_new(FunctionCallCommand);
    CommandClosures_add_arg(cc, func);
    CommandClosures_add_arg(cc, e->curt_func->vari_stack_size);
    CommandClosures_add_arg(cc, e->curt_func->vari_str_stack_size);
    FunctionClosures_add_command(e->curt_func, cc);
    return ret;
}
SymbolValue FuncCall2(ParserEnv* e, SymbolValue func, SymbolValue a)
{
    ///parser_exception exce = {"error"};
    ///apush(e->exce_array, exce);
    ///printf("FuncCall2\n");
    FunctionClosures fc = func.data.func;
    ///uint n = Tree_count(fc->input_param_tree);
    uint n = array_n(fc->input_param_array);

    SymbolValue input;
    input.type = FuncArgValueArray;
    ///SymbolValue null_value;
    ///null_value.type = Unknown;
    ///memset(&null_value.data, 0, sizeof(null_value.data));
    ///input.data.value_array = array_new(SymbolValue, n, null_value);
    input.data.value_array = _symbol_value_array_new(n);
    ///Iterator iter = Tree_begin(fc->input_param_tree);
    ///while (iter)
    for (uint i = 0; i < n; i++)
    {
        ///var data = Tree_get_value(iter);
        ///SymbolValue* sv = data.vptr_var;
        ///apush(input.data.value_array, *sv);
        ///iter = Tree_next(iter);
        apush(input.data.value_array, *fc->input_param_array[i]);
    }
    ///apush(ret.data.value_array, v);
    MultSymbolOperator(e, AssignOp, input, a);
    return FuncCall(e, func);
}
///

SymbolValue PrintProc(ParserEnv* e, SymbolValue value)
{
    CommandClosures cc = NULL;
    switch (value.type)
    {
    case IntValue:
        cc = CommandClosures_new(PrintIntCommand);
        break;
    case FloatValue:
        cc = CommandClosures_new(PrintFloatCommand);
        break;
    case BoolValue:
        cc = CommandClosures_new(PrintBoolCommand);
        break;
    case ValueArray:
    {
        uint n = array_n(value.data.value_array);
        for (uint i = 0; i < n; i++)
        {
            SymbolValue v = value.data.value_array[i];
            PrintProc(e, v);
        }
        return value;
    }
    break;
    default:
    {
        parser_exception exce = s_exce_table[Invalid_operation];
        ParserEnv_push_exce(e, exce);
    }
    break;
    }
    if (cc)
    {
        CommandClosures_add_arg(cc, value);
        FunctionClosures_add_command(e->curt_func, cc);
    }
    return value;
}
/**
SymbolValue LessProc(ParserEnv* e, SymbolValue a, SymbolValue b);
SymbolValue GreaterProc(ParserEnv* e, SymbolValue a, SymbolValue b);
SymbolValue EqualProc(ParserEnv* e, SymbolValue a, SymbolValue b);
SymbolValue LessEqualProc(ParserEnv* e, SymbolValue a, SymbolValue b);
SymbolValue GreaterEqualProc(ParserEnv* e, SymbolValue a, SymbolValue b);

SymbolValue MarkIf(ParserEnv* e, SymbolValue bool_value);
SymbolValue MarkElseIf(ParserEnv* e, SymbolValue bool_value);
SymbolValue MarkElse(ParserEnv* e);
**/
SymbolValue LessProc(ParserEnv* e, SymbolValue a, SymbolValue b)
{
    SymbolValue ret = MultSymbolOperator(e, LessOp, a, b);
    return ret;
}
SymbolValue GreaterProc(ParserEnv* e, SymbolValue a, SymbolValue b)
{
    SymbolValue ret = MultSymbolOperator(e, GreaterOp, a, b);
    return ret;
}
SymbolValue EqualProc(ParserEnv* e, SymbolValue a, SymbolValue b)
{
    SymbolValue ret = MultSymbolOperator(e, EqualOp, a, b);
    return ret;
}
SymbolValue LessEqualProc(ParserEnv* e, SymbolValue a, SymbolValue b)
{
    SymbolValue ret = MultSymbolOperator(e, LessEqualOp, a, b);
    return ret;
}
SymbolValue GreaterEqualProc(ParserEnv* e, SymbolValue a, SymbolValue b)
{
    SymbolValue ret = MultSymbolOperator(e, GreaterEqualOp, a, b);
    return ret;
}
SymbolValue MarkIf(ParserEnv* e, SymbolValue bool_value)
{
    FunctionClosures_new_branch_block(e->curt_func);
    BranchBlock block = FunctionClosures_get_top_branch_block(e->curt_func);

    SymbolValue offs = _alloc_cons_value(e, IntValue);
    CommandClosures cc = CommandClosures_new(IfFalseJumpCommand);
    CommandClosures_add_arg(cc, offs);
    CommandClosures_add_arg(cc, bool_value);

    uint n = array_n(e->curt_func->cmds);
    branch_mark mark = {(int)n, cc, NULL};
    apush(block->branch_marks, mark);
    block->braces_count++;

    FunctionClosures_add_command(e->curt_func, cc);
    return bool_value;
}
SymbolValue MarkElseIf(ParserEnv* e, SymbolValue bool_value)
{
    return MarkIf(e, bool_value);
}
SymbolValue MarkElse(ParserEnv* e)
{
	SymbolValue bool_value;
	bool_value.type = (SymbolValueType)UnknownValue;
	memset(&bool_value.data, 0, sizeof(bool_value.data));
    return bool_value;
}
void MarkEnd(ParserEnv* e)
{
    /// 找到合适的顶部分支块
    BranchBlock block = FunctionClosures_get_top_branch_block(e->curt_func);
    block->braces_count--;
    if (block->braces_count < 0)
    {
        FunctionClosures_pop_branch_block(e->curt_func);
        block = FunctionClosures_get_top_branch_block(e->curt_func);
        block->braces_count--;
    }

    /// 填充跳转到出口命令
    SymbolValue _export0 = _alloc_cons_value(e, IntValue);
    CommandClosures end_cc = CommandClosures_new(JumpCommand);
    CommandClosures_add_arg(end_cc, _export0);
    FunctionClosures_add_command(e->curt_func, end_cc);

    /// 对所有的跳转到出口命令进行重定向
    int block_export = (int)array_n(e->curt_func->cmds);
    uint n = array_n(block->branch_marks);
    for (uint i = 0; i < n; i++)
    {
        branch_mark* mark = &block->branch_marks[i];
        if (mark->end_cmd)
        {
            int* p = (int*)_get_ptr(e, mark->end_cmd->args[0]);
            *p = block_export;
        }
        else
        {
            mark->end_cmd = end_cc;
            int* p = (int*)_get_ptr(e, end_cc->args[0]);
            *p = block_export;
        }
    }
    /// 对分支检测跳转方位进行重定向
    branch_mark last_mark = array_safe_get(block->branch_marks, array_n(block->branch_marks) - 1);
    int curt_tag = (int)array_n(e->curt_func->cmds);
    int addr = curt_tag;
    int* p = (int*)_get_ptr(e, last_mark.begin_cmd->args[0]);

    *p = addr;
}

void ForBegin(ParserEnv* e)
{
    SymbolStack_push(&e->sym_stack);
    FunctionClosures_new_for_loop_block(e->curt_func);
}
void ForEnd(ParserEnv* e)
{
    ForLoopBlock block = FunctionClosures_get_top_for_loop_block(e->curt_func);
    SymbolValue inc = _alloc_cons_value(e, IntValue);
    CommandClosures cc = CommandClosures_new(JumpCommand);
    CommandClosures_add_arg(cc, inc);
    FunctionClosures_add_command(e->curt_func, cc);
    block->loop_goto_inc_cmd = cc;
    block->export_tag = array_n(e->curt_func->cmds);

    /// 重定向所有的跳转位置
    {
        int* p = (int*)_get_ptr(e, block->cond_goto_loop_cmd->args[0]);
        *p = block->loop_tag;
        p = (int*)_get_ptr(e, block->cond_goto_export_cmd->args[0]);
        *p = block->export_tag;
        p = (int*)_get_ptr(e, block->inc_goto_cond_cmd->args[0]);
        *p = block->cond_tag;
        p = (int*)_get_ptr(e, block->loop_goto_inc_cmd->args[0]);
        *p = block->inc_tag;
    }

    SymbolStack_pop(&e->sym_stack);
    FunctionClosures_pop_for_loop_block(e->curt_func);
}

void MarkForCondBegin(ParserEnv* e)
{
    ForLoopBlock block = FunctionClosures_get_top_for_loop_block(e->curt_func);
    block->cond_tag = array_n(e->curt_func->cmds);
}

void MarkForCond(ParserEnv* e, SymbolValue bool_value)
{
    ForLoopBlock block = FunctionClosures_get_top_for_loop_block(e->curt_func);
    {
        SymbolValue loop = _alloc_cons_value(e, IntValue);
        CommandClosures cc = CommandClosures_new(IfTrueJumpCommand);
        CommandClosures_add_arg(cc, loop);
        CommandClosures_add_arg(cc, bool_value);
        FunctionClosures_add_command(e->curt_func, cc);
        block->cond_goto_loop_cmd = cc;
    }
    {
        SymbolValue _export1 = _alloc_cons_value(e, IntValue);
        CommandClosures cc = CommandClosures_new(JumpCommand);
        CommandClosures_add_arg(cc, _export1);
        FunctionClosures_add_command(e->curt_func, cc);
        block->cond_goto_export_cmd = cc;
    }

    block->inc_tag = array_n(e->curt_func->cmds);
}
void MarkForAlways(ParserEnv* e)
{
    ForLoopBlock block = FunctionClosures_get_top_for_loop_block(e->curt_func);
    block->inc_tag = array_n(e->curt_func->cmds);
}

void ForInc(ParserEnv* e)
{
    ForLoopBlock block = FunctionClosures_get_top_for_loop_block(e->curt_func);

    SymbolValue loop = _alloc_cons_value(e, IntValue);
    CommandClosures cc = CommandClosures_new(JumpCommand);
    CommandClosures_add_arg(cc, loop);
    FunctionClosures_add_command(e->curt_func, cc);
    block->inc_goto_cond_cmd = cc;

    block->loop_tag = array_n(e->curt_func->cmds);
}

void NewStruct(ParserEnv* e, SymbolValue struct_name)
{
    ParserEnv_new_struct_prototype(e, struct_name.data.name_str);
    EString_delete(struct_name.data.name_str);
}
void MarkStructMember(ParserEnv* e, SymbolValue struct_member, SymbolValue member_type)
{
    StructPrototype_add_elem(e, e->curt_struct, struct_member.data.name_str, member_type.type);
    EString_delete(struct_member.data.name_str);
}
void CompleteStruct(ParserEnv* e)
{
    ParserEnv_complete_struct_prototype(e);
}
