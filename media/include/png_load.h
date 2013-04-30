#ifndef PNG_LOAD_H
#define PNG_LOAD_H
#include "common.h"
#include "etypes.h"
#include "image.h"
Image png_load(const char* _file_name);
Image png_load(FILE* file_stream);
#endif
