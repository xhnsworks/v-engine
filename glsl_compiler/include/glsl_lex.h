#ifndef GLSL_LEX_H
#define GLSL_LEX_H
namespace GLSL
{
	enum ExpectedSymType
	{
        EmptyType = 0,
		SymbolType,
        NumericalType,
		TextType,
	};
	typedef int GLSLSymbolValue;
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
	};
#include "glsl.tab.h"
	int yylex(GLSLSymbolValue *lvalp, GLSLParserEnv* e);
}

#endif