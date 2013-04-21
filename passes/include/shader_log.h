/*
* Copyright (c) 2013, �캣��
* All rights reserved.
*
* �ļ����ƣ�shader_log.h
* �ļ���������ɫ����log��ӡ��
*
* ��ǰ�汾��0.1
* ��    �ߣ��캣��
* ������ڣ�2013��1��27��
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
