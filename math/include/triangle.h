#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "float_base.h"
#include "matrix4x4.h"
struct _triangle;
typedef struct _triangle
{
    EFloat3 vertex0;
    EFloat3 vertex1;
    EFloat3 vertex2;
} triangle;
typedef struct _triangle* Triangle;

API_EXPORT bool FORCE_ALIGN_ARG_POINTER Triangle_point_is_in_triangle(Triangle self, EFloat3* point);
#endif
