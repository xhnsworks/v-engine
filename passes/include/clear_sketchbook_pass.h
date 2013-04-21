#ifndef CLEAR_SKETCHBOOK_PASS_H
#define CLEAR_SKETCHBOOK_PASS_H
#include "common.h"
#include "etypes.h"
#include "shader.h"
#include "pass.h"
#include "shader_node.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"
#include "vertex_declaration.h"
#include "lighting.h"

API_EXPORT Pass create_clear_sketchbook_pass(VertexDecl _decl);

#endif // CLEAR_SKETCHBOOK_PASS_H
