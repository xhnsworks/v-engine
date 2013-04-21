#include "estring.h"
#include "hash.h"
#include "mem.h"
typedef struct _estring
{
    uint32 length;
    uint32 hash;
    char str[0];
} estring;

_INLINE_ estring* _get_estring(const char* _str)
{
    char* ret = _str - sizeof(estring);
    return (estring*)ret;
}

_INLINE_ uint32 _string_length(const char* _str)
{
    uint32 count = 0;
    while(_str[count])
    {
        count++;
    }
    return count;
}
char* _EString_new(const char* _str, const char* _file_name, uint _file_line)
{
    uint32 length = _string_length(_str);
    estring* ret = _Malloc(sizeof(estring) + length + 1, _file_name, _file_line);

    ret->length = length;
    memcpy(&ret->str[0], _str, length + 1);
    ret->hash = calc_hashnr(&ret->str[0], length);
    return &ret->str[0];
}

void _EString_delete(const char* _str, const char* _file_name, uint _file_line)
{
    ///estring* estr = (estring*)((ref_ptr)_str - (ref_ptr)sizeof(estring));
    estring* estr = _get_estring(_str);
    Mfree(estr);
}

char* _EString_add(const char* _str0, const char* _str1, const char* _file_name, uint _file_line)
{
    /**
    estring* estr = (estring*)((ref_ptr)_str0 - (ref_ptr)sizeof(estring));
    uint32 length = strlen(_str1);
    estring* ret = _Malloc(sizeof(estring) + estr->length + length + 1, _file_name, _file_line);
    memcpy(&ret->str[0], &estr->str[0], estr->length);
    memcpy((vptr)((ref_ptr)&ret->str[0] + (ref_ptr)estr->length), _str1, length + 1);
    ret->length = estr->length + length;
    ret->hash = calc_hashnr(&ret->str[0], ret->length);
    return &ret->str[0];
    **/
    estring* estr = _get_estring(_str0);
    uint32 length = strlen(_str1);
    estring* ret = _Malloc(sizeof(estring) + estr->length + length + 1, _file_name, _file_line);

    char* dest = &ret->str[0];
    memcpy(dest, _str0, estr->length); dest += estr->length;
    memcpy(dest, _str1, length + 1);

    ret->length = estr->length + length;
    ret->hash = calc_hashnr(&ret->str[0], ret->length);

    return &ret->str[0];
}

char* _EString_add_sint(const char* _str, sint _si, const char* _file_name, uint _file_line)
{
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "%d", _si);
    return EString_add(_str, mbuf);
}

char* _EString_add_uint(const char* _str, uint _ui, const char* _file_name, uint _file_line)
{
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "%d", _ui);
    return EString_add(_str, mbuf);
}

char* _EString_add_float(const char* _str, float _f, const char* _file_name, uint _file_line)
{
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "%f", _f);
    return EString_add(_str, mbuf);
}

bool EString_less(const char* _str0, const char* _str1)
{
    estring* estr0 = (estring*)((ref_ptr)_str0 - (ref_ptr)sizeof(estring));
    estring* estr1 = (estring*)((ref_ptr)_str1 - (ref_ptr)sizeof(estring));
    if (estr0->hash != estr1->hash)
    {
        return estr0->hash < estr1->hash;
    }
    else
    {
        if (estr0->length != estr1->length)
        {
            return estr0->length < estr1->length;
        }
        else
        {
			uint i = 0;
            for (; i < estr0->length; i++)
            {
                if (estr0->str[i] != estr1->str[i])
                    return estr0->str[i] < estr1->str[i];
            }
        }
    }
    return false;
}

bool EString_greater(const char* _str0, const char* _str1)
{
    estring* estr0 = (estring*)((ref_ptr)_str0 - (ref_ptr)sizeof(estring));
    estring* estr1 = (estring*)((ref_ptr)_str1 - (ref_ptr)sizeof(estring));
    if (estr0->hash != estr1->hash)
    {
        return estr0->hash > estr1->hash;
    }
    else
    {
        if (estr0->length != estr1->length)
        {
            return estr0->length > estr1->length;
        }
        else
        {
			uint i = 0;
            for (; i < estr0->length; i++)
            {
                if (estr0->str[i] != estr1->str[i])
                    return estr0->str[i] > estr1->str[i];
            }
        }
    }
    return false;
}
