/*
* Copyright (c) 2013, �캣��
* All rights reserved.
*
* �ļ����ƣ�shadow_emitter.h
* �ļ���������Ӱ�����������Բ�����Ӱ���Խڵ�
*
* ��ǰ�汾��0.1
* ��    �ߣ��캣��
* ������ڣ�2013��1��27��
*/
#ifndef SHADOW_EMITTER_H
#define SHADOW_EMITTER_H
#include "common.h"
#include "etypes.h"
#include "std_pass.h"
#include "shader_buffer.h"
API_EXPORT ShaderNode create_shadow2d_test_node();
API_EXPORT ShaderNode create_shadowcube_test_node();
API_EXPORT ShaderNode create_white_screen_node();
API_EXPORT Pass create_shadow_pass(VertexDecl _dec, int num_shadow_emitters, int idx_emitter);
#endif
