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

extern ShaderNode create_std_vertex_shader_node(VertexDeclaration _dec, bool _use_proj_mat);
extern VertexShaderBuffer create_std_vertex_shader_buffer(VertexDeclaration _dec, bool _use_proj_mat);
extern Pass create_std_pass_from_dec(VertexDeclaration _dec, std_pass_status* _status);
#endif
