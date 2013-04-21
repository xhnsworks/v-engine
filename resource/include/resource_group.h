#ifndef RESOURCE_GROUP_H
#define RESOURCE_GROUP_H
///*************************************************************************************************************************///
///                                                     include begin                                                       ///
///*************************************************************************************************************************///
#include "common.h"
#include "etypes.h"
#include "xhn_static_string.hpp"
#include "xhn_set.hpp"
#include "xhn_map.hpp"
#include "xhn_vector.hpp"
#include "xhn_smart_ptr.hpp"
#include "rtti.hpp"
#include "emem.hpp"
#include "resource.h"
///*************************************************************************************************************************///
///                                                     include end                                                         ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class FileImpl : public RefObject
{
public:
	virtual ~FileImpl() {}
	virtual bool New(const xhn::string& path) = 0;
	virtual bool Delete(const xhn::string& path) = 0;
	virtual FileStream Open(const xhn::string& path) = 0;
	virtual void Close(FileStream stream) = 0;
	virtual bool IsExist(const xhn::string& path) = 0;
};
class DefaultFileImpl : public FileImpl
{
public:
	virtual bool New(const xhn::string& path);
	virtual bool Delete(const xhn::string& path);
	virtual FileStream Open(const xhn::string& path);
	virtual void Close(FileStream stream);
	virtual bool IsExist(const xhn::string& path);
};
typedef xhn::SmartPtr<FileImpl> FileImplPtr;
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class ResourceSystem;
class ResourceGroup : public RefObject
{
	friend class ResourceSystem;
	friend class Resource;
public:
	inline const xhn::static_string GetName() const {
		return m_name;
	}
	void RegisterResourceImplement(ResourceImplementPtr impl);
	ResourcePtr Load(const xhn::string& resName);
	ResourcePtr Load(const xhn::static_string resName);
	ResourcePtr New(const xhn::string& resName);
    ResourcePtr New(const xhn::static_string resName);
	void RegisterResourceDirectory(xhn::static_string dir, DerivedType derivedType);
protected:
	ResourceSystem* m_owner;
	FileImplPtr m_fileImpl;
	xhn::static_string m_name;
	ResourceGroup* m_parent;
	DerivedType m_derivedType;

	xhn::set<xhn::static_string> m_publicDirectorys;
	xhn::set<xhn::static_string> m_protectedDirectorys;
	xhn::set<xhn::static_string> m_privateDirectorys;
	xhn::map<xhn::static_string, ResourceImplementPtr> m_resourceImplementMap;

	xhn::set<ResourcePtr> m_loadedResouceSet;
protected:
	ResourceGroup(ResourceSystem* owner, const xhn::static_string name, ResourceGroup* parent, DerivedType derivedType, FileImplPtr fileImpl = ENEW DefaultFileImpl);
	ResourceGroup(ResourceSystem* owner, const xhn::static_string name, FileImplPtr fileImpl = ENEW DefaultFileImpl);
	~ResourceGroup();

	bool __Load(const xhn::string& resName, xhn::set<xhn::static_string>& dirSet, ResourcePtr& result);
	bool __New(const xhn::string& resName, ResourcePtr& result);
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class ResourceSystem : public MemObject
{
private:
	ResourceGroup* m_baseGroup;
	xhn::map<xhn::static_string, ResourceGroup*> m_resGrps;
	xhn::vector<ResourceTypeDetectorPtr> m_resTypeDetectors;
public:
	ResourceSystem();
	~ResourceSystem();
	void NewResourceGroup(const xhn::static_string grpName, const xhn::static_string parentGrpName, DerivedType dt);
	ResourceGroup* GetResourceGroup(const xhn::static_string grpName);
	void RegisterResourceTypeDetector(ResourceTypeDetectorPtr rtd);
	xhn::static_string DetectResourceType(const xhn::string& resName, FileStream stream);
	xhn::static_string DetectResourceType(const xhn::string& resName);
	xhn::static_string GetWorkDirectory();
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
#endif
