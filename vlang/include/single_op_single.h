#ifndef SINGLE_OP_SINGLE_H
#define SINGLE_OP_SINGLE_H
#include "common.h"
#include "etypes.h"

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

extern parser_exception s_exce_table[];
CommandClosures SingleSymbolOperator1(ParserEnv* e, OperatorType op, SymbolValue dst, SymbolValue src);
CommandClosures SingleSymbolOperator2(ParserEnv* e, OperatorType op, SymbolValue dst, SymbolValue src);
CommandClosures SingleSymbolOperator3(ParserEnv* e, OperatorType op, SymbolValue dst, SymbolValue src);
#endif
