#include "float_op_int.h"

void FloatAddIntCommand(ParserEnv* e, SymbolValue* args)
{
    float* dst = (float*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = *a + (float)*b;
}
void FloatSubIntCommand(ParserEnv* e, SymbolValue* args)
{
    float* dst = (float*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = *a - (float)*b;
}
void FloatMulIntCommand(ParserEnv* e, SymbolValue* args)
{
    float* dst = (float*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = *a * (float)*b;
}
void FloatDivIntCommand(ParserEnv* e, SymbolValue* args)
{
    float* dst = (float*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = *a / (float)*b;
}

void FloatLessIntCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = *a < (float)*b;
}
void FloatGreaterIntCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = *a > (float)*b;
}
void FloatEqualIntCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = (*a == (float)*b);
}
void FloatLessEqualIntCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = (*a <= (int)*b);
}
void FloatGreaterEqualIntCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    int* b = (int*)_get_ptr(e, args[0]);
    *dst = (*a >= (float)*b);
}

void FloatAssignIntCommand(ParserEnv* e, SymbolValue* args)
{
    float* dst = (float*)_get_ptr(e, args[1]);
    int* src = (int*)_get_ptr(e, args[0]);
    printf("\n");
    *dst = (float)*src;
}
