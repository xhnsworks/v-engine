#ifndef MATERIAL_PROTOTYPE_H
#define MATERIAL_PROTOTYPE_H
#include "common.h"
#include "etypes.h"
#include "shader_object.h"
#include "vertex_declaration.h"
#include "pixel_shader_buffer.h"
#include "shader_node_generator.h"
#include "circuit_board.h"
#include "branch_node.h"
#include "pass.h"

typedef void (*LightingProc)(PxlSdrBuf, SdrNdGen, bool);
typedef void (*DisplayProc)(PxlSdrBuf, BranchNode, ShaderObject, int);
typedef ShaderNode (*DisplayProc2)(PxlSdrBuf, int);
typedef struct _s_display_proc
{
    DisplayProc2 proc;
} SDisplayProc;

typedef void (*LightingProc2)(vptr, PxlSdrBuf, SdrNdGen, CircuitBoard, bool);

typedef struct _lighting_decl
{
    LightingProc2 light_proc;
    int mat_id;
} lighting_decl;

typedef struct _material_decl
{
    DisplayProc2 disp_proc;
    int mat_id;
} material_decl;

typedef struct _material_prototype
{
    sint material_id;
    DisplayProc disp_proc;
    draw_mode draw_mode;
    bool double_sided_flag;
} material_prototype;
typedef material_prototype* MaterialPrototype;

API_EXPORT MaterialPrototype MaterialPrototype_new(VertexDecl dec, sint material_id,
                                                   SDisplayProc _disp_proc,
                                                   draw_mode draw_mode, bool double_sided_flag);
API_EXPORT void MaterialPrototype_delete(MaterialPrototype self);
#endif
