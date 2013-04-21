#include "jump.h"

void JumpCommand(ParserEnv* e, SymbolValue* args)
{
    int* addr = (int*)_get_ptr(e, args[0]);
    e->curt_func->cmd_cnt = *addr - 1;
}

void IfTrueJumpCommand(ParserEnv* e, SymbolValue* args)
{
    bool* cond = (bool*)_get_ptr(e, args[1]);
    int* addr = (int*)_get_ptr(e, args[0]);
    if (*cond)
        e->curt_func->cmd_cnt = *addr - 1;
}
void IfFalseJumpCommand(ParserEnv* e, SymbolValue* args)
{
    bool* cond = (bool*)_get_ptr(e, args[1]);
    int* addr = (int*)_get_ptr(e, args[0]);
    if (!*cond)
        e->curt_func->cmd_cnt = *addr - 1;
}
