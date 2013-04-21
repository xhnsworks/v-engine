#ifndef COLLADA_LOADER_H
#define COLLADA_LOADER_H
#include "common.h"
#include "etypes.h"
#include "mesh.h"
typedef struct _collada_state* ColladaState;
API_EXPORT ColladaState ColladaState_new();
API_EXPORT void ColladaState_load_mesh(ColladaState _self, const char* filepath);
API_EXPORT Mesh ColladaState_create_mesh(ColladaState _self);
API_EXPORT void ColladaState_log(ColladaState _self);
#endif // COLLADA_H
