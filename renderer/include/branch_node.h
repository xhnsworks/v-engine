/*
* Copyright (c) 2013, �캣��
* All rights reserved.
*
* �ļ����ƣ�branch_node.h
* �ļ�����������������·��ķ�֧�ڵ�
*
* ��ǰ�汾��0.1
* ��    �ߣ��캣��
* ������ڣ�2013��1��27��
*/
#ifndef BRANCH_NODE_H
#define BRANCH_NODE_H
#include "common.h"
#include "etypes.h"
#include "circuit_board.h"
typedef struct _branch_node* BranchNode;

API_EXPORT BranchNode BranchNode_new();
API_EXPORT void BranchNode_delete(BranchNode _self);
API_EXPORT CircuitBoard BranchNode_add_branch(BranchNode _self,
                                              ShaderObject _a,
                                              compare_operation _op,
                                              ShaderObject _b);
API_EXPORT CircuitBoard BranchNode_set_else_branch(BranchNode _self);
#endif
