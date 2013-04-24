#ifndef GLSL_LEX_H
#define GLSL_LEX_H
#include "common.h"
#include "etypes.h"
#include "xhn_map.hpp"
#include "xhn_string.hpp"
namespace GLSL
{
	#include "glsl.tab.hpp"
	typedef int GLSLSymbolValue;
	enum ExpectedSymType
	{
        EmptyType = 0,
		SymbolType,
        NumericalType,
		TextType,
	};
	class GLSLLexStatus
	{
	public:
		int remainder;
		int stringSize;
		int curtExpType;
		char* str;
		char mbuf[1025];
	public:
		void Init();
	};
	
	class GLSLParserEnv
	{
	public:
		const char* m_text;
		int m_charCount;
		int m_lineCount;
		GLSLLexStatus m_lexStatus;
		xhn::map<xhn::string, GLSLSymbolValue> m_symValue;
		GLSLParserEnv(const char* str);
	};
	int yylex(GLSLSymbolValue *lvalp, GLSLParserEnv* e);
	void yyerror(GLSLParserEnv* e, const char* error);
	void test();
}

#endif