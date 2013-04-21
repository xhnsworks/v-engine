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
public:
	Attribute* m_attr;
	xhn::RWLockPtr m_lock;
public:
	AttributeHandle()
		: m_attr(NULL)
	{}
	AttributeHandle(const AttributeHandle& ah)
		: m_attr(ah.m_attr)
		, m_lock(ah.m_lock)
	{}
	inline void operator = (const AttributeHandle& ah)
	{
		m_attr = ah.m_attr;
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
};
#endif