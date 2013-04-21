#ifndef XHN_UTILITY_HPP
#define XHN_UTILITY_HPP
#include "common.h"
#include "etypes.h"
#include "hash.h"
#include "emem.h"
inline uint _strlen(const char* s)
{
    uint count = 0;
    while (s[count])
        count++;
    return count;
}
#define to_struct_addr(t, member, addr) \
	(t*)( ((char*)addr) - (sint)(&((t*)0)->member) )
namespace xhn
{
class buffer
{
private:
    vptr m_buf;
public:
    inline buffer(uint size) {
        m_buf = Malloc(size);
    }
    inline ~buffer() {
        Mfree(m_buf);
    }
    inline vptr get() {
        return m_buf;
    }
};
template<typename T>
uint _real_size()
{
    uint num_qwords = sizeof ( T ) / 16;
    uint mod = sizeof ( T ) % 16;

    if ( mod ) {
        num_qwords++;
    }

    return num_qwords * 16;
}

inline uint _hash ( char *&key )
{
    return calc_hashnr ( key, _strlen ( key ) );
}

inline uint _hash ( wchar_t *&key )
{
    int count = 0;
    while (key[count]) {
        count++;
    }
    return calc_hashnr ( (const char*)key, count * sizeof(wchar_t) );
}

template < typename InputIterator, typename OutputIterator, typename UnaryOperator >
OutputIterator transform ( InputIterator first1, InputIterator last1,
                           OutputIterator result, UnaryOperator op )
{
    while ( first1 != last1 ) {
        *result = op ( *first1 ); // or: *result=binary_op(*first1,*first2++);
        ++result;
        ++first1;
    }

    return result;
}

template < typename T >
struct FCtorProc {
    void operator () (T* ptr) {
        new ( ptr ) T ();
    }
    void operator () (T* dst, T& src) {
        new ( dst ) T ( src );
    }
};

template < typename T >
struct FDestProc {
    void operator () (T* ptr) {
        ptr->~T();
    }
};

template <typename T>
struct FGetElementRealSizeProc {
    uint operator() () {
        uint num_qwords = sizeof ( T ) / 16;
        uint mod = sizeof ( T ) % 16;

        if ( mod ) {
            num_qwords++;
        }

        return num_qwords * 16;
    }
};

template <typename CHAR>
struct FGetCharRealSizeProc {
    uint operator() () {
        return sizeof(CHAR);
    }
};

inline char to_lower(char c)
{
    if (c >= 'A' && c <= 'Z') {
        char d = c - 'A';
        return 'a' + d;
    } else
        return c;
}
}
#endif
