// xstl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>

extern "C" {
#include "mem.h"
#include "elog.h"
#pragma comment(lib, "ecg.lib")
}

namespace xstd
{
	template <class T>
	class Allocator
	{
	public:
		typedef  T         value_type;
        typedef  T*        pointer;
        typedef  const T*   const_pointer;
        typedef  T&        reference;
        typedef  const T&   const_reference;
        typedef  size_t      size_type;
        typedef  ptrdiff_t    difference_type;
	public:
		template <typename U>
        struct rebind{
            typedef Allocator <U> other;
        };
		Allocator(){}
		template <class U> Allocator(const Allocator<U> &){}
		template <class U> Allocator(Allocator<U> &){}
		~Allocator(){}
		pointer allocate(size_type n, const void* = 0)
		{
			return (pointer)Malloc(sizeof(T) * n);
		}
		void deallocate(pointer p, size_type n)
		{
			Mfree(p);
		}
		size_type max_size() const
		{
			return 1 * 1024 * 1024 * 1024;
		}
		pointer address(reference x) const
		{
			return &x;
		}
		const_pointer address(const_reference x) const
		{
			return &x;
		}
		void construct(pointer p, const T& x)
		{
			*p = T(x);
		}
		void destroy(pointer p)
		{
			p->~T();
		}
	};
}
int _tmain(int argc, _TCHAR* argv[])
{
	MInit();
	ELog_Init();
	int* a = (int*)Malloc(sizeof(int));
	Mfree(a);

	std::vector<int, xstd::Allocator<int>> newarray;
	newarray.push_back(1);
	newarray.push_back(2);
	printf("here %d\n", newarray[0]);
	return 0;
}

