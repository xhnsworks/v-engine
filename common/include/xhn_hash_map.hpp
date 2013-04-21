#ifndef XHN_HASH_MAP_H
#define XHN_HASH_MAP_H
#include "common.h"
#include "etypes.h"
#include "xhn_utility.hpp"
#include "xhn_string.hpp"
#include "xhn_pair.hpp"
#include "xhn_list.hpp"
#include "xhn_lock.hpp"
#define HASH_BUCKET_COUNT (1<<15)
#define HASH_MASK         (HASH_BUCKET_COUNT - 1)
namespace xhn
{
	template <typename T>
	struct FHashProc
	{
		uint operator() (const T& v) {
			return calc_hashnr ( (const char*)v, sizeof(T) );
		}
	};
	template< typename K, typename V, typename HASH_PROC = FHashProc<K> >
	class hash_map : public RefObject
	{
	public:
		class bucket
		{
		public:
			list< pair<K, V> > m_value_list;
			RWLock2 m_lock;
		public:
			typename list< pair<K, V> >::iterator insert ( const K& key, const V &value ) {
				RWLock2::Instance inst = m_lock.GetWriteLock();
				if ( !m_value_list.size() ) {
					return m_value_list.push_back ( make_pair(key, value) );
				} else {
					typename list< pair<K, V> >::iterator iter = m_value_list.begin();

					for ( ; iter != m_value_list.end(); iter++ ) {
						if ( iter->first == key ) {
							return iter;
						}
					}

					return m_value_list.push_back ( make_pair(key, value) );
				}
			}
			bool erase ( const K& key ) {
                RWLock2::Instance inst = m_lock.GetWriteLock();
				if ( !m_value_list.size() ) {
					return false;
				} else {
					typename list< pair<K, V> >::iterator iter = m_value_list.begin();

					for ( ; iter != m_value_list.end(); iter++ ) {
						if ( iter->first == key ) {
							m_value_list.remove(iter);
							return true;
						}
					}

					return false;
				}
			}
			bool test ( const K &key ) {
				RWLock2::Instance inst = m_lock.GetReadLock();
				if ( !m_value_list.size() ) {
					return false;
				} else {
					typename list< pair<K, V> >::iterator iter = m_value_list.begin();

					for ( ; iter != m_value_list.end(); iter++ ) {
						if ( iter->first == key ) {
							return true;
						}
					}

					return false;
				}
			}
			V* find( const K &key ) {
				RWLock::Instance inst = m_lock.GetReadLock();
				if ( !m_value_list.size() ) {
					return NULL;
				} else {
					typename list< pair<K, V> >::iterator iter = m_value_list.begin();

					for ( ; iter != m_value_list.end(); iter++ ) {
						if ( iter->first == key ) {
							return &iter->second;
						}
					}

					return NULL;
				}
			}
			RWLock2::Instance get_write_lock() {
				return m_lock.GetWriteLock();
			}
			RWLock2::Instance get_read_lock() {
				return m_lock.GetReadLock();
			}
			V* find_unlock( const K &key ) {
				if ( !m_value_list.size() ) {
					return NULL;
				} else {
					typename list< pair<K, V> >::iterator iter = m_value_list.begin();

					for ( ; iter != m_value_list.end(); iter++ ) {
						if ( iter->first == key ) {
							return &iter->second;
						}
					}

					return NULL;
				}
			}
		};
	public:
		bucket m_buckets[HASH_BUCKET_COUNT];
		HASH_PROC m_hash_proc;
		const pair<K, V> &insert ( const K &key, const V& value ) {
			uint hash_value = m_hash_proc (key);
			uint ukey = hash_value & HASH_MASK;
			typename list< pair<K, V> >::iterator iter = m_buckets[ukey].insert ( key, value );
			return *iter;
		}
		bool test ( const K &key ) {
			uint hash_value = m_hash_proc(key);
			uint ukey = hash_value & HASH_MASK;
			return m_buckets[ukey].test ( key );
		}
		V* find( const K &key ) {
			uint hash_value = m_hash_proc(key);
			uint ukey = hash_value & HASH_MASK;
			return m_buckets[ukey].find ( key );
		}
		bucket* find_bucket( const K &key ) {
			uint hash_value = m_hash_proc(key);
			uint ukey = hash_value & HASH_MASK;
			return &m_buckets[ukey];
		}
		bool erase( const K &key ) {
			uint hash_value = m_hash_proc(key);
			uint ukey = hash_value & HASH_MASK;
			return m_buckets[ukey].erase(key);
		}
	};
}
#endif