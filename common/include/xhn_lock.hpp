#ifndef XHN_LOCK_HPP
#define XHN_LOCK_HPP
#include "common.h"
#include "etypes.h"
#include "emem.hpp"
#include "xhn_smart_ptr.hpp"
namespace xhn
{
class RWLock : public RefObject
{
private:
	pthread_rwlock_t m_lock;
public:
	class Instance
	{
		friend class RWLock;
	private:
		pthread_rwlock_t* m_prototype;
		inline Instance(pthread_rwlock_t* lock)
			: m_prototype(lock)
		{}
	public:
		inline Instance(const Instance& inst)
			: m_prototype(inst.m_prototype)
		{}
		inline ~Instance()
		{
			pthread_rwlock_unlock(m_prototype);
		}
	};
public:
	inline RWLock()
	{
		pthread_rwlock_init(&m_lock, NULL);
	}
	inline ~RWLock()
	{
		pthread_rwlock_destroy(&m_lock);
	}
	inline Instance GetWriteLock()
	{
		pthread_rwlock_wrlock(&m_lock);
		return Instance(&m_lock);
	}
	inline Instance GetReadLock()
	{
		pthread_rwlock_rdlock(&m_lock);
		return Instance(&m_lock);
	}
};
typedef SmartPtr<RWLock> RWLockPtr;
/**
class SpinLock : public RefObject
{
private:
	pthread_spinlock_t m_lock;
public:
	class Instance
	{
		friend class SpinLock;
	private:
		pthread_spinlock_t* m_prototype;
		inline Instance(pthread_spinlock_t* lock)
			: m_prototype(lock)
		{}
	public:
		inline Instance(const Instance& inst)
			: m_prototype(inst.m_prototype)
		{}
		inline ~Instance()
		{
			pthread_spin_unlock(m_prototype);
		}
	};
public:
	inline SpinLock()
	{
		pthread_spin_init(&m_lock, PTHREAD_PROCESS_PRIVATE);
	}
	inline ~SpinLock()
	{
		pthread_spin_destroy(&m_lock);
	}
	inline Instance GetLock()
	{
		pthread_spin_lock(&m_lock);
		return Instance(&m_lock);
	}
};
typedef SmartPtr<SpinLock> SpinLockPtr;
**/
class RWLock2
{
private:
    pthread_rwlock_t m_lock;
public:
	class Instance
	{
		friend class RWLock2;
	private:
		pthread_rwlock_t* m_prototype;
		inline Instance(pthread_rwlock_t* lock)
			: m_prototype(lock)
		{}
	public:
        inline Instance(const Instance& inst)
			: m_prototype(inst.m_prototype)
		{}
		inline ~Instance()
		{
			pthread_rwlock_unlock(m_prototype);
		}
	};
public:
    inline RWLock2()
	{
		pthread_rwlock_init(&m_lock, NULL);
	}
	inline ~RWLock2()
	{
		pthread_rwlock_destroy(&m_lock);
	}
	inline Instance GetWriteLock()
	{
		pthread_rwlock_wrlock(&m_lock);
        return Instance(&m_lock);
	}
	inline Instance GetReadLock()
	{
		pthread_rwlock_rdlock(&m_lock);
		return Instance(&m_lock);
	}
};
}
#endif