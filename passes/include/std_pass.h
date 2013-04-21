/*
* Copyright (c) 2013, �캣��
* All rights reserved.
*
* �ļ����ƣ�std_pass.h
* �ļ���������׼pass
*
* ��ǰ�汾��0.1
* ��    �ߣ��캣��
* ������ڣ�2013��1��27��
*/
#ifndef STD_PASS_H
#define STD_PASS_H
#include "common.h"
#include "etypes.h"
#include "shader.h"
#include "pass.h"
#include "shader_node.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"
#include "vertex_declaration.h"
#include "lighting.h"

#include "pass_declaration.h"
struct std_pass_status
{
    pass_decl dec;
    uint num_render_pipes;
    bool use_proj_mat;
    bool use_world_mat;
    bool use_gausscian_blur;
    render_pipe render_pipes[4];
	std_pass_status()
		: num_render_pipes(0)
		, use_proj_mat(false)
		, use_world_mat(false)
		, use_gausscian_blur(false)
	{}
};

API_EXPORT ShaderNode create_std_vertex_shader_node(VertexDecl _dec, VtxSdrBuf _vsb,  
													bool _use_proj_mat, bool _use_world_mat);
API_EXPORT VtxSdrBuf create_std_vertex_shader_buffer(VertexDecl _dec, bool _use_proj_mat, bool _use_camera_world_mat);
API_EXPORT Pass create_std_pass_from_dec(std_pass_status* _status, bool write_log);

API_EXPORT const char* get_render_target_string(render_target _tgt);
#endif
