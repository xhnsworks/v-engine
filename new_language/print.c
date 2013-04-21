#include "print.h"

void PrintFloatCommand(ParserEnv* e, SymbolValue* args)
{
    printf("##Print:\n");
    printf("SRC:");
    float* dst = (float*)_get_ptr(e, args[0]);
    printf("\n");
    printf("%f\n", *dst);
}

void PrintIntCommand(ParserEnv* e, SymbolValue* args)
{
    printf("##Print:\n");
    printf("SRC:");
    int* dst = (int*)_get_ptr(e, args[0]);
    printf("\n");
    printf("%d\n", *dst);
}

void PrintBoolCommand(ParserEnv* e, SymbolValue* args)
{
    printf("##Print:\n");
    printf("SRC:");
    bool* dst = (bool*)_get_ptr(e, args[0]);
    printf("\n");
    ///printf("%d\n", *dst);
    if (*dst)
        printf("TRUE\n");
    else
        printf("FALSE\n");
}
