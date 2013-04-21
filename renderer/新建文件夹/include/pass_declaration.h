#ifndef PASS_DECLARATION_H
#define PASS_DECLARATION_H
#include "common.h"
#include "etypes.h"

typedef struct _pass_decl
{
    VertexDecl dec;
    uint usage_tex_flags;
    ///bool usage_mat_id;
} pass_decl;

typedef struct _pass_decl* PassDecl;
API_EXPORT PassDecl PassDecl_clone(PassDecl dec);
API_EXPORT int PassDecl_compare(PassDecl p0, PassDecl p1);
#endif
