#ifndef VLANG_H
#define VLANG_H
#include "common.h"
#include "etypes.h"
#include "xhn_vector.hpp"
#include "xhn_pair.hpp"
#include "xhn_string.hpp"
#include "parser_env.h"
API_EXPORT void vlang_run(const char* buffer);

class CFunctionConfig
{
private:
	typedef xhn::vector< xhn::pair<xhn::string, ValueType> > Params;
	Params m_input_params;
	Params m_output_params;
	Command m_func;
public:
	CFunctionConfig(Command func)
		: m_func(func)
	{}
	inline void AddInputParam(const xhn::string& name, ValueType type) {
		m_input_params.push_back(xhn::make_pair(name, type));
	}
	inline void AddOutputParam(const xhn::string& name, ValueType type) {
		m_output_params.push_back(xhn::make_pair(name, type));
	}
	void _AddParam(ParserEnv* e, const Params& params);
	void AttachToParseEnv(ParserEnv* e, const xhn::string& funcName);
	void AttachToParseEnvTest();
};
#endif