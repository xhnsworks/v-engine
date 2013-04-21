#include "int_op_float.h"

void IntAddFloatCommand(ParserEnv* e, SymbolValue* args)
{
    int* dst = (int*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = *a + (int)*b;
}
void IntSubFloatCommand(ParserEnv* e, SymbolValue* args)
{
    int* dst = (int*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = *a - (int)*b;
}
void IntMulFloatCommand(ParserEnv* e, SymbolValue* args)
{
    int* dst = (int*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = *a * (int)*b;
}
void IntDivFloatCommand(ParserEnv* e, SymbolValue* args)
{
    int* dst = (int*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = *a / (int)*b;
}

void IntLessFloatCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = *a < (int)*b;
}
void IntGreaterFloatCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = *a > (int)*b;
}
void IntEqualFloatCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = (*a == (int)*b);
}
void IntLessEqualFloatCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = (*a <= (int)*b);
}
void IntGreaterEqualFloatCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    int* a = (int*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = (*a >= (int)*b);
}

void IntAssignFloatCommand(ParserEnv* e, SymbolValue* args)
{
    int* dst = (int*)_get_ptr(e, args[1]);
    float* src = (float*)_get_ptr(e, args[0]);
    *dst = (int)*src;
}
