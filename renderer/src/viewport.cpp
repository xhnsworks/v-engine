#include "pch.h"
#include "viewport.h"
///#include <GL/gl.h>

typedef struct _viewport
{
    euint width;
    euint height;
}viewport;

#if defined(_WIN32) || defined(_WIN64)
static viewport g_main_viewpore = {0, 0};
void ViewPort_Init(HINSTANCE hInstance, euint _width, euint _height)
{
    g_main_viewpore.width = _width;
    g_main_viewpore.height = _height;
}
#endif
