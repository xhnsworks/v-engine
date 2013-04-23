#ifndef GLSL_LEX_H
#define GLSL_LEX_H
namespace GLSL
{
	class GLSLParserEnv
	{
	public:
		const char* m_text;
		int m_charCount;
	};
	int _glsl_lex();
}

#endif