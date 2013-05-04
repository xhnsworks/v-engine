#ifndef XHN_STRING_HPP
#define XHN_STRING_HPP
#include "common.h"
#include "etypes.h"
#include "emem.h"
#include "xhn_utility.hpp"
#include <string.h>
#include "xhn_string_base.hpp"

namespace xhn
{
struct FStrCmpProc {
    int operator() (const char* str0, const char* str1) const {
        return strcmp(str0, str1);
    }
};
struct FDefaultStrProc {
    const char* operator() () const {
        return "";
    }
};
typedef string_base<char, FStrCmpProc, FDefaultStrProc> string;

struct FWStrCmpProc {
    int operator() (const wchar_t* str0, const wchar_t* str1) const {
        int count = 0;
        while (str0[count]) {
            if (!str1[count])
                return 1;
            if (str0[count] < str1[count])
                return -1;
            else if (str0[count] > str1[count])
                return 1;
            count++;
        }
        if (str1[count])
            return -1;
        return 0;
    }
};
struct FDefaultWStrProc {
    const wchar_t* operator() () const {
        return L"";
    }
};
typedef string_base<wchar_t, FWStrCmpProc, FDefaultWStrProc> wstring;

}
#endif
