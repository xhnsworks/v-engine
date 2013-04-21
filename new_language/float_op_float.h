#ifndef FLOAT_OP_FLOAT_H
#define FLOAT_OP_FLOAT_H
#include "etypes.h"
#include "common.h"
#include "parser_env.h"
void FloatAddFloatCommand(ParserEnv* e, SymbolValue* args);
void FloatSubFloatCommand(ParserEnv* e, SymbolValue* args);
void FloatMulFloatCommand(ParserEnv* e, SymbolValue* args);
void FloatDivFloatCommand(ParserEnv* e, SymbolValue* args);

void FloatLessFloatCommand(ParserEnv* e, SymbolValue* args);
void FloatGreaterFloatCommand(ParserEnv* e, SymbolValue* args);
void FloatEqualFloatCommand(ParserEnv* e, SymbolValue* args);
void FloatLessEqualFloatCommand(ParserEnv* e, SymbolValue* args);
void FloatGreaterEqualFloatCommand(ParserEnv* e, SymbolValue* args);

void FloatAssignFloatCommand(ParserEnv* e, SymbolValue* args);
///void FuncTestFloatAssignFloatCommand(ParserEnv* e, SymbolValue* args);
#endif
