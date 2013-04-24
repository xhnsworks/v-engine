#ifndef INPUT_WIN32_H
#define INPUT_WIN32_H

#include "common.h"
#include "etypes.h"
#include "rwbuffer.h"
#include "xhn_vector.hpp"
API_EXPORT bool input_Init(HWND h);
API_EXPORT void input_Dest();

API_EXPORT void input_Proc(const xhn::vector<input_buffer>& buffer);

#endif
