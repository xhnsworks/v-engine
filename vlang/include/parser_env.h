#ifndef PARSER_ENV
#define PARSER_ENV
#include "common.h"
#include "etypes.h"
#include "stack.h"
#include "symbol_stack.h"
#include "elog.h"
#include "estring.h"
#include "array.h"
#include "eassert.h"
#ifdef __cplusplus
#include "test.tab.h"
#endif
typedef enum _memory_type
{
    VariMem = 0,
    VariStrMem,
    ConsMem,
    ConsStrMem,
    MaxMem,
} MemoryType;

typedef struct _parser_env ParserEnv;
typedef void (*Command)(ParserEnv*, SymbolValue*);
typedef struct _command_closures
{
    Command cmd;
    SymbolValue* args;
} command_closures;
typedef struct _command_closures* CommandClosures;
CommandClosures CommandClosures_new(Command cmd);
void CommandClosures_delete(CommandClosures _self);
void CommandClosures_add_arg(CommandClosures _self, SymbolValue _a);
void CommandClosures_execute(ParserEnv* e, CommandClosures _self);
SymbolValue CommandClosures_get_last_arg(CommandClosures _self);

typedef enum _function_closures_status
{
    DeclareInputParamsStatus,
    DeclareOutputParamsStatus,
    AddCommandsStatus,
    CompleteStatus,
} function_closures_status;

typedef struct _function_closures* FunctionClosures;
typedef int SymbolValueType;
struct _symbol_value
{
    SymbolValueType type;
    union {
        struct {
            MemoryType mem_type;
            int offs;
        } mem_addr;
        Iterator tree_iter;
        SymbolValue* value_array;
        FunctionClosures func;
        /// 用完即回收
        EString name_str;
    } data;
};

/// 因为考虑到数据对齐问题，要保留一些额外空间
typedef struct _struct_elem
{
    int type;
    int offs;
    int size;
} struct_elem;
typedef struct _struct_elem* StructElem;
typedef struct _struct_prototype
{
    Tree symbol_to_elem_tree;
    int size;
} struct_prototype;
typedef struct _struct_prototype* StructPrototype;
StructPrototype StructPrototype_new();
void StructPrototype_delete(StructPrototype _self);
void StructPrototype_add_elem(ParserEnv* e, StructPrototype _self, const char* name, int type);

typedef struct _branch_mark
{
    int tag;
    CommandClosures begin_cmd;
    CommandClosures end_cmd;
} branch_mark;
typedef struct _branch_block
{
    int braces_count;
    branch_mark* branch_marks;
} _branch_block;
typedef struct _branch_block* BranchBlock;
BranchBlock BranchBlock_new();
void BranchBlock_delete(BranchBlock _self);

/// 若当前函数调用子函数，则子函数堆栈基地址要高于当前堆栈基地址
/// 而子函数的返回值或输入参数的SymbolValue是基于子函数的堆栈指针的
/// 因此在向子函数传递参数或者取返回值的时候需要抬高堆栈指针
/// function arg op:
///           dst                  src                  ret
/// type A_1: FuncArgValueArray op value             to value
/// type B_1: value             op FuncArgValueArray to value
/// type C_1: FuncArgValueArray op FuncArgValueArray to value
///           dst                  src                  ret
/// type A_2: FuncArgValueArray op value             to FuncArgValueArray
/// type B_2: value             op FuncArgValueArray to FuncArgValueArray
/// type C_2: FuncArgValueArray op FuncArgValueArray to FuncArgValueArray

/// operation type:
/// type 1: src dst
/// type 2: src dst ret

/// init
/// cond if_true goto loop
/// goto export
/// inc
/// goto cond
/// loop
/// goto inc
/// export

typedef struct _for_loop_block
{
    int cond_tag;
    int inc_tag;
    int loop_tag;
    int export_tag;
    CommandClosures cond_goto_loop_cmd;
    CommandClosures cond_goto_export_cmd;
    CommandClosures inc_goto_cond_cmd;
    CommandClosures loop_goto_inc_cmd;
} for_loop_block;
typedef struct _for_loop_block* ForLoopBlock;
ForLoopBlock ForLoopBlock_Init(ForLoopBlock _self);

typedef struct _function_closures
{
    int cmd_cnt;
	/// 每个命令闭包各自持有SymbolValue，命令闭包里持有的SymbolValue的offset是相对于当前函数堆栈的
    CommandClosures* cmds;
    /// 以下命令数组只拥有命令使用权，没有所有权
    CommandClosures* func_arg_op_cmds_type_a_1;
    CommandClosures* func_arg_op_cmds_type_b_1;
    CommandClosures* func_arg_op_cmds_type_c_1;
    CommandClosures* func_arg_op_cmds_type_a_2;
    CommandClosures* func_arg_op_cmds_type_b_2;
    CommandClosures* func_arg_op_cmds_type_c_2;

    FixedStack       branch_stack;

/// 从 if 开始括号计数，初始值是0
/// 遇到 { 括号计数加一，遇到 } 括号计数减一
/// 若括号计数小于0，则分支栈回退
/// if
/// { ->
///     if
///     { ->
///         { ->
///         } <-
///     } <-
///     else if
///     { ->
///     } <-
/// } <-

    /// for循环相比分支块要简单一些
    FixedStack for_loop_stack;

    SymbolValue vari_stack_size;
    SymbolValue vari_str_stack_size;
	/// vari_tree里的SymbolValue的offset是相对于调用者的函数堆栈
    Tree vari_tree;
    /// 输入参数和输出参数是变量的一种，但变量不都是输入参数或输出参数
    ///Tree input_param_tree;
    ///Tree output_param_tree;
    /// 参数树是根据红黑树排序的，顺序是乱的，所以需要另一个参数数组
    SymbolValue** input_param_array;
    SymbolValue** output_param_array;

    function_closures_status status;
} function_closures;

FunctionClosures FunctionClosures_new(ParserEnv* e);
void FunctionClosures_delete(FunctionClosures _self);
void FunctionClosures_add_command(FunctionClosures _self, CommandClosures _cc);

void FunctionClosures_add_c_function_command(FunctionClosures _self, Command _c_func);
/// 这个函数在不同的阶段作用是不同的
/// 在函数最后完成时，该函数的作用是重新定位变量目标
void FunctionClosures_complete(ParserEnv* e, FunctionClosures _self);
///void FunctionClosures_complete_branchs(ParserEnv* e, FunctionClosures _self);
void FunctionClosures_new_branch_block(FunctionClosures _self);
BranchBlock FunctionClosures_get_top_branch_block(FunctionClosures _self);
void FunctionClosures_pop_branch_block(FunctionClosures _self);

void FunctionClosures_new_for_loop_block(FunctionClosures _self);
ForLoopBlock FunctionClosures_get_top_for_loop_block(FunctionClosures _self);
void FunctionClosures_pop_for_loop_block(FunctionClosures _self);

void FunctionClosures_execute(ParserEnv* e, FunctionClosures _self);
void FunctionClosures_push_vari(FunctionClosures _self, const char* var_name, SymbolValue* v);
vptr FunctionClosures_get_vari_addr(ParserEnv* e, FunctionClosures _self, const char* var_name);
sint FunctionClosures_get_vari_offs(ParserEnv* e, FunctionClosures _self, const char* var_name, int* vari_type_out);

EString FunctionClosures_print_out(ParserEnv* e, FunctionClosures _self);

typedef struct _return_point
{
    int vari_offs;
    int vari_str_offs;
    FunctionClosures func;
    int cmd_cnt;
} return_point;

typedef struct _parser_exception
{
    int exce_id;
    const char* exce_desc;
} parser_exception;

struct _parser_env
{
    EString text;
    int char_count;
    Tree key_word_tree;

    Tree func_tree;
    FunctionClosures curt_func;

    symbol_stack sym_stack;
    FixedStack return_point_stack;

    uint line_count;
    parser_exception* exce_array;

    Tree struct_tree;
    StructPrototype curt_struct;

    value_allocator vari_alloc;
    value_allocator vari_str_alloc;
    value_allocator cons_alloc;
    value_allocator cons_str_alloc;
    /// 128k mem
    char* mems[MaxMem];

	struct _lex_state
	{
	    int remainder;
        int string_size;
        char mbuf[1024];
#ifndef __cplusplus
        enum yytokentype curtSymType;
#else
        yytokentype curtSymType;
#endif
        char* str;
	}lex_state;

};

void ParserEnv_lex_Init(ParserEnv* e);

void ParserEnv_Init(ParserEnv* e, const char* str);
FunctionClosures ParserEnv_new_function(ParserEnv* e, const char* _func_name);
void ParserEnv_complete_function(ParserEnv* e);
StructPrototype ParserEnv_new_struct_prototype(ParserEnv* e, const char* _struct_name);
void ParserEnv_complete_struct_prototype(ParserEnv* e);
FunctionClosures ParserEnv_find_function(ParserEnv* e, const char* _func_name);
void ParserEnv_print_all_function(ParserEnv* e);
void ParserEnv_insert_unknown_symbol(ParserEnv* e, const char* str, SymbolValue* result);
SymbolValue* ParserEnv_new_unknown_symbol(ParserEnv* e, const char* str);
vptr ParserEnv_get_addr(ParserEnv* e, int mem_type, sint offs);
void ParserEnv_push_return_point(ParserEnv* e, int vari_offs, int vari_str_offs, FunctionClosures curt_func, int curt_cmd_cnt);
void ParserEnv_pop_return_point(ParserEnv* e);

static inline void ParserEnv_push_exce(ParserEnv* e, parser_exception exce)
{
    apush(e->exce_array, exce);
}

static inline const char* _get_type_str(ValueType type)
{
    switch (type)
    {
    case IntValue:
        return "IntValue";
    case BoolValue:
        return "BoolValue";
    case FloatValue:
        return "FloatValue";
    case Float2Value:
        return "Float2Value";
    case Float3Value:
        return "Float3Value";
    case Float4Value:
        return "Float4Value";
    case Matrix4x4Value:
        return "Matrix4x4Value";
    case StringValue:
        return "StringValue";
    case ValueArray:
        return "ValueArray";
    case FuncArgValueArray:
        return "FuncArgValueArray";
    case Function:
        return "Function";
    default:
        return "Unknown";
    }
}
static inline const char* _get_mem_type_str(MemoryType type)
{
	switch(type)
	{
	case VariMem:
		return "VariMem";
	case VariStrMem:
		return "VariStrMem";
	case ConsMem:
		return "ConsMem";
	case ConsStrMem:
		return "ConsStrMem";
	default:
		return "UnknownMem";
	}
}
typedef void (*StringOutputProc)(const char*);
static inline void _value_string_output(ParserEnv* e, SymbolValue value, StringOutputProc proc)
{
    if (value.type != ValueArray && value.type != FuncArgValueArray)
    {
        string_buffer_new(STRING_BUFFER_SIZE);
        if (value.data.mem_addr.mem_type >= MaxMem)
        {
            proc("Invalid Mem");
            return;
        }
        char* mem = e->mems[value.data.mem_addr.mem_type] + value.data.mem_addr.offs;
        sbuf_printf( "MEMTYPE %s OFFS %d VALUE ", _get_mem_type_str(value.data.mem_addr.mem_type), value.data.mem_addr.offs );
        switch (value.type)
        {
        case IntValue:
            sbuf_printf( "%d", ((int*)mem)[0] );
            proc(get_string_buffer);
            break;
        case FloatValue:
            sbuf_printf( "%f", ((float*)mem)[0] );
            proc(get_string_buffer);
            break;
        case Float2Value:
            sbuf_printf( "%f, %f", ((float*)mem)[0], ((float*)mem)[1] );
            proc(get_string_buffer);
            break;
        case Float3Value:
            sbuf_printf( "%f, %f, %f", ((float*)mem)[0], ((float*)mem)[1], ((float*)mem)[2] );
            proc(get_string_buffer);
            break;
        case Float4Value:
            sbuf_printf( "%f, %f, %f, %f", ((float*)mem)[0], ((float*)mem)[1], ((float*)mem)[2], ((float*)mem)[3] );
            proc(get_string_buffer);
            break;
        case Matrix4x4Value:
            sbuf_printf( "%f, %f, %f, %f\n"
                         "%f, %f, %f, %f\n"
                         "%f, %f, %f, %f\n"
                         "%f, %f, %f, %f\n",
                         ((float*)mem)[0], ((float*)mem)[1], ((float*)mem)[2], ((float*)mem)[3],
                         ((float*)mem)[4], ((float*)mem)[5], ((float*)mem)[6], ((float*)mem)[7],
                         ((float*)mem)[8], ((float*)mem)[9], ((float*)mem)[10], ((float*)mem)[11],
                         ((float*)mem)[12], ((float*)mem)[13], ((float*)mem)[14], ((float*)mem)[15]);
            proc(get_string_buffer);
            break;
        case StringValue:
            sbuf_printf( "%s", ((char**)mem)[0] );
            proc(get_string_buffer);
            break;
        case BoolValue:
            if (((bool*)mem)[0])
            {
                sbuf_printf( "TRUE" );
            }
            else
            {
                sbuf_printf( "FALSE" );
            }
            proc(get_string_buffer);
            break;
        case Function:
            {
                FunctionClosures fc = ((FunctionClosures*)mem)[0];
                EString func_str = FunctionClosures_print_out(e, fc);
                sbuf_printf(func_str);
                EString_delete(func_str);
                proc(get_string_buffer);
            }
            break;
        default:
            break;
        }
    }
    else
    {
        uint n = array_n(value.data.value_array);
        for (uint i = 0; i < n; i++)
        {
            SymbolValue sv = value.data.value_array[i];
            if (value.type == FuncArgValueArray)
                proc("##FUNC ARG##");
            _value_string_output(e, sv, proc);
        }
    }
}

static inline SymbolValue _alloc_float_value(ParserEnv* e, float v)
{
    SymbolValue ret;
    int offs = ValueAllocator_alloc(&e->vari_alloc, FloatValue);
    ret.type = FloatValue;
    ret.data.mem_addr.mem_type = VariMem;
    char* mem = e->mems[VariMem];
    memcpy(mem + offs, &v, sizeof(v));
    ret.data.mem_addr.offs = offs;
    return ret;
}

static inline SymbolValue _alloc_bool_value(ParserEnv* e, bool b)
{
    SymbolValue ret;
    int offs = ValueAllocator_alloc(&e->vari_alloc, BoolValue);
    ret.type = BoolValue;
    ///ret.value = e->vari_mem;
    ret.data.mem_addr.mem_type = VariMem;
    char* mem = e->mems[VariMem];
    memset(mem + offs, 0, offs);
    memcpy(mem + offs, &b, sizeof(b));
    ret.data.mem_addr.offs = offs;
    return ret;
}

static inline SymbolValue _alloc_vari_bool_value(ParserEnv* e)
{
    SymbolValue ret;
    int offs = ValueAllocator_alloc(&e->vari_alloc, BoolValue);
    ret.type = BoolValue;
    ///ret.value = e->vari_mem;
    ret.data.mem_addr.mem_type = VariMem;
    char* mem = e->mems[VariMem];
    memset(mem + offs, 0, offs);
    ret.data.mem_addr.offs = offs;
    return ret;
}

static inline SymbolValue _alloc_vari_value(ParserEnv* e, ValueType type)
{
    SymbolValue ret;
    if (type != StringValue)
    {
        int offs = ValueAllocator_alloc(&e->vari_alloc, type);
        ret.type = type;
        ///ret.value = e->vari_mem;
        ret.data.mem_addr.mem_type = VariMem;
        ret.data.mem_addr.offs = offs;
    }
    else
    {
        int offs = ValueAllocator_alloc(&e->vari_str_alloc, type);
        ret.type = type;
        ///ret.value = e->vari_str_mem;
        ret.data.mem_addr.mem_type = VariStrMem;
        ret.data.mem_addr.offs = offs;
    }
    return ret;
}

static inline SymbolValue _alloc_cons_value(ParserEnv* e, ValueType type)
{
    SymbolValue ret;
    if (type != StringValue)
    {
        int offs = ValueAllocator_alloc(&e->cons_alloc, type);
        ret.type = type;
        ///ret.value = e->vari_mem;
        ret.data.mem_addr.mem_type = ConsMem;
        ret.data.mem_addr.offs = offs;
    }
    else
    {
        int offs = ValueAllocator_alloc(&e->cons_str_alloc, type);
        ret.type = type;
        ///ret.value = e->vari_str_mem;
        ret.data.mem_addr.mem_type = ConsStrMem;
        ret.data.mem_addr.offs = offs;
    }
    return ret;
}
/**
static inline const char* _get_mem_type_str(MemoryType type)
{
    switch (type)
    {
    case VariMem:
        return "VariMem";
    case VariStrMem:
        return "VariStrMem";
    case ConsMem:
        return "ConsMam";
    case ConsStrMem:
        return "ConsStrMem";
    default:
        return "UnknownMem";
    }
}
**/
static inline vptr _get_ptr(ParserEnv* e, SymbolValue a)
{
    ///const char* mem_type_str = _get_mem_type_str(a.data.mem_addr.mem_type);
    char* mem = e->mems[a.data.mem_addr.mem_type];
    vptr ret = (vptr)((char*)mem + a.data.mem_addr.offs);
    return ret;
}

static inline vptr _get_ptr_print_info(ParserEnv* e, SymbolValue a)
{
    const char* mem_type_str = _get_mem_type_str(a.data.mem_addr.mem_type);
    VPRINT("MEM_TYPE %s, MEM_OFFS %d ", mem_type_str, a.data.mem_addr.offs);
    ///return (vptr)((char*)a.value + a.offs);
    char* mem = e->mems[a.data.mem_addr.mem_type];
    vptr ret = (vptr)((char*)mem + a.data.mem_addr.offs);
    VPRINT("ADDR %x", (uint)ret);
    return ret;
}

static inline SymbolValue* _symbol_value_array_new(uint n)
{
    SymbolValue null_value;
    null_value.type = UnknownValue;
    memset(&null_value.data, 0, sizeof(null_value.data));
    null_value.data.mem_addr.mem_type = MaxMem;
    return array_new(SymbolValue, n, null_value);
}

static inline int _get_symbol_type_size(ParserEnv* e, int type)
{
    int ret = _get_value_type_size((ValueType)type);
    if (ret > 0)
        return ret;
    else
    {
        return -1;
    }
}
#endif
