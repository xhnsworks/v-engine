/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：shader_log.h
* 文件描述：着色器的log打印器
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef SHADER_LOG_H
#define SHADER_LOG_H
#include "common.h"
#include "etypes.h"
#define SHADER_LOG_BUFFER_SIZE 4096 * 4
typedef enum _shader_log_type
{
    StdPassLog,
    LightingPassLog,
    PostPassLog,
} ShaderLogType;
API_EXPORT void ShaderLog_Init();
API_EXPORT void ShaderLog_Release();
extern char g_shader_log_buffer[SHADER_LOG_BUFFER_SIZE];
API_EXPORT void ShaderLog_write(ShaderLogType log_type);
#define slog(t, fmt,...) { snprintf(g_shader_log_buffer,SHADER_LOG_BUFFER_SIZE - 2,fmt,##__VA_ARGS__); ShaderLog_write(t); }
#endif // SHADER_LOG_H
