#include "pch.h"
#include "emem.h"
#include "material_prototype.h"
#include "material_instance.h"
#include "pass.h"
#include "display_pass.h"

MaterialPrototype MaterialPrototype_new(VertexDecl dec, esint32 material_id,
                                        SDisplayProc _disp_proc,
                                        e_draw_mode draw_mode, bool double_sided_flag)
{
    MaterialPrototype ret = (MaterialPrototype)SMalloc(sizeof(material_prototype));
    ret->material_id = material_id;
    ret->disp_proc = _disp_proc.proc;
    ret->draw_mode = draw_mode;
    ret->double_sided_flag = double_sided_flag;
    return ret;
}
void MaterialPrototype_delete(MaterialPrototype self)
{
    Mfree(self);
}
