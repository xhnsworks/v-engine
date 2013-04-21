#ifndef GL_UTILITY_H
#define GL_UTILITY_H
#include <GL/glew.h>
#include <GL/gl.h>
#include "common.h"
#include "etypes.h"

API_EXPORT const char* convertInternalFormatToString(GLenum format);
API_EXPORT const char* getRenderbufferParameters(GLuint id);
API_EXPORT const char* getTextureParameters(GLuint id);
API_EXPORT void printFramebufferInfo();
API_EXPORT bool checkFramebufferStatus();
API_EXPORT void _error_proc(const char* _file, uint _line);
#define ERROR_PROC _error_proc(__FILE__, __LINE__)
API_EXPORT void _shader_log(const char* _file, uint _line, uint32 _id);
API_EXPORT void _program_log(const char* _file, uint _line, uint32 _id);
#define shader_log(id) _shader_log(__FILE__, __LINE__, id)
#define program_log(id) _program_log(__FILE__, __LINE__, id)
#endif
