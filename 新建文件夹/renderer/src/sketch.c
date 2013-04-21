#include "sketch.h"
#include "array.h"
#include "texture2d.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_declaration.h"
#include "vertex_element.h"
#include "list.h"
#include "gl_utility.h"
#include <stdarg.h>    ////用变参函数所必须的
#include "eassert.h"

static uint32 g_render_buffer_id = 0xffffffff;

typedef struct _renderable_plane
{
    VertexDeclaration dec;
    VertexBuffer vbf;
    IndexBuffer ibf;
} renderable_plane;

RenderablePlane RenderablePlane_Init(struct _renderable_plane* _self)
{
    _self->dec = VertexDeclaration_new();
    VertexElement pos = VertexElement_new(Float32_3, Position);
    VertexElement tex = VertexElement_new(Float32_2, TexCoord);
    VertexElement color = VertexElement_new(Float32_4, Color);
    VertexElement nor = VertexElement_new(Float32_3, Normal);
    VertexElement tgt = VertexElement_new(Float32_3, Tangent);
    VertexElement binor = VertexElement_new(Float32_3, Binormal);

    VertexDeclaration_add_element(_self->dec, pos);
    VertexDeclaration_add_element(_self->dec, tex);
    VertexDeclaration_add_element(_self->dec, color);
    VertexDeclaration_add_element(_self->dec, nor);
    VertexDeclaration_add_element(_self->dec, tgt);
    VertexDeclaration_add_element(_self->dec, binor);
    _self->vbf = VertexBuffer_new(_self->dec);
    _self->ibf = IndexBuffer_new();

    float* fpos = VertexBuffer_access(_self->vbf, Position, 0);
    fpos[0] = -1.0;
    fpos[1] = 1.0;
    fpos[2] = 0.0;

    fpos = VertexBuffer_access(_self->vbf, Position, 1);
    fpos[0] = -1.0;
    fpos[1] = -1.0;
    fpos[2] = 0.0;

    fpos = VertexBuffer_access(_self->vbf, Position, 2);
    fpos[0] = 1.0;
    fpos[1] = -1.0;
    fpos[2] = 0.0;

    fpos = VertexBuffer_access(_self->vbf, Position, 3);
    fpos[0] = 1.0;
    fpos[1] = 1.0;
    fpos[2] = 0.0;

    float* ftex = VertexBuffer_access(_self->vbf, TexCoord, 0);
    ftex[0] = 0.0;
    ftex[1] = 1.0;

    ftex = VertexBuffer_access(_self->vbf, TexCoord, 1);
    ftex[0] = 0.0;
    ftex[1] = 0.0;

    ftex = VertexBuffer_access(_self->vbf, TexCoord, 2);
    ftex[0] = 1.0;
    ftex[1] = 0.0;

    ftex = VertexBuffer_access(_self->vbf, TexCoord, 3);
    ftex[0] = 1.0;
    ftex[1] = 1.0;

    for (uint i = 0; i < 4; i++)
    {
        float* fnor = VertexBuffer_access(_self->vbf, Normal, i);
        fnor[0] = 0.0;
        fnor[1] = 0.0;
        fnor[2] = -1.0;
    }
    for (uint i = 0; i < 4; i++)
    {
        float* ftgt = VertexBuffer_access(_self->vbf, Tangent, i);
        ftgt[0] = 1.0;
        ftgt[1] = 0.0;
        ftgt[2] = 0.0;
    }
    for (uint i = 0; i < 4; i++)
    {
        float* fbinor = VertexBuffer_access(_self->vbf, Binormal, i);
        fbinor[0] = 0.0;
        fbinor[1] = 1.0;
        fbinor[2] = 0.0;
    }
    for (uint i = 0; i < 4; i++)
    {
        float* fcolor = VertexBuffer_access(_self->vbf, Color, i);
        fcolor[0] = 1.0;
        fcolor[1] = 1.0;
        fcolor[2] = 1.0;
        fcolor[3] = 1.0;
    }

    uint32* idx = IndexBuffer_get_buffer(_self->ibf);
    idx[0] = 0;
    idx[1] = 1;
    idx[2] = 2;

    idx[3] = 0;
    idx[4] = 2;
    idx[5] = 3;

    RenderablePlane ret = {_self};
    return ret;
}

RenderablePlane RenderablePlane_new()
{
    renderable_plane* ret = Malloc(sizeof(renderable_plane));
    return RenderablePlane_Init(ret);
}

void RenderablePlane_delete(RenderablePlane _self)
{
    ///VertexBuffer_Dest(to_ptr(_self.self->vbf));
    VertexBuffer_delete(_self.self->vbf);
    ///IndexBuffer_Dest(to_ptr(_self.self->ibf));
    IndexBuffer_delete(_self.self->ibf);

    VertexDeclaration_delete(_self.self->dec);
    Mfree(_self.self);
}

VertexBuffer RenderablePlane_get_vertex_buffer(RenderablePlane _self)
{
    return _self.self->vbf;
}

IndexBuffer RenderablePlane_get_index_buffer(RenderablePlane _self)
{
    return _self.self->ibf;
}

VertexDeclaration RenderablePlane_get_vertex_declaration(RenderablePlane _self)
{
    return _self.self->dec;
}

typedef struct _sketch_book
{
    RenderablePlane screen_plane;
    uint32 frame_buffer_id;
    ///uint32 render_buffer_id;
    Texture2D sketchs[4];
    ///Texture2D depth;
    uint32 curt_used_skhs;
    uint32 width;
    uint32 height;
} sketch_book;

SketchBook SketchBook_new(uint32 _width, uint32 _height)
{
    SketchBook ret;
    ret.self = Malloc(sizeof(sketch_book));
    for (uint32 i = 0; i < 4; i++)
    {
        ret.self->sketchs[i] = Texture2D_new();
        Texture2D_create(ret.self->sketchs[i], RGBA16F, _width, _height);
    }
    ret.self->width = _width;
    ret.self->height = _height;
    ///============================================================================================///

    glGenFramebuffers(1, &ret.self->frame_buffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, ret.self->frame_buffer_id);

    if (g_render_buffer_id == 0xffffffff)
    {
        glGenRenderbuffers(1, &g_render_buffer_id);
    }
    glBindRenderbuffer(GL_RENDERBUFFER, g_render_buffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, g_render_buffer_id);
    ///
    for (uint32 i = 0; i < 4; i++)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 + i,
                               GL_TEXTURE_2D,
                               Texture2D_get_id(ret.self->sketchs[i]),
                               0);
    }

    printFramebufferInfo();
    bool status = checkFramebufferStatus();

    EAssert(status, "OGL status error");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ret.self->curt_used_skhs = 0;

    return ret;
}

void SketchBook_delete(SketchBook _self)
{
    glDeleteFramebuffers(1, &_self.self->frame_buffer_id);
    for (uint32 i = 0; i < 4; i++)
    {
        Texture2D_delete(_self.self->sketchs[i]);
    }
    Mfree(_self.self);
}

void SketchBook_draw_begin(SketchBook _self, uint32 _use_skhs)
{
    EAssert(!_self.self->curt_used_skhs, "SketchBook is binded");

    glBindFramebuffer(GL_FRAMEBUFFER, _self.self->frame_buffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, g_render_buffer_id);

    GLenum buffers[_use_skhs];
    for (uint32 i = 0; i < _use_skhs; i++)
    {
        buffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glDrawBuffers(_use_skhs, buffers);

    _self.self->curt_used_skhs = _use_skhs;
}

void SketchBook_draw_end(SketchBook _self)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    _self.self->curt_used_skhs = 0;
}

Texture2D SketchBook_get_sketch(SketchBook _self, uint32 _i)
{
    return _self.self->sketchs[_i];
}
