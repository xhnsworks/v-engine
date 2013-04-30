/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：gl_utility.h
* 文件描述：opengl的实用工具
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef GL_UTILITY_H
#define GL_UTILITY_H
///#include <GL/gl.h>
#include "common.h"
#include "etypes.h"

API_EXPORT const char* convertInternalFormatToString(GLenum format);
API_EXPORT const char* getRenderbufferParameters(GLuint id);
API_EXPORT const char* getTextureParameters(GLuint id);
API_EXPORT void printFramebufferInfo();
API_EXPORT bool checkFramebufferStatus();
API_EXPORT void _error_proc(const char* _file, euint _line);
#define ERROR_PROC _error_proc(__FILE__, __LINE__)
API_EXPORT void _shader_log(const char* _file, euint _line, uint32 _id);
API_EXPORT void _program_log(const char* _file, euint _line, uint32 _id);
#define shader_log(id) _shader_log(__FILE__, __LINE__, id)
#define program_log(id) _program_log(__FILE__, __LINE__, id)
#endif
