#ifndef JUMP_H
#define JUMP_H
#include "common.h"
#include "etypes.h"

#include "parser_env.h"
void JumpCommand(ParserEnv* e, SymbolValue* args);
void IfTrueJumpCommand(ParserEnv* e, SymbolValue* args);
void IfFalseJumpCommand(ParserEnv* e, SymbolValue* args);
#endif
