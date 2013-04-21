#ifndef PASS_CONSOLE_H
#define PASS_CONSOLE_H
#include "common.h"
#include "etypes.h"
#include "shader.h"
#include "pass.h"
#include "shader_node.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"
#include "vertex_declaration.h"
#include "std_material.h"

#define CAMERA_POSITION "CameraPosition"
#define NORMAL_MAP      "NormalMap"
#define POSITION_MAP    "PositionMap"
#define LIGHTING_MAP    "LightingMap"
#define TEXCOORD        "vTexcoord"
#define COLOR_OUT       "gl_FragData[0]"

extern Shader create_shader_from_file(const char* _file_name, shader_type _type);
extern Pass create_pass_from_shader(Shader _vtx_sdr, Shader _pxl_sdr);
///extern Pass create_std_pass();

///extern Pass create_lighting_pass(VertexDeclaration _dec);

///extern Pass create_first_lighting_pass(VertexDeclaration _dec);

extern ShaderNode create_map_sample3_node();
extern ShaderNode create_map_sample4_node();
extern ShaderNode create_clear_lighting_value_node();
#endif
