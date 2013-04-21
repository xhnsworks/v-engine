#include "parser_env.h"
#include "emem.h"
#include "array.h"
#include "tree.h"
#include "test.tab.h"
#include "symbol_stack.h"
#include "stack.h"
#include "array.h"
#include "function_print.h"
#include "single_op_single.h"
CommandClosures CommandClosures_new(Command cmd)
{
    CommandClosures ret = Malloc(sizeof(command_closures));
    ///SymbolValue null_arg;
    ///null_arg.type = Unknown;
    ///memset(&null_arg.data, 0, sizeof(null_arg.data));
    ret->cmd = cmd;
    ///ret->args = array_new(SymbolValue, 5, null_arg);
    ret->args = _symbol_value_array_new(5);
    return ret;
}
void CommandClosures_delete(CommandClosures _self)
{
    array_delete(_self->args);
    Mfree(_self);
}
void CommandClosures_add_arg(CommandClosures _self, SymbolValue _a)
{
    apush(_self->args, _a);
}
void CommandClosures_execute(ParserEnv* e, CommandClosures _self)
{
    _self->cmd(e, _self->args);
}
SymbolValue CommandClosures_get_last_arg(CommandClosures _self)
{
    return array_safe_get(_self->args, array_n(_self->args) - 1);
}

StructPrototype StructPrototype_new()
{
    StructPrototype ret = Malloc(sizeof(struct_prototype));
    ret->symbol_to_elem_tree = Tree_new(String, Vptr, Ealloc, Efree);
    ret->size = 0;
    return ret;
}
void StructPrototype_delete(StructPrototype _self)
{
    Iterator iter = Tree_begin(_self->symbol_to_elem_tree);
    while (iter)
    {
        var data = Tree_get_value(iter);
        StructElem elem = data.vptr_var;
        Mfree(elem);
        iter = Tree_next(iter);
    }
    Tree_Dest(_self->symbol_to_elem_tree);
    Mfree(_self);
}
void StructPrototype_add_elem(ParserEnv* e, StructPrototype _self, const char* name, int type)
{
    var key, data;
    key.str_var = name;
    if (Tree_find(_self->symbol_to_elem_tree, key, &data))
    {
        parser_exception exce = s_exce_table[The_element_name_is_defined_more_than_once_in_struct];
        ParserEnv_push_exce(e, exce);
        return;
    }
    uint elem_size = _get_symbol_type_size(e, type);
    if (elem_size < 0)
    {
        parser_exception exce = s_exce_table[Unknown_data_type];
        ParserEnv_push_exce(e, exce);
        return;
    }
    StructElem elem = Malloc(sizeof(struct_elem));
    elem->type = type;
    elem->offs = _self->size;
    elem->size = elem_size;
    _self->size += elem_size;
    data.vptr_var = elem;
    Tree_insert(_self->symbol_to_elem_tree, key, data);
}

BranchBlock BranchBlock_new()
{
    BranchBlock ret = Malloc(sizeof(BranchBlock));
    branch_mark null_mark = {-1, NULL, NULL};
    ret->branch_marks = array_new(branch_mark, 5, null_mark);
    ret->braces_count = 0;
    return ret;
}
void BranchBlock_delete(BranchBlock _self)
{
    array_delete(_self->branch_marks);
    Mfree(_self);
}

ForLoopBlock ForLoopBlock_Init(ForLoopBlock _self)
{
    _self->cond_tag = -1;
    _self->inc_tag = -1;
    _self->loop_tag = -1;
    _self->export_tag = -1;
    _self->cond_goto_loop_cmd = NULL;
    _self->cond_goto_export_cmd = NULL;
    _self->inc_goto_cond_cmd = NULL;
    _self->loop_goto_inc_cmd = NULL;
    return _self;
}

FunctionClosures FunctionClosures_new(ParserEnv* e)
{
    FunctionClosures ret = Malloc(sizeof(function_closures));
    ret->cmd_cnt = 0;
    ret->cmds = array_new(CommandClosures, 20, NULL);
    ret->func_arg_op_cmds_type_a_1 = array_new(CommandClosures, 20, NULL);
    ret->func_arg_op_cmds_type_b_1 = array_new(CommandClosures, 20, NULL);
    ret->func_arg_op_cmds_type_c_1 = array_new(CommandClosures, 20, NULL);
    ret->func_arg_op_cmds_type_a_2 = array_new(CommandClosures, 20, NULL);
    ret->func_arg_op_cmds_type_b_2 = array_new(CommandClosures, 20, NULL);
    ret->func_arg_op_cmds_type_c_2 = array_new(CommandClosures, 20, NULL);

    ret->branch_stack = FixedStack_new(sizeof(BranchBlock));
    ret->for_loop_stack = FixedStack_new(sizeof(for_loop_block));

    ret->vari_stack_size = _alloc_cons_value(e, IntValue);
    ret->vari_str_stack_size = _alloc_cons_value(e, IntValue);
    ret->vari_tree = Tree_new(String, Vptr, Ealloc, Efree);

    ///ret->input_param_tree = Tree_new(String, Vptr, Ealloc, Efree);
    ///ret->output_param_tree = Tree_new(String, Vptr, Ealloc, Efree);

    ret->input_param_array = array_new(SymbolValue*, 10, NULL);
    ret->output_param_array = array_new(SymbolValue*, 5, NULL);

    ret->status = DeclareInputParamsStatus;
    return ret;
}
void FunctionClosures_delete(FunctionClosures _self)
{
    uint n = array_n(_self->cmds);
    for (uint i = 0; i < n; i++)
    {
        CommandClosures_delete(_self->cmds[i]);
    }
    array_delete(_self->cmds);
    array_delete(_self->func_arg_op_cmds_type_a_1);
    array_delete(_self->func_arg_op_cmds_type_b_1);
    array_delete(_self->func_arg_op_cmds_type_c_1);
    array_delete(_self->func_arg_op_cmds_type_a_2);
    array_delete(_self->func_arg_op_cmds_type_b_2);
    array_delete(_self->func_arg_op_cmds_type_c_2);

    while (FixedStack_count(_self->branch_stack)) {
        BranchBlock block;
        FixedStack_pop(_self->branch_stack, &block);
        BranchBlock_delete(block);
    }
    FixedStack_Dest(_self->branch_stack);
    FixedStack_Dest(_self->for_loop_stack);

    Iterator iter = Tree_begin(_self->vari_tree);
    while (iter)
    {
        var data = Tree_get_value(iter);
        SymbolValue* sv = data.vptr_var;
        Mfree(sv);
        iter = Tree_next(iter);
    }
    Tree_Dest(_self->vari_tree);
    /// 输入参数和输出参数树只有指针使用权，没有指针所有权
    ///Tree_Dest(_self->input_param_tree);
    ///Tree_Dest(_self->output_param_tree);

    array_delete(_self->input_param_array);
    array_delete(_self->output_param_array);
    Mfree(_self);
}
void FunctionClosures_add_command(FunctionClosures _self, CommandClosures _cc)
{
    apush(_self->cmds, _cc);
}

static inline void _grow(ParserEnv* e, SymbolValue* sv)
{
    switch(sv->data.mem_addr.mem_type)
    {
    case VariMem:
        sv->data.mem_addr.offs += ValueAllocator_totel_size(&e->vari_alloc);
        break;
    case VariStrMem:
        sv->data.mem_addr.offs += ValueAllocator_totel_size(&e->vari_str_alloc);
        break;
    default:
        break;
    }
}
/**
void FunctionClosures_complete_branchs(ParserEnv* e, FunctionClosures _self)
{
    uint n = array_n(_self->branch_marks);
    for (uint i = 0; i < n - 1; i++)
    {
        CommandClosures cc = _self->branch_marks[i].cmd;
        int next_mark = _self->branch_marks[i + 1].tag;
        int* p = _get_ptr(e, cc->args[0]);
        *p = next_mark;
    }
    array_n(_self->branch_marks) = 0;
}
**/
void FunctionClosures_new_branch_block(FunctionClosures _self)
{
    BranchBlock block = BranchBlock_new();
    FixedStack_push(_self->branch_stack, &block);
}
BranchBlock FunctionClosures_get_top_branch_block(FunctionClosures _self)
{
    BranchBlock ret;
    if (FixedStack_get_top(_self->branch_stack, &ret))
        return ret;
    else
        return NULL;
}
void FunctionClosures_pop_branch_block(FunctionClosures _self)
{
    BranchBlock top;
    if (FixedStack_pop(_self->branch_stack, &top)) {
        BranchBlock_delete(top);
    }
}

void FunctionClosures_new_for_loop_block(FunctionClosures _self)
{
    for_loop_block block;
    ForLoopBlock_Init(&block);
    FixedStack_push(_self->for_loop_stack, &block);
}
ForLoopBlock FunctionClosures_get_top_for_loop_block(FunctionClosures _self)
{
    return FixedStack_get_top_ptr(_self->for_loop_stack);
}
void FunctionClosures_pop_for_loop_block(FunctionClosures _self)
{
    for_loop_block block;
    FixedStack_pop(_self->for_loop_stack, &block);
}

void FloatAssignFloatCommand(ParserEnv* e, SymbolValue* args);
///void FuncTestFloatAssignFloatCommand(ParserEnv* e, SymbolValue* args);
void FunctionClosures_complete(ParserEnv* e, FunctionClosures _self)
{
    switch (_self->status)
    {
    case DeclareInputParamsStatus:
        _self->status = DeclareOutputParamsStatus;
        break;
    case DeclareOutputParamsStatus:
        _self->status = AddCommandsStatus;
        break;
    case AddCommandsStatus:
        _self->status = CompleteStatus;

        {
            ///FunctionClosures_complete_branchs(e, _self);

            printf("\n\n##GET STACK SIZE\n");
            int* ip = _get_ptr(e, _self->vari_stack_size);
            *ip = ValueAllocator_totel_size(&e->vari_alloc);
            printf("VARI STACK SIZE %d\n", *ip);
            ip = _get_ptr(e, _self->vari_str_stack_size);
            *ip = ValueAllocator_totel_size(&e->vari_str_alloc);
            printf("VARI STR STACK SIZE %d\n", *ip);
            printf("\n");

            uint n = 0;
            n = array_n(_self->func_arg_op_cmds_type_a_1);
            for (uint i = 0; i < n; i++) {
                CommandClosures cc = _self->func_arg_op_cmds_type_a_1[i];
                ///
                if (FloatAssignFloatCommand == cc->cmd)
                    cc->cmd = FloatAssignFloatCommand;
                ///
                SymbolValue* sv = &cc->args[1];
                _grow(e, sv);
            }
            n = array_n(_self->func_arg_op_cmds_type_b_1);
            for (uint i = 0; i < n; i++) {
                CommandClosures cc = _self->func_arg_op_cmds_type_b_1[i];
                ///
                if (FloatAssignFloatCommand == cc->cmd)
                    cc->cmd = FloatAssignFloatCommand;
                ///
                SymbolValue* sv = &cc->args[0];
                _grow(e, sv);
            }
            n = array_n(_self->func_arg_op_cmds_type_c_1);
            for (uint i = 0; i < n; i++) {
                CommandClosures cc = _self->func_arg_op_cmds_type_c_1[i];
                ///
                if (FloatAssignFloatCommand == cc->cmd)
                    cc->cmd = FloatAssignFloatCommand;
                ///
                {
                    SymbolValue* sv = &cc->args[0];
                    _grow(e, sv);
                }
                {
                    SymbolValue* sv = &cc->args[1];
                    _grow(e, sv);
                }
            }

            n = array_n(_self->func_arg_op_cmds_type_a_2);
            for (uint i = 0; i < n; i++) {
                CommandClosures cc = _self->func_arg_op_cmds_type_a_2[i];
                ///
                if (FloatAssignFloatCommand == cc->cmd)
                    cc->cmd = FloatAssignFloatCommand;
                ///
                {
                    SymbolValue* sv = &cc->args[1];
                    _grow(e, sv);
                }
                {
                    SymbolValue* sv = &cc->args[2];
                    _grow(e, sv);
                }
            }
            n = array_n(_self->func_arg_op_cmds_type_b_2);
            for (uint i = 0; i < n; i++) {
                CommandClosures cc = _self->func_arg_op_cmds_type_b_2[i];
                ///
                if (FloatAssignFloatCommand == cc->cmd)
                    cc->cmd = FloatAssignFloatCommand;
                ///
                {
                    SymbolValue* sv = &cc->args[0];
                    _grow(e, sv);
                }
                {
                    SymbolValue* sv = &cc->args[2];
                    _grow(e, sv);
                }
            }
            n = array_n(_self->func_arg_op_cmds_type_c_2);
            for (uint i = 0; i < n; i++) {
                CommandClosures cc = _self->func_arg_op_cmds_type_c_2[i];
                ///
                if (FloatAssignFloatCommand == cc->cmd)
                    cc->cmd = FloatAssignFloatCommand;
                ///
                {
                    SymbolValue* sv = &cc->args[0];
                    _grow(e, sv);
                }
                {
                    SymbolValue* sv = &cc->args[1];
                    _grow(e, sv);
                }
                {
                    SymbolValue* sv = &cc->args[2];
                    _grow(e, sv);
                }
            }
        }
        ValueAllocator_clear(&e->vari_alloc);
        ValueAllocator_clear(&e->vari_str_alloc);
        break;
    default:
        break;
    }
}
void FunctionClosures_push_vari(FunctionClosures _self, const char* var_name, SymbolValue* v)
{
    SymbolValue* sv = Malloc(sizeof(SymbolValue));
    memcpy(sv, v, sizeof(SymbolValue));
    var key, data;
    key.str_var = (vptr)var_name;
    data.vptr_var = sv;
    Tree_insert(_self->vari_tree, key, data);
    switch (_self->status)
    {
    case DeclareInputParamsStatus:
        ///Tree_insert(_self->input_param_tree, key, data);
        {
            apush(_self->input_param_array, sv);
        }
        break;
    case DeclareOutputParamsStatus:
        ///Tree_insert(_self->output_param_tree, key, data);
        {
            apush(_self->output_param_array, sv);
        }
        break;
    default:
        break;
    }
}
void FunctionClosures_execute(ParserEnv* e, FunctionClosures _self)
{
    e->curt_func = _self;
    uint n = array_n(_self->cmds);
    for (_self->cmd_cnt = 0; _self->cmd_cnt < n; _self->cmd_cnt++)
    {
        CommandClosures cc = _self->cmds[_self->cmd_cnt];
        CommandClosures_execute(e, cc);
    }
}
vptr FunctionClosures_get_vari_addr(ParserEnv* e, FunctionClosures _self, const char* var_name)
{
    var key, data;
    key.str_var = (vptr)var_name;
    if (Tree_find(_self->vari_tree, key, &data))
    {
        SymbolValue* sv = data.vptr_var;
        char* mem = e->mems[sv->data.mem_addr.mem_type];
        return (vptr)(mem + sv->data.mem_addr.offs);
    }
    return NULL;
}
sint FunctionClosures_get_vari_offs(ParserEnv* e, FunctionClosures _self, const char* var_name, int* vari_type_out)
{
    var key, data;
    key.str_var = (vptr)var_name;
    if (Tree_find(_self->vari_tree, key, &data))
    {
        SymbolValue* sv = data.vptr_var;
        *vari_type_out = sv->data.mem_addr.mem_type;
        return sv->data.mem_addr.offs;
    }
    return -1;
}

EString FunctionClosures_print_out(ParserEnv* e, FunctionClosures _self)
{
    EString ret = EString_new("this a function");
    return ret;
}
void ParserEnv_Init(ParserEnv* e, const char* str)
{
    e->char_count = 0;
    /**
    e->text = "func test2 (a float) (b float)\n"
              "{\n"
              "    b = a / 2.0;\n"
              "    print a;\n"
              "}\n"
              "func ttt (a float, b float) (c float)\n"
              "{\n"
              "    var t float;\n"
              "    var g float;\n"
              "    var i int;\n"
              "    i = 3;\n"
              "    t = 4.0 * i;\n"
              "    g = test2(3.0);\n"
              "    j := t * g;\n"
              "    print j;\n"
              "}";
              **/
    /**
    e->text = "func ttt (a float, b float) (c float)\n"
              "{\n"
              "    var t float;\n"
              "    var g float;\n"
              "    t = 3.0;\n"
              "    g = 1.0;\n"
              "    print t > g;\n"
              "}";
              **/
/**
    e->text = "func test2 (a float) (b float)\n"
              "{\n"
              "    b = a / 2.0;\n"
              "    print b;\n"
              "}\n"
              "func ttt (a float, b float) (c float)\n"
              "{\n"
              "    var t float;\n"
              "    var g float;\n"
              "    t = 1.0;\n"
              "    g = 2.0;\n"
              "    if ( t < g )\n"
              "    {\n"
              "        aaa := t + 1.0;\n"
              "        if (aaa == g)\n"
              "        {\n"
              "            print 100.0;\n"
              "        }\n"
              "        j := 9.0;\n"
              "        j = test2(j);\n"
              "        print j;\n"
              "    }\n"
              "    else if (t > g)\n"
              "    {\n"
              "        j := 12.0;\n"
              "        print j;\n"
              "    }\n"
              "    print 2.0;\n"
              "}\n";
**/
    e->text = (EString)str;
    ///e->sym_2_value_node_tree = Tree_new(String, Vptr, Ealloc, Efree);
    ///e->value_2_value_node_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    e->key_word_tree = Tree_new(String, Vptr, Ealloc, Efree);
    e->func_tree = Tree_new(String, Vptr, Ealloc, Efree);
    e->struct_tree = Tree_new(String, Vptr, Ealloc, Efree);
    e->curt_func = NULL;

    void _insert_key_word(char* str, int type)
    {
        var key, data;
        key.str_var = str;
        SymbolValue* v = Malloc(sizeof(SymbolValue));
        v->type = type;
        v->data.mem_addr.mem_type = MaxMem;
        data.vptr_var = v;
        Tree_insert(e->key_word_tree, key, data);
    }
    _insert_key_word("int", int_type);
    _insert_key_word("float", float_type);
    _insert_key_word("if", _if);
    _insert_key_word("else", _else);
    _insert_key_word("var", _var);
    _insert_key_word("func", _funcdef);
    _insert_key_word("print", _print);
    _insert_key_word("for", _for);

    SymbolStack_Init(&e->symbol_stack);
    SymbolStack_push(&e->symbol_stack);

    e->return_point_stack = FixedStack_new(sizeof(return_point));

    e->line_count = 0;
    parser_exception null_exce = {-1, NULL};
    e->exce_array = array_new(parser_exception, 5, null_exce);
    /**
    e->vari_mem = Malloc(128 * 1024);
    e->vari_str_mem = Malloc(128 * 1024);
    e->cons_mem = Malloc(128 * 1024);
    e->cons_str_mem = Malloc(128 * 1024);
    **/
    for (int i = 0; i < MaxMem; i++)
    {
        e->mems[i] = Malloc(128 * 1024);
    }

    ValueAllocator_Init(&e->vari_alloc);
    ValueAllocator_Init(&e->vari_str_alloc);
    ValueAllocator_Init(&e->cons_alloc);
    ValueAllocator_Init(&e->cons_str_alloc);
}

FunctionClosures ParserEnv_new_function(ParserEnv* e, const char* _func_name)
{
    FunctionClosures func = FunctionClosures_new(e);
    var key, data;
    key.str_var = (vptr)_func_name;
    data.vptr_var = func;
    Tree_insert(e->func_tree, key, data);
    e->curt_func = func;
    SymbolStack_push(&e->symbol_stack);
    return func;
}
void ParserEnv_complete_function(ParserEnv* e)
{
    FunctionClosures_complete(e, e->curt_func);
    SymbolStack_pop(&e->symbol_stack);
}
StructPrototype ParserEnv_new_struct_prototype(ParserEnv* e, const char* _struct_name)
{
    var key, data;
    key.str_var = _struct_name;
    if (Tree_find(e->struct_tree, key, &data))
    {
        parser_exception exce = s_exce_table[Structure_is_defined_more_than_once];
        ParserEnv_push_exce(e, exce);
        return NULL;
    }
    StructPrototype ret = StructPrototype_new();
    data.vptr_var = ret;
    Tree_insert(e->struct_tree, key, data);
    e->curt_struct = ret;
    return ret;
}
void ParserEnv_complete_struct_prototype(ParserEnv* e)
{
}

FunctionClosures ParserEnv_find_function(ParserEnv* e, const char* _func_name)
{
    var key, data;
    key.str_var = (vptr)_func_name;
    if (Tree_find(e->func_tree, key, &data))
        return data.vptr_var;
    else
        return NULL;
}
void ParserEnv_print_all_function(ParserEnv* e)
{
    Iterator iter = Tree_begin(e->func_tree);
    while(iter)
    {
        var key = Tree_get_key(iter);
        var data = Tree_get_value(iter);
        FunctionClosures fc = data.vptr_var;
        printf("##FUNC:%s\n", (EString)key.str_var);
        FunctionPrint(e, fc);
        iter = Tree_next(iter);
    }
}
/// unknown symbol Entry = {string, SymbolValue} SymbolValue = {Unknown, Entry, -1}
/// value symbol SymbolValue = {_TYPE_, memory address, offset}
///
/// _TYPE_ contains the following components:
/// IntValue
/// FloatValue
/// Float2Value
/// Float3Value
/// Float4Value
/// Matrix4x4Value
/// StringValue
///
/// memory address contains the following components:
/// variable address
/// variable string address
/// constant address
/// constant string address
///
void ParserEnv_insert_unknown_symbol(ParserEnv* e, const char* str, SymbolValue* result)
{
    SymbolValue* v = SymbolStack_insert_unknown_symbol(&e->symbol_stack, str);
    memcpy(result, v, sizeof(SymbolValue));
}
vptr ParserEnv_get_addr(ParserEnv* e, int mem_type, sint offs)
{
    return e->mems[mem_type] + offs;
}
void ParserEnv_push_return_point(ParserEnv* e, int vari_offs, int vari_str_offs, FunctionClosures curt_func, int curt_cmd_cnt)
{
    return_point ret_pt = {vari_offs, vari_str_offs, curt_func, curt_cmd_cnt};
    FixedStack_push(e->return_point_stack, &ret_pt);
    e->mems[VariMem] += vari_offs;
    e->mems[VariStrMem] += vari_str_offs;
}
void ParserEnv_pop_return_point(ParserEnv* e)
{
    return_point ret_pt;
    FixedStack_pop(e->return_point_stack, &ret_pt);
    e->mems[VariMem] -= ret_pt.vari_offs;
    e->mems[VariStrMem] -= ret_pt.vari_str_offs;
    e->curt_func = ret_pt.func;
    e->curt_func->cmd_cnt = ret_pt.cmd_cnt;
}
