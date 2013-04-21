#include "float_op_float.h"
void FloatAddFloatCommand(ParserEnv* e, SymbolValue* args)
{
    float* dst = (float*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = *a + *b;
}
void FloatSubFloatCommand(ParserEnv* e, SymbolValue* args)
{
    float* dst = (float*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = *a - *b;
}
void FloatMulFloatCommand(ParserEnv* e, SymbolValue* args)
{
    float* dst = (float*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = *a * *b;
}
void FloatDivFloatCommand(ParserEnv* e, SymbolValue* args)
{
    float* dst = (float*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = *a / *b;
}

void FloatLessFloatCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = *a < *b;
}
void FloatGreaterFloatCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = *a > *b;
}
void FloatEqualFloatCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = (*a == *b);
}
void FloatLessEqualFloatCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = (*a <= *b);
}
void FloatGreaterEqualFloatCommand(ParserEnv* e, SymbolValue* args)
{
    bool* dst = (bool*)_get_ptr(e, args[2]);
    float* a = (float*)_get_ptr(e, args[1]);
    float* b = (float*)_get_ptr(e, args[0]);
    *dst = (*a >= *b);
}

void FloatAssignFloatCommand(ParserEnv* e, SymbolValue* args)
{
    float* dst = (float*)_get_ptr(e, args[1]);
    float* src = (float*)_get_ptr(e, args[0]);
    *dst = *src;
}
/**
void FuncTestFloatAssignFloatCommand(ParserEnv* e, SymbolValue* args)
{
    float* dst = (float*)_get_ptr(e, args[1]);
    float* src = (float*)_get_ptr(e, args[0]);
    *dst = *src;
}
**/
