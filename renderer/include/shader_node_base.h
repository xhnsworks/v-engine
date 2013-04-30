#ifndef SHADER_NODE_BASE_H
#define SHADER_NODE_BASE_H
#include "common.h"
#include "etypes.h"

typedef struct _shader_node_base
{
    shader_node_init_proc init_proc;
    shader_node_dest_proc dest_proc;
    shader_node_compile_proc compile_proc;
    shader_node_clone_proc clone_proc;
} shader_node_base;

API_EXPORT void ShaderNodeBase_delete(ShaderNodeBase _self, const char* _file, euint _line);

#define USE_RECOMPILE
#endif
