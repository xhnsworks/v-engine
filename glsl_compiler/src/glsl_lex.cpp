#include "glsl_lex.h"

void GLSL::GLSLLexStatus::Init()
{
	remainder = 1024;
	stringSize = 0;
	curtExpType = GLSL::EmptyType;
	str = mbuf;
	str[0] = 0x00;
}

void _push_const_string_value(GLSL::GLSLParserEnv* e, GLSL::GLSLSymbolValue * lvalp, const char* str)
{
}
int _push_value(GLSL::GLSLSymbolValue * lvalp, GLSL::GLSLParserEnv* e)
{
	if (e->m_lexStatus.stringSize) {
		if (e->m_lexStatus.curtExpType == GLSL::NumericalType) {
			/// 这是一个数值型
			return GLSL::NumericalType;
		}
		else if (e->m_lexStatus.curtExpType == GLSL::SymbolType) {
            /// 这是一个符号型，有可能是关键字或函数名或变量名神马的
			return GLSL::SymbolType;
		}
		else if (e->m_lexStatus.curtExpType == GLSL::TextType) {
			int offs = snprintf(e->m_lexStatus.str, e->m_lexStatus.remainder, " ");
			e->m_lexStatus.str += offs;
			e->m_lexStatus.remainder -= offs;
			e->m_lexStatus.stringSize += offs;
			return 0;
		}
		else if (e->m_lexStatus.curtExpType == GLSL::EmptyType) {
			// 什麼也不做
			return 0;
		}
	}
	return -1;
}

int _cale_sym_proc(GLSL::GLSLSymbolValue * lvalp, GLSL::GLSLParserEnv* e, int retValue)
{
	if (e->m_lexStatus.curtExpType != GLSL::EmptyType) {
		int ret = _push_value(lvalp, e);
		if (ret == -1)
			return 0;
		else if (ret > 0)
			return ret;
		else {
			_push_const_string_value(e, lvalp, e->m_lexStatus.mbuf);
			return GLSL::TextType;
		}
	}
	else {
		e->m_charCount++;
		return retValue;
	}
}

int _yylex (GLSL::GLSLSymbolValue * lvalp, GLSL::GLSLParserEnv* e)
{
	/**
	remainder = 1024;
	string_size = 0;
	curtSymType = empty;
	str = mbuf;
	**/
	e->m_lexStatus.Init();
	e->m_lexStatus.remainder--;
START:
	while (e->m_text[e->m_charCount])
	{
		if ( (e->m_text[e->m_charCount] >= 'A' &&
			e->m_text[e->m_charCount] <= 'Z') ||
			(e->m_text[e->m_charCount] >= 'a' &&
			e->m_text[e->m_charCount] <= 'z') )
		{
			if (e->m_lexStatus.curtExpType == GLSL::EmptyType)
				e->m_lexStatus.curtExpType = GLSL::SymbolType;
			if (e->m_lexStatus.curtExpType != GLSL::SymbolType &&
				e->m_lexStatus.curtExpType != GLSL::TextType)
				return 0;
		}
		else if (e->m_text[e->m_charCount] >= '0' &&
			e->m_text[e->m_charCount] <= '9')
		{
			if (e->m_lexStatus.curtExpType == GLSL::EmptyType)
				e->m_lexStatus.curtExpType = GLSL::NumericalType;
			if (e->m_lexStatus.curtExpType != GLSL::NumericalType &&
				e->m_lexStatus.curtExpType != GLSL::SymbolType &&
				e->m_lexStatus.curtExpType != GLSL::TextType)
				return 0;
		}
		else
		{
			switch(e->m_text[e->m_charCount]) {
			case '\n':
				e->m_lineCount++;
				e->m_charCount++;
				if (e->m_lexStatus.stringSize)
				    return _push_value(lvalp, e);
				else
				    continue;
			case ' ':
			case '\r': {
				if (e->m_lexStatus.curtExpType != GLSL::EmptyType) {
					int ret = _push_value(lvalp, e);
					if (ret == -1)
						return 0;
					else if (ret > 0) {
						e->m_charCount++;
						return ret;
					}
				}
				else {
					e->m_charCount++;
					continue;
				}
					   }
					   break;
			/**
			case '"': {
				if (e->m_lexStatus.curtExpType == GLSL::EmptyType)
					e->m_lexStatus.curtExpType = GLSL::TextType;
				else if (e->m_lexStatus.curtExpType == GLSL::TextType) {
					_push_const_string_value(e, lvalp, e->m_lexStatus.mbuf);
					e->m_charCount++;
					return _text;
				}
				else
					return 0;
					  }
					  break;
			**/
			case '+':
				return _cale_sym_proc(lvalp, e, GLSL::_PLUS);
			case '-':
				return _cale_sym_proc(lvalp, e, GLSL::_DASH);
			case '*':
				return _cale_sym_proc(lvalp, e, GLSL::_STAR);
			case '/':
				{
					/// skip the annotation...
					char next_char = e->m_text[e->m_charCount + 1];
					if (next_char) {
						if (next_char == '/') {
							e->m_charCount++;
							while (e->m_text[e->m_charCount] && e->m_text[e->m_charCount] != '\n') {
								e->m_charCount++;
							}
							goto START;
						}
					}
				}
				return _cale_sym_proc(lvalp, e, GLSL::_SLASH);
			case '(':
				return _cale_sym_proc(lvalp, e, GLSL::_LEFT_PAREN);
			case ')':
				return _cale_sym_proc(lvalp, e, GLSL::_RIGHT_PAREN);
			case '[':
				return _cale_sym_proc(lvalp, e, GLSL::_LEFT_BRACKET);
			case ']':
				return _cale_sym_proc(lvalp, e, GLSL::_RIGHT_BRACKET);
			case '{':
				///SymbolStack_push(&e->sym_stack);
				return _cale_sym_proc(lvalp, e, GLSL::_LEFT_BRACE);
			case '}':
				///SymbolStack_pop(&e->sym_stack);
				return _cale_sym_proc(lvalp, e, GLSL::_RIGHT_BRACE);
			case '=': {
				char next_char = e->m_text[e->m_charCount + 1];
				if (next_char) {
					if (next_char == '=') {
						if (e->m_lexStatus.curtExpType == GLSL::EmptyType)
							e->m_charCount++;
						return _cale_sym_proc(lvalp, e, GLSL::_EQ_OP);
					}
					else
						return _cale_sym_proc(lvalp, e, GLSL::_EQUAL);
				}
				else
					return 0;
					  }
			case '<': {
				char next_char = e->m_text[e->m_charCount + 1];
				if (next_char) {
					if (next_char == '=') {
						if (e->m_lexStatus.curtExpType == GLSL::EmptyType)
							e->m_charCount++;
						return _cale_sym_proc(lvalp, e, GLSL::_LE_OP);
					}
					else
						return _cale_sym_proc(lvalp, e, GLSL::_LEFT_ANGLE);
				}
				else
					return 0;
					  }
			case '>': {
				char next_char = e->m_text[e->m_charCount + 1];
				if (next_char) {
					if (next_char == '=') {
						if (e->m_lexStatus.curtExpType == GLSL::EmptyType)
							e->m_charCount++;
						return _cale_sym_proc(lvalp, e, GLSL::_GE_OP);
					}
					else
						return _cale_sym_proc(lvalp, e, GLSL::_RIGHT_ANGLE);
				}
				else
					return 0;
					  }
			case ':':
				return _cale_sym_proc(lvalp, e, GLSL::_COLON);
			case ',':
				return _cale_sym_proc(lvalp, e, GLSL::_COMMA);
			case ';':
				return _cale_sym_proc(lvalp, e, GLSL::_SEMICOLON);
			}
		}

		int offs = snprintf(e->m_lexStatus.str, e->m_lexStatus.remainder, "%c", e->m_text[e->m_charCount]);
		e->m_lexStatus.str += offs;
		e->m_lexStatus.remainder -= offs;
		e->m_lexStatus.stringSize += offs;
		e->m_charCount++;
	}
	return 0;
}

int GLSL::yylex (GLSL::GLSLSymbolValue * lvalp, GLSL::GLSLParserEnv* e)
{
	int ret = _yylex (lvalp, e);
	return ret;
}

void GLSL::yyerror(GLSLParserEnv* e, const char* error)
{
}

GLSL::GLSLParserEnv::GLSLParserEnv(const char* str)
: m_text(str)
, m_charCount(0)
, m_lineCount(0)
{
    m_lexStatus.Init();
}

void GLSL::test()
{
	char* str =
	"#version 130\n"
	"///shader///\n"
	"///attribute///\n"
	"attribute vec4 Position;\n"
	"attribute vec2 TexCoord;"
	"attribute vec4 Color;\n"
	"attribute vec3 Normal;\n"
	"attribute vec3 Tangent;\n"
	"attribute vec3 Binormal;\n"
	"///uniform///\n"
	"///varying///\n"
	"varying vec4 vPosition;\n"
	"varying vec2 vTexCoord;\n"
	"varying vec4 vColor;\n"
	"varying vec3 vNormal;\n"
	"varying vec3 vTangent;\n"
	"varying vec3 vBinormal;\n"
	"///function///\n"
	"void VertexProc(){\n"
	"	vec4 pos = vec4(Position.xyz, 1.0);\n"
	"	gl_Position = pos;\n"
	"	vPosition = pos;\n"
	"	vTexCoord = TexCoord;\n"
	"	vColor = Color;\n"
	"	vNormal = Normal;\n"
	"	vTangent = Tangent;\n"
	"	vBinormal = Binormal;\n"
	"}\n"
	"///main///\n"
	"void main(void)\n"
	"{\n"
	"	VertexProc(  );\n"
	"}\n";

	GLSLParserEnv e(str);
	while (yylex(NULL, &e)) {
	}
}