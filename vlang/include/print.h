#ifndef PRINT_H
#define PRINT_H
#include "common.h"
#include "etypes.h"

#include "parser_env.h"
void PrintFloatCommand(ParserEnv* e, SymbolValue* args);
void PrintIntCommand(ParserEnv* e, SymbolValue* args);
void PrintBoolCommand(ParserEnv* e, SymbolValue* args);
#endif
