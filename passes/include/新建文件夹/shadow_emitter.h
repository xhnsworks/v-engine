#ifndef SHADOW_EMITTER_H
#define SHADOW_EMITTER_H
#include "common.h"
#include "etypes.h"
#include "std_pass.h"
#include "shader_buffer.h"
API_EXPORT ShaderNode create_shadow2d_test_node();
API_EXPORT Pass create_shadow_pass(VertexDecl _dec, int num_shadow_emitters, int idx_emitter);
#endif
