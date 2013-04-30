#include "texture_utility.h"

euint32 _get_pixel_size(pixel_format _fmt)
{
	switch (_fmt)
	{
	case RGBA8:
		return 4;
	case RGB8:
		return 3;
	case RGBA32F:
		return sizeof(float) * 4;
	case RG32F:
		return sizeof(float) * 2;
	case RG16:
		return sizeof(euint16) * 2;
	case RGB32F:
		return sizeof(float) * 3;
	case RGBA16F:
	case RGBA16:
		return sizeof(euint16) * 4;
	case RGB16F:
	case RGB16:
		return sizeof(euint16) * 3;
	case ALPHA8:
		return sizeof(euint8);
	default:
		return UINT32_EXCE;
	}
}