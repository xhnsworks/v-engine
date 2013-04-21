#include "function_call.h"

void FunctionCallCommand(ParserEnv* e, SymbolValue* args)
{
    SymbolValue func = args[0];
    SymbolValue vari_stack_size = args[1];
    SymbolValue vari_str_stack_size = args[2];
    int* vari_offs = _get_ptr(e, vari_stack_size);
    int* vari_str_offs = _get_ptr(e, vari_str_stack_size);
    FunctionClosures fc = func.data.func;
    ParserEnv_push_return_point(e, *vari_offs, *vari_str_offs, e->curt_func, e->curt_func->cmd_cnt);
    FunctionClosures_execute(e, fc);
    ParserEnv_pop_return_point(e);
}
