#ifndef GL_UTILITY_H
#define GL_UTILITY_H
#include <common.h>
#include <etypes.h>
#include <GL/glew.h>
#include <GL/gl.h>
extern const char* convertInternalFormatToString(GLenum format);
extern const char* getRenderbufferParameters(GLuint id);
extern const char* getTextureParameters(GLuint id);
extern void printFramebufferInfo();
extern bool checkFramebufferStatus();
#endif
