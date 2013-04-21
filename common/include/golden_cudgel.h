#ifndef GOLDEN_CUDGEL_H
#define GOLDEN_CUDGEL_H
#include "common.h"
#include "etypes.h"
typedef struct _golden_cudgel* GoldenCudgel;

API_EXPORT GoldenCudgel GoldenCudgel_new(uint ele_size);
API_EXPORT void GoldenCudgel_delete(GoldenCudgel self);
API_EXPORT uint GoldenCudgel_get_element_size(GoldenCudgel self);
API_EXPORT void GoldenCudgel_grow_up(GoldenCudgel self, vptr eles, uint ele_count);
API_EXPORT void GoldenCudgel_grow_up_not_fill(GoldenCudgel self, uint ele_count);
API_EXPORT void GoldenCudgel_cut_down(GoldenCudgel self, uint ele_count);
API_EXPORT vptr GoldenCudgel_get_element_pointer(GoldenCudgel self, uint i);
API_EXPORT void GoldenCudgel_grow_to(GoldenCudgel self, vptr to);
#endif
