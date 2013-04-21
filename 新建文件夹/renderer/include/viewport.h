#ifndef VIEWPORT_H
#define VIEWPORT_H
#include <common.h>
#include <etypes.h>
#include <windows.h>
struct _viewport;
typedef struct _viewport Viewport;
extern void ViewPort_Init(HINSTANCE hInstance, uint _width, uint _height);
#endif
