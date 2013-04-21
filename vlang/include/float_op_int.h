#ifndef FLOAT_OP_INT_H
#define FLOAT_OP_INT_H
#include "common.h"
#include "etypes.h"

#include "parser_env.h"
void FloatAddIntCommand(ParserEnv* e, SymbolValue* args);
void FloatSubIntCommand(ParserEnv* e, SymbolValue* args);
void FloatMulIntCommand(ParserEnv* e, SymbolValue* args);
void FloatDivIntCommand(ParserEnv* e, SymbolValue* args);

void FloatLessIntCommand(ParserEnv* e, SymbolValue* args);
void FloatGreaterIntCommand(ParserEnv* e, SymbolValue* args);
void FloatEqualIntCommand(ParserEnv* e, SymbolValue* args);
void FloatLessEqualIntCommand(ParserEnv* e, SymbolValue* args);
void FloatGreaterEqualIntCommand(ParserEnv* e, SymbolValue* args);

void FloatAssignIntCommand(ParserEnv* e, SymbolValue* args);
#endif
