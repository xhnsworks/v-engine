#include "print.h"

void PrintFloatCommand(ParserEnv* e, SymbolValue* args)
{
    VPRINT("##Print:\n");
    VPRINT("SRC:");
    float* dst = (float*)_get_ptr(e, args[0]);
    VPRINT("\n");
    VPRINT("%f\n", *dst);
}

void PrintIntCommand(ParserEnv* e, SymbolValue* args)
{
    VPRINT("##Print:\n");
    VPRINT("SRC:");
    int* dst = (int*)_get_ptr(e, args[0]);
    VPRINT("\n");
    VPRINT("%d\n", *dst);
}

void PrintBoolCommand(ParserEnv* e, SymbolValue* args)
{
    VPRINT("##Print:\n");
    VPRINT("SRC:");
    bool* dst = (bool*)_get_ptr(e, args[0]);
    VPRINT("\n");
    ///printf("%d\n", *dst);
    if (*dst) {
        VPRINT("TRUE\n");
	}
    else {
        VPRINT("FALSE\n");
	}
}
