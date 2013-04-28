#ifndef VIEWPORT_H
#define VIEWPORT_H
#include "common.h"
#include "etypes.h"
struct _viewport;
typedef struct _viewport Viewport;
#if defined(_WIN32) || defined(_WIN64)
API_EXPORT void ViewPort_Init(HINSTANCE hInstance, uint _width, uint _height);
#endif
#endif
