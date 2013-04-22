#ifdef _MSC_VER
#pragma warning(disable:4819)
#endif
#ifndef COMMON_H
#define COMMON_H
#define VPRINT elog
typedef void* Iterator;

#define STD_NAMESPACE xhn
/**
#ifdef  __GNUC__
#if __GNUC__ >= 3 // GCC3.0“‘…œ

#ifdef  _MSC_VER£®∑«VC±‡“Î∆˜∫‹∂‡µÿ∑Ω“≤”–∂®“Â£©
#if _MSC_VER >=1000 // VC++4.0“‘…œ
#if _MSC_VER >=1100 // VC++5.0“‘…œ
#if _MSC_VER >=1200 // VC++6.0“‘…œ
#if _MSC_VER >=1300 // VC2003“‘…œ
#if _MSC_VER >=1400 // VC2005“‘…œ

#ifdef  __BORLANDC__

UNIX
#ifdef  __unix
or
#ifdef  __unix__

#ifdef  __linux
or
#ifdef  __linux__

#ifdef  __FreeBSD__
#ifdef  __NetBSD__

#ifdef  _WIN32£®ªÚ’ﬂWIN32£©
#ifdef  _WIN64

#ifdef  _WINDOWS
#ifdef  _CONSOLE

#if (WINVER >= 0x030a) // Windows 3.1“‘…œ
#if (WINVER >= 0x0400) // Windows 95/NT 4.0“‘…œ
#if (WINVER >= 0x0410) // Windows 98“‘…œ
#if (WINVER >= 0x0500) // Windows Me/2000“‘…œ
#if (WINVER >= 0x0501) // Windows XP“‘…œ
#if (WINVER >= 0x0600) // Windows Vista“‘…œ

#ifdef  _WIN32_WINDOWS
#if (_WIN32_WINDOWS >= 0x0400) // Windows 95“‘…œ
#if (_WIN32_WINDOWS >= 0x0410) // Windows 98“‘…œ
#if (_WIN32_WINDOWS >= 0x0500) // Windows Me“‘…œ

#if (_WIN32_WINNT  >= 0x0500) // Windows 2000“‘…œ
#if (_WIN32_WINNT  >= 0x0501) // Windows XP“‘…œ
#if (_WIN32_WINNT  >= 0x0600) // Windows Vista“‘…œ

#ifdef  _WIN32_WCE

#ifdef  __CYGWIN__

#ifdef  __CYGWIN32__

#ifdef  __MINGW32__
**/

#ifdef __GNUC__
#ifndef unlikely
    #define unlikely(x) __builtin_expect(!!(x), 0)
#endif
#ifndef likely
    #define likely(x) __builtin_expect(!!(x), 1)
#endif
#else
#define unlikely(x) x
#define likely(x) x
#endif

#define to_ptr(o) *((vptr*)&o)
#define to_radian(a) a / 180.0 * 3.1415926535897932
#define to_angle(r) r / 3.1415926535897932 * 180.0
#ifndef _MSC_VER
///#define to_ShaderBuffer(__tsb__) ({ShaderBuffer __sb__; __sb__ = (struct _shader_buffer*)__tsb__; __sb__;})
#define to_ShaderBuffer(__tsb__) (ShaderBuffer)__tsb__
#else
#define to_ShaderBuffer(__tsb__) ToShaderBuffer(__tsb__)
#endif


#ifdef __GNUC__
#define _INLINE_ static inline
#define FORCE_ALIGN_ARG_POINTER __attribute__ ((force_align_arg_pointer))
#else
#define _INLINE_ static inline
#define FORCE_ALIGN_ARG_POINTER
#define snprintf sprintf_s
#endif

#ifndef __APPLE__
#define BIT_WIDTH 32
#else
#define BIT_WIDTH 64
#endif

#define USE_SSE

#define SELF (*_self.self)

#ifdef DLL_EXPORT
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT
#endif

#define GLSL_VERSION "#version 130\n"

#define NEG_NAN 0xffc00000
#define POS_NAN 0x7fc00000

#ifdef __GNUC__
#define SafeFOpen fopen
#elif defined _MSC_VER
#include <stdio.h>
#include <stdlib.h>
inline FILE* SafeFOpen(const char* path, const char* mode)
{
	FILE* ret = NULL;
	fopen_s(&ret, path, mode);
	return ret;
}
#endif

#define ALIGN_FLAG(n) __declspec(align(n))

#ifdef __cplusplus
#define ENEW new(__FILE__, __LINE__)
#define EDELETE delete(__FILE__, __LINE__)
#define ENEW_ARRAY new
#define EDELETE_ARRAY delete
#endif

#ifndef __APPLE__
#include <pthread.h>
#endif
#endif
