#ifndef DISPLAY_PASS_H
#define DISPLAY_PASS_H
#include "common.h"
#include "etypes.h"
#include "shader.h"
#include "pass.h"
#include "shader_node.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"
#include "vertex_declaration.h"
#include "lighting.h"

extern Pass create_display_pass(VertexDeclaration _dec);
extern Pass create_normal_display_pass(VertexDeclaration _dec);
extern Pass create_depth_display_pass(VertexDeclaration _dec);
extern Pass create_light_display_pass(VertexDeclaration _dec);
extern Pass create_position_display_pass(VertexDeclaration _dec);
#endif
