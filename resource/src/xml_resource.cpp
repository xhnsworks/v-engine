#include "pch.h"
#include "xml_resource.h"
#include "xhn_vector.hpp"
ImplementRTTI(XMLResource, Resource);
ResourceTypeDetectorPtr XMLImplement::MakeResourceTypeDetector()
{
    ResourceTypeDetectorPtr ret;
	ret = new XMLDetector("XML");
	return ret;
}
ResourcePtr XMLImplement::New(ResourceGroup* resGrp, const xhn::static_string resName)
{
    ResourcePtr ret;
	XMLResource* res = ENEW XMLResource(resGrp);
	ret = res;
	return ret;
}
ResourcePtr XMLImplement::Load(ResourceGroup* resGrp, FileStream stream)
{
    ResourcePtr ret;
	if (stream) {
		fseek(stream, 0, SEEK_END);
		euint length = ftell(stream);
		fseek(stream, 0, SEEK_SET);
		xhn::buffer buf(length);
		fread(buf.get(), length, 1, stream);
		XMLResource* res = ENEW XMLResource(resGrp);
		res->m_doc.load_buffer(buf.get(), length);
		ret = res;
	}
	return ret;
}

xhn::static_string XMLDetector::Detect(xhn::static_string resName, FileStream stream)
{
	xhn::string ecgResName( resName.c_str() );
	xhn::transform(ecgResName.begin(), ecgResName.end(), ecgResName.begin(), xhn::to_lower);
	xhn::string extName = ".xml";
	euint pos = ecgResName.find_last_of(extName);
	if (pos != xhn::string::npos && pos +  extName.size() == ecgResName.size())
		return xhn::static_string("XML");
	else
		return xhn::static_string("");
}
xhn::static_string XMLDetector::Detect(xhn::static_string resName)
{
	xhn::string ecgResName( resName.c_str() );
	xhn::transform(ecgResName.begin(), ecgResName.end(), ecgResName.begin(), xhn::to_lower);
	xhn::string extName = ".xml";
	euint pos = ecgResName.find_last_of(extName);
	if (pos != xhn::string::npos && pos +  extName.size() == ecgResName.size())
		return xhn::static_string("XML");
	else
		return xhn::static_string("");
}