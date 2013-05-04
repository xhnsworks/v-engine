#include "pch.h"
#include "shadow_renderer.h"
#include "render_system.h"
#include "gl_utility.h"
#include "renderer.h"
ShadowRenderer::ShadowRenderer()
{
    memset ( &depth_sketch_book, 0, sizeof ( depth_sketch_book ) );
    memset ( &depth_sketch_cfg, 0, sizeof ( depth_sketch_cfg ) );
    memset ( &blurred_depth_sketch_cube, 0, sizeof ( blurred_depth_sketch_cube ) );
    blurred_depth_sketch_cube_size = 0;
    memset ( &blur_pass, 0, sizeof ( blur_pass ) );
    blurred_sketch_book null_blurred_sketch_book = {NULL, NULL};
    blurred_sketch_books = array_new ( blurred_sketch_book, 3, null_blurred_sketch_book );
}
ShadowRenderer::~ShadowRenderer()
{
    if ( depth_sketch_book ) {
        SketchBook_delete ( depth_sketch_book );
    }

    if ( depth_sketch_cfg ) {
        SketchBookConfig_delete ( depth_sketch_cfg );
    }

    if ( blurred_depth_sketch_cube ) {
        SketchCube_delete ( blurred_depth_sketch_cube );
    }

    if ( blur_pass ) {
        Pass_delete ( blur_pass );
    }

    delete_blurred_sketck_books();
}
void ShadowRenderer::delete_blurred_sketck_books()
{
    if ( blurred_sketch_books ) {
        euint n = array_n ( blurred_sketch_books );

        for ( euint i = 0; i < n; i++ ) {
            blurred_sketch_book bskb = blurred_sketch_books[i];
            SketchBook_delete ( bskb.sketch_book );
            SketchBookConfig_delete ( bskb.sketch_cfg );
        }

        array_delete ( blurred_sketch_books );
        blurred_sketch_books = NULL;
    }
}
void ShadowRenderer::prepare ( RenderablePlane render_plane )
{
    if ( depth_sketch_book ) {
        SketchBook_delete ( depth_sketch_book );
    }

    if ( depth_sketch_cfg ) {
        SketchBookConfig_delete ( depth_sketch_cfg );
    }

    if ( blurred_depth_sketch_cube ) {
        SketchCube_delete ( blurred_depth_sketch_cube );
    }

    if ( blur_pass ) {
        Pass_delete ( blur_pass );
    }

    euint32 shadow_map_size = 512;

    euint32 width = shadow_map_size;
    euint32 height = shadow_map_size;

    delete_blurred_sketck_books();
    {
        SketchBookConfig cfg = SketchBookConfig_new();
        SketchBookConfig_set_size ( cfg, width, height );
        SketchBookConfig_set_sketch_format ( cfg, 0, RG32F );

        ///SketchBookConfig_set_plaster_format(cfg, DEPTH32F);
        SketchBook skb = SketchBook_new ( cfg );
        ///SketchBookConfig_delete(cfg);
        depth_sketch_book = skb;
        depth_sketch_cfg = cfg;
    }
    {
        blurred_depth_sketch_cube_size = shadow_map_size;
        SketchCube skc = SketchCube_new ( blurred_depth_sketch_cube_size, RG32F );
        ERROR_PROC;
        blurred_depth_sketch_cube = skc;
    }

    blur_pass = RenderSystem_register_blur_pass ( RenderablePlane_get_vertex_declaration ( render_plane ) );
    blurred_sketch_book null_blurred_sketch_book = {NULL, NULL};
    blurred_sketch_books = array_new ( blurred_sketch_book, 3, null_blurred_sketch_book );

    euint num_blur_passes = 1;

    for ( euint i = 0; i < num_blur_passes; i++ ) {
        SketchBookConfig cfg = SketchBookConfig_new();
        SketchBookConfig_set_size ( cfg, width, height );
        SketchBookConfig_set_sketch_format ( cfg, 0, RG32F );
        SketchBook skb = SketchBook_new ( cfg );

        blurred_sketch_book bskb = {skb, cfg};
        apush ( blurred_sketch_books, bskb );
    }
}
void ShadowRenderer::depth_map_blur ( RendererBase *rdr )
{
    SketchBook prev_skb = depth_sketch_book;

    euint num_blur_passes = array_n ( blurred_sketch_books );

    for ( euint i = 0; i < num_blur_passes; i++ ) {
        rdr->curt_color_sketch = SketchBook_get_sketch ( prev_skb, 0 );
        blurred_sketch_book bskb = blurred_sketch_books[i];
        euint32 width = SketchBookConfig_get_width ( bskb.sketch_cfg );
        euint32 height = SketchBookConfig_get_height ( bskb.sketch_cfg );

        clear_sketch_book ( bskb.sketch_book );

        SketchBook_draw_begin ( bskb.sketch_book, 1 );
        rdr->curt_color_sketch = SketchBook_get_sketch ( prev_skb, 0 );
        glViewport ( 0, 0, width, height );
        glEnable ( GL_DEPTH_TEST );
        ERROR_PROC;
        glEnable ( GL_STENCIL_TEST );
        ERROR_PROC;
        Pass_auto_set_uniform_params ( blur_pass, rdr, false );
        Pass_render_plane ( blur_pass, rdr->get_render_plane() );
        SketchBook_draw_end ( bskb.sketch_book );

        prev_skb = bskb.sketch_book;
    }
}
void ShadowRenderer::depth_map_blur_to_cube_map ( RendererBase *rdr, euint face )
{
    /// 这里目前只允许单pass模糊
    SketchBook prev_skb = depth_sketch_book;

    rdr->curt_color_sketch = SketchBook_get_sketch ( prev_skb, 0 );
    ///blurred_sketch_book bskb = _self->blurred_sketch_books[i];
    SketchCube bskc = blurred_depth_sketch_cube;

    euint32 width = blurred_depth_sketch_cube_size;
    euint32 height = blurred_depth_sketch_cube_size;

    clear_sketch_cube_face ( bskc, ( cube_face_index ) face );

    ///SketchBook_draw_begin(bskb.sketch_book, 1);
    SketchCube_draw_begin ( bskc, ( cube_face_index ) face );
    ///_self->curt_color_sketch = SketchBook_get_sketch(prev_skb, 0);
    glViewport ( 0, 0, width, height );
    glEnable ( GL_DEPTH_TEST );
    ERROR_PROC;
    glEnable ( GL_STENCIL_TEST );
    ERROR_PROC;
    Pass_auto_set_uniform_params ( blur_pass, rdr, false );
    Pass_render_plane ( blur_pass, rdr->get_render_plane() );
    SketchCube_draw_end ( bskc, ( cube_face_index ) face );
}
Texture2DPtr ShadowRenderer::get_shadow_map ( euint i )
{
    if ( i == 0 ) {
        euint num_blur_passes = array_n ( blurred_sketch_books );

        if ( num_blur_passes > 0 ) {
            blurred_sketch_book bskb = blurred_sketch_books[num_blur_passes - 1];
            return SketchBook_get_sketch ( bskb.sketch_book, 0 );
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
}