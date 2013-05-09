#ifndef RESOURCE_H
#define RESOURCE_H
#include "common.h"
#include "etypes.h"
#include "xhn_static_string.hpp"
#include "xhn_set.hpp"
#include "xhn_map.hpp"
#include "xhn_vector.hpp"
#include "xhn_smart_ptr.hpp"
#include "rtti.hpp"
#include "emem.hpp"
#include "image.h"
#include "texture2d.h"

class ResourceGroup;
class Resource : public RefObject
{
	friend class ResourceGroup;
	DeclareRootRTTI;
private:
	ResourceGroup* m_owner;
public:
	Resource(ResourceGroup* owner)
		: m_owner(owner)
	{}
	virtual ~Resource() {}
};
typedef xhn::SmartPtr<Resource> ResourcePtr;
typedef FILE* FileStream;

class ResourceTypeDetector : public RefObject
{
private:
	xhn::static_string m_type;
public:
	ResourceTypeDetector(const xhn::static_string type)
		: m_type(type)
	{}
	virtual ~ResourceTypeDetector() {}
	virtual xhn::static_string Detect(xhn::static_string resName, FileStream stream) = 0;
	virtual xhn::static_string Detect(xhn::static_string resName) = 0;
	inline const xhn::static_string GetType() const {
		return m_type;
    }
};
typedef xhn::SmartPtr<ResourceTypeDetector> ResourceTypeDetectorPtr;
class ResourceImplement : public RefObject
{
private:
	ResourceTypeDetectorPtr m_resDetector;
public:
	ResourceImplement() {}
	virtual ResourceTypeDetectorPtr MakeResourceTypeDetector() = 0;
	inline const xhn::static_string GetType() {
        return GetResourceTypeDetector()->GetType();
	}
	inline ResourceTypeDetectorPtr GetResourceTypeDetector() {
		if (!m_resDetector.get()) {
			m_resDetector = MakeResourceTypeDetector();
		}
		return m_resDetector;
	}
	virtual ~ResourceImplement() {}
	virtual ResourcePtr New(ResourceGroup* resGrp, const xhn::static_string resName) = 0;
	virtual ResourcePtr Load(ResourceGroup* resGrp, FileStream stream) = 0;
};
typedef xhn::SmartPtr<ResourceImplement> ResourceImplementPtr;

template <typename SMART_PTR>
inline bool TestResourcePtr(const SMART_PTR& ptr) {
	return ptr.get() != NULL;
}

typedef xhn::pair<xhn::static_string, xhn::static_string> ResourceID;
#endif
