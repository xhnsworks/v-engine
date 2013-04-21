#include "viewport.h"
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>

typedef struct _viewport
{
    uint width;
    uint height;
}viewport;

static viewport g_main_viewpore = {0, 0};

void ViewPort_Init(HINSTANCE hInstance, uint _width, uint _height)
{
    g_main_viewpore.width = _width;
    g_main_viewpore.height = _height;
}

