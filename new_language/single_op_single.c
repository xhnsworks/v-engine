#include "single_op_single.h"
#include "float_op_float.h"
#include "float_op_int.h"
#include "int_op_float.h"
#include "int_op_int.h"
parser_exception s_exce_table[] = {
    {Invalid_operation, "这个操作是无效的"},
    {Symbol_is_defined_more_than_once, "符号被多次定义"},
    {The_element_name_is_defined_more_than_once_in_struct, "结构体中的元素被多次定义"},
    {Unknown_data_type, "未知的数据类型"},
    {Structure_is_defined_more_than_once, "结构体被多次定义"},
};

typedef struct _arithmetic_command_table
{
    const char* desc;
    int dst_type;
    Command src_type[MaxValidValue];
} arithmetic_command_table;

typedef struct _result_type_table
{
    int dst_type;
    int src_type[MaxValidValue];
} result_type_table;

static result_type_table s_result_type_table[] =
{
    ///              IntValue,    FloatValue, Float2Value, Float3Value, Float4Value, Matrix4x4Value, StringValue, BoolValue, Function
    {IntValue,       {IntValue,   IntValue,   Unknown,     Unknown,     Unknown,     Unknown,        Unknown,     Unknown,   Unknown}},
    {FloatValue,     {FloatValue, FloatValue, Unknown,     Unknown,     Unknown,     Unknown,        Unknown,     Unknown,   Unknown}},
    {Float2Value,    {Unknown,    Unknown,    Unknown,     Unknown,     Unknown,     Unknown,        Unknown,     Unknown,   Unknown}},
    {Float3Value,    {Unknown,    Unknown,    Unknown,     Unknown,     Unknown,     Unknown,        Unknown,     Unknown,   Unknown}},
    {Float4Value,    {Unknown,    Unknown,    Unknown,     Unknown,     Unknown,     Unknown,        Unknown,     Unknown,   Unknown}},
    {Matrix4x4Value, {Unknown,    Unknown,    Unknown,     Unknown,     Unknown,     Unknown,        Unknown,     Unknown,   Unknown}},
    {StringValue,    {Unknown,    Unknown,    Unknown,     Unknown,     Unknown,     Unknown,        Unknown,     Unknown,   Unknown}},
    {BoolValue,      {Unknown,    Unknown,    Unknown,     Unknown,     Unknown,     Unknown,        Unknown,     Unknown,   Unknown}},
    {Function,       {Unknown,    Unknown,    Unknown,     Unknown,     Unknown,     Unknown,        Unknown,     Unknown,   Unknown}},
};

static arithmetic_command_table s_add_command_table[] =
{
    ///                                     IntValue,            FloatValue,           Float2Value, Float3Value, Float4Value, Matrix4x4Value, StringValue, BoolValue, Function
    {"int add value",       IntValue,       {IntAddIntCommand,   IntAddFloatCommand,   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float add value",     FloatValue,     {FloatAddIntCommand, FloatAddFloatCommand, NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float2 add value",    Float2Value,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float3 add value",    Float3Value,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float4 add value",    Float4Value,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"matrix4x4 add value", Matrix4x4Value, {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"string add value",    StringValue,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"bool add value",      BoolValue,      {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"function add value",  Function,       {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
};

static arithmetic_command_table s_sub_command_table[] =
{
    ///                                     IntValue,            FloatValue,           Float2Value, Float3Value, Float4Value, Matrix4x4Value, StringValue, BoolValue, Function
    {"int sub value",       IntValue,       {IntSubIntCommand,   IntSubFloatCommand,   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float sub value",     FloatValue,     {FloatSubIntCommand, FloatSubFloatCommand, NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float2 sub value",    Float2Value,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float3 sub value",    Float3Value,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float4 sub value",    Float4Value,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"matrix4x4 sub value", Matrix4x4Value, {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"string sub value",    StringValue,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"bool sub value",      BoolValue,      {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"function sub value",  Function,       {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
};

static arithmetic_command_table s_mul_command_table[] =
{
    ///                                     IntValue,            FloatValue,           Float2Value, Float3Value, Float4Value, Matrix4x4Value, StringValue, BoolValue, Function
    {"int mul value",       IntValue,       {IntMulIntCommand,   IntMulFloatCommand,   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float mul value",     FloatValue,     {FloatMulIntCommand, FloatMulFloatCommand, NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float2 mul value",    Float2Value,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float3 mul value",    Float3Value,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float4 mul value",    Float4Value,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"matrix4x4 mul value", Matrix4x4Value, {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"string mul value",    StringValue,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"bool mul value",      BoolValue,      {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"function mul value",  Function,       {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
};

static arithmetic_command_table s_div_command_table[] =
{
    ///                                     IntValue,            FloatValue,           Float2Value, Float3Value, Float4Value, Matrix4x4Value, StringValue, BoolValue, Function
    {"int div value",       IntValue,       {IntDivIntCommand,   IntDivFloatCommand,   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float div value",     FloatValue,     {FloatDivIntCommand, FloatDivFloatCommand, NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float2 div value",    Float2Value,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float3 div value",    Float3Value,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float4 div value",    Float4Value,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"matrix4x4 div value", Matrix4x4Value, {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"string div value",    StringValue,    {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"bool div value",      BoolValue,      {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"function div value",  Function,       {NULL,               NULL,                 NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
};

static arithmetic_command_table s_ass_command_table[] =
{
    ///                                     IntValue,               FloatValue,              Float2Value, Float3Value, Float4Value, Matrix4x4Value, StringValue, BoolValue,
    {"int ass value",       IntValue,       {IntAssignIntCommand,   IntAssignFloatCommand,   NULL,        NULL,        NULL,        NULL,            NULL,       NULL}},
    {"float ass value",     FloatValue,     {FloatAssignIntCommand, FloatAssignFloatCommand, NULL,        NULL,        NULL,        NULL,            NULL,       NULL}},
    {"float2 ass value",    Float2Value,    {NULL,                  NULL,                    NULL,        NULL,        NULL,        NULL,            NULL,       NULL}},
    {"float3 ass value",    Float3Value,    {NULL,                  NULL,                    NULL,        NULL,        NULL,        NULL,            NULL,       NULL}},
    {"float4 ass value",    Float4Value,    {NULL,                  NULL,                    NULL,        NULL,        NULL,        NULL,            NULL,       NULL}},
    {"matrix4x4 ass value", Matrix4x4Value, {NULL,                  NULL,                    NULL,        NULL,        NULL,        NULL,            NULL,       NULL}},
    {"string ass value",    StringValue,    {NULL,                  NULL,                    NULL,        NULL,        NULL,        NULL,            NULL,       NULL}},
    {"bool ass value",      BoolValue,      {NULL,                  NULL,                    NULL,        NULL,        NULL,        NULL,            NULL,       NULL}},
};

static arithmetic_command_table s_less_command_table[] =
{
    ///                                      IntValue,             FloatValue,            Float2Value, Float3Value, Float4Value, Matrix4x4Value, StringValue, BoolValue, Function
    {"int less value",       BoolValue,      {IntLessIntCommand,   IntLessFloatCommand,   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float less value",     FloatValue,     {FloatLessIntCommand, FloatLessFloatCommand, NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float2 less value",    Float2Value,    {NULL,                NULL,                  NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float3 less value",    Float3Value,    {NULL,                NULL,                  NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float4 less value",    Float4Value,    {NULL,                NULL,                  NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"matrix4x4 less value", Matrix4x4Value, {NULL,                NULL,                  NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"string less value",    StringValue,    {NULL,                NULL,                  NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"bool less value",      BoolValue,      {NULL,                NULL,                  NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"function less value",  Function,       {NULL,                NULL,                  NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
};

static arithmetic_command_table s_greater_command_table[] =
{
    ///                                         IntValue,                FloatValue,               Float2Value, Float3Value, Float4Value, Matrix4x4Value, StringValue, BoolValue, Function
    {"int greater value",       BoolValue,      {IntGreaterIntCommand,   IntGreaterFloatCommand,   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float greater value",     FloatValue,     {FloatGreaterIntCommand, FloatGreaterFloatCommand, NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float2 greater value",    Float2Value,    {NULL,                   NULL,                     NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float3 greater value",    Float3Value,    {NULL,                   NULL,                     NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float4 greater value",    Float4Value,    {NULL,                   NULL,                     NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"matrix4x4 greater value", Matrix4x4Value, {NULL,                   NULL,                     NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"string greater value",    StringValue,    {NULL,                   NULL,                     NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"bool greater value",      BoolValue,      {NULL,                   NULL,                     NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"function greater value",  Function,       {NULL,                   NULL,                     NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
};

static arithmetic_command_table s_equal_command_table[] =
{
    ///                                       IntValue,            FloatValue,               Float2Value, Float3Value, Float4Value, Matrix4x4Value, StringValue, BoolValue, Function
    {"int equal value",       BoolValue,      {IntEqualIntCommand,   IntEqualFloatCommand,   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float equal value",     FloatValue,     {FloatEqualIntCommand, FloatEqualFloatCommand, NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float2 equal value",    Float2Value,    {NULL,                 NULL,                   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float3 equal value",    Float3Value,    {NULL,                 NULL,                   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float4 equal value",    Float4Value,    {NULL,                 NULL,                   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"matrix4x4 equal value", Matrix4x4Value, {NULL,                 NULL,                   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"string equal value",    StringValue,    {NULL,                 NULL,                   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"bool equal value",      BoolValue,      {NULL,                 NULL,                   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"function equal value",  Function,       {NULL,                 NULL,                   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
};

static arithmetic_command_table s_less_equal_command_table[] =
{
    ///                                            IntValue,                  FloatValue,                 Float2Value, Float3Value, Float4Value, Matrix4x4Value, StringValue, BoolValue, Function
    {"int less equal value",       BoolValue,      {IntLessEqualIntCommand,   IntLessEqualFloatCommand,   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float less equal value",     FloatValue,     {FloatLessEqualIntCommand, FloatLessEqualFloatCommand, NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float2 less equal value",    Float2Value,    {NULL,                     NULL,                       NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float3 less equal value",    Float3Value,    {NULL,                     NULL,                       NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float4 less equal value",    Float4Value,    {NULL,                     NULL,                       NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"matrix4x4 less equal value", Matrix4x4Value, {NULL,                     NULL,                       NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"string less equal value",    StringValue,    {NULL,                     NULL,                       NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"bool less equal value",      BoolValue,      {NULL,                     NULL,                       NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"function less equal value",  Function,       {NULL,                     NULL,                       NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
};

static arithmetic_command_table s_greater_equal_command_table[] =
{
    ///                                               IntValue,                     FloatValue,                    Float2Value, Float3Value, Float4Value, Matrix4x4Value, StringValue, BoolValue, Function
    {"int greater equal value",       BoolValue,      {IntGreaterEqualIntCommand,   IntGreaterEqualFloatCommand,   NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float greater equal value",     FloatValue,     {FloatGreaterEqualIntCommand, FloatGreaterEqualFloatCommand, NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float2 greater equal value",    Float2Value,    {NULL,                        NULL,                          NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float3 greater equal value",    Float3Value,    {NULL,                        NULL,                          NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"float4 greater equal value",    Float4Value,    {NULL,                        NULL,                          NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"matrix4x4 greater equal value", Matrix4x4Value, {NULL,                        NULL,                          NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"string greater equal value",    StringValue,    {NULL,                        NULL,                          NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"bool greater equal value",      BoolValue,      {NULL,                        NULL,                          NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
    {"function greater equal value",  Function,       {NULL,                        NULL,                          NULL,        NULL,        NULL,        NULL,            NULL,       NULL,      NULL}},
};

Command _get_cmd(OperatorType op, int dst_type, int src_type)
{
    switch (op)
    {
    case AddOp:
        return s_add_command_table[dst_type].src_type[src_type];
    case SubOp:
        return s_sub_command_table[dst_type].src_type[src_type];
    case MulOp:
        return s_mul_command_table[dst_type].src_type[src_type];
    case DivOp:
        return s_div_command_table[dst_type].src_type[src_type];
    case AssignOp:
        return s_ass_command_table[dst_type].src_type[src_type];
    case LessOp:
        return s_less_command_table[dst_type].src_type[src_type];
    case GreaterOp:
        return s_greater_command_table[dst_type].src_type[src_type];
    case EqualOp:
        return s_equal_command_table[dst_type].src_type[src_type];
    case LessEqualOp:
        return s_less_equal_command_table[dst_type].src_type[src_type];
    case GreaterEqualOp:
        return s_greater_equal_command_table[dst_type].src_type[src_type];
    default:
        return NULL;
    }
}

SymbolValueType _get_ret_type(SymbolValueType dst_type, SymbolValueType src_type)
{
    return s_result_type_table[dst_type].src_type[src_type];
}

/// src, dst
CommandClosures SingleSymbolOperator1(ParserEnv* e, OperatorType op, SymbolValue dst, SymbolValue src)
{
    Command cmd = _get_cmd(op, dst.type, src.type);
    if (!cmd)
    {
        parser_exception exce = s_exce_table[Invalid_operation];
        ParserEnv_push_exce(e, exce);
    }
    CommandClosures cc = CommandClosures_new(cmd);
    CommandClosures_add_arg(cc, src);
    CommandClosures_add_arg(cc, dst);
    return cc;
}

/// src, dst, ret
CommandClosures SingleSymbolOperator2(ParserEnv* e, OperatorType op, SymbolValue dst, SymbolValue src)
{
    CommandClosures cc = SingleSymbolOperator1(e, op, dst, src);
    SymbolValue ret;
    ret.type = _get_ret_type(dst.type, src.type);
    switch (ret.type)
    {
    case IntValue:
    case FloatValue:
    case Float2Value:
    case Float3Value:
    case Float4Value:
    case Matrix4x4Value:
    case BoolValue:
    case Function:
        ret.data.mem_addr.mem_type = VariMem;
        ret.data.mem_addr.offs = ValueAllocator_alloc(&e->vari_alloc, ret.type);
        break;
    case StringValue:
        ret.data.mem_addr.mem_type = VariStrMem;
        ret.data.mem_addr.offs = ValueAllocator_alloc(&e->vari_str_alloc, ret.type);
        break;
    default:
        memset(&ret.data, 0, sizeof(ret.data));
        break;
    }
    CommandClosures_add_arg(cc, ret);
    return cc;
}

/// src, dst, ret
/// ret is bool
CommandClosures SingleSymbolOperator3(ParserEnv* e, OperatorType op, SymbolValue dst, SymbolValue src)
{
    CommandClosures cc = SingleSymbolOperator1(e, op, dst, src);
    SymbolValue ret;
    ret.type = BoolValue;
    ret.data.mem_addr.mem_type = VariMem;
    ret.data.mem_addr.offs = ValueAllocator_alloc(&e->vari_alloc, ret.type);
    CommandClosures_add_arg(cc, ret);
    return cc;
}
