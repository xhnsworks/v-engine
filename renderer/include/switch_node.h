#ifndef SWITCH_NODE_H
#define SWITCH_NODE_H
#include "common.h"
#include "etypes.h"
#include "circuit_board.h"
typedef struct _switch_node* SwitchNode;

API_EXPORT SwitchNode _SwitchNode_new(const char* _file, uint _line);
#define SwitchNode_new() _SwitchNode_new(__FILE__, __LINE__)
API_EXPORT void _SwitchNode_delete(SwitchNode _self, const char* _file, uint _line);
#define SwitchNode_delete(s) _SwitchNode_delete(s, __FILE__, __LINE__)
API_EXPORT CircuitBoard SwitchNode_add_branch_block(SwitchNode _self, sint* _tags);
API_EXPORT void SwitchNode_set_switch_object(SwitchNode _self, ShaderObject _so);
#endif
