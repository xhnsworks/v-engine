#ifndef XHN_MAP_H
#define XHN_MAP_H
#include "common.h"
#include "etypes.h"
#include "emem.h"
#include "emem.hpp"
#include <new>

#include "xhn_iterator.hpp"
#include "xhn_utility.hpp"
#include "xhn_rbtree.hpp"
#include "xhn_pair.hpp"

namespace xhn
{
template<typename K, typename V>
class map : public RefObject
{
private:
    struct FNodeCreateProc {
        rbtree_node<K> *operator() () {
            pair<K, V> *ret = ( pair<K, V> * ) Malloc ( sizeof ( pair<K, V> ) );
            new (ret) pair<K, V>();
            return ret;
        }
    };
    rbtree<K, V, FNodeCreateProc> m_rbtree;
public:
    typedef rbtree<K, V, FNodeCreateProc> rbtree_type;
    typedef pair<K, V> rbnode_type;
    template<typename T>
    struct FReadProc {
        void operator() ( T *from, T &to ) {
            to = *from;
        }
    };
    template<typename T>
    struct FWriteProc {
        void operator() ( T *to, T &from ) {
            *to = from;
        }
    };
    template<typename T>
    struct FNextProc {
        void operator() ( T *from, T *&to, uint ele_real_size ) {
            to = ( T * ) from->iter_next;
        }
    };
    template<typename T>
    struct FPrevProc {
        void operator() ( T *from, T *&to, uint ele_real_size ) {
            to = ( T * ) from->iter_prev;
        }
    };
public:
    class iterator : public bidirectional_readwrite_iterator<rbnode_type, FReadProc<rbnode_type>, FWriteProc<rbnode_type>, FNextProc<rbnode_type>, FPrevProc<rbnode_type> >
    {
    public:
        typedef bidirectional_readwrite_iterator<rbnode_type, FReadProc<rbnode_type>, FWriteProc<rbnode_type>, FNextProc<rbnode_type>, FPrevProc<rbnode_type> > base_type;
        iterator ( rbnode_type *a, uint ele_real_size )
            : base_type (
                a, ele_real_size, FReadProc<rbnode_type>(), FWriteProc<rbnode_type>(), FNextProc<rbnode_type>(), FPrevProc<rbnode_type>() )
        {}
        inline iterator &operator++() {
            base_type::next();
            return *this;
        }
        inline iterator operator++ ( int ) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }
        inline iterator &operator--() {
            base_type::prev();
            return *this;
        }
        inline iterator operator-- ( int ) {
            iterator tmp = *this;
            --*this;
            return tmp;
        }
    };

	class reverse_iterator : public bidirectional_readwrite_iterator<rbnode_type, FReadProc<rbnode_type>, FWriteProc<rbnode_type>, FPrevProc<rbnode_type>, FNextProc<rbnode_type> >
	{
	public:
		typedef bidirectional_readwrite_iterator<rbnode_type, FReadProc<rbnode_type>, FWriteProc<rbnode_type>, FPrevProc<rbnode_type>, FNextProc<rbnode_type> > base_type;
		reverse_iterator ( rbnode_type *a, uint ele_real_size )
			: base_type (
			a, ele_real_size, FReadProc<rbnode_type>(), FWriteProc<rbnode_type>(), FPrevProc<rbnode_type>(), FNextProc<rbnode_type>() )
		{}
		inline reverse_iterator &operator++() {
			base_type::next();
			return *this;
		}
		inline reverse_iterator operator++ ( int ) {
			reverse_iterator tmp = *this;
			++*this;
			return tmp;
		}
		inline reverse_iterator &operator--() {
			base_type::prev();
			return *this;
		}
		inline reverse_iterator operator-- ( int ) {
			reverse_iterator tmp = *this;
			--*this;
			return tmp;
		}
	};

    class const_iterator : public const_bidirectional_readwrite_iterator<rbnode_type, FReadProc<rbnode_type>, FWriteProc<rbnode_type>, FNextProc<rbnode_type>, FPrevProc<rbnode_type> >
    {
    public:
        typedef const_bidirectional_readwrite_iterator<rbnode_type, FReadProc<rbnode_type>, FWriteProc<rbnode_type>, FNextProc<rbnode_type>, FPrevProc<rbnode_type> > base_type;
        const_iterator ( rbnode_type *a, uint ele_real_size )
            : base_type (
                a, ele_real_size, FReadProc<rbnode_type>(), FWriteProc<rbnode_type>(), FNextProc<rbnode_type>(), FPrevProc<rbnode_type>() )
        {}
        inline const_iterator &operator++() {
            base_type::next();
            return *this;
        }
        inline const_iterator operator++ ( int ) {
            const_iterator tmp = *this;
            ++*this;
            return tmp;
        }
        inline const_iterator &operator--() {
            base_type::prev();
            return *this;
        }
        inline const_iterator operator-- ( int ) {
            const_iterator tmp = *this;
            --*this;
            return tmp;
        }
    };

	class const_reverse_iterator : public const_bidirectional_readwrite_iterator<rbnode_type, FReadProc<rbnode_type>, FWriteProc<rbnode_type>, FPrevProc<rbnode_type>, FNextProc<rbnode_type> >
	{
	public:
		typedef const_bidirectional_readwrite_iterator<rbnode_type, FReadProc<rbnode_type>, FWriteProc<rbnode_type>, FPrevProc<rbnode_type>, FNextProc<rbnode_type> > base_type;
		const_reverse_iterator ( rbnode_type *a, uint ele_real_size )
			: base_type (
			a, ele_real_size, FReadProc<rbnode_type>(), FWriteProc<rbnode_type>(), FPrevProc<rbnode_type>(), FNextProc<rbnode_type>() )
		{}
		inline const_reverse_iterator &operator++() {
			base_type::next();
			return *this;
		}
		inline const_reverse_iterator operator++ ( int ) {
			const_reverse_iterator tmp = *this;
			++*this;
			return tmp;
		}
		inline const_reverse_iterator &operator--() {
			base_type::prev();
			return *this;
		}
		inline const_reverse_iterator operator-- ( int ) {
			const_iterator tmp = *this;
			--*this;
			return tmp;
		}
	};

	map()
	{}

	map(const map& m)
	{
        const_iterator iter = m.begin();
		const_iterator end = m.end();
		for (; iter != end; iter++)
		{
			insert(*iter.m_ptr);
		}
	}

    void insert ( const K key, const V &value ) {
        rbnode_type *node = ( rbnode_type * ) rbtree_type::_find ( &m_rbtree, (K&)key, true );
        node->second = value;
    }
    iterator insert ( const pair<K, V>& key_value_pair ) {
        pair<K, V>& n_key_value_pair = (pair<K, V>&)key_value_pair;
        rbnode_type *node = ( rbnode_type * ) rbtree_type::_find ( &m_rbtree, n_key_value_pair.first, true );
        node->second = n_key_value_pair.second;
        return iterator ( node, 0 );
    }
    iterator find ( const K &key ) {
        rbnode_type *node = ( rbnode_type * ) rbtree_type::_find ( &m_rbtree, (K&)key, false );
        return iterator ( node, 0 );
    }
    const_iterator find ( const K &key ) const {
        rbnode_type *node = ( rbnode_type * ) rbtree_type::_find ( &m_rbtree, (K&)key );
        return const_iterator ( node, 0 );
    }
    V &operator[] ( const K &key ) {
        rbnode_type *node = ( rbnode_type * ) rbtree_type::_find ( &m_rbtree, (K&)key, true );
        return node->second;
    }
    uint erase ( const K &key ) {
        return rbtree_type::_remove ( &m_rbtree, key );
    }
    uint erase ( iterator &iter ) {
        return rbtree_type::_erase ( &m_rbtree, & ( *iter ) );
    }
    iterator erase ( const_iterator &iter ) {
        iterator ret ( iter->iter_next, 0 );
        rbtree_type::_erase ( &m_rbtree, & ( *iter ) );
        return ret;
    }
    iterator begin() {
        return iterator ( ( rbnode_type * ) m_rbtree.head, 0 );
    }
    iterator end() {
        return iterator ( NULL, 0 );
    }
	reverse_iterator rbegin() {
		return reverse_iterator ( ( rbnode_type * ) m_rbtree.tail, 0 );
	}
	reverse_iterator rend() {
		return reverse_iterator ( NULL, 0 );
	}
    const_iterator begin() const {
        return const_iterator ( ( rbnode_type * ) m_rbtree.head, 0 );
    }
    const_iterator end() const {
        return const_iterator ( NULL, 0 );
    }
	const_reverse_iterator rbegin() const {
		return const_reverse_iterator ( ( rbnode_type * ) m_rbtree.tail, 0 );
	}
	const_reverse_iterator rend() const {
		return const_reverse_iterator ( NULL, 0 );
	}
    uint size() const {
        return m_rbtree.count;
    }
    void clear() {
        m_rbtree.clear();
    }
    bool empty() const {
        return m_rbtree.count == 0;
    }

	map* clone() {
		map* ret = ENEW map(*this);
		return ret;
	}
};
}
#endif
