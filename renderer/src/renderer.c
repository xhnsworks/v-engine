#include "renderer.h"
#include "mem.h"
#include "array.h"
#include "camera.h"
#include "sketch.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "pass_console.h"
#include "float3.h"
#include "gl_utility.h"
#include "shader_buffer.h"
#include "renderable_base.h"
#include "shader_node_generator.h"

#include "tree.h"
#include "math_base.h"
#include "sfloat3.h"
#include "sfloat4.h"

#include "shader_test.h"

#define NUM_SKETCH_BOOKS 3

static EFloat3 g_ambi_color = {0.15f, 0.10f, 0.04f};

void _renderable_tree_dest(Tree _t)
{
    Iterator iter = Tree_begin(_t);
    while (iter)
    {
        var data;
        data = Tree_get_value(iter);
        renderable* rdl = data.vptr_var;
        VertexBuffer_delete(rdl->vtx_buf);
        IndexBuffer_delete(rdl->idx_buf);
        Iterator mesh_iter = List_begin(rdl->mesh_list);
        while (mesh_iter)
        {
            var data = List_get_value(mesh_iter);
            Mesh mesh = data.vptr_var;
            Mesh_delete(mesh);
            mesh_iter = List_next(mesh_iter);
        }
        List_Dest(rdl->mesh_list);
        Matrix4x4_delete(rdl->world_matrix);
        MaterialInstance_delete(rdl->material);
        Mfree(rdl);
        iter = Tree_next(iter);
    }
    Tree_Dest(_t);
}

void _point_light_tree_dest(Tree _t)
{
    Iterator iter = Tree_begin(_t);
    while (iter)
    {
        var data;
        data = Tree_get_value(iter);
        PointLight pt = data.vptr_var;
        PointLight_delete(pt);
        iter = Tree_next(iter);
    }
    Tree_Dest(_t);
}

void _render_one_frame_begin()
{
    glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDepthMask(GL_TRUE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0);
    glClearStencil(0);
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_BACK);
    ERROR_PROC;
}

void default_material_proc2(PixelShaderBuffer _psb, BranchNode _bn, ShaderObject _mat_id, int _id)
{
    ShaderBuffer sb = to_ShaderBuffer(_psb);

    _tex_2d_obj_test(sb, "ColorMap");
    _tex_2d_obj_test(sb, "LightingMap");
    _vec2_obj_test(sb, "vTexCoord");

    ShaderNode psn = ShaderNode_new();

    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "PixelProc%d", _id);
    ShaderNode_set_name(psn, mbuf);

    ShaderNode_set_function(psn,
                            "{\n"
                            "    vec3 cmap = texture2D(ColorMap, vTexCoord).rgb;\n"
                            "    vec3 lmap = texture2D(LightingMap, vTexCoord).rgb;\n"
                            "    gl_FragData[0] = vec4( clamp(cmap * lmap, 0.0, 1.0), 1.0 );"
                            "}\n");

    ShaderBuffer_add_prototype_node(sb, psn);
    ShaderObject imme_so = ShaderBuffer_new_immediate_int_object(sb, _id);
    CircuitBoard cb = BranchNode_add_branch(_bn, _mat_id, Equal, imme_so);
    CircuitBoard_add_reference_node(cb, psn);

    ShaderNode_delete(psn);
}

void default_point_lighting_proc2(PixelShaderBuffer _psb, ShaderNodeGenerator _sng, CircuitBoard _cb, bool _has_lighting_map)
{
    ShaderBuffer sb = to_ShaderBuffer(_psb);
    ///
    ShaderObject nor = ShaderBuffer_new_object(sb, Float4_Obj, "Normal", 1);
    ShaderObject depth = ShaderBuffer_new_object(sb, Float4_Obj, "Depth", 1);
    ShaderObject tgt_pos = ShaderBuffer_new_object(sb, Float3_Obj, "TargetPosition", 1);
    ShaderObject tgt_nor = ShaderBuffer_new_object(sb, Float3_Obj, "TargetNormal", 1);
    ShaderObject lt_value = ShaderBuffer_new_object(sb, Float3_Obj, "LightingValue", 1);
    ShaderObject nor_map = ShaderBuffer_find_object(sb, NORMAL_MAP);
    ShaderObject depth_map = ShaderBuffer_find_object(sb, DEPTH_MAP);
    ShaderObject tex_crd = ShaderBuffer_find_object(sb, TEXCOORD);
    ShaderObject width =  ShaderBuffer_find_object(sb, "CamWidth");
    ShaderObject height =  ShaderBuffer_find_object(sb, "CamHeight");
    ShaderObject near_plane =  ShaderBuffer_find_object(sb, "CamNearPlane");
    ShaderObject far_plane =  ShaderBuffer_find_object(sb, "CamFarPlane");

    ShaderObject inv_cam_world_mat =  ShaderBuffer_find_object(sb, "InvCamWorldMatrix");
    ShaderObject inv_cam_proj_mat =  ShaderBuffer_find_object(sb, "InvCamProjMatrix");

    ShaderObject pt_pos_array = ShaderBuffer_find_object(sb, POINT_LIGHT_POSITIOIN);
    ShaderObject pt_col_array = ShaderBuffer_find_object(sb, POINT_LIGHT_COLOR);
    ShaderObject pt_ac_array = ShaderBuffer_find_object(sb, POINT_LIGHT_ATTE_COEF);

    if (_has_lighting_map)
    {
        ShaderNode sampler_node = ShaderNodeGenerator_add_reference_node2(_sng, _cb, MapSampleNode);
        ShaderObject lt_map = ShaderBuffer_find_object(sb, LIGHTING_MAP);
        ShaderNode_add_input_link(sampler_node, lt_map, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(sampler_node, tex_crd, INVALID_ARRAY_INDEX);
        ShaderNode_add_output_link(sampler_node, lt_value, INVALID_ARRAY_INDEX);
    }
    else
    {
        ShaderNode clear_node = ShaderNodeGenerator_add_reference_node2(_sng, _cb, ClearLightingValueNode);
        ShaderNode_set_result_link(clear_node, lt_value, INVALID_ARRAY_INDEX);
    }
    {
        ShaderNode sampler_node = ShaderNodeGenerator_add_reference_node2(_sng, _cb, MapSampleNodeRGBA);
        ShaderNode_add_input_link(sampler_node, nor_map, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(sampler_node, tex_crd, INVALID_ARRAY_INDEX);
        ShaderNode_add_output_link(sampler_node, nor, INVALID_ARRAY_INDEX);
        sampler_node = ShaderNodeGenerator_add_reference_node2(_sng, _cb, MapSampleNodeRGBA);
        ShaderNode_add_input_link(sampler_node, depth_map, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(sampler_node, tex_crd, INVALID_ARRAY_INDEX);
        ShaderNode_add_output_link(sampler_node, depth, INVALID_ARRAY_INDEX);
    }
    {
        ///ShaderNode decode_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(pos_decode_node));
        ShaderNode decode_node = ShaderNodeGenerator_add_reference_node2(_sng, _cb, PositionDecodeNode);
        ShaderNode_add_input_link(decode_node, tex_crd, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, depth, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, near_plane, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, far_plane, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, width, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, height, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, inv_cam_world_mat, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(decode_node, inv_cam_proj_mat, INVALID_ARRAY_INDEX);
        ShaderNode_set_result_link(decode_node, tgt_pos, INVALID_ARRAY_INDEX);
    }
    {
        ///ShaderNode decode_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(nor_decode_node));
        ShaderNode decode_node = ShaderNodeGenerator_add_reference_node2(_sng, _cb, NormalDecodeNode);
        ShaderNode_add_input_link(decode_node, nor, INVALID_ARRAY_INDEX);
        ShaderNode_set_result_link(decode_node, tgt_nor, INVALID_ARRAY_INDEX);
    }
    {
        ///ShaderNode pt_array_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(point_array_lighting_node));
        ShaderNode pt_node = ShaderNodeGenerator_add_reference_node2(_sng, _cb, PointLightingNode);
        ShaderNode_add_input_link(pt_node, lt_value, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(pt_node, tgt_pos, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(pt_node, tgt_nor, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(pt_node, pt_pos_array, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(pt_node, pt_col_array, INVALID_ARRAY_INDEX);
        ShaderNode_add_input_link(pt_node, pt_ac_array, INVALID_ARRAY_INDEX);
        ShaderNode_set_result_link(pt_node, lt_value, INVALID_ARRAY_INDEX);
    }
    {
        ///ShaderNode out_node = ShaderBuffer_add_reference_node(sb, ShaderNode_get_name(output_node));
        ShaderNode out_node = ShaderNodeGenerator_add_reference_node2(_sng, _cb, LightingOutputNode);
        ShaderNode_add_input_link(out_node, lt_value, INVALID_ARRAY_INDEX);
    }
}

typedef struct _renderer_base
{
    RenderablePlane render_plane;

    Tree used_renderable_tree;
    Tree unused_renderable_tree;

    Pass display_pass;

    uint x;
    uint y;
    uint width;
    uint height;
    Camera camera;

    float cam_width;
    float cam_height;
    float cam_near_plane;
    float cam_far_plane;

    Matrix4x4 proj_matrix;
    Matrix4x4 inv_proj_matrix;

    Matrix4x4 cam_world_matrix;
    Matrix4x4 inv_cam_world_matrix;

    bool own_camera;
} renderer_base;

void RendererBase_Init(struct _renderer_base* _self, uint x, uint y, uint width, uint height,
                       const char* _file, uint _line)
{
    _self->render_plane = _RenderablePlane_new(_file, _line);
    VertexDecl pdec = RenderablePlane_get_vertex_declaration(_self->render_plane);
    /**
    if (default_proc)
        _self->display_pass = create_display_pass_ex(pdec, default_proc);
        **/
    _self->display_pass = NULL;

    _self->camera = Camera_new(width, height);
    _self->x = x;
    _self->y = y;
    _self->width = width;
    _self->height = height;
    Camera_translate(_self->camera, 0.0, 0.0, 1.5f);

    _self->proj_matrix = Camera_get_render_matrix(_self->camera);
    _self->inv_proj_matrix = Camera_get_inv_render_matrix(_self->camera);

    _self->cam_world_matrix = Camera_get_world_matrix(_self->camera);
    _self->inv_cam_world_matrix = Camera_get_inv_world_matrix(_self->camera);

    _self->used_renderable_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    _self->unused_renderable_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);

    _self->own_camera = true;
}

void RendererBase_Init_from_prev_renderer_base(struct _renderer_base* _self, renderer_base* _prev_renderer_base,
        const char* _file, uint _line)
{
    _self->render_plane = _RenderablePlane_new(_file, _line);
    VertexDecl pdec = RenderablePlane_get_vertex_declaration(_self->render_plane);
    ///_self->display_pass = create_display_pass_ex(pdec, default_proc);
    _self->display_pass = NULL;
    _self->camera = _prev_renderer_base->camera;
    _self->x = _prev_renderer_base->x;
    _self->y = _prev_renderer_base->y;
    _self->width = _prev_renderer_base->width;
    _self->height = _prev_renderer_base->height;

    _self->proj_matrix = Camera_get_render_matrix(_self->camera);
    _self->inv_proj_matrix = Camera_get_inv_render_matrix(_self->camera);

    _self->cam_world_matrix = Camera_get_world_matrix(_self->camera);
    _self->inv_cam_world_matrix = Camera_get_inv_world_matrix(_self->camera);
    _self->used_renderable_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    _self->unused_renderable_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);

    _self->own_camera = false;
}

void RendererBase_Dest(struct _renderer_base* _self)
{
    RenderablePlane_delete(_self->render_plane);
    if (_self->display_pass)
        Pass_delete(_self->display_pass);
    _renderable_tree_dest(_self->used_renderable_tree);
    _renderable_tree_dest(_self->unused_renderable_tree);

    if (_self->own_camera)
        Camera_delete(_self->camera);
}

void RendererBase_camera_update(struct _renderer_base* _self)
{
    _self->cam_width = Camera_get_width(_self->camera);
    _self->cam_height = Camera_get_height(_self->camera);
    _self->cam_near_plane = Camera_get_near_plane(_self->camera);
    _self->cam_far_plane = Camera_get_far_plane(_self->camera);
}

Renderable _RendererBase_new_renderable(struct _renderer_base* _self, VertexDecl _dec, MaterialInstance _m_inst, mesh_mode _mesh_mode,
                                        const char* _file, uint _line)
{
    Renderable ret = Malloc(sizeof(renderable));
    ret->vtx_dec = RenderSystem_register_vertex_declaration(_dec);
    ret->vtx_buf = _VertexBuffer_new(_dec, _file, _line);
    ret->idx_buf = _IndexBuffer_new(_mesh_mode, _file, _line);
    ret->mesh_list = List_new(Vptr, Ealloc, Efree);
    ret->material = MaterialInstance_clone(_m_inst);
    ret->std_pass_dirty_flag = true;
    ret->depth_pass_dirty_flag = true;
    ret->shadow_pass_dirty_flag = true;
    Matrix4x4 mat = Matrix4x4_new();

    Matrix4x4_set_one(mat);
    ret->world_matrix = mat;
    ret->ower = _self;
    var key, data;
    key.vptr_var = ret;
    data.vptr_var = ret;
    Tree_insert(_self->unused_renderable_tree, key, data);
    return ret;
}

void RendererBase_use_renderable(struct _renderer_base* _self, Renderable _rbl)
{
    var key, data;
    key.vptr_var = _rbl;
    Iterator iter = Tree_find(_self->unused_renderable_tree, key, &data);
    if (iter)
    {
        key.vptr_var = _rbl;
        data.vptr_var = _rbl;
        Tree_insert(_self->used_renderable_tree, key, data);
        Tree_remove(_self->unused_renderable_tree, key);
    }
}

typedef struct _shadow_renderer
{
    renderer_base base;
    SketchBook depth_sketch_book;
    Pass display_pass;
    Light light;
    /// light_camera �̳���base
    Camera light_camera;
    Matrix4x4 light_cam_proj_matrix;
    Matrix4x4 light_cam_world_matrix;
    /// view_camera ΪShadowRenderer�����
    Camera view_camera;
    Matrix4x4 view_cam_proj_matrix;
    Matrix4x4 view_cam_world_matrix;

    Pass shadow_pass;
} shadow_renderer;

void ShadowRenderer_Init(struct _shadow_renderer* _self, const char* _file, uint _line)
{
    RendererBase_Init(&_self->base, 0, 0, 512, 512, _file, _line);
    _self->base.display_pass = create_display_texture_pass(RenderablePlane_get_vertex_declaration(_self->base.render_plane));
    SketchBookConfig cfg = SketchBookConfig_new();
    SketchBookConfig_set_plaster_format(cfg, DEPTH32F);
    SketchBookConfig_set_size(cfg, 512, 512);
    _self->depth_sketch_book = SketchBook_new(cfg);
    SketchBookConfig_delete(cfg);
    _self->light_camera = _self->base.camera;
    _self->light_cam_proj_matrix = Camera_get_render_matrix(_self->base.camera);
    _self->light_cam_world_matrix = Camera_get_world_matrix(_self->base.camera);

    _self->view_camera = Camera_new(512, 512);
    _self->view_cam_proj_matrix = Camera_get_render_matrix(_self->view_camera);
    _self->view_cam_world_matrix = Camera_get_world_matrix(_self->view_camera);
    Camera_translate(_self->view_camera, 0.0f, 0.0f, 2.0f);
    ///shadow_pass = create_shadow_pass()
}

void ShadowRenderer_Dest(struct _shadow_renderer* _self)
{
    RendererBase_Dest(&_self->base);
    SketchBook_delete(_self->depth_sketch_book);
    if (to_ptr(_self->view_camera))
        Camera_delete(_self->view_camera);
}

ShadowRenderer _ShadowRenderer_new(Light _lt, const char* _file, uint _line)
{
    ShadowRenderer ret = _Malloc(sizeof(shadow_renderer), _file, _line);
    ShadowRenderer_Init(ret, _file, _line);
    ret->light = _lt;
    return ret;
}

void ShadowRenderer_delete(ShadowRenderer _self)
{
    ShadowRenderer_Dest(_self);
    Mfree(_self);
}

Renderable _ShadowRenderer_new_renderable(ShadowRenderer _self, VertexDecl _dec, MaterialInstance _m_inst, mesh_mode _mesh_mode,
        const char* _file, uint _line)
{
    return _RendererBase_new_renderable(&_self->base, _dec, _m_inst, _mesh_mode, _file, _line);
}

void ShadowRenderer_use_renderable(ShadowRenderer _self, Renderable _rbl)
{
    RendererBase_use_renderable(&_self->base, _rbl);
}

void ShadowRenderer_render(ShadowRenderer _self)
{
    _render_one_frame_begin();

    if (!Tree_count(_self->base.used_renderable_tree))
        return;

    light_type t = Light_get_type(_self->light);
    if (Point == t)
    {
        PointLight pt = to_point_light(_self->light);
        EFloat3 pos = PointLight_get_position(pt);
        EFloat3 look_at = {0.0f, 0.0f, 0.0f};
        Camera_set_position(_self->light_camera, pos);
        Camera_look_at(_self->light_camera, look_at);
    }

    ///RendererBase_camera_update(_self);

    SketchBook prev_render_skb = {NULL};
    Pass depth_pass = {NULL};
    Pass shadow_pass = {NULL};
    prev_render_skb = _self->depth_sketch_book;

    glViewport(0, 0, _self->base.width, _self->base.height);

    SketchBook_bind_framebuffer(prev_render_skb);
    SketchBook_bind_renderbuffer();
    glClearDepth(1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    SketchBook_unbind_framebuffer();
    SketchBook_unbind_renderbuffer();

    /// Ԥ��Ⱦ�׶�
    Iterator r_iter = Tree_begin(_self->base.used_renderable_tree);
    while (r_iter)
    {
        var key, data;
        data = Tree_get_value(r_iter);
        Renderable rbl = data.vptr_var;

        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
        glDisable(GL_CULL_FACE);
        ERROR_PROC;

        SketchBook_draw_begin(prev_render_skb, 0);
        ERROR_PROC;

        Renderable_depth_prev_render(rbl);
        depth_pass = rbl->depth_pass;

        Pass_set_uniform_param_mat4x4(depth_pass, "ProjMatrix", _self->light_cam_proj_matrix);

        EFloat3 pos = Camera_get_position(_self->light_camera);
        EFloat3 dir = Camera_get_direction(_self->light_camera);
        Pass_set_uniform_param_float3(depth_pass, CAMERA_POSITION, pos);
        Pass_set_uniform_param_float3(depth_pass, CAMERA_DIRECTION, dir);
        Pass_set_uniform_param_float(depth_pass, "MaterialID", 255.0f);
        Pass_set_uniform_param_mat4x4(depth_pass, "CameraWorldMatrix", _self->light_cam_world_matrix);

        ///Matrix4x4_log(_self->light_cam_proj_matrix);
        ///Matrix4x4_log(_self->light_cam_world_matrix);

        uint face_count = IndexBuffer_get_num_faces(rbl->idx_buf);
        mesh_mode mode = IndexBuffer_get_mesh_mode(rbl->idx_buf);

        if (mode == Triangular)
            Pass_render(depth_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 3, mode);
        else
            Pass_render(depth_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 2, mode);

        ERROR_PROC;
        SketchBook_draw_end(prev_render_skb);
        ERROR_PROC;

        r_iter = Tree_next(r_iter);
    }

    glDisable(GL_DEPTH_TEST);
    ERROR_PROC;
    glDisable(GL_STENCIL_TEST);
    ERROR_PROC;

    if (to_ptr(_self->view_camera))
    {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE);
        ///SketchBook_bind_framebuffer(prev_render_skb);
        ///SketchBook_bind_renderbuffer();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ///SketchBook_unbind_framebuffer();
        ///SketchBook_unbind_renderbuffer();

        /// Ԥ��Ⱦ�׶�
        Iterator r_iter = Tree_begin(_self->base.used_renderable_tree);
        while (r_iter)
        {
            var key, data;
            data = Tree_get_value(r_iter);
            Renderable rbl = data.vptr_var;

            glPolygonMode(GL_FRONT, GL_FILL);

            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
            glEnable(GL_CULL_FACE);
            ERROR_PROC;

            ///SketchBook_draw_begin(prev_render_skb, 0, true);
            ERROR_PROC;

            Renderable_shadow_prev_render(rbl);
            shadow_pass = rbl->shadow_pass;

            Pass_set_uniform_param_mat4x4(shadow_pass, "ProjMatrix", _self->view_cam_proj_matrix);

            EFloat3 pos = Camera_get_position(_self->light_camera);
            EFloat3 dir = Camera_get_direction(_self->light_camera);
            Pass_set_uniform_param_float3(shadow_pass, CAMERA_POSITION, pos);
            Pass_set_uniform_param_float3(shadow_pass, CAMERA_DIRECTION, dir);

            Pass_set_uniform_param_mat4x4(shadow_pass, "CameraWorldMatrix", _self->view_cam_world_matrix);

            Pass_set_uniform_param_tex2d(shadow_pass, "UnifDepthMap", SketchBook_get_plaster(prev_render_skb));
            Pass_set_uniform_param_mat4x4(shadow_pass, "UnifLightWorldMatrix", _self->light_cam_world_matrix);
            Pass_set_uniform_param_mat4x4(shadow_pass, "UnifLightProjectionMatrix", _self->light_cam_proj_matrix);

            uint face_count = IndexBuffer_get_num_faces(rbl->idx_buf);
            mesh_mode mode = IndexBuffer_get_mesh_mode(rbl->idx_buf);

            if (mode == Triangular)
                Pass_render(shadow_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 3, mode);
            else
                Pass_render(shadow_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 2, mode);

            ERROR_PROC;
            ///SketchBook_draw_end(prev_render_skb);
            ERROR_PROC;

            r_iter = Tree_next(r_iter);
        }

        glDisable(GL_DEPTH_TEST);
        ERROR_PROC;
        glDisable(GL_STENCIL_TEST);
        ERROR_PROC;
    }
    else
    {
        /// ����׶�
        Pass_set_uniform_param_tex2d(_self->base.display_pass, "DepthMap", SketchBook_get_plaster(prev_render_skb));
        Pass_set_uniform_param_float3(_self->base.display_pass, "AmbiColor", g_ambi_color);
        glViewport(_self->base.x, _self->base.y, _self->base.width, _self->base.height);
        Pass_render_plane(_self->base.display_pass, _self->base.render_plane);
    }
}

typedef struct _renderer
{
    renderer_base base;

    Renderer prev_renderer;

    Tree param_proc_tree;

    SketchBook lighting_sketch_book_array[NUM_SKETCH_BOOKS];

    Tree point_light_tree;
    ///Pass lighting_pass_array[2];

    Tree material_table;
    sint material_count;

    SketchBook curt_geom_buffer;

    MaterialInstance curt_mat_inst;
    MaterialPrototype curt_mat_proto;
    PointLight curt_point_light;
    Texture2D curt_plaster;
    Texture2D curt_color_sketch;
    Texture2D curt_normal_sketch;
    Texture2D curt_lighting_sketch;
    Texture2D curt_material_id_sketch;

    Pass first_lighting_pass;
    Pass lighting_pass;

    bool dirty_flag;

    bool clear_color_buffer_flag;
    bool clear_depth_buffer_flag;
} renderer;

renderer_param_value getCameraPositionProc(Renderer _self)
{
    renderer_param_value ret;
    EFloat3* tmp = Malloc(sizeof(EFloat3));
    *tmp = Camera_get_position(_self->base.camera);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraDirectionProc(Renderer _self)
{
    renderer_param_value ret;
    EFloat3* tmp = Malloc(sizeof(EFloat3));
    *tmp = Camera_get_direction(_self->base.camera);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraNearPlaneProc(Renderer _self)
{
    renderer_param_value ret;
    float* tmp = Malloc(sizeof(float));
    *tmp = Camera_get_near_plane(_self->base.camera);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraFarPlaneProc(Renderer _self)
{
    renderer_param_value ret;
    float* tmp = Malloc(sizeof(float));
    *tmp = Camera_get_far_plane(_self->base.camera);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraPlaneWidthProc(Renderer _self)
{
    renderer_param_value ret;
    float* tmp = Malloc(sizeof(float));
    *tmp = Camera_get_width(_self->base.camera);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraPlaneHeightProc(Renderer _self)
{
    renderer_param_value ret;
    float* tmp = Malloc(sizeof(float));
    *tmp = Camera_get_height(_self->base.camera);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraProjectionMatrixProc(Renderer _self)
{
    renderer_param_value ret;
    Matrix4x4 tmp = Matrix4x4_new();
    Matrix4x4_assign(tmp, Camera_get_render_matrix(_self->base.camera));
    ret.value = tmp;;
    return ret;
}
renderer_param_value getCameraWorldMatrixProc(Renderer _self)
{
    renderer_param_value ret;
    Matrix4x4 tmp = Matrix4x4_new();
    Matrix4x4_assign(tmp, Camera_get_world_matrix(_self->base.camera));
    ret.value = tmp;
    return ret;
}
renderer_param_value getInvertCameraProjectionMatrixProc(Renderer _self)
{
    renderer_param_value ret;
    Matrix4x4 tmp = Matrix4x4_new();
    Matrix4x4_assign(tmp, Camera_get_inv_render_matrix(_self->base.camera));
    ret.value = tmp;
    return ret;
}
renderer_param_value getInvertCameraWorldMatrixProc(Renderer _self)
{
    renderer_param_value ret;
    Matrix4x4 tmp = Matrix4x4_new();
    Matrix4x4_assign(tmp, Camera_get_inv_world_matrix(_self->base.camera));
    ret.value = tmp;
    return ret;
}
renderer_param_value getCurrentMaterialIDProc(Renderer _self)
{
    renderer_param_value ret;
    float* tmp = Malloc(sizeof(float));
    MaterialPrototype mp = _self->curt_mat_proto;
    *tmp = (float)mp->material_id;
    ret.value = tmp;
    return ret;
}
renderer_param_value getColorMap0Proc(Renderer _self)
{
    renderer_param_value ret;
    Texture2D* tmp = Malloc(sizeof(Texture2D));
    *tmp = MaterialInstance_get_color_texture(_self->curt_mat_inst);
    ret.value = tmp;
    return ret;
}
renderer_param_value getNormalMap0Proc(Renderer _self)
{
    renderer_param_value ret;
    Texture2D* tmp = Malloc(sizeof(Texture2D));
    *tmp = MaterialInstance_get_normal_texture(_self->curt_mat_inst);
    ret.value = tmp;
    return ret;
}
renderer_param_value getPlasterProc(Renderer _self)
{
    renderer_param_value ret;
    Texture2D* tmp = Malloc(sizeof(Texture2D));
    *tmp = _self->curt_plaster;
    ret.value = tmp;
    return ret;
}
renderer_param_value getColorSketchProc(Renderer _self)
{
    renderer_param_value ret;
    Texture2D* tmp = Malloc(sizeof(Texture2D));
    *tmp = _self->curt_color_sketch;
    ret.value = tmp;
    return ret;
}
renderer_param_value getNormalSketchProc(Renderer _self)
{
    renderer_param_value ret;
    Texture2D* tmp = Malloc(sizeof(Texture2D));
    *tmp = _self->curt_normal_sketch;
    ret.value = tmp;
    return ret;
}
renderer_param_value getMaterialIDSketchProc(Renderer _self)
{
    renderer_param_value ret;
    Texture2D* tmp = Malloc(sizeof(Texture2D));
    *tmp = _self->curt_material_id_sketch;
    ret.value = tmp;
    return ret;
}
renderer_param_value getLightingSketchProc(Renderer _self)
{
    renderer_param_value ret;
    Texture2D* tmp = Malloc(sizeof(Texture2D));
    *tmp = _self->curt_lighting_sketch;
    ret.value = tmp;
    return ret;
}
renderer_param_value getPointLightPositionProc(Renderer _self)
{
    renderer_param_value ret;
    EFloat3* tmp = Malloc(sizeof(EFloat3));
    *tmp = PointLight_get_position(_self->curt_point_light);
    ret.value = tmp;
    return ret;
}
renderer_param_value getPointLightColorProc(Renderer _self)
{
    renderer_param_value ret;
    EFloat4* tmp = Malloc(sizeof(EFloat4));
    *tmp = PointLight_get_color(_self->curt_point_light);
    ret.value = tmp;
    return ret;
}
renderer_param_value getPointLightAtteCoefProc(Renderer _self)
{
    renderer_param_value ret;
    EFloat* tmp = Malloc(sizeof(EFloat));
    *tmp = PointLight_get_atte_coef(_self->curt_point_light);
    ret.value = tmp;
    return ret;
}

RendererParamEntry RendererParamEntry_new(GetRendererParamProc _proc, param_type _type, sint _array_size)
{
    RendererParamEntry ret = Malloc(sizeof(renderer_param_entry));
    ret->get_value_proc = _proc;
    ret->type = _type;
    ret->array_size = _array_size;
    return ret;
}
void RendererParamEntry_delete(RendererParamEntry _self)
{
    Mfree(_self);
}

void _Renderer_common_Init(renderer* _self)
{
    _self->param_proc_tree = Tree_new(Sint32, Vptr, Ealloc, Efree);
    {
        var key, data;
        key.sint32_var = CameraPosition;               data.vptr_var = RendererParamEntry_new(getCameraPositionProc, Float32x3_Param, 1);               Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = CameraDirection;              data.vptr_var = RendererParamEntry_new(getCameraDirectionProc, Float32x3_Param, 1);              Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = CameraNearPlane;              data.vptr_var = RendererParamEntry_new(getCameraNearPlaneProc, Float32_Param, 1);                Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = CameraFarPlane;               data.vptr_var = RendererParamEntry_new(getCameraFarPlaneProc, Float32_Param, 1);                 Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = CameraPlaneWidth;             data.vptr_var = RendererParamEntry_new(getCameraPlaneWidthProc, Float32_Param, 1);               Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = CameraPlaneHeight;            data.vptr_var = RendererParamEntry_new(getCameraPlaneHeightProc, Float32_Param, 1);              Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = CameraProjectionMatrix;       data.vptr_var = RendererParamEntry_new(getCameraProjectionMatrixProc, Matrix4x4_Param, 1);       Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = CameraWorldMatrix;            data.vptr_var = RendererParamEntry_new(getCameraWorldMatrixProc, Matrix4x4_Param, 1);            Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = InvertCameraProjectionMatrix; data.vptr_var = RendererParamEntry_new(getInvertCameraProjectionMatrixProc, Matrix4x4_Param, 1); Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = InvertCameraWorldMatrix;      data.vptr_var = RendererParamEntry_new(getInvertCameraWorldMatrixProc, Matrix4x4_Param, 1);      Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = CurrentMaterialID;            data.vptr_var = RendererParamEntry_new(getCurrentMaterialIDProc, Float32_Param, 1);              Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = ColorMap0;                    data.vptr_var = RendererParamEntry_new(getColorMap0Proc, Texture2D_Param, 1);                    Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = NormalMap0;                   data.vptr_var = RendererParamEntry_new(getNormalMap0Proc, Texture2D_Param, 1);                   Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = Plaster;                      data.vptr_var = RendererParamEntry_new(getPlasterProc, Texture2D_Param, 1);                      Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = ColorSketch;                  data.vptr_var = RendererParamEntry_new(getColorSketchProc, Texture2D_Param, 1);                  Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = NormalSketch;                 data.vptr_var = RendererParamEntry_new(getNormalSketchProc, Texture2D_Param, 1);                 Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = MaterialIDSketch;             data.vptr_var = RendererParamEntry_new(getMaterialIDSketchProc, Texture2D_Param, 1);             Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = LightingSketch;               data.vptr_var = RendererParamEntry_new(getLightingSketchProc, Texture2D_Param, 1);               Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = PointLightPosition;           data.vptr_var = RendererParamEntry_new(getPointLightPositionProc, Float32x3_Param, 1);           Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = PointLightColor;              data.vptr_var = RendererParamEntry_new(getPointLightColorProc, Float32x4_Param, 1);              Tree_insert(_self->param_proc_tree, key, data);
        key.sint32_var = PointLightAtteCoef;           data.vptr_var = RendererParamEntry_new(getPointLightAtteCoefProc, Float32_Param, 1);             Tree_insert(_self->param_proc_tree, key, data);
    }

    memset(_self->lighting_sketch_book_array, 0, sizeof(_self->lighting_sketch_book_array));

    _self->point_light_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    VertexDecl pdec = RenderablePlane_get_vertex_declaration(_self->base.render_plane);

    _self->material_table = Tree_new(String, Vptr, Ealloc, Efree);
    var key, data;
    key.str_var = "default_material";
    MaterialPrototype mp;
    {
        ShaderNodeGenerator sng = ShaderNodeGenerator_new();
        ShaderNodeGenerator_register_default_nodes(sng);
        ///_self->lighting_pass_array[0] = create_lighting_pass_ex(sng, default_point_lighting_proc, pdec, false);
        ///_self->lighting_pass_array[1] = create_lighting_pass_ex(sng, default_point_lighting_proc, pdec, true);

        mp = MaterialPrototype_new(pdec, 2,
                                   default_material_proc2,
                                   Shaded, false);
        ShaderNodeGenerator_delete(sng);
    }
    data.vptr_var = mp;
    Tree_insert(_self->material_table, key, data);
    _self->material_count = 3;
    _self->dirty_flag = true;
    _self->clear_color_buffer_flag = true;
    _self->clear_depth_buffer_flag = true;

    _self->first_lighting_pass = NULL;
    _self->lighting_pass = NULL;
    ///
    ///_lightint_proc_test(RenderablePlane_get_vertex_declaration(_self->base.render_plane));
    ///
}

void Renderer_Init_from_prev_renderer(renderer* _self, Renderer prev_renderer, const char* _file, uint _line)
{
    RendererBase_Init_from_prev_renderer_base(&_self->base, &prev_renderer->base, _file, _line);
    _Renderer_common_Init(_self);
    _self->prev_renderer = prev_renderer;
}

void Renderer_Init(renderer* _self, uint x, uint y, uint width, uint height, const char* _file, uint _line)
{
    RendererBase_Init(&_self->base, x, y, width, height, _file, _line);
    _Renderer_common_Init(_self);
    _self->prev_renderer = NULL;
}

void Renderer_Dest(renderer* _self)
{
    RendererBase_Dest(&_self->base);
    if (!_self->prev_renderer)
    {
        for (uint i = 0; i < NUM_SKETCH_BOOKS; i++)
        {
            SketchBook skb = _self->lighting_sketch_book_array[i];
            if (skb)
                SketchBook_delete(skb);
        }
    }

    if (_self->first_lighting_pass)
        Pass_delete(_self->first_lighting_pass);
    if (_self->lighting_pass)
        Pass_delete(_self->lighting_pass);

    Iterator iter = Tree_begin(_self->material_table);
    while (iter)
    {
        var data = Tree_get_value(iter);
        MaterialPrototype mp = data.vptr_var;
        MaterialPrototype_delete(mp);
        iter = Tree_next(iter);
    }
    Tree_Dest(_self->material_table);

    _point_light_tree_dest(_self->point_light_tree);

    iter = Tree_begin(_self->param_proc_tree);
    while (iter)
    {
        var data = Tree_get_value(iter);
        RendererParamEntry entry = data.vptr_var;
        RendererParamEntry_delete(entry);
        iter = Tree_next(iter);
    }
    Tree_Dest(_self->param_proc_tree);
}

Renderer _Renderer_new(uint x, uint y, uint width, uint height, const char* _file, uint _line)
{
    Renderer ret;
    ret = _Malloc(sizeof(renderer), _file, _line);
    Renderer_Init(ret, x, y, width, height, _file, _line);
    return ret;
}

Renderer _Renderer_new_from_prev_renderer(Renderer prev_renderer, const char* _file, uint _line)
{
    Renderer ret;
    ret = _Malloc(sizeof(renderer), _file, _line);
    Renderer_Init_from_prev_renderer(ret, prev_renderer, _file, _line);
    return ret;
}

void Renderer_delete(Renderer _self)
{
    Renderer_Dest(_self);
    Mfree(_self);
}

PointLight Renderer_add_point_light(Renderer _self)
{
    PointLight ret = PointLight_new();

    var key, data;
    key.vptr_var = ret;
    data.vptr_var = ret;
    Tree_insert(_self->point_light_tree, key, data);
    return ret;
}

void Renderer_register_material(Renderer _self, const char* mat_name, DisplayProc disp_proc, draw_mode draw_mode, bool double_sided_flag)
{
    var key, data;
    key.str_var = mat_name;
    Iterator iter = Tree_find(_self->material_table, key, &data);
    if (!iter)
    {
        VertexDecl dec = RenderablePlane_get_vertex_declaration(_self->base.render_plane);
        MaterialPrototype mp;
        {
            ShaderNodeGenerator sng = ShaderNodeGenerator_new();
            ShaderNodeGenerator_register_default_nodes(sng);
            mp = MaterialPrototype_new(dec, _self->material_count,
                                       disp_proc,
                                       draw_mode, double_sided_flag);
            ShaderNodeGenerator_delete(sng);
        }
        data.vptr_var = mp;
        Tree_insert(_self->material_table, key, data);
        _self->material_count++;
    }
}

sint Renderer_get_material_id(Renderer _self, const char* mat_name)
{
    var key, data;
    key.str_var = mat_name;
    Iterator iter = Tree_find(_self->material_table, key, &data);
    if (iter)
    {
        MaterialPrototype mp = data.vptr_var;
        return mp->material_id;
    }
    else
        return -1;
}

void Renderer_render(Renderer _self)
{
    _render_one_frame_begin();

    if (!Tree_count(_self->base.used_renderable_tree))
        return;

    if (_self->dirty_flag)
    {
        if (!_self->prev_renderer)
        {
            for (uint i = 0; i < NUM_SKETCH_BOOKS; i++)
            {
                SketchBook skb = _self->lighting_sketch_book_array[i];
                if (to_ptr(skb))
                    SketchBook_delete(skb);
            }

            for (uint i = 0; i < NUM_SKETCH_BOOKS; i++)
            {
                SketchBookConfig cfg = SketchBookConfig_new();
                SketchBookConfig_set_size(cfg, _self->base.width, _self->base.height);

                SketchBookConfig_set_sketch_format(cfg, 0, ALPHA8);
                SketchBookConfig_set_sketch_format(cfg, 1, RGB8);
                SketchBookConfig_set_sketch_format(cfg, 2, RGBA16F);

                SketchBookConfig_set_plaster_format(cfg, DEPTH32F);
                SketchBook skb = SketchBook_new(cfg);
                SketchBookConfig_delete(cfg);
                _self->lighting_sketch_book_array[i] = skb;
            }
        }
        else
        {
            for (uint i = 0; i < NUM_SKETCH_BOOKS; i++)
            {
                _self->lighting_sketch_book_array[i] = _self->prev_renderer->lighting_sketch_book_array[i];
            }
        }
        /// generate display pass
        {
            if (_self->base.display_pass)
                Pass_delete(_self->base.display_pass);
            Iterator mat_iter = Tree_begin(_self->material_table);
            material_decl null_mat_decl = {NULL, -1};
            material_decl* decls = array_new(material_decl, Tree_count(_self->material_table), null_mat_decl);
            while (mat_iter)
            {
                var data = Tree_get_value(mat_iter);
                MaterialPrototype mp = data.vptr_var;
                material_decl d = {mp->disp_proc, mp->material_id};
                apush(decls, d);
                mat_iter = Tree_next(mat_iter);
            }
            VertexDecl vtx_decl = RenderablePlane_get_vertex_declaration(_self->base.render_plane);
            _self->base.display_pass = create_display_pass_ex2(vtx_decl, decls);

            array_delete(decls);
        }
        /// generate lighting pass
        {
            if (_self->first_lighting_pass)
                Pass_delete(_self->first_lighting_pass);
            if (_self->lighting_pass)
                Pass_delete(_self->lighting_pass);

            Iterator mat_iter = Tree_begin(_self->material_table);
            lighting_decl null_lt_decl = {NULL, -1};
            lighting_decl* decls = array_new(lighting_decl, Tree_count(_self->material_table), null_lt_decl);
            while (mat_iter)
            {
                var data = Tree_get_value(mat_iter);
                MaterialPrototype mp = data.vptr_var;
                lighting_decl d = {default_point_lighting_proc2, mp->material_id};
                apush(decls, d);
                mat_iter = Tree_next(mat_iter);
            }

            VertexDecl vtx_decl = RenderablePlane_get_vertex_declaration(_self->base.render_plane);
            _self->first_lighting_pass = create_lighting_pass_ex2(_self, vtx_decl, decls, false);
            _self->lighting_pass = create_lighting_pass_ex2(_self, vtx_decl, decls, true);

            array_delete(decls);
        }
    }

    RendererBase_camera_update(&_self->base);

    SketchBook prev_render_skb = {NULL};
    Pass std_pass = {NULL};
    prev_render_skb = _self->lighting_sketch_book_array[0];

    glViewport(0, 0, _self->base.width, _self->base.height);

    SketchBook_bind_framebuffer(prev_render_skb);
    SketchBook_bind_renderbuffer();
    uint clear_flag = 0;
    if (_self->clear_color_buffer_flag)
        clear_flag |= GL_COLOR_BUFFER_BIT;
    if (_self->clear_depth_buffer_flag)
        clear_flag |= GL_DEPTH_BUFFER_BIT;
    if (clear_flag)
        glClear(clear_flag);
    glDepthMask(GL_TRUE);
    SketchBook_unbind_framebuffer();
    SketchBook_unbind_renderbuffer();

    ///
    Iterator r_iter = Tree_begin(_self->base.used_renderable_tree);
    uint count = 0;
    while (r_iter)
    {
        var key, data;
        data = Tree_get_value(r_iter);
        Renderable rbl = data.vptr_var;

        const char* mat_name = MaterialInstance_get_material_name(rbl->material);

        key.str_var = mat_name;
        Iterator iter = Tree_find(_self->material_table, key, &data);
        if (!iter)
        {
            continue;
        }

        MaterialPrototype mp = data.vptr_var;

        if (mp->draw_mode == Shaded)
            glPolygonMode(GL_FRONT, GL_FILL);
        else
            glPolygonMode(GL_FRONT, GL_LINE);
        ERROR_PROC;

        if (mp->double_sided_flag)
            glDisable(GL_CULL_FACE);
        else
            glEnable(GL_CULL_FACE);
        ERROR_PROC;

        SketchBook_draw_begin(prev_render_skb, 3);

        Renderable_prev_render(rbl);
        std_pass = rbl->std_pass;

        _self->curt_mat_proto = mp;
        _self->curt_mat_inst = rbl->material;
        Pass_auto_set_uniform_params(std_pass, _self);

        uint face_count = IndexBuffer_get_num_faces(rbl->idx_buf);
        mesh_mode mode = IndexBuffer_get_mesh_mode(rbl->idx_buf);
        if (mode == Triangular)
            Pass_render(std_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 3, mode);
        else
            Pass_render(std_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 2, mode);

        ERROR_PROC;
        SketchBook_draw_end(prev_render_skb);
        ERROR_PROC;

        count++;
        r_iter = Tree_next(r_iter);
    }

    _self->curt_normal_sketch = SketchBook_get_sketch(prev_render_skb, 2);
    _self->curt_plaster = SketchBook_get_plaster(prev_render_skb);
    _self->curt_material_id_sketch = SketchBook_get_sketch(prev_render_skb, 0);

    Iterator iter = NULL;

    ///
    glDisable(GL_DEPTH_TEST);
    ERROR_PROC;
    glDisable(GL_STENCIL_TEST);
    ERROR_PROC;

    _self->curt_geom_buffer = prev_render_skb;

    SketchBook curt_skb = _self->lighting_sketch_book_array[1];
    SketchBook prev_skb = _self->lighting_sketch_book_array[2];

    void point_lighting_render(Pass lighting_pass, PointLight light, SketchBook curt_skb, Texture2D lighting_map)
    {
        SketchBook_draw_begin(curt_skb, 1);
        _self->curt_lighting_sketch = lighting_map;

        Pass_auto_set_uniform_params(lighting_pass, _self);

        ERROR_PROC;
        Pass_render_plane(lighting_pass, _self->base.render_plane);
        ERROR_PROC;
        SketchBook_draw_end(curt_skb);
        ERROR_PROC;
    }

    _self->curt_material_id_sketch = SketchBook_get_sketch(prev_render_skb, 0);

    uint32 light_count = 0;

    Iterator light_iter = Tree_begin(_self->point_light_tree);
    while (light_iter)
    {
        var data = Tree_get_value(light_iter);
        PointLight pl = data.vptr_var;

        _self->curt_point_light = pl;

        if (light_count != 0)
        {
            Pass lighting_pass = _self->lighting_pass;
            point_lighting_render(lighting_pass, pl, curt_skb, SketchBook_get_sketch(prev_skb, 0));
        }
        else
        {
            Pass lighting_pass = _self->first_lighting_pass;
            point_lighting_render(lighting_pass, pl, curt_skb, NULL);
        }

        SketchBook tmp = prev_skb;
        prev_skb = curt_skb;
        curt_skb = tmp;

        light_iter = Tree_next(light_iter);
        light_count++;
    }

    glDisable(GL_DEPTH_TEST);
    ERROR_PROC;
    glDisable(GL_STENCIL_TEST);
    ERROR_PROC;

    ///
    _self->curt_mat_proto = NULL;
    _self->curt_material_id_sketch = SketchBook_get_sketch(prev_render_skb, 0);
    _self->curt_color_sketch = SketchBook_get_sketch(prev_render_skb, 1);
    _self->curt_normal_sketch = SketchBook_get_sketch(prev_render_skb, 2);
    _self->curt_lighting_sketch = SketchBook_get_sketch(prev_skb, 0);

    Pass_auto_set_uniform_params(_self->base.display_pass, _self);
    glViewport(_self->base.x, _self->base.y, _self->base.width, _self->base.height);
    Pass_render_plane(_self->base.display_pass, _self->base.render_plane);

    _self->dirty_flag = false;
}

Renderable _Renderer_new_renderable(Renderer _self, VertexDecl _dec, MaterialInstance _m_inst, mesh_mode _mesh_mode,
                                    const char* _file, uint _line)
{
    return _RendererBase_new_renderable(&_self->base, _dec, _m_inst, _mesh_mode, _file, _line);
}

void Renderer_use_renderable(Renderer _self, Renderable _rbl)
{
    RendererBase_use_renderable(_self, _rbl);
}

Camera Renderer_get_camera(Renderer _self)
{
    return _self->base.camera;
}

void Renderer_viewport_refresh(Renderer _self, uint x, uint y, uint width, uint height)
{
    Matrix4x4 mat = Matrix4x4_new();
    Camera cam = _self->base.camera;
    float near = Camera_get_near_plane(cam);
    float far = Camera_get_far_plane(cam);

    Matrix4x4_projection(mat, 1.0f, (float)width / (float)height, near, far);
    Camera_set_proj_matrix(cam, mat);
    Matrix4x4_delete(mat);

    _self->base.x = x;
    _self->base.y = y;
    _self->base.width = width;
    _self->base.height = height;
    _self->dirty_flag = true;
}

void Renderer_set_clear_color_buffer(Renderer _self, bool flag)
{
    _self->clear_color_buffer_flag = flag;
}

void Renderer_set_clear_depth_buffer(Renderer _self, bool flag)
{
    _self->clear_depth_buffer_flag = flag;
}

void Renderer_draw_point_light_shape(Renderer _self, DrawPointLightShapeProc proc)
{
    Iterator iter = Tree_begin(_self->point_light_tree);
    while (iter)
    {
        var data = Tree_get_value(iter);
        PointLight pl = data.vptr_var;
        proc(pl);
        iter = Tree_next(iter);
    }
}

void Renderer_get_mouse_ray(Renderer _self, int x, int y, EFloat3* ori, EFloat3* dir)
{
    int window_width, window_height;
    RenderSystem_get_window_width_height(&window_width, &window_height);

    int totel_x = x - (int)_self->base.x;
    int totel_y = y - (int)_self->base.y;

    *ori = Camera_screen_position_to_world_position(_self->base.camera, (float)totel_x, (float)totel_y);
    sfloat3 s_ori = SFloat3_assign_from_efloat3(ori);
    EFloat3 cam_pos = Camera_get_position(_self->base.camera);
    sfloat3 s_cam_pos = SFloat3_assign_from_efloat3(&cam_pos);

    sfloat3 s_dir = SFloat3_sub(s_ori, s_cam_pos);
    s_dir = SFloat3_normalize(s_dir);
    *dir = SFloat3_convert(s_dir);
}

void shader_object_value_Dest(renderer_param_value _obj)
{
    if (_obj.value)
        Mfree(_obj.value);
}

bool Renderer_is_uniform_param_source(Renderer _self, sint32 _src)
{
    var key, data;
    key.sint32_var = _src;
    if (Tree_find(_self->param_proc_tree, key, &data))
        return true;
    else
        return false;
}
renderer_param_value Renderer_get_shader_object_value(Renderer _self, sint32 _src)
{
    var key, data;
    key.sint32_var = _src;
    if (Tree_find(_self->param_proc_tree, key, &data))
    {
        ///GetRendererParamProc grpp = data.vptr_var;
        RendererParamEntry entry = data.vptr_var;
        return entry->get_value_proc(_self);
    }
    else
    {
        renderer_param_value null_ret = {NULL, Void_Obj, -1};
        return null_ret;
    }
}

void Renderer_register_renderer_param(Renderer _self, sint32 _id, param_type _type, sint _array_size, GetRendererParamProc _proc)
{
    var key, data;
    key.sint32_var = _id; data.vptr_var = RendererParamEntry_new(_proc, _type, _array_size);
    Tree_insert(_self->param_proc_tree, key, data);
}
RendererParamEntry Renderer_get_param_entry(Renderer _self, sint32 _id)
{
    var key, data;
    key.sint32_var = _id;
    if (Tree_find(_self->param_proc_tree, key, &data))
    {
        RendererParamEntry entry = data.vptr_var;
        return entry;
    }
    else
    {
        return NULL;
    }
}
