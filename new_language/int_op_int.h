#ifndef INT_OP_INT_H
#define INT_OP_INT_H
#include "etypes.h"
#include "common.h"
#include "parser_env.h"
void IntAddIntCommand(ParserEnv* e, SymbolValue* args);
void IntSubIntCommand(ParserEnv* e, SymbolValue* args);
void IntMulIntCommand(ParserEnv* e, SymbolValue* args);
void IntDivIntCommand(ParserEnv* e, SymbolValue* args);

void IntLessIntCommand(ParserEnv* e, SymbolValue* args);
void IntGreaterIntCommand(ParserEnv* e, SymbolValue* args);
void IntEqualIntCommand(ParserEnv* e, SymbolValue* args);
void IntLessEqualIntCommand(ParserEnv* e, SymbolValue* args);
void IntGreaterEqualIntCommand(ParserEnv* e, SymbolValue* args);

void IntAssignIntCommand(ParserEnv* e, SymbolValue* args);
#endif
