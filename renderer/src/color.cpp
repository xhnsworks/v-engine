#include "color.h"
void EColor::Convert(vptr _dst, pixel_format _fmt) const
{
    switch (_fmt)
	{
	case RGBA8:
		{
			unsigned char* dst = (unsigned char*)_dst;
			dst[0] = (unsigned char)(red * 255.0f);
			dst[1] = (unsigned char)(green * 255.0f);
			dst[2] = (unsigned char)(blue * 255.0f);
			dst[3] = (unsigned char)(alpha * 255.0f);
		}
		break;
	case RGB8:
		{
			unsigned char* dst = (unsigned char*)_dst;
			dst[0] = (unsigned char)(red * 255.0f);
			dst[1] = (unsigned char)(green * 255.0f);
			dst[2] = (unsigned char)(blue * 255.0f);
		}
		break;
	case RGBA32F:
		{
			float* dst = (float*)_dst;
			dst[0] = red;
			dst[1] = green;
			dst[2] = blue;
			dst[3] = alpha;
		}
		break;
	case RG32F:
		{
			float* dst = (float*)_dst;
			dst[0] = red;
			dst[1] = green;
		}
		break;
	case RG16:
		{
			unsigned short* dst = (unsigned short*)_dst;
			dst[0] = (unsigned short)(red * 65535.0f);
			dst[1] = (unsigned short)(green * 65535.0f);
		}
		break;
	case RGB32F:
		{
			float* dst = (float*)_dst;
			dst[0] = red;
			dst[1] = green;
			dst[2] = blue;
		}
		break;
	case RGBA16F:
		{
			/// implement me
		}
		break;
	case RGB16F:
		{
			/// implement me
		}
		break;
	case DEPTH32F:
		{
			float* dst = (float*)_dst;
			dst[0] = alpha;
		}
		break;
	case ALPHA8:
		{
			unsigned char* dst = (unsigned char*)_dst;
			dst[0] = (unsigned char)(alpha * 255.0f);
		}
		break;
	case RGBA16:
		{
			unsigned short* dst = (unsigned short*)_dst;
			dst[0] = (unsigned short)(red * 65535.0f);
			dst[1] = (unsigned short)(green * 65535.0f);
			dst[2] = (unsigned short)(blue * 65535.0f);
			dst[3] = (unsigned short)(alpha * 65535.0f);
		}
		break;
	case RGB16:
		{
			unsigned short* dst = (unsigned short*)_dst;
			dst[0] = (unsigned short)(red * 65535.0f);
			dst[1] = (unsigned short)(green * 65535.0f);
			dst[2] = (unsigned short)(blue * 65535.0f);
		}
		break;
	default:
		break;
	}
}