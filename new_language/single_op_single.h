#ifndef SINGLE_OP_SINGLE_H
#define SINGLE_OP_SINGLE_H
#include "etypes.h"
#include "common.h"
#include "parser_env.h"
typedef enum _operator_type
{
    AddOp,
    SubOp,
    MulOp,
    DivOp,
    AssignOp,
    LessOp,
    GreaterOp,
    EqualOp,
    LessEqualOp,
    GreaterEqualOp,
} OperatorType;
typedef enum _exce_id
{
    Invalid_operation = 0,
    Symbol_is_defined_more_than_once,
    The_element_name_is_defined_more_than_once_in_struct,
    Unknown_data_type,
    Structure_is_defined_more_than_once,
    MaxExce,
} exce_id;
extern parser_exception s_exce_table[];
CommandClosures SingleSymbolOperator1(ParserEnv* e, OperatorType op, SymbolValue dst, SymbolValue src);
CommandClosures SingleSymbolOperator2(ParserEnv* e, OperatorType op, SymbolValue dst, SymbolValue src);
CommandClosures SingleSymbolOperator3(ParserEnv* e, OperatorType op, SymbolValue dst, SymbolValue src);
#endif
