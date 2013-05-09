#ifndef XML_RESOURCE_H
#define XML_RESOURCE_H
#include "common.h"
#include "etypes.h"
#include "resource.h"
#include "pugixml.hpp"
class XMLLoader;
class XMLResource : public Resource
{
	friend class XMLImplement;
	friend class xhn::SmartPtr<XMLResource>;
	DeclareRTTI;
public:
	pugi::xml_document m_doc;
public:
	inline pugi::xml_document& GetDocument() {
		return m_doc;
	}
private:
	XMLResource(ResourceGroup* owner)
		: Resource(owner)
	{}
	virtual ~XMLResource() {}
};
typedef xhn::SmartPtr<XMLResource> XMLResourcePtr;

class XMLImplement : public ResourceImplement
{
public:
	virtual ResourceTypeDetectorPtr MakeResourceTypeDetector();
	virtual ResourcePtr New(ResourceGroup* resGrp, const xhn::static_string resName);
	virtual ResourcePtr Load(ResourceGroup* resGrp, FileStream stream);
};

class XMLDetector : public ResourceTypeDetector
{
public:
	XMLDetector(const xhn::static_string type)
		: ResourceTypeDetector(type)
	{}
	virtual xhn::static_string Detect(xhn::static_string resName, FileStream stream);
	virtual xhn::static_string Detect(xhn::static_string resName);
};
#endif
