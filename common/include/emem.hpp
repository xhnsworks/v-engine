#ifndef EMEM_HPP
#define EMEM_HPP
#include "emem.h"
#include "eassert.h"
#include "elog.h"
#include <pthread.h>
class MemObject
{
public:
    MemObject() {}
    ~MemObject() {}
//////////////////////////////////////////////////////////////////////////
	void* operator new( size_t nSize )
    {
        return Malloc( nSize );
    }
	void operator delete( void *p)
	{
		Mfree(p);
	}
//////////////////////////////////////////////////////////////////////////
    void* operator new( size_t nSize, const char* file,int line )
    {
        return _Malloc( nSize, file, line );
    }

	void operator delete( void *p, const char* file,int line )
    {
        Mfree(p);
    }
//////////////////////////////////////////////////////////////////////////
	void* operator new( size_t nSize, void* ptr )
	{
		EAssert(((ref_ptr)ptr % 16) == 0, "new object error");
		return ptr;
	}

	void operator delete( void *p, void* ptr )
	{
	}
//////////////////////////////////////////////////////////////////////////
	void* operator new[]( size_t nSize )
	{
		return Malloc( nSize );
	}

	void operator delete[]( void* ptr, size_t nSize )
	{
		Mfree(ptr);
	}
};

class RefObject : public MemObject
{
public:
	pthread_spinlock_t lock;
	volatile int ref_count;
	RefObject()
	{
		pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);
		ref_count = 0;
	}
	~RefObject()
	{
		pthread_spin_destroy(&lock);
	}
};

#endif