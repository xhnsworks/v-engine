#ifndef TEXTURE2D_RESOURCE_H
#define TEXTURE2D_RESOURCE_H
///*************************************************************************************************************************///
///                                                     include begin                                                       ///
///*************************************************************************************************************************///
#include "common.h"
#include "etypes.h"
#include "resource.h"
///*************************************************************************************************************************///
///                                                     include end                                                         ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class Texture2DLoader;
class Texture2DResource : public Resource
{
	friend class Texture2DImplement;
	friend class DefaultTexture2DImplement;
	DeclareRTTI;
public:
	Texture2DPtr m_tex;
public:
	inline Texture2DPtr GetTexture() {
		return m_tex;
	}
private:
	Texture2DResource(ResourceGroup* owner, Texture2DPtr tex)
		: Resource(owner)
		, m_tex(tex)
	{}
	virtual ~Texture2DResource();
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class Texture2DImplement : public ResourceImplement
{
public:
	Texture2DImplement();
	Texture2DPtr create_texture_from_file(FILE* file_stream);
	virtual ResourceTypeDetectorPtr MakeResourceTypeDetector();
	virtual ResourcePtr New(ResourceGroup* resGrp, const xhn::static_string resName);
	virtual ResourcePtr Load(ResourceGroup* resGrp, FileStream stream);
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
class Texture2DDetector : public ResourceTypeDetector
{
public:
	Texture2DDetector(const xhn::static_string type)
		: ResourceTypeDetector(type) 
	{}
	virtual xhn::static_string Detect(xhn::static_string resName, FileStream stream);
	virtual xhn::static_string Detect(xhn::static_string resName);
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class DefaultTexture2DImplement : public ResourceImplement
{
private:
	ResourcePtr m_defaultTexRes;
public:
	DefaultTexture2DImplement();
	virtual ResourceTypeDetectorPtr MakeResourceTypeDetector();
	virtual ResourcePtr New(ResourceGroup* resGrp, const xhn::static_string resName);
	virtual ResourcePtr Load(ResourceGroup* resGrp, FileStream stream);
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
///*************************************************************************************************************************///
///                                                  class define begin                                                     ///
///*************************************************************************************************************************///
class DefaultTexture2DDetector : public ResourceTypeDetector
{
public:
	DefaultTexture2DDetector(const xhn::static_string type)
		: ResourceTypeDetector(type) 
	{}
	virtual xhn::static_string Detect(xhn::static_string resName, FileStream stream);
	virtual xhn::static_string Detect(xhn::static_string resName);
};
///*************************************************************************************************************************///
///                                                   class define end                                                      ///
///*************************************************************************************************************************///
#endif