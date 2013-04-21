#ifndef INT_OP_FLOAT_H
#define INT_OP_FLOAT_H
#include "common.h"
#include "etypes.h"

#include "parser_env.h"
void IntAddFloatCommand(ParserEnv* e, SymbolValue* args);
void IntSubFloatCommand(ParserEnv* e, SymbolValue* args);
void IntMulFloatCommand(ParserEnv* e, SymbolValue* args);
void IntDivFloatCommand(ParserEnv* e, SymbolValue* args);

void IntLessFloatCommand(ParserEnv* e, SymbolValue* args);
void IntGreaterFloatCommand(ParserEnv* e, SymbolValue* args);
void IntEqualFloatCommand(ParserEnv* e, SymbolValue* args);
void IntLessEqualFloatCommand(ParserEnv* e, SymbolValue* args);
void IntGreaterEqualFloatCommand(ParserEnv* e, SymbolValue* args);

void IntAssignFloatCommand(ParserEnv* e, SymbolValue* args);
#endif
