#include "pch.h"
#include "sketch.h"
#include "array.h"
#include "texture2d.h"
#include "texture_cube.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_declaration.h"
#include "vertex_element.h"
#include "list.h"
#include "gl_utility.h"
#include <stdarg.h>    ////用变参函数所必须的
#include "eassert.h"
#include "xhn_vector.hpp"

static uint32 g_render_buffer_id = 0xffffffff;

typedef struct _renderable_plane
{
    VertexDecl dec;
    VertexBuffer vbf;
    IndexBuffer ibf;
} renderable_plane;

RenderablePlane RenderablePlane_Init(struct _renderable_plane* _self, const char* _file, uint _line)
{
    /**
    _self->dec = VertexDecl_new();
    VertexElement pos = VertexElement_new(Float32_3, Position);
    VertexElement tex = VertexElement_new(Float32_2, TexCoord);
    VertexElement color = VertexElement_new(Float32_4, Color);
    VertexElement nor = VertexElement_new(Float32_3, Normal);
    VertexElement tgt = VertexElement_new(Float32_3, Tangent);
    VertexElement binor = VertexElement_new(Float32_3, Binormal);

    VertexDecl_add_element(_self->dec, pos);
    VertexDecl_add_element(_self->dec, tex);
    VertexDecl_add_element(_self->dec, color);
    VertexDecl_add_element(_self->dec, nor);
    VertexDecl_add_element(_self->dec, tgt);
    VertexDecl_add_element(_self->dec, binor);
    **/
    _self->dec = VertexDecl_new_from_string("pf3"
                                            "tf2"
                                            "cf4"
                                            "nf3"
                                            "Tf3"
                                            "Bf3");

    _self->vbf = _VertexBuffer_new(_self->dec, _file, _line);
    _self->ibf = _IndexBuffer_new(Triangular, _file, _line);

    float* fpos = (float*)VertexBuffer_insert(_self->vbf, Position, 0);
    fpos[0] = -1.0;
    fpos[1] = 1.0;
    fpos[2] = 0.0;

    fpos = (float*)VertexBuffer_insert(_self->vbf, Position, 1);
    fpos[0] = -1.0;
    fpos[1] = -1.0;
    fpos[2] = 0.0;

    fpos = (float*)VertexBuffer_insert(_self->vbf, Position, 2);
    fpos[0] = 1.0;
    fpos[1] = -1.0;
    fpos[2] = 0.0;

    fpos = (float*)VertexBuffer_insert(_self->vbf, Position, 3);
    fpos[0] = 1.0;
    fpos[1] = 1.0;
    fpos[2] = 0.0;

    float* ftex = (float*)VertexBuffer_insert(_self->vbf, TexCoord, 0);
    ftex[0] = 0.0;
    ftex[1] = 1.0;

    ftex = (float*)VertexBuffer_insert(_self->vbf, TexCoord, 1);
    ftex[0] = 0.0;
    ftex[1] = 0.0;

    ftex = (float*)VertexBuffer_insert(_self->vbf, TexCoord, 2);
    ftex[0] = 1.0;
    ftex[1] = 0.0;

    ftex = (float*)VertexBuffer_insert(_self->vbf, TexCoord, 3);
    ftex[0] = 1.0;
    ftex[1] = 1.0;

    for (uint i = 0; i < 4; i++)
    {
        float* fnor = (float*)VertexBuffer_insert(_self->vbf, Normal, i);
        fnor[0] = 0.0;
        fnor[1] = 0.0;
        fnor[2] = -1.0;
    }
    for (uint i = 0; i < 4; i++)
    {
        float* ftgt = (float*)VertexBuffer_insert(_self->vbf, Tangent, i);
        ftgt[0] = 1.0;
        ftgt[1] = 0.0;
        ftgt[2] = 0.0;
    }
    for (uint i = 0; i < 4; i++)
    {
        float* fbinor = (float*)VertexBuffer_insert(_self->vbf, Binormal, i);
        fbinor[0] = 0.0;
        fbinor[1] = 1.0;
        fbinor[2] = 0.0;
    }
    for (uint i = 0; i < 4; i++)
    {
        float* fcolor = (float*)VertexBuffer_insert(_self->vbf, Color, i);
        fcolor[0] = 1.0;
        fcolor[1] = 1.0;
        fcolor[2] = 1.0;
        fcolor[3] = 1.0;
    }

    ///uint32* idx = IndexBuffer_get_buffer(_self->ibf);
    uint32* idx = (uint32*)IndexBuffer_insert(_self->ibf, 0);
    idx[0] = 0;
    idx[1] = 1;
    idx[2] = 2;

    idx = (uint32*)IndexBuffer_insert(_self->ibf, 1);
    idx[0] = 0;
    idx[1] = 2;
    idx[2] = 3;

    RenderablePlane ret = {_self};
    return ret;
}

RenderablePlane _RenderablePlane_new(const char* _file, uint _line)
{
    renderable_plane* ret = (renderable_plane*)SMalloc(sizeof(renderable_plane));
    return RenderablePlane_Init(ret, _file, _line);
}

void RenderablePlane_delete(RenderablePlane _self)
{
    VertexBuffer_delete(_self->vbf);
    IndexBuffer_delete(_self->ibf);

    ///VertexDecl_delete(_self->dec);
    Mfree(_self);
}

VertexBuffer RenderablePlane_get_vertex_buffer(RenderablePlane _self)
{
    return _self->vbf;
}

IndexBuffer RenderablePlane_get_index_buffer(RenderablePlane _self)
{
    return _self->ibf;
}

VertexDecl RenderablePlane_get_vertex_declaration(RenderablePlane _self)
{
    return _self->dec;
}

struct sketch_book : public MemObject
{
	~sketch_book();
    ///RenderablePlane screen_plane;
    uint32 frame_buffer_id;

    ///Texture2DPtr* sketchs;
	xhn::vector<Texture2DPtr> sketchs;
    Texture2DPtr plaster;

    uint32 curt_used_skhs;
    uint32 width;
    uint32 height;
};

typedef struct _sketch_book_config
{
    uint width;
    uint height;
    pixel_format* sketch_format_array;
    bool has_plaster;
    pixel_format plaster_format;
} sketch_book_config;

typedef struct _sketch_cube
{
    ///RenderablePlane screen_plane;
    ///uint32 frame_buffer_id;
    uint32 frame_buffer_ids[6];

    TextureCube sketch;

    uint32 size;
} sketch_cube;

void SketchBookConfig_Init(SketchBookConfig _self)
{
	_self->has_plaster = false;
	_self->width = 0;
	_self->height = 0;
	_self->sketch_format_array = NULL;
	_self->plaster_format = RGBA8;
}

SketchBookConfig SketchBookConfig_new()
{
    SketchBookConfig ret;
    ret = (SketchBookConfig)SMalloc(sizeof(sketch_book_config));
	SketchBookConfig_Init(ret);
    return ret;
}
void SketchBookConfig_delete(SketchBookConfig _self)
{
    if (_self->sketch_format_array)
    {
        array_delete(_self->sketch_format_array);
    }
    Mfree(_self);
}
void SketchBookConfig_set_sketch_format(SketchBookConfig _self, uint _idx, pixel_format _fmt)
{
    if (_self->sketch_format_array)
    {
        uint n = array_n(_self->sketch_format_array);
        if (_idx >= n)
        {
            _self->sketch_format_array = array_resize(_self->sketch_format_array, _idx + 1);
        }
    }
    else
    {
        _self->sketch_format_array = array_new(pixel_format, _idx + 1, RGBA8);
        _self->sketch_format_array = array_resize(_self->sketch_format_array, _idx + 1);
    }
    uint n = array_n(_self->sketch_format_array);
    _self->sketch_format_array[_idx] = _fmt;
}

void SketchBookConfig_set_plaster_format(SketchBookConfig _self, pixel_format _fmt)
{
    _self->plaster_format = _fmt;
    _self->has_plaster = true;
}

bool SketchBookConfig_has_plaster(SketchBookConfig _self)
{
    return _self->has_plaster;
}

pixel_format SketchBookConfig_get_plaster_format(SketchBookConfig _self)
{
    return _self->plaster_format;
}

void SketchBookConfig_set_size(SketchBookConfig _self, uint _width, uint _height)
{
    if (_width > 0 && _height > 0)
    {
        _self->width = _width;
        _self->height = _height;
    }
}

uint SketchBookConfig_get_num_sketchs(SketchBookConfig _self)
{
    if (_self->sketch_format_array)
        return array_n(_self->sketch_format_array);
    else
        return 0;
}

uint SketchBookConfig_get_width(SketchBookConfig _self)
{
    return _self->width;
}

uint SketchBookConfig_get_height(SketchBookConfig _self)
{
    return _self->height;
}

pixel_format SketchBookConfig_get_sketch_format(SketchBookConfig _self, uint _idx)
{
    return array_safe_get(_self->sketch_format_array, _idx);
}

SketchBook SketchBook_new(SketchBookConfig _cfg)
{
    SketchBook ret = ENEW sketch_book;
    ///ret = (SketchBook)SMalloc(sizeof(sketch_book));

    uint width = SketchBookConfig_get_width(_cfg);
    uint height = SketchBookConfig_get_height(_cfg);
    uint n = SketchBookConfig_get_num_sketchs(_cfg);
    if (n)
    {
        ///ret->sketchs = array_new(Texture2DPtr, n, NULL);
        ///ret->sketchs = array_resize(ret->sketchs, n);
		ret->sketchs.resize(n);
        for (uint i = 0; i < n; i++)
        {
            ret->sketchs[i] = ENEW Texture2D;
            pixel_format fmt = SketchBookConfig_get_sketch_format(_cfg, i);
            ///Texture2D_create(ret->sketchs[i], fmt, width, height);
			ret->sketchs[i]->Create(fmt, width, height);
        }
    }

    if (SketchBookConfig_has_plaster(_cfg))
    {
        ret->plaster = ENEW Texture2D;
        pixel_format fmt = SketchBookConfig_get_plaster_format(_cfg);
        ///Texture2D_create(ret->plaster, fmt, width, height);
		ret->plaster->Create(fmt, width, height);
    }
    ret->width = width;
    ret->height = height;

    glGenFramebuffers(1, &ret->frame_buffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, ret->frame_buffer_id);

    if (g_render_buffer_id == 0xffffffff)
    {
        glGenRenderbuffers(1, &g_render_buffer_id);
    }
    glBindRenderbuffer(GL_RENDERBUFFER, g_render_buffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    ERROR_PROC;
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, g_render_buffer_id);
    ERROR_PROC;

    for (uint i = 0; i < n; i++)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 + i,
                               GL_TEXTURE_2D,
                               ///Texture2D_get_id(ret->sketchs[i]),
							   ret->sketchs[i]->GetID(),
                               0);
    }
    if (ret->plaster.get())
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_2D,
                               ///Texture2D_get_id(ret->plaster),
							   ret->plaster->GetID(),
                               0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    ERROR_PROC;

    printFramebufferInfo();
    bool status = checkFramebufferStatus();

	ERROR_PROC;

    EAssert(status, "%s", "OGL status error");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ret->curt_used_skhs = 0;

    return ret;
}

sketch_book::~sketch_book()
{
	glDeleteFramebuffers(1, &frame_buffer_id);
}

void SketchBook_delete(SketchBook _self)
{
    delete _self;
}

void SketchBook_draw_begin(SketchBook _self, uint _use_skhs)
{
    EAssert(!_self->curt_used_skhs, "%s", "SketchBook is binded");

    glBindFramebuffer(GL_FRAMEBUFFER, _self->frame_buffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, g_render_buffer_id);

    GLenum buffers[8];
    for (uint i = 0; i < _use_skhs; i++)
    {
        buffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    if (_use_skhs)
        glDrawBuffers(_use_skhs, buffers);
    _self->curt_used_skhs = _use_skhs;
}

void SketchBook_draw_end(SketchBook _self)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    _self->curt_used_skhs = 0;
}

void SketchBook_bind_framebuffer(SketchBook _self)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _self->frame_buffer_id);
}
void SketchBook_unbind_framebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SketchBook_bind_renderbuffer()
{
    glBindRenderbuffer(GL_RENDERBUFFER, g_render_buffer_id);
}
void SketchBook_unbind_renderbuffer()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Texture2DPtr SketchBook_get_sketch(SketchBook _self, uint _i)
{
    return _self->sketchs[_i];
}

Texture2DPtr SketchBook_get_plaster(SketchBook _self)
{
    return _self->plaster;
}

SketchCube SketchCube_new(uint size, pixel_format format)
{
    SketchCube ret;
    ret = (SketchCube)SMalloc(sizeof(sketch_cube));

    /// 这行代码很关键，原因不明
    ///glActiveTexture(GL_TEXTURE21);

    ret->sketch = TextureCube_new();
    ERROR_PROC;
    TextureCube_create(ret->sketch, format, size);
    ERROR_PROC;
    ret->size = size;

    ERROR_PROC;

    glGenFramebuffers(6, ret->frame_buffer_ids);
    for (uint i = 0; i < 6; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, ret->frame_buffer_ids[i]);

        if (g_render_buffer_id == 0xffffffff)
        {
            glGenRenderbuffers(1, &g_render_buffer_id);
        }
        glBindRenderbuffer(GL_RENDERBUFFER, g_render_buffer_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size, size);
        ERROR_PROC;

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                               TextureCube_get_id(ret->sketch),
                               0);

        ///glDrawBuffer(GL_NONE);
        ///glReadBuffer(GL_NONE);

        ///printFramebufferInfo();
        ERROR_PROC;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    ERROR_PROC;
    bool status = checkFramebufferStatus();

    EAssert(status, "%s", "OGL status error");
    ERROR_PROC;

    return ret;
}

void SketchCube_delete(SketchCube _self)
{
    glDeleteFramebuffers(6, _self->frame_buffer_ids);
    TextureCube_delete(_self->sketch);
    Mfree(_self);
}

void SketchCube_draw_begin(SketchCube _self, cube_face_index index)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _self->frame_buffer_ids[index]);
    glBindRenderbuffer(GL_RENDERBUFFER, g_render_buffer_id);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void SketchCube_draw_end(SketchCube _self, cube_face_index index)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
void SketchCube_bind_framebuffer(SketchCube _self, cube_face_index index)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _self->frame_buffer_ids[index]);
}
void SketchCube_bind_renderbuffer()
{
    glBindRenderbuffer(GL_RENDERBUFFER, g_render_buffer_id);
}
void SketchCube_unbind_framebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void SketchCube_unbind_renderbuffer()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
TextureCube SketchCube_get_sketch(SketchCube _self)
{
    return _self->sketch;
}

void clear_sketch_book ( SketchBook curt_skb )
{
    SketchBook_bind_framebuffer ( curt_skb );
    SketchBook_bind_renderbuffer();

    glClearColor ( 1.0f, 1.0f, 1.0f, 1.0f );
    glClearDepth ( 1.0 );

    glClear ( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    glDepthMask ( GL_TRUE );

    SketchBook_unbind_framebuffer();
    SketchBook_unbind_renderbuffer();
}
void clear_sketch_cube_face ( SketchCube curt_skc, cube_face_index face )
{
    SketchCube_bind_framebuffer ( curt_skc, face );
    SketchCube_bind_renderbuffer();

    glClearColor ( 1.0f, 1.0f, 1.0f, 1.0f );
    glClearDepth ( 1.0 );

    glClear ( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    glDepthMask ( GL_TRUE );

    SketchCube_unbind_framebuffer();
    SketchCube_unbind_renderbuffer();
}