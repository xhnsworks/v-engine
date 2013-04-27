#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include "emem.hpp"
#include "xhn_lock.hpp"
class Attribute : public MemObject
{
public:
	enum Type
	{
		Float,
		Float2,
		Float3,
		Float4,
	};
};
class AttributeHandle
{
private:
	struct FAttrDestProc
	{
		inline void operator() (xhn::RWLock* lock) {
            Attribute* attr = (Attribute*)lock->GetUserdata();
			if (attr) {
				delete attr;
			}
		}
	};
public:
	/// if the lock is existed, then the attribute is existed;
	xhn::SmartPtr<xhn::RWLock, FAttrDestProc> m_lock;
public:
	AttributeHandle()
	{}
	AttributeHandle(const AttributeHandle& ah)
		: m_lock(ah.m_lock)
	{}
	inline void operator = (const AttributeHandle& ah)
	{
		m_lock = ah.m_lock;
	}
	inline xhn::RWLock::Instance GetReadLock()
	{
		return m_lock->GetReadLock();
	}
	inline xhn::RWLock::Instance GetWriteLock()
	{
		return m_lock->GetWriteLock();
	}
	template <typename T>
	void AttachAttribute()
	{
		if (m_lock.get() && !m_lock->GetUserdata()) {
			m_lock->SetUserdata(ENEW T);
		}
	}
	inline Attribute* GetAttribute() {
		if (m_lock.get()) {
			return (Attribute*)m_lock->GetUserdata();
		}
		else
			return NULL;
	}
};
#endif