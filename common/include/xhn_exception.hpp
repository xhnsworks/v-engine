//
//  xhn_exception.hpp
//  ecg
//
//  Created by 徐 海宁 on 13-5-3.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

#ifndef ecg_xhn_exception_hpp
#define ecg_xhn_exception_hpp

#include <exception>
#include "rtti.hpp"
#include "xhn_string.hpp"
#include "elog.h"
class Exception : public std::exception
{
    DeclareRootRTTI;
public:
    const char* m_file;
    euint32 m_line;
    xhn::string m_msg;
    xhn::string m_what;
public:
    Exception(const char* file, euint32 line, xhn::string msg)
    : m_file(file)
    , m_line(line)
    , m_msg(msg)
    {
		elog("error: %s", what());
	}
    const char* what() {
        const RTTI* rtti = GetRTTI();
        
        m_what = "VEngineError:";
        m_what += rtti->GetTypeName();
        m_what += m_msg;
        
        return m_what.c_str();
    }
};
class FunctionException : public Exception
{
	DeclareRTTI;
public:
	FunctionException(const char* file, euint32 line, xhn::string msg)
		: Exception(file, line, msg)
	{}
};
/// 函数的输入参数为非法
class FunctionArgumentException : public FunctionException
{
    DeclareRTTI;
public:
    FunctionArgumentException(const char* file, euint32 line, xhn::string msg)
    : FunctionException(file, line, msg)
    {}
};
/// 函数执行过程的异常
class FunctionExecutionException : public FunctionException
{
    DeclareRTTI;
public:
    FunctionExecutionException(const char* file, euint32 line, xhn::string msg)
    : FunctionException(file, line, msg)
    {}
};
/// 函数结果不是期望值
class FunctionResultException : public FunctionException
{
    DeclareRTTI;
public:
    FunctionResultException(const char* file, euint32 line, xhn::string msg)
    : FunctionException(file, line, msg)
    {}
};

class ObjectException : public Exception
{
	DeclareRTTI;
public:
	ObjectException(const char* file, euint32 line, xhn::string msg)
		: Exception(file, line, msg)
	{}
};
    
/// switch case里的枚举值是无效的
class InvalidEnumerationException : public FunctionExecutionException
{
    DeclareRTTI;
public:
    InvalidEnumerationException(const char* file, euint32 line, xhn::string msg)
    : FunctionExecutionException(file, line, msg)
    {}
};

class ObjectUninitializedException : public ObjectException
{
	DeclareRTTI;
public:
	ObjectUninitializedException(const char* file, euint32 line, xhn::string msg)
		: ObjectException(file, line, msg)
	{}
};
#define VEngineExce(e, m) throw e(__FILE__, __LINE__, (m))
#endif
