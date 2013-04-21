#include "shader_node_base.h"
#include "emem.h"
void ShaderNodeBase_delete(ShaderNodeBase _self, const char* _file, uint _line)
{
    _self->shader_node_dest_proc(_self, _file, _line);
    Mfree(_self);
}
