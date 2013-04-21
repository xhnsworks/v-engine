#ifndef PROC_H
#define PROC_H
#include "common.h"
#include "etypes.h"
#include "parser_env.h"
void AddParam(ParserEnv* e, SymbolValue _sym, SymbolValue valuetype);
void PrintSymbol(ParserEnv* e, SymbolValue _sym);
void CompleteParamDef(ParserEnv* e);
void CompleteOutputDef(ParserEnv* e);
SymbolValue FloatTypeSym();
SymbolValue IntTypeSym();
#endif