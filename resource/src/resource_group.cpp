#include "pch.h"
#include "resource_group.h"
#include "texture2d_resource.h"
#include "xml_resource.h"
bool DefaultFileImpl::New(const xhn::string& path)
{
	return NULL;
}
bool DefaultFileImpl::Delete(const xhn::string& path)
{
	return NULL;
}
FileStream DefaultFileImpl::Open(const xhn::string& path)
{
	return SafeFOpen(path.c_str(), "rb");
}
void DefaultFileImpl::Close(FileStream stream)
{
	if (stream)
	    fclose(stream);
}
bool DefaultFileImpl::IsExist(const xhn::string& path)
{
	FILE* fp = SafeFOpen(path.c_str(), "rb");
	if (fp) {
		fclose(fp);
		return true;
	}
	else {
		return false;
	}
}

ResourceGroup::ResourceGroup(ResourceSystem* owner, const xhn::static_string name, ResourceGroup* parent, DerivedType derivedType, FileImplPtr fileImpl)
	: m_owner(owner)
	, m_fileImpl(fileImpl)
	, m_name(name)
	, m_parent(parent)
	, m_derivedType(derivedType)
{
	switch (derivedType) {
	case Public:
		{
			xhn::set<xhn::static_string>::iterator iter = parent->m_publicDirectorys.begin();
			xhn::set<xhn::static_string>::iterator end = parent->m_publicDirectorys.end();
			for (; iter != end; iter++)
			{
				m_publicDirectorys.insert(*iter);
			}
			iter = parent->m_protectedDirectorys.begin();
			end = parent->m_protectedDirectorys.end();
			for (; iter != end; iter++)
			{
				m_protectedDirectorys.insert(*iter);
			}
		}
		break;
	case Protected:
		{
			xhn::set<xhn::static_string>::iterator iter = parent->m_publicDirectorys.begin();
			xhn::set<xhn::static_string>::iterator end = parent->m_publicDirectorys.end();
			for (; iter != end; iter++)
			{
				m_protectedDirectorys.insert(*iter);
			}
			iter = parent->m_protectedDirectorys.begin();
			end = parent->m_protectedDirectorys.end();
			for (; iter != end; iter++)
			{
				m_protectedDirectorys.insert(*iter);
			}
		}
		break;
	case Private:
		{
			xhn::set<xhn::static_string>::iterator iter = parent->m_publicDirectorys.begin();
			xhn::set<xhn::static_string>::iterator end = parent->m_publicDirectorys.end();
			for (; iter != end; iter++)
			{
				m_privateDirectorys.insert(*iter);
			}
			iter = parent->m_protectedDirectorys.begin();
			end = parent->m_protectedDirectorys.end();
			for (; iter != end; iter++)
			{
				m_privateDirectorys.insert(*iter);
			}
		}
		break;
	}
	xhn::map<xhn::static_string, ResourceImplementPtr>::iterator iter = parent->m_resourceImplementMap.begin();
	xhn::map<xhn::static_string, ResourceImplementPtr>::iterator end = parent->m_resourceImplementMap.end();
	for (; iter != end; iter++)
	{
		m_resourceImplementMap[iter->first] = iter->second;
	}
}
ResourceGroup::ResourceGroup(ResourceSystem* owner, const xhn::static_string name, FileImplPtr fileImpl)
	: m_owner(owner)
	, m_fileImpl(fileImpl)
	, m_name(name)
	, m_parent(NULL)
	, m_derivedType(Public)
{
}
ResourceGroup::~ResourceGroup()
{
}
void ResourceGroup::RegisterResourceImplement(ResourceImplementPtr impl)
{
	m_resourceImplementMap.insert(xhn::make_pair(impl->GetType(), impl));
	m_owner->RegisterResourceTypeDetector(impl->GetResourceTypeDetector());
}

bool ResourceGroup::__Load(const xhn::string& resName, xhn::set<xhn::static_string>& dirSet, ResourcePtr& result)
{
	xhn::set<xhn::static_string>::iterator iter = dirSet.begin();
	xhn::set<xhn::static_string>::iterator end = dirSet.end();
	for (; iter != end; iter++)
	{
		xhn::string dir = iter->c_str();
		dir += "\\";
		xhn::string path = dir + resName;
		FileStream stream = m_fileImpl->Open(path);
		{
			xhn::static_string type = m_owner->DetectResourceType(resName, stream);
			if (type.size()) {
				xhn::map<xhn::static_string, ResourceImplementPtr>::iterator loaderIter = m_resourceImplementMap.find(type);
				if (loaderIter != m_resourceImplementMap.end())
				{
					ResourceImplementPtr resLoader = loaderIter->second;
					result = resLoader->Load(this, stream);
					m_loadedResouceSet.insert(result);
				}
				m_fileImpl->Close(stream);
				return true;
			}
		}
	}
	return false;
}
bool ResourceGroup::__New(const xhn::string& resName, ResourcePtr& result)
{
	xhn::static_string type = m_owner->DetectResourceType(resName);
	if (type.size()) {
		xhn::map<xhn::static_string, ResourceImplementPtr>::iterator loaderIter = m_resourceImplementMap.find(type);
		if (loaderIter != m_resourceImplementMap.end())
		{
			ResourceImplementPtr resLoader = loaderIter->second;
			result = resLoader->New(this, xhn::static_string(resName.c_str()));
			m_loadedResouceSet.insert(result);
		}
		return true;
	}
	return false;
}
ResourcePtr ResourceGroup::Load(const xhn::string& resName)
{
	ResourcePtr ret;
	__Load(resName, m_publicDirectorys, ret);
	return ret;
}
ResourcePtr ResourceGroup::Load(const xhn::static_string resName)
{
	ResourcePtr ret;
	__Load(xhn::string(resName.c_str()), m_publicDirectorys, ret);
	return ret;
}
ResourcePtr ResourceGroup::New(const xhn::string& resName)
{
	ResourcePtr ret;
	__New(resName, ret);
	return ret;
}
ResourcePtr ResourceGroup::New(const xhn::static_string resName)
{
	ResourcePtr ret;
	__New(resName.c_str(), ret);
	return ret;
}
void ResourceGroup::RegisterResourceDirectory(xhn::static_string dir, DerivedType derivedType)
{
	switch (derivedType)
	{
	case Public:
		m_publicDirectorys.insert(dir);
		break;
	case Protected:
		m_protectedDirectorys.insert(dir);
		break;
	case Private:
		m_privateDirectorys.insert(dir);
		break;
	default:
		break;
	}
}

ResourceSystem::ResourceSystem()
{
	xhn::static_string grpName("BaseGroup");
	ResourceGroup* grp = ENEW ResourceGroup(this, grpName);
	m_resGrps.insert(xhn::make_pair(grpName, grp));
	m_baseGroup = grp;
}
ResourceSystem::~ResourceSystem()
{
	xhn::map<xhn::static_string, ResourceGroup*>::iterator iter = m_resGrps.begin();
	xhn::map<xhn::static_string, ResourceGroup*>::iterator end = m_resGrps.end();
	for (; iter != end; iter++)
	{
		ResourceGroup* grp = iter->second;
		delete grp;
	}
}
#ifndef __APPLE__
#include <direct.h>
#else
#include <sys/dirent.h>
#include <unistd.h>
#endif
xhn::static_string ResourceSystem::GetWorkDirectory()
{
	char mbuf[260];
#ifndef __APPLE__
	_getcwd(mbuf, 259);
#else
    getcwd(mbuf, 259);
#endif
	return xhn::static_string(mbuf);
}
void ResourceSystem::NewResourceGroup(const xhn::static_string grpName, const xhn::static_string parentGrpName, DerivedType dt)
{
	xhn::map<xhn::static_string, ResourceGroup*>::iterator iter = m_resGrps.find(parentGrpName);
	if (iter != m_resGrps.end()) {
		ResourceGroup* parentGrp = iter->second;
		ResourceGroup* grp = ENEW ResourceGroup(this, grpName, parentGrp, dt);
		m_resGrps.insert(xhn::make_pair(grpName, grp));
	}
}
ResourceGroup* ResourceSystem::GetResourceGroup(const xhn::static_string grpName)
{
	xhn::map<xhn::static_string, ResourceGroup*>::iterator iter = m_resGrps.find(grpName);
	if (iter != m_resGrps.end())
		return iter->second;
	return NULL;
}
void ResourceSystem::RegisterResourceTypeDetector(ResourceTypeDetectorPtr rtd)
{
	m_resTypeDetectors.push_back(rtd);
}
xhn::static_string ResourceSystem::DetectResourceType(const xhn::string& resName, FileStream stream)
{
	xhn::static_string ret("");
	xhn::vector<ResourceTypeDetectorPtr>::iterator iter = m_resTypeDetectors.begin();
	xhn::vector<ResourceTypeDetectorPtr>::iterator end = m_resTypeDetectors.end();
	for (; iter != end; iter++)
	{
		ResourceTypeDetectorPtr rtd = *iter;
		ret = rtd->Detect(resName, stream);
		if (ret.size() > 0)
			return ret;
	}
	return ret;
}
xhn::static_string ResourceSystem::DetectResourceType(const xhn::string& resName)
{
	xhn::static_string ret("");
	xhn::vector<ResourceTypeDetectorPtr>::iterator iter = m_resTypeDetectors.begin();
	xhn::vector<ResourceTypeDetectorPtr>::iterator end = m_resTypeDetectors.end();
	for (; iter != end; iter++)
	{
		ResourceTypeDetectorPtr rtd = *iter;
		ret = rtd->Detect(resName);
		if (ret.size() > 0)
			return ret;
	}
	return ret;
}