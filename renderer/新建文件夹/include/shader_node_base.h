#ifndef SHADER_NODE_BASE_H
#define SHADER_NODE_BASE_H
#include "common.h"
#include "etypes.h"

typedef struct _shader_node_base
{
    shader_node_init_proc shader_node_init_proc;
    shader_node_dest_proc shader_node_dest_proc;
    compile_proc compile_proc;
    shader_node_clone_proc shader_node_clone_proc;
} shader_node_base;

API_EXPORT void ShaderNodeBase_delete(ShaderNodeBase _self, const char* _file, uint _line);
#endif
