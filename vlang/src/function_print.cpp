#include "function_print.h"

#include "float_op_float.h"
#include "float_op_int.h"
#include "int_op_float.h"
#include "int_op_int.h"

#include "single_op_single.h"

#include "function_call.h"
#include "jump.h"

#include "print.h"
typedef struct _command_entry
{
    Command cmd;
    const char* desc;
} command_entry;

command_entry s_command_table[] = {
    {FloatAddFloatCommand,          "FloatAddFloatCommand"},
    {FloatSubFloatCommand,          "FloatSubFloatCommand"},
    {FloatMulFloatCommand,          "FloatMulFloatCommand"},
    {FloatDivFloatCommand,          "FloatDivFloatCommand"},
    {FloatLessFloatCommand,         "FloatLessFloatCommand"},
    {FloatGreaterFloatCommand,      "FloatGreaterFloatCommand"},
    {FloatEqualFloatCommand,        "FloatEqualFloatCommand"},
    {FloatLessEqualFloatCommand,    "FloatLessEqualFloatCommand"},
    {FloatGreaterEqualFloatCommand, "FloatGreaterEqualFloatCommand"},
    {FloatAssignFloatCommand,       "FloatAssignFloatCommand"},

    {FloatAddIntCommand,            "FloatAddIntCommand"},
    {FloatSubIntCommand,            "FloatSubIntCommand"},
    {FloatMulIntCommand,            "FloatMulIntCommand"},
    {FloatDivIntCommand,            "FloatDivIntCommand"},
    {FloatLessIntCommand,           "FloatLessIntCommand"},
    {FloatGreaterIntCommand,        "FloatGreaterIntCommand"},
    {FloatEqualIntCommand,          "FloatEqualIntCommand"},
    {FloatLessEqualIntCommand,      "FloatLessEqualIntCommand"},
    {FloatGreaterEqualIntCommand,   "FloatGreaterEqualIntCommand"},
    {FloatAssignIntCommand,         "FloatAssignIntCommand"},

    {IntAddFloatCommand,            "IntAddFloatCommand"},
    {IntSubFloatCommand,            "IntSubFloatCommand"},
    {IntMulFloatCommand,            "IntMulFloatCommand"},
    {IntDivFloatCommand,            "IntDivFloatCommand"},
    {IntLessFloatCommand,           "IntLessFloatCommand"},
    {IntGreaterFloatCommand,        "IntGreaterFloatCommand"},
    {IntEqualFloatCommand,          "IntEqualFloatCommand"},
    {IntLessEqualFloatCommand,      "IntLessEqualFloatCommand"},
    {IntGreaterEqualFloatCommand,   "IntGreaterEqualFloatCommand"},
    {IntAssignFloatCommand,         "IntAssignFloatCommand"},

    {IntAddIntCommand,              "IntSubIntCommand"},
    {IntSubIntCommand,              "IntSubIntCommand"},
    {IntMulIntCommand,              "IntMulIntCommand"},
    {IntDivIntCommand,              "IntDivIntCommand"},
    {IntLessIntCommand,             "IntLessIntCommand"},
    {IntGreaterIntCommand,          "IntGreaterIntCommand"},
    {IntEqualIntCommand,            "IntEqualIntCommand"},
    {IntLessEqualIntCommand,        "IntLessEqualIntCommand"},
    {IntGreaterEqualIntCommand,     "IntGreaterEqualIntCommand"},
    {IntAssignIntCommand,           "IntAssignIntCommand"},

    {FunctionCallCommand,           "FunctionCallCommand"},

    {IfTrueJumpCommand,             "IfTrueJumpCommand"},
    {IfFalseJumpCommand,            "IfFalseJumpCommand"},
    {JumpCommand,                   "JumpCommand"},

    {PrintFloatCommand,             "PrintFloatCommand"},
    {PrintIntCommand,               "PrintIntCommand"},
    {PrintBoolCommand,              "PrintBoolCommand"},
};

void PrintSymbolValue(ParserEnv* e, const char* str, SymbolValue value);
void _print_command(ParserEnv* e, CommandClosures cc)
{
    uint n = sizeof(s_command_table) / sizeof(s_command_table[0]);
    for (uint i = 0; i < n; i++)
    {
        if (cc->cmd == s_command_table[i].cmd) {
            VPRINT("%s\n", s_command_table[i].desc);
            uint n = array_n(cc->args);
            for (uint i = 0; i < n; i++)
            {
                PrintSymbolValue(e, "", cc->args[i]);
            }
            VPRINT("\n");
            return;
        }
    }
    VPRINT("unknown command\n\n");
}
void FunctionPrint(ParserEnv* e, FunctionClosures func)
{
    uint n = array_n(func->cmds);
    for (uint i = 0; i < n; i++)
    {
        VPRINT("ADDR: %d, ", i);
        _print_command(e, func->cmds[i]);
    }
}
