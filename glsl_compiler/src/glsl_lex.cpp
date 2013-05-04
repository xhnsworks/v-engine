#include "glsl_lex.h"

void GLSL::GLSLLexStatus::Init()
{
	remainder = 1024;
	stringSize = 0;
	curtExpType = GLSL::EmptyType;
	str = mbuf;
	str[0] = 0x00;
}

void GLSL::GLSLParserEnv::PushConstStringValue(GLSL::GLSLSymbolValue * lvalp, const char* str)
{
}
int GLSL::GLSLParserEnv::PushValue(GLSL::GLSLSymbolValue * lvalp)
{
	if (m_lexStatus.stringSize) {
		if (m_lexStatus.curtExpType == GLSL::NumericalType) {
			m_output += m_lexStatus.mbuf;
			/// 这是一个数值型
			return GLSL::NumericalType;
		}
		else if (m_lexStatus.curtExpType == GLSL::SymbolType) {
			m_output += m_lexStatus.mbuf;
			/// 这是一个符号型，有可能是关键字或函数名或变量名神马的
			return GLSL::SymbolType;
		}
		else if (m_lexStatus.curtExpType == GLSL::TextType) {
			int offs = snprintf(m_lexStatus.str, m_lexStatus.remainder, " ");
			m_lexStatus.str += offs;
			m_lexStatus.remainder -= offs;
			m_lexStatus.stringSize += offs;
			return 0;
		}
		else if (m_lexStatus.curtExpType == GLSL::EmptyType) {
			// 什麼也不做
			return 0;
		}
	}
	return -1;
}
void GLSL::GLSLParserEnv::PushSpace()
{
	m_lexStatus.curtExpType = GLSL::EmptyType;
    m_output += " ";
}
void GLSL::GLSLParserEnv::PushNextLine()
{
    m_output += "\n";
}
int GLSL::GLSLParserEnv::DecodeSymbol(GLSL::GLSLSymbolValue * lvalp, int retValue)
{
	if (m_lexStatus.curtExpType != GLSL::EmptyType) {
		int ret = PushValue(lvalp);
		if (ret == -1)
			return 0;
		else if (ret > 0)
			return ret;
		else {
			PushConstStringValue(lvalp, m_lexStatus.mbuf);
			return GLSL::TextType;
		}
	}
	else {
		m_charCount++;
		switch (retValue)
		{
		case __DOT:
			m_output += ".";
			break;
		case __SEMICOLON:
			m_output += ";";
			break;
		case __EQ_OP:
			m_output += "==";
			break;
		case __EQUAL:
			m_output += "=";
			break;
		case __LE_OP:
		case __LEFT_ANGLE:
			m_output += "<";
			break;
		case __GE_OP:
		case __RIGHT_ANGLE:
			m_output += ">";
			break;
		case __COLON:
			m_output += ":";
			break;
		case __COMMA:
			m_output += ",";
			break;
		case __PLUS:
			m_output += "+";
			break;
		case __DASH:
			m_output += "-";
			break;
		case __STAR:
			m_output += "*";
			break;
		case __SLASH:
			m_output += "/";
			break;
		case __LEFT_PAREN:
			m_output += "(";
			break;
		case __RIGHT_PAREN:
			m_output += ")";
			break;
		case __LEFT_BRACKET:
			m_output += "[";
			break;
		case __RIGHT_BRACKET:
			m_output += "]";
			break;
		case __LEFT_BRACE:
			m_output += "{";
			break;
		case __RIGHT_BRACE:
			m_output += "}";
			break;
		}
		return retValue;
	}
}

int GLSL::ShaderTranslator::PushValue(GLSL::GLSLSymbolValue * lvalp)
{
	if (m_lexStatus.stringSize) {
		if (m_lexStatus.curtExpType == GLSL::NumericalType) {
			m_output += m_lexStatus.mbuf;
			/// 这是一个数值型
			return GLSL::NumericalType;
		}
		else if (m_lexStatus.curtExpType == GLSL::SymbolType) {
			xhn::map<xhn::string, xhn::string>::iterator iter = m_translationTable.find(m_lexStatus.mbuf);
			if (iter != m_translationTable.end())
				m_output += iter->second;
			else
				m_output += m_lexStatus.mbuf;
			/// 这是一个符号型，有可能是关键字或函数名或变量名神马的
			return GLSL::SymbolType;
		}
		else if (m_lexStatus.curtExpType == GLSL::TextType) {
			int offs = snprintf(m_lexStatus.str, m_lexStatus.remainder, " ");
			m_lexStatus.str += offs;
			m_lexStatus.remainder -= offs;
			m_lexStatus.stringSize += offs;
			return 0;
		}
		else if (m_lexStatus.curtExpType == GLSL::EmptyType) {
			// 什麼也不做
			return 0;
		}
	}
	return -1;
}

int _yylex (GLSL::GLSLSymbolValue * lvalp, GLSL::GLSLParserEnv* e)
{
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
			if (e->m_lexStatus.curtExpType == GLSL::NumericalType) {
				if (e->m_text[e->m_charCount] == 'f') {
				    /// nothing
				}
				else {
					return 0;
				}
			}
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
				if (e->m_lexStatus.stringSize) {
				    ///return _push_value(lvalp, e);
					int ret = e->PushValue(lvalp);
					e->PushNextLine();
					return ret;
				}
				else {
					e->PushNextLine();
				    continue;
				}
			case '.':
				if (e->m_lexStatus.curtExpType == GLSL::EmptyType) {
					char next_char = e->m_text[e->m_charCount + 1];
					if (next_char) {
						if (next_char <= '9' && next_char >= '0') {
							e->m_lexStatus.curtExpType = GLSL::NumericalType;
						}
						else {
                            ///return _cale_sym_proc(lvalp, e, GLSL::_DOT);
							return e->DecodeSymbol(lvalp, __DOT);
						}
					}
					else
						///return _cale_sym_proc(lvalp, e, GLSL::_DOT);
						return e->DecodeSymbol(lvalp, __DOT);
				}
				else if (e->m_lexStatus.curtExpType == GLSL::SymbolType) {
                    ///return _cale_sym_proc(lvalp, e, GLSL::_DOT);
					return e->DecodeSymbol(lvalp, __DOT);
				}
				break;
			case ' ':
				if (e->m_lexStatus.curtExpType != GLSL::EmptyType) {
					return e->PushValue(lvalp);
				}
				e->PushSpace();
			case '\r': {
				if (e->m_lexStatus.curtExpType != GLSL::EmptyType) {
					///int ret = _push_value(lvalp, e);
					int ret = e->PushValue(lvalp);
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
			case '"': {
				if (e->m_lexStatus.curtExpType == GLSL::EmptyType)
					e->m_lexStatus.curtExpType = GLSL::TextType;
				else if (e->m_lexStatus.curtExpType == GLSL::TextType) {
					///_push_const_string_value(e, lvalp, e->m_lexStatus.mbuf);
					e->PushConstStringValue(lvalp, e->m_lexStatus.mbuf);
					e->m_charCount++;
					return GLSL::TextType;
				}
				else
					return 0;
					  }
					  break;
			case '+':
				///return _cale_sym_proc(lvalp, e, GLSL::_PLUS);
				return e->DecodeSymbol(lvalp, __PLUS);
			case '-':
				///return _cale_sym_proc(lvalp, e, GLSL::_DASH);
				return e->DecodeSymbol(lvalp, __DASH);
			case '*':
				///return _cale_sym_proc(lvalp, e, GLSL::_STAR);
				return e->DecodeSymbol(lvalp, __STAR);
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
				///return _cale_sym_proc(lvalp, e, GLSL::_SLASH);
				return e->DecodeSymbol(lvalp, __SLASH);
			case '(':
				///return _cale_sym_proc(lvalp, e, GLSL::_LEFT_PAREN);
				return e->DecodeSymbol(lvalp, __LEFT_PAREN);
			case ')':
				///return _cale_sym_proc(lvalp, e, GLSL::_RIGHT_PAREN);
				return e->DecodeSymbol(lvalp, __RIGHT_PAREN);
			case '[':
				///return _cale_sym_proc(lvalp, e, GLSL::_LEFT_BRACKET);
				return e->DecodeSymbol(lvalp, __LEFT_BRACKET);
			case ']':
				///return _cale_sym_proc(lvalp, e, GLSL::_RIGHT_BRACKET);
				return e->DecodeSymbol(lvalp, __RIGHT_BRACKET);
			case '{':
				///SymbolStack_push(&e->sym_stack);
				///return _cale_sym_proc(lvalp, e, GLSL::_LEFT_BRACE);
				return e->DecodeSymbol(lvalp, __LEFT_BRACE);
			case '}':
				///SymbolStack_pop(&e->sym_stack);
				///return _cale_sym_proc(lvalp, e, GLSL::_RIGHT_BRACE);
				return e->DecodeSymbol(lvalp, __RIGHT_BRACE);
			case '=': {
				char next_char = e->m_text[e->m_charCount + 1];
				if (next_char) {
					if (next_char == '=') {
						if (e->m_lexStatus.curtExpType == GLSL::EmptyType)
							e->m_charCount++;
						///return _cale_sym_proc(lvalp, e, GLSL::_EQ_OP);
						return e->DecodeSymbol(lvalp, __EQ_OP);
					}
					else
						///return _cale_sym_proc(lvalp, e, GLSL::_EQUAL);
						return e->DecodeSymbol(lvalp, __EQUAL);
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
						///return _cale_sym_proc(lvalp, e, GLSL::_LE_OP);
						return e->DecodeSymbol(lvalp, __LE_OP);
					}
					else
						///return _cale_sym_proc(lvalp, e, GLSL::_LEFT_ANGLE);
						return e->DecodeSymbol(lvalp, __LEFT_ANGLE);
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
						///return _cale_sym_proc(lvalp, e, GLSL::_GE_OP);
						return e->DecodeSymbol(lvalp, __GE_OP);
					}
					else
						///return _cale_sym_proc(lvalp, e, GLSL::_RIGHT_ANGLE);
						return e->DecodeSymbol(lvalp, __RIGHT_ANGLE);
				}
				else
					return 0;
					  }
			case ':':
				///return _cale_sym_proc(lvalp, e, GLSL::_COLON);
				return e->DecodeSymbol(lvalp, __COLON);
			case ',':
				///return _cale_sym_proc(lvalp, e, GLSL::_COMMA);
				return e->DecodeSymbol(lvalp, __COMMA);
			case ';':
				///return _cale_sym_proc(lvalp, e, GLSL::_SEMICOLON);
				return e->DecodeSymbol(lvalp, __SEMICOLON);
			}
		}
		if (e->m_text[e->m_charCount] < 32) {
			printf("invalid characters\n");
			return -1;
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
	const char* str =
		/**
	"#version 130\n"
	"///shader///\n"
	"///attribute///\n"
	"attribute vec4 Position;\n"
	"attribute vec2 TexCoord;\n"
	"attribute vec4 Color;\n"
	"attribute vec3 Normal;\n"
	"attribute vec3 Tangent;\n"
	"attribute vec3 Binormal;\n"
	"///uniform///\n"
	"///varying///\n"
	**/
	"varying vec4 vPosition;\n"
	"varying vec2 vTexCoord;\n"
	"varying vec4 vColor;\n"
	"varying vec3 vNormal;\n"
	"varying vec3 vTangent;\n"
	"varying vec3 vBinormal;\n"
	"///function///\n"
	/**
	"void VertexProc(){\n"
	**/
	/**
	"	vec4 pos = vec4(Position.xyz, 1.0);\n"
	"	gl_Position = pos;\n"
	**/
	"    vPosition = pos;\n"
	"    vTexCoord = TexCoord;\n"
	"    vColor = Color;\n"
	"    vNormal = Normal;\n"
	"    vTangent = Tangent;\n"
	"    vBinormal = Binormal;\n"
	/**
	"}\n"
	"///main///\n"
	"void main(void)\n"
	"{\n"
	"	VertexProc(  );\n"
	"}\n"
	**/;

	ShaderTranslator e(str);
	e.m_translationTable.insert(xhn::make_pair(xhn::string("vPosition"), xhn::string("PPP")));
	e.m_translationTable.insert(xhn::make_pair(xhn::string("vTexCoord"), xhn::string("TTT")));
	while (yylex(NULL, &e)) {
	}
	printf("%s", e.m_output.c_str());
}