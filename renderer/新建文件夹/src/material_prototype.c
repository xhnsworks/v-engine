#include "mem.h"
#include "material_prototype.h"
#include "material_instance.h"
#include "pass.h"
#include "display_pass.h"

MaterialPrototype MaterialPrototype_new(VertexDecl dec, sint material_id,
                                        SDisplayProc _disp_proc,
                                        draw_mode draw_mode, bool double_sided_flag)
{
    MaterialPrototype ret = Malloc(sizeof(material_prototype));
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
