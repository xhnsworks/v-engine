#include "int_op_int.h"

void IntAddIntCommand(ParserEnv* e, SymbolValue* args)
{
    int* dst = (int*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = *a + *b;
}
void IntSubIntCommand(ParserEnv* e, SymbolValue* args)
{
    int* dst = (int*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = *a - *b;
}
void IntMulIntCommand(ParserEnv* e, SymbolValue* args)
{
    int* dst = (int*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = *a * *b;
}
void IntDivIntCommand(ParserEnv* e, SymbolValue* args)
{
    int* dst = (int*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = *a / *b;
}

void IntLessIntCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = *a < *b;
}
void IntGreaterIntCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = *a > *b;
}
void IntEqualIntCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = (*a == *b);
}
void IntLessEqualIntCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = (*a <= *b);
}
void IntGreaterEqualIntCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = (*a >= *b);
}

void IntAssignIntCommand(ParserEnv* e, SymbolValue* args)
{
    int* dst = (int*)_get_ptr(e, args[1]);
    int* src = (int*)_get_ptr(e, args[0]);
    *dst = *src;
}
