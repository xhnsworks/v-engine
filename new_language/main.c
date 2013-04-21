#include <stdio.h>
#include <stdlib.h>
#include "test.tab.h"
#include "etypes.h"
#include "parser_env.h"
#include "tree.h"
#include "emem.h"
#include "symbol_stack.h"
#include "path.h"

#include "single_op_single.h"

#include "function_print.h"
///static int s_symbols[] = {_float, _add, _float, _mul, left_parenthese, _float, _add, _float, right_parenthese, line_end};
///static int s_count = 0;
/**e->text = "func test2 (a float) (b float)\n"
              "{\n"
              "    b = a / 2.0;\n"
              "    print a;\n"
              "}\n"
              "func ttt (a float, b float) (c float)\n"
              "{\n"
              "    var t float;\n"
              "    var g float;\n"
              "    var i int;\n"
              "    i = 3;\n"
              "    t = 4.0 * i;\n"
              "    g = test2(3.0);\n"
              "    j := t * g;\n"
              "    print j;\n"
              "}";
int yylex   (int *lvalp)
{
    return s_symbols[s_count++];
}
**/
int yylex (SymbolValue * lvalp, ParserEnv* e);

void yyerror(ParserEnv* e, const char* error)
{
    printf("%s\n", error);
    parser_exception exce = s_exce_table[Invalid_operation];
    ParserEnv_push_exce(e, exce);
}


#include <direct.h>
#include <sys\stat.h>

void yyparse(ParserEnv* e);
int main()
{
    ///FILE* fp = fopen();
    ///struct _finddata_t fd;
    ///_findfirst();

    ///_chdir("D:\\Projects\\experimental-platform-for-computer-graphics\\new_language\\bin\\Debug");

    MInit();

    char mbuf[MAX_PATH];
    EStringArray str_array = EStringArray_new();
    ///_getcwd(mbuf, MAX_PATH);
    snprintf(mbuf, 255, "F:\\new_project\\ecg\\new_language\\bin\\Debug");
    ///GetAllFileNamesInDir((const char*)mbuf, "*.txt", &str_array);
    GetAllFileNamesInDir(mbuf, "*.txt", &str_array);
    uint n = array_n(str_array);
    if (n)
    {
        char fullpath[MAX_PATH];
        snprintf(fullpath, MAX_PATH - 1, "%s\\%s", mbuf, str_array[0]);
        ///snprintf(fullpath, MAX_PATH - 1, "D:\\aaa.txt");
        struct _stat info;
        _stat(fullpath, &info);
        if (info.st_size)
        {
            FILE* fp = fopen(fullpath, "rb");
            if (fp)
            {
                char* buffer = Malloc(info.st_size + 1);
                fread(buffer, 1, info.st_size, fp);
                buffer[info.st_size] = 0x00;

                ParserEnv e;
                ParserEnv_Init(&e, buffer);
                yyparse(&e);
                ///vptr addr = FunctionClosures_get_vari_addr(&e, e.curt_func, "a");
                if (array_n(e.exce_array))
                {
                    printf("error line %d\n", e.line_count);
                    for (uint i = 0; i < array_n(e.exce_array); i++)
                    {
                        parser_exception exce = e.exce_array[i];
                        printf("EXCE:%s\n", exce.exce_desc);
                    }
                    getchar();
                    return 0;
                }
                ParserEnv_print_all_function(&e);
                int vari_type;
                FunctionClosures fc = ParserEnv_find_function(&e, "main");
                ///FunctionPrint(&e, fc);
                getchar();
                if (fc)
                    e.curt_func = fc;
                sint offs = FunctionClosures_get_vari_offs(&e, e.curt_func, "a", &vari_type);
                vptr addr = ParserEnv_get_addr(&e, vari_type, offs);
                printf("addr %x\n, offs %d\n", (uint)addr, offs);
                float* faddr = (float*)addr;
                *faddr = 3.0f;
                printf("############################\n");
                FunctionClosures_execute(&e, e.curt_func);

                Mfree(buffer);
                fclose(fp);
            }
        }
    }
    EStringArray_delete(str_array);

    getchar();
    return 0;
}
