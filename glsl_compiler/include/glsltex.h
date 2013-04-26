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
		xhn::string m_output;
		xhn::map<xhn::string, GLSLSymbolValue> m_symValue;
		GLSLParserEnv(const char* str);
		virtual void PushConstStringValue(GLSL::GLSLSymbolValue * lvalp, const char* str);
		virtual int PushValue(GLSL::GLSLSymbolValue * lvalp);
		virtual void PushSpace();
		virtual void PushNextLine();
		virtual int DecodeSymbol(GLSL::GLSLSymbolValue * lvalp, int retValue);
	};

	class ShaderTranslator : public GLSLParserEnv
	{
	public:
		ShaderTranslator(const char* str)
			: GLSLParserEnv(str)
		{}
		xhn::map<xhn::string, xhn::string> m_translationTable;
		virtual int PushValue(GLSL::GLSLSymbolValue * lvalp);
	};

	int yylex(GLSLSymbolValue *lvalp, GLSLParserEnv* e);
	void yyerror(GLSLParserEnv* e, const char* error);
	void test();
}

#endif