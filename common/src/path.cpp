#include "pch.h"
#include "path.h"
#ifdef __MINGW32__
#include <dir.h>
#elif defined(_MSC_VER)
#include <io.h>
#include <string.h>
#endif
#ifndef __APPLE__
#include <direct.h>
#else
#include <sys/dir.h>
#include <sys/dirent.h>
#endif
void GetAllFileNamesInDir(const char* dir, const char* filter_rule, EStringArray* result)
{
    struct _finddata_t fd;
    array_n(*result) = 0;

    EString fulldir = EString_new(dir);
    uint size = EString_size(fulldir);
    if (size)
    {
        char end_char = fulldir[size - 1];
        if ('\\' != end_char && '/' != end_char)
        {
            EString tmp0 = EString_add(fulldir, "\\");
            EString tmp1 = EString_add(tmp0, filter_rule);
            EString_delete(fulldir);
            EString_delete(tmp0);
            fulldir = tmp1;
        }
        else
        {
            EString tmp0 = EString_add(fulldir, filter_rule);
            EString_delete(fulldir);
            fulldir = tmp0;
        }
    }

    long h = _findfirst(fulldir, &fd);
    int ret = 0;
    while (h != -1L && !ret)
    {
        if (strcmp(".", fd.name) != 0 && strcmp("..", fd.name) != 0)
        {
            EString filename = EString_new(fd.name);
            apush(*result, filename);
        }
        ret = _findnext(h, &fd);
    }
    _findclose(h);
    EString_delete(fulldir);
}
