#include "pch.h"
#include "pass_declaration.h"
#include "emem.h"
PassDecl PassDecl_clone(PassDecl dec)
{
    PassDecl ret = (PassDecl)SMalloc(sizeof(pass_decl));
    ret->dec = dec->dec;
    ret->usage_tex_flags = dec->usage_tex_flags;
    ret->vtx_color_func = dec->vtx_color_func;
    return ret;
}

int PassDecl_compare(PassDecl p0, PassDecl p1)
{
    if      (p0->dec < p1->dec)
        return -1;
    else if (p0->dec > p1->dec)
        return 1;
    else
    {
        if      (p0->usage_tex_flags < p1->usage_tex_flags)
            return -1;
        else if (p0->usage_tex_flags > p1->usage_tex_flags)
            return 1;
        else
        {
            if      (p0->vtx_color_func < p1->vtx_color_func)
                return -1;
            else if (p0->vtx_color_func > p1->vtx_color_func)
                return 1;
            else
                return 0;
            return 0;
        }
    }
}
