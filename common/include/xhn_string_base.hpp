#ifndef XHN_STRING_BASE_HPP
#define XHN_STRING_BASE_HPP
#include "common.h"
#include "etypes.h"
#include "emem.h"
#include "xhn_utility.hpp"
#include "xhn_vector.hpp"
#include <string.h>
namespace xhn
{
template <typename C, typename STR_CMP_PROC, typename DEFAULT_STR_PROC>
class string_base
{
private:
    euint m_hash;
    euint m_size;
    C *m_str;
    STR_CMP_PROC m_str_cmp_proc;
    DEFAULT_STR_PROC m_default_str_proc;
public:
    const static euint npos = ( euint )-1;
    typedef C* iterator;
    typedef const C* const_iterator;
    string_base() {
        m_str = ( C * ) Malloc ( sizeof(C) );
        m_str[0] = 0;
        m_size = 0;
        m_hash = 0;
    }
    string_base ( const C *str ) {
        if (!str)
            str = m_default_str_proc();

        int count = 0;

        while ( str[count] ) {
            count++;
        }

        m_str = ( C * ) Malloc ( (count + 1) * sizeof(C) );
        memcpy ( m_str, str, (count + 1) * sizeof(C) );
        m_size = count;
        m_hash = _hash ( m_str );
    }
	string_base ( const vector< C, FGetCharRealSizeProc<C> >& str ) {
        euint count = str.size();

        m_str = ( C * ) Malloc ( (count + 1) * sizeof(C) );
		C* s = str.get();
        memcpy ( m_str, s, count * sizeof(C) );
		m_str[count] = 0;
        m_size = count;
        m_hash = _hash ( m_str );
    }
    string_base ( const string_base &str ) {
        m_str = ( C * ) Malloc ( (str.m_size + 1) * sizeof(C) );
        memcpy ( m_str, str.m_str, (str.m_size + 1) * sizeof(C) );
        m_size = str.m_size;
        m_hash = str.m_hash;
    }
    ~string_base() {
        Mfree ( m_str );
    }
    iterator begin() {
        return m_str;
    }
    iterator end() {
        return &m_str[m_size];
    }
    const_iterator begin() const {
        return m_str;
    }
    const_iterator end() const {
        return &m_str[m_size];
    }
    const C *c_str() const {
        return m_str;
    }
    bool operator == ( const C* s ) const {
        return m_str_cmp_proc(m_str, s) == 0;
    }
    bool operator != ( const C* s ) const {
        return m_str_cmp_proc(m_str, s) != 0;
    }
    bool operator < ( const string_base &str ) const {
        return m_str_cmp_proc ( m_str, str.m_str ) < 0;
    }
    bool operator == ( const string_base &str ) const {
        return m_str_cmp_proc ( m_str, str.m_str ) == 0;
    }
    bool operator != ( const string_base &str ) const {
        return m_str_cmp_proc ( m_str, str.m_str ) != 0;
    }
    string_base &operator = ( const string_base &str ) {
        Mfree ( m_str );
        m_str = ( C * ) Malloc ( (str.m_size + 1) * sizeof(C) );
        memcpy ( m_str, str.m_str, (str.m_size + 1) * sizeof(C) );
        m_hash = str.m_hash;
        m_size = str.m_size;
        return *this;
    }
    string_base &operator = ( const C *str ) {
        euint count = 0;

        while ( str[count] ) {
            count++;
        }

        Mfree ( m_str );
        m_str = ( C * ) Malloc ( (count + 1) * sizeof(C) );
        memcpy ( m_str, str, (count + 1) * sizeof(C) );
        m_size = count;
        m_hash = _hash ( m_str );
        return *this;
    }
	string_base &operator = ( const vector< C, FGetCharRealSizeProc<C> >& str ) {
        euint count = str.size();

        Mfree ( m_str );
        m_str = ( C * ) Malloc ( (count + 1) * sizeof(C) );
		C* s = str.get();
        memcpy ( m_str, s, count * sizeof(C) );
		m_str[count] = 0;
        m_size = count;
        m_hash = _hash ( m_str );
        return *this;
    }
    string_base operator + ( const string_base &str ) const {
        string_base ret;
        ret.m_size = m_size + str.m_size;
        Mfree ( ret.m_str );
        ret.m_str = ( C * ) Malloc ( (ret.m_size + 1) * sizeof(C) );
        memcpy ( ret.m_str, m_str, m_size * sizeof(C) );
        memcpy ( &ret.m_str[m_size], str.m_str, (str.m_size + 1) * sizeof(C) );
        ret.m_hash = _hash ( ret.m_str );
        return ret;
    }
    string_base operator + ( const C *str ) const {
        euint count = 0;

        while ( str[count] ) {
            count++;
        }

        string_base ret;
        ret.m_size = m_size + count;
        Mfree ( ret.m_str );
        ret.m_str = ( C * ) Malloc ( (ret.m_size + 1) * sizeof(C) );
        memcpy ( ret.m_str, m_str, m_size * sizeof(C) );
        memcpy ( &ret.m_str[m_size], str, (count + 1) * sizeof(C) );
        ret.m_hash = _hash ( ret.m_str );
        return ret;
    }
	string_base operator + ( C s ) const {
		string_base ret;
		ret.m_size = m_size + 1;
		Mfree ( ret.m_str );
		ret.m_str = ( C * ) Malloc ( (ret.m_size + 1) * sizeof(C) );
		memcpy ( ret.m_str, m_str, m_size * sizeof(C) );
		memcpy ( &ret.m_str[m_size], &s, 2 * sizeof(C) );
		ret.m_hash = _hash ( ret.m_str );
		return ret;
	}
    string_base &operator += ( const string_base &str ) {
        C *tmp = ( C * ) Malloc ( (m_size + str.m_size + 1) * sizeof(C) );
        memcpy ( tmp, m_str, m_size * sizeof(C) );
        memcpy ( &tmp[m_size], str.m_str, (str.m_size + 1) * sizeof(C) );
        m_size += str.m_size;
        Mfree ( m_str );
        m_str = tmp;
        m_hash = _hash ( m_str );
        return *this;
    }
    string_base &operator += ( const C *str ) {
        euint count = 0;

        while ( str[count] ) {
            count++;
        }

        C *tmp = ( C * ) Malloc ( (m_size + count + 1) * sizeof(C) );
        memcpy ( tmp, m_str, m_size * sizeof(C) );
        memcpy ( &tmp[m_size], str, (count + 1) * sizeof(C) );
        m_size += count;
        Mfree ( m_str );
        m_str = tmp;
        m_hash = _hash ( m_str );
        return *this;
    }
	string_base &operator += ( C s ) {
		C *tmp = ( C * ) Malloc ( (m_size + 2) * sizeof(C) );
		memcpy ( tmp, m_str, m_size * sizeof(C) );
		memcpy ( &tmp[m_size], &s, 2 * sizeof(C) );
		m_size += 1;
		Mfree ( m_str );
		m_str = tmp;
		m_hash = _hash ( m_str );
		return *this;
	}
    euint find ( const string_base &str, euint pos = 0 ) const {
        if ( m_size <= pos || !m_size || !str.m_size ) {
            return npos;
        }

        euint count = pos;
        euint match_count = 0;
        euint match_pos = pos;
        bool matching = false;

        while ( m_str[count] ) {
            if ( matching ) {
                if ( match_count == str.m_size ) {
                    return match_pos;
                }

                if ( m_str[count] != str.m_str[match_count] ) {
                    match_count = 0;
                    matching = false;
                }

                match_count++;
            } else {
                if ( m_str[count] != str.m_str[0] ) {
                    match_pos++;
                } else {
                    matching = true;
                    match_count = 1;
                }
            }

            count++;
        }

		if (matching)
			return match_pos;
		else
            return npos;
    }
    euint rfind ( const string_base &str, euint pos = npos ) const {
        if ( pos == npos ) {
            pos = m_size - 1;
        }

        if ( !m_size || !str.m_size ) {
            return npos;
        }

        int count = pos;
        int match_count = ( int ) str.m_size - 1;
        int match_pos = ( int ) pos;
        bool matching = false;

        while ( count >= 0 ) {
            if ( matching ) {
                if ( match_count < 0 ) {
                    return ( euint ) match_pos - str.m_size + 1;
                }

                if ( m_str[count] != str.m_str[match_count] ) {
                    match_count = str.m_size - 1;
                    matching = false;
                }

                match_count--;
            } else {
                if ( m_str[count] != str.m_str[str.m_size - 1] ) {
                    match_pos--;
                } else {
                    matching = true;
                    match_count = str.m_size - 2;
                }
            }

            count--;
        }

		if (matching)
			return match_pos;
		else
            return npos;
    }
    euint find ( const C *str, euint pos = 0 ) const {
        string_base tmp ( str );
        return find ( tmp, pos );
    }
    euint rfind ( const C *str, euint pos = npos ) const {
        string_base tmp ( str );
        return rfind ( tmp, pos );
    }
    euint find_first_of ( const string_base &str, euint pos = 0 ) const {
        return find ( str, pos );
    }
    euint find_first_of ( const C *s, euint pos = 0 ) const {
        return find ( s, pos );
    }
    euint find_last_of ( const string_base &str, euint pos = npos ) const {
        return rfind ( str, pos );
    }
    euint find_last_of ( const C *s, euint pos = npos ) const {
        return rfind ( s, pos );
    }
    string_base substr ( euint pos = 0, euint len = npos ) const {
        if ( pos >= m_size ) {
            return string_base();
        }

        if ( pos == 0 && len == npos ) {
            return string_base ( *this );
        }

        if ( len + pos > m_size ) {
            len = m_size - pos;
        }

        C *str = ( C * ) Malloc ( (len + 1) * sizeof(C) );
        memcpy ( str, &m_str[pos], len * sizeof(C) );
        str[len] = 0;
        string_base ret;
        Mfree ( ret.m_str );
        ret.m_str = str;
        ret.m_size = len;
        ret.m_hash = _hash ( str );
        return ret;
    }
    euint size() const {
        return m_size;
    }
    inline euint get_hash() const {
        return m_hash;
    }
    C operator[] (euint pos) const {
        return m_str[pos];
    }
	void clear() {
		Mfree(m_str);
		m_str = ( C * ) Malloc ( sizeof(C) );
        m_str[0] = 0;
        m_size = 0;
        m_hash = 0;
	}
};

template <typename C, typename STR_CMP_PROC, typename DEFAULT_STR_PROC>
inline euint _hash ( const string_base<C, STR_CMP_PROC, DEFAULT_STR_PROC> &key )
{
    return key.get_hash();
}

}
#endif
