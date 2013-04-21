#ifndef JUMP_H
#define JUMP_H
#include "etypes.h"
#include "common.h"
#include "parser_env.h"
void JumpCommand(ParserEnv* e, SymbolValue* args);
void IfTrueJumpCommand(ParserEnv* e, SymbolValue* args);
void IfFalseJumpCommand(ParserEnv* e, SymbolValue* args);
#endif
