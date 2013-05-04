#include "pch.h"
#include "shader_node_base.h"
#include "emem.h"
void ShaderNodeBase_delete(ShaderNodeBase _self)
{
    _self->dest_proc(_self);
    Mfree(_self);
}
