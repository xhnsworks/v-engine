#include "lighting_model.h"
#include "mem.h"
LightingModelRendererParamTable LightingModelRendererParamTable_new()
{
    LightingModelRendererParamTable ret = Malloc(sizeof(lighting_model_renderer_param_table));
    ret->param_table = Tree_new(Sint32, String, Ealloc, Efree);
    return ret;
}
void LightingModelRendererParamTable_delete(LightingModelRendererParamTable _self)
{
    Tree_Dest(_self->param_table);
    Mfree(_self);
}
