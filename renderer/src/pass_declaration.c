#include "pass_declaration.h"
#include "mem.h"
PassDecl PassDecl_clone(PassDecl dec)
{
    PassDecl ret = Malloc(sizeof(pass_decl));
    ret->dec = dec->dec;
    ret->map_flags = dec->map_flags;
    ret->use_mat_id = dec->use_mat_id;
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
        if      (p0->map_flags < p1->map_flags)
            return -1;
        else if (p0->map_flags > p1->map_flags)
            return 1;
        else
        {
            if      (p0->use_mat_id < p1->use_mat_id)
                return -1;
            else if (p0->use_mat_id > p1->use_mat_id)
                return 1;
            else
                return 0;
        }
    }
}
