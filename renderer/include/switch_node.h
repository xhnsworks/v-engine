#ifndef SWITCH_NODE_H
#define SWITCH_NODE_H
#include "common.h"
#include "etypes.h"
#include "circuit_board.h"
typedef struct _switch_node* SwitchNode;

API_EXPORT SwitchNode SwitchNode_new();
API_EXPORT void SwitchNode_delete(SwitchNode _self);
API_EXPORT CircuitBoard SwitchNode_add_branch_block(SwitchNode _self, esint* _tags);
API_EXPORT void SwitchNode_set_switch_object(SwitchNode _self, ShaderObject _so);
#endif
