#include "pch.h"
#include "shader_node_base.h"
#include "emem.h"
void ShaderNodeBase_delete(ShaderNodeBase _self, const char* _file, euint _line)
{
    _self->dest_proc(_self, _file, _line);
    Mfree(_self);
}
