#include "pch.h"
#include "texture2d_resource.h"
#include "png_load.h"

Texture2DResource::~Texture2DResource()
{
	m_tex = NULL;
}
ImplementRTTI(Texture2DResource, Resource);

Texture2DPtr Texture2DImplement::create_texture_from_file(FILE* file_stream)
{
    Image img = png_load(file_stream);

    if (to_ptr(img))
    {
        euint32 h = Image_get_num_rows(img);
        euint32 w = Image_get_row_width(img);
        pixel_format fmt = Image_get_pixel_format(img);
        Texture2DPtr tex = ENEW Texture2D;
        vptr pxls = Image_get_row(img, 0);
        ///Texture2D_load_from_mem(tex, pxls, fmt, w, h, Image_get_size_in_byte(img), true);
		tex->LoadFromMem(pxls, fmt, w, h, Image_get_size_in_byte(img), true);

        Image_delete(img);

        return tex;
    }

    return NULL;
}

Texture2DImplement::Texture2DImplement()
{
}

ResourceTypeDetectorPtr Texture2DImplement::MakeResourceTypeDetector()
{
    ResourceTypeDetectorPtr ret;
    ret = new Texture2DDetector("Texture2D");
	return ret;
}
ResourcePtr Texture2DImplement::New(ResourceGroup* resGrp, const xhn::static_string resName)
{
	ResourcePtr ret;
	Texture2DPtr tex = ENEW Texture2D;
	Texture2DResource* res = ENEW Texture2DResource(resGrp, tex);
	ret = res;
	return ret;
}
ResourcePtr Texture2DImplement::Load(ResourceGroup* resGrp, FileStream stream)
{
	ResourcePtr ret;
	Texture2DPtr tex = create_texture_from_file(stream);
	Texture2DResource* res = ENEW Texture2DResource(resGrp, tex);
	ret = res;
	return ret;
}

char toLower(char c)
{
	if (c >= 'A' && c <= 'Z')
	{
		char d = c - 'A';
		return 'a' + d;
	}
	else
		return c;
}
xhn::static_string Texture2DDetector::Detect(const xhn::string& resName, FileStream stream)
{
	xhn::string ecgResName( resName );
	xhn::transform(ecgResName.begin(), ecgResName.end(), ecgResName.begin(), toLower);
	xhn::string extName = ".png";
	euint pos = ecgResName.find_last_of(extName);
	if (pos != xhn::string::npos && pos +  extName.size() == ecgResName.size())
		return xhn::static_string("Texture2D");
	else
		return xhn::static_string("");
}
xhn::static_string Texture2DDetector::Detect(const xhn::string& resName)
{
	xhn::string ecgResName( resName );
	xhn::transform(ecgResName.begin(), ecgResName.end(), ecgResName.begin(), toLower);
	xhn::string extName = ".png";
	euint pos = ecgResName.find_last_of(extName);
	if (pos != xhn::string::npos && pos +  extName.size() == ecgResName.size())
		return xhn::static_string("Texture2D");
	else
		return xhn::static_string("");
}
DefaultTexture2DImplement::DefaultTexture2DImplement()
{
	Texture2DPtr tex = ENEW Texture2D;
	euint32 pixels[64 * 64];
	for (euint i = 0; i < 64 * 64; i++)
	{
		pixels[i] = 0xffffffff;
	}
	tex->LoadFromMem(pixels, RGBA8, 64, 64, sizeof(pixels), false);
    Texture2DResource* res = ENEW Texture2DResource(NULL, tex);
	m_defaultTexRes = res;
}
ResourceTypeDetectorPtr DefaultTexture2DImplement::MakeResourceTypeDetector()
{
    ResourceTypeDetectorPtr ret;
	ret = new DefaultTexture2DDetector("DefaultTexture2D");
	return ret;
}

ResourcePtr DefaultTexture2DImplement::New(ResourceGroup* resGrp, const xhn::static_string resName)
{
    ResourcePtr ret;
	return ret;
}
ResourcePtr DefaultTexture2DImplement::Load(ResourceGroup* resGrp, FileStream stream)
{
    return m_defaultTexRes;
}
xhn::static_string DefaultTexture2DDetector::Detect(const xhn::string& resName, FileStream stream)
{
	if (resName == "default")
	    return xhn::static_string("DefaultTexture2D");
	else
		return xhn::static_string("");
}
xhn::static_string DefaultTexture2DDetector::Detect(const xhn::string& resName)
{
	if (resName == "default")
		return xhn::static_string("DefaultTexture2D");
	else
		return xhn::static_string("");
}