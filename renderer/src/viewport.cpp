#include "pch.h"
#include "viewport.h"
///#include <GL/gl.h>

typedef struct _viewport
{
    uint width;
    uint height;
}viewport;

static viewport g_main_viewpore = {0, 0};
#if defined(_WIN32) || defined(_WIN64)
void ViewPort_Init(HINSTANCE hInstance, uint _width, uint _height)
{
    g_main_viewpore.width = _width;
    g_main_viewpore.height = _height;
}
#endif
