#ifndef XHN_HASH_SET_H
#define XHN_HASH_SET_H
#include "common.h"
#include "etypes.h"
#include "xhn_utility.hpp"
#include "xhn_string.hpp"
#include "xhn_list.hpp"
#include "xhn_lock.hpp"
#define HASH_BUCKET_COUNT (1<<15)
#define HASH_MASK         (HASH_BUCKET_COUNT - 1)
namespace xhn
{
template<typename T>
class hash_set : public RefObject
{
public:
    class bucket
    {
    public:
        list<T> m_value_list;
		RWLock2 m_lock;
    public:
        typename list<T>::iterator insert ( const T &value ) {
			RWLock2::Instance inst = m_lock.GetWriteLock();
            if ( !m_value_list.size() ) {
                return m_value_list.push_back ( value );
            } else {
                typename list<T>::iterator iter = m_value_list.begin();

                for ( ; iter != m_value_list.end(); iter++ ) {
                    if ( *iter == value ) {
                        return iter;
                    }
                }

                return m_value_list.push_back ( value );
            }
        }
		bool test ( const T &value ) {
			RWLock::Instance inst = m_lock.GetReadLock();
			if ( !m_value_list.size() ) {
				return false;
			} else {
				typename list<T>::iterator iter = m_value_list.begin();

				for ( ; iter != m_value_list.end(); iter++ ) {
					if ( *iter == value ) {
						return true;
					}
				}

				return false;
			}
		}
    };
public:
    bucket m_buckets[HASH_BUCKET_COUNT];
    const T &insert ( const T &value ) {
        euint hash_value = _hash(value);
        euint key = hash_value & HASH_MASK;
        typename list<T>::iterator iter = m_buckets[key].insert ( value );
        return *iter;
    }
	bool test ( const T &value ) {
		euint hash_value = _hash(value);
		euint key = hash_value & HASH_MASK;
		return m_buckets[key].test ( value );
	}
};
}
#endif
