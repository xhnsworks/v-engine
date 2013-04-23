#include "common.h"
#include "etypes.h"
#include "glsl_lex.h"

void GLSL::GLSLLexStatus::Init()
{
	remainder = 1024;
	stringSize = 0;
	curtExpType = GLSL::EmptyType;
	str = mbuf;
	str[0] = 0x00;
}

int _push_value(GLSL::GLSLSymbolValue * lvalp, GLSL::GLSLParserEnv* e)
{
	return 0;
}
void _push_const_string_value(GLSL::GLSLParserEnv* e, GLSL::GLSLSymbolValue * lvalp, const char* str)
{
}
int _cale_sym_proc(GLSL::GLSLSymbolValue * lvalp, GLSL::GLSLParserEnv* e, int sym)
{
	return sym;
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
				return _cale_sym_proc(lvalp, e, GLSL::PLUS);
			case '-':
				return _cale_sym_proc(lvalp, e, GLSL::DASH);
			case '*':
				return _cale_sym_proc(lvalp, e, GLSL::STAR);
			case '/':
				return _cale_sym_proc(lvalp, e, GLSL::SLASH);
			case '(':
				return _cale_sym_proc(lvalp, e, GLSL::LEFT_PAREN);
			case ')':
				return _cale_sym_proc(lvalp, e, GLSL::RIGHT_PAREN);
			case '[':
				return _cale_sym_proc(lvalp, e, GLSL::LEFT_BRACKET);
			case ']':
				return _cale_sym_proc(lvalp, e, GLSL::RIGHT_BRACKET);
			case '{':
				///SymbolStack_push(&e->sym_stack);
				return _cale_sym_proc(lvalp, e, GLSL::LEFT_BRACE);
			case '}':
				///SymbolStack_pop(&e->sym_stack);
				return _cale_sym_proc(lvalp, e, GLSL::RIGHT_BRACE);
			case '=': {
				char next_char = e->m_text[e->m_charCount + 1];
				if (next_char) {
					if (next_char == '=') {
						if (e->m_lexStatus.curtExpType == GLSL::EmptyType)
							e->m_charCount++;
						return _cale_sym_proc(lvalp, e, GLSL::EQ_OP);
					}
					else
						return _cale_sym_proc(lvalp, e, GLSL::EQUAL);
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
						return _cale_sym_proc(lvalp, e, GLSL::LE_OP);
					}
					else
						return _cale_sym_proc(lvalp, e, GLSL::LEFT_ANGLE);
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
						return _cale_sym_proc(lvalp, e, GLSL::GE_OP);
					}
					else
						return _cale_sym_proc(lvalp, e, GLSL::RIGHT_ANGLE);
				}
				else
					return 0;
					  }
			case ':':
				return _cale_sym_proc(lvalp, e, GLSL::COLON);
			case ',':
				return _cale_sym_proc(lvalp, e, GLSL::COMMA);
			case ';':
				return _cale_sym_proc(lvalp, e, GLSL::SEMICOLON);
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

