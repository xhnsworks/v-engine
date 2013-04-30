#include "vlang.h"
#include "symbol_stack.h"

#include "single_op_single.h"
#include "proc.h"

int yylex (SymbolValue * lvalp, ParserEnv* e);

void yyerror(ParserEnv* e, const char* error)
{
	VPRINT("%s\n", error);
	parser_exception exce = s_exce_table[Invalid_operation];
	ParserEnv_push_exce(e, exce);
}
#ifdef __cplusplus
extern "C"
{
#endif
#include "test.tab.h"
#ifdef __cplusplus
}
#endif

int yyparse(ParserEnv* e);

void CFuncTest(ParserEnv* e, SymbolValue* sv)
{
	elog("##KILL YOU");
    euint n = array_n(sv);
	elog("num args %d", n);
	for (euint i = 0; i < n; i++)
	{
		SymbolValue v = sv[i];
		PrintSymbol(e, v);
		if ((ValueType)v.type == Function)
		{
			FunctionClosures fc = v.data.func;
            vptr value_addr = FunctionClosures_get_vari_addr(e, fc, "x");
			elog("value addr %x", value_addr);
			value_addr = FunctionClosures_get_vari_addr(e, fc, "ret");
			elog("ret addr %x", value_addr);

			*((float*)value_addr) = 100.0f;
		}
	}
}

void CFunctionConfig::_AddParam(ParserEnv* e, const Params& params)
{
	for (euint i = 0; i < params.size(); i++)
	{
		const xhn::pair<xhn::string, ValueType>& p = params[i];
		SymbolValue type;
		switch (p.second)
		{
		case IntValue:
			type = IntTypeSym();
			break;
		case FloatValue:
			type = FloatTypeSym();
			break;
		default:
			return;
		}
		SymbolValue* sv = ParserEnv_new_unknown_symbol(e, p.first.c_str());
		AddParam(e, *sv, type);
	}
}
void CFunctionConfig::AttachToParseEnv(ParserEnv* e, const xhn::string& funcName)
{
    FunctionClosures c_fc = ParserEnv_new_function(e, funcName.c_str());
	_AddParam(e, m_input_params);
	CompleteParamDef(e);
	_AddParam(e, m_output_params);
	CompleteOutputDef(e);
	FunctionClosures_add_c_function_command(c_fc, m_func);
	ParserEnv_complete_function(e);
}
void CFunctionConfig::AttachToParseEnvTest()
{
	for (euint i = 0; i < m_input_params.size(); i++)
	{
		const xhn::pair<xhn::string, ValueType>& p = m_input_params[i];
		elog("param name %s", p.first.c_str());
	}
}
void vlang_run(const char* buffer)
{
	ParserEnv e;
	ParserEnv_Init(&e, buffer);

	CFunctionConfig cfc(CFuncTest);
	cfc.AddInputParam(xhn::string("x"), FloatValue);
	cfc.AddOutputParam(xhn::string("ret"), FloatValue);
	cfc.AttachToParseEnv(&e, xhn::string("c_test"));

	yyparse(&e);
	///vptr addr = FunctionClosures_get_vari_addr(&e, e.curt_func, "a");
	if (array_n(e.exce_array))
	{
		elog("error line %d char count %d\n", e.line_count, e.char_count);
		for (euint i = 0; i < array_n(e.exce_array); i++)
		{
			parser_exception exce = e.exce_array[i];
			elog("EXCE:%s\n", exce.exce_desc);
		}
		return;
	}
	ParserEnv_print_all_function(&e);
	///int vari_type;
	FunctionClosures fc = ParserEnv_find_function(&e, "main");
	///FunctionPrint(&e, fc);
	if (fc)
		e.curt_func = fc;
	elog("############################\n");
	FunctionClosures_execute(&e, e.curt_func);
}