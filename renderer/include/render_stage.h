#ifndef RENDER_STAGE_H
#define RENDER_STAGE_H
#include "common.h"
#include "etypes.h"
#include "tree.h"
#include "sketch.h"
#include "pass.h"
#include "renderer.h"
#include "renderable.h"
#include "MaterialPrototype.h"

typedef void (*SetUniformParams)(Pass, MaterialPrototype, Renderer, SketchBook);
typedef void (*RendererUpdate)(Renderer);
typedef struct _render_stage
{
    SketchBook input;
    Pass render_pass;
    SketckBook output;
    SetUniformParams set_param_proc;
    RendererUpdate update;
} render_stage;
typedef struct _render_stage* RenderStage;

RenderStage RenderStage_new(Renderer owen, SetUniformParams sup, RenderUpdate ru);
void RenderStage_delete(RenderStage self);

void RenderStage_set_pass(RenderStage self, Pass p);
void RenderStage_set_input_link(RenderStage self, SketchBook skb);
void RenderStage_set_output_link(RenderStage self, SketchBook skb);

void RenderStage_render(RenderStage self);
#endif
