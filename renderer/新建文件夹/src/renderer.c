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
#include "shader_object.h"
#include "exception.h"

#include "render_system.h"
#include "lighting_model.h"

#define NUM_LIGHTING_SKETCH_BOOKS 2

/**< 渲染器基类数据结构 */
typedef struct _renderer_base
{
    RenderablePlane render_plane;

    Tree used_renderable_tree;
    Tree unused_renderable_tree;

    Pass display_pass;
    Pass clear_sketchbook_pass;

    uint x;
    uint y;
    uint width;
    uint height;
    Camera camera_base;

    Camera curt_render_cam;

    bool own_camera;
} renderer_base;

typedef struct _blurred_sketch_book
{
    SketchBook sketch_book;
    SketchBookConfig sketch_cfg;
} blurred_sketch_book;
/**<  渲染器数据结构*/
typedef struct _renderer
{
    renderer_base base;
    Renderer prev_renderer;

    Tree param_proc_tree;

    SketchBook std_sketch_book;
    SketchBook lighting_sketch_book_array[NUM_LIGHTING_SKETCH_BOOKS];

    SketchBook depth_sketch_book;
    SketchBookConfig depth_sketch_cfg;
    Pass blur_pass;
    /**
    SketchBook blurred_depth_sketch_book;
    SketchBookConfig blurred_depth_sketch_book_cfg;
    **/
    blurred_sketch_book* blurred_sketch_books;

    Tree dir_light_tree2;
    Tree spot_light_tree2;

    Tree material_table;
    sint material_count;

    Matrix4x4 curt_rend_world_matrix;

    MaterialInstance curt_mat_inst;
    MaterialPrototype curt_mat_proto;

    Texture2D curt_plaster;
    Texture2D curt_color_sketch;
    Texture2D curt_normal_sketch;
    Texture2D curt_lighting_sketch;
    Texture2D curt_material_id_sketch;

    Texture2D curt_diffuse_lighting_sketch;
    Texture2D curt_specular_lighting_sketch;

    LightBase2 curt_light;

    Pass first_dir_lighting_pass;
    Pass dir_lighting_pass;
    Pass first_spot_lighting_pass;
    Pass spot_lighting_pass;

    SketchBook prev_render_skb;
    SketchBook prev_lighting_skb;

    bool dirty_flag;

    bool clear_color_buffer_flag;
    bool clear_depth_buffer_flag;

    bool use_deferred_shading;
} renderer;

/** \brief 销毁renderable的树，结构必须是 <Renderable, Renderable>
 *
 * \param 欲销毁的树
 * \param
 * \return 无返回值
 *
 */
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

/** \brief LightBase树的销毁
 *
 * \param LightBase树
 * \param
 * \return 无返回值
 *
 */

void _light_base_2_tree_dest(Tree _t)
{
    Iterator iter = Tree_begin(_t);
    while (iter)
    {
        var data;
        data = Tree_get_value(iter);
        LightBase2 pt = data.vptr_var;
        LightBase2_delete(pt);
        iter = Tree_next(iter);
    }
    Tree_Dest(_t);
}
/** \brief 在每帧的渲染之前执行的状态
 *
 * \param
 * \param
 * \return 无返回值
 *
 */

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

/** \brief 生成材质的Proc函数
 *
 * \param _psb 用于生成材质的像素着色器缓冲
 * \param _id 材质id
 * \return 材质对应的着色器节点
 *
 */

ShaderNode default_material_proc3(PxlSdrBuf _psb, int _id)
{
    ShaderBuffer sb = to_ShaderBuffer(_psb);

    _tex_2d_obj_test(sb, "ColorMap");
    _tex_2d_obj_test(sb, "DiffuseLightingMap");
    _tex_2d_obj_test(sb, "SpecularLightingMap");
    _vec2_obj_test(sb, "vTexCoord");

    ShaderNode psn = ShaderNode_new();

    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "PixelProc%d", _id);
    ShaderNode_set_name(psn, mbuf);

    ShaderNode_set_function(psn,
                            "{\n"
                            "    vec3 cmap = texture(ColorMap, vTexCoord).rgb;\n"
                            "    vec3 dmap = texture(DiffuseLightingMap, vTexCoord).rgb;\n"
                            "    vec3 smap = texture(SpecularLightingMap, vTexCoord).rgb;\n"
                            "    gl_FragData[0] = vec4( clamp(cmap * dmap + smap, 0.0, 1.0), 1.0 );"
                            "}\n");

    return psn;
}

void RendererBase_Init(struct _renderer_base* _self, uint x, uint y, uint width, uint height,
                       const char* _file, uint _line)
{
    _self->render_plane = _RenderablePlane_new(_file, _line);
    VertexDecl pdec = RenderablePlane_get_vertex_declaration(_self->render_plane);

    _self->display_pass = NULL;
    _self->clear_sketchbook_pass = create_clear_sketchbook_pass(pdec);

    _self->camera_base = Camera_new(width, height);
    _self->x = x;
    _self->y = y;
    _self->width = width;
    _self->height = height;
    Camera_translate(_self->camera_base, 0.0, 0.0, 1.5f);

    _self->curt_render_cam = _self->camera_base;

    _self->used_renderable_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    _self->unused_renderable_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);

    _self->own_camera = true;
}

void RendererBase_Init_from_prev_renderer_base(struct _renderer_base* _self, renderer_base* _prev_renderer_base,
        const char* _file, uint _line)
{
    _self->render_plane = _RenderablePlane_new(_file, _line);
    VertexDecl pdec = RenderablePlane_get_vertex_declaration(_self->render_plane);

    _self->display_pass = NULL;
    _self->clear_sketchbook_pass = NULL;

    _self->camera_base = _prev_renderer_base->camera_base;
    _self->x = _prev_renderer_base->x;
    _self->y = _prev_renderer_base->y;
    _self->width = _prev_renderer_base->width;
    _self->height = _prev_renderer_base->height;

    _self->curt_render_cam = _self->camera_base;

    _self->used_renderable_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);
    _self->unused_renderable_tree = Tree_new(Vptr, Vptr, Ealloc, Efree);

    _self->own_camera = false;
}

void RendererBase_Dest(struct _renderer_base* _self)
{
    RenderablePlane_delete(_self->render_plane);
    if (_self->display_pass)
        Pass_delete(_self->display_pass);
    if (_self->clear_sketchbook_pass)
        Pass_delete(_self->clear_sketchbook_pass);

    _renderable_tree_dest(_self->used_renderable_tree);
    _renderable_tree_dest(_self->unused_renderable_tree);

    if (_self->own_camera)
        Camera_delete(_self->camera_base);
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

renderer_param_value getCameraPositionProc(Renderer _self)
{
    renderer_param_value ret;
    EFloat3* tmp = Malloc(sizeof(EFloat3));
    *tmp = Camera_get_position(_self->base.curt_render_cam);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraDirectionProc(Renderer _self)
{
    renderer_param_value ret;
    EFloat3* tmp = Malloc(sizeof(EFloat3));
    *tmp = Camera_get_direction(_self->base.curt_render_cam);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraPlaneNearProc(Renderer _self)
{
    renderer_param_value ret;
    float* tmp = Malloc(sizeof(float));
    *tmp = Camera_get_plane_near(_self->base.curt_render_cam);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraPlaneFarProc(Renderer _self)
{
    renderer_param_value ret;
    float* tmp = Malloc(sizeof(float));
    *tmp = Camera_get_plane_far(_self->base.curt_render_cam);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraPlaneWidthProc(Renderer _self)
{
    renderer_param_value ret;
    float* tmp = Malloc(sizeof(float));
    *tmp = Camera_get_width(_self->base.curt_render_cam);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraPlaneHeightProc(Renderer _self)
{
    renderer_param_value ret;
    float* tmp = Malloc(sizeof(float));
    *tmp = Camera_get_height(_self->base.curt_render_cam);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraProjectionMatrixProc(Renderer _self)
{
    renderer_param_value ret;
    Matrix4x4 tmp = Matrix4x4_new();
    Matrix4x4_assign(tmp, Camera_get_render_matrix(_self->base.curt_render_cam));
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraWorldMatrixProc(Renderer _self)
{
    renderer_param_value ret;
    Matrix4x4 tmp = Matrix4x4_new();
    Matrix4x4_assign(tmp, Camera_get_world_matrix(_self->base.curt_render_cam));
    ret.value = tmp;
    return ret;
}
renderer_param_value getRenderableWorldMatrixProc(Renderer _self)
{
    renderer_param_value ret;
    Matrix4x4 tmp = Matrix4x4_new();
    Matrix4x4_assign(tmp, _self->curt_rend_world_matrix);
    Matrix4x4_turn(tmp);
    ret.value = tmp;
    return ret;
}
renderer_param_value getInvertCameraProjectionMatrixProc(Renderer _self)
{
    renderer_param_value ret;
    Matrix4x4 tmp = Matrix4x4_new();
    Matrix4x4_assign(tmp, Camera_get_inv_render_matrix(_self->base.curt_render_cam));
    ret.value = tmp;
    return ret;
}
renderer_param_value getInvertCameraWorldMatrixProc(Renderer _self)
{
    renderer_param_value ret;
    Matrix4x4 tmp = Matrix4x4_new();
    Matrix4x4_assign(tmp, Camera_get_inv_world_matrix(_self->base.curt_render_cam));
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
renderer_param_value getLightInnerCos(Renderer _self)
{
    renderer_param_value ret;
    float* tmp = Malloc(sizeof(float));
    *tmp = LightBase2_get_inner_cos(_self->curt_light);
    ret.value = tmp;
    return ret;
}
renderer_param_value getLightOuterCos(Renderer _self)
{
    renderer_param_value ret;
    float* tmp = Malloc(sizeof(float));
    *tmp = LightBase2_get_outer_cos(_self->curt_light);
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
renderer_param_value getDiffuseLightingSketchProc(Renderer _self)
{
    renderer_param_value ret;
    Texture2D* tmp = Malloc(sizeof(Texture2D));
    *tmp = _self->curt_diffuse_lighting_sketch;
    ret.value = tmp;
    return ret;
}
renderer_param_value getSpecularLightingSketchProc(Renderer _self)
{
    renderer_param_value ret;
    Texture2D* tmp = Malloc(sizeof(Texture2D));
    *tmp = _self->curt_specular_lighting_sketch;
    ret.value = tmp;
    return ret;
}
renderer_param_value getColorSketchPixelSizeProc(Renderer _self)
{
    renderer_param_value ret;
    EFloat2* tmp = Malloc(sizeof(EFloat2));
    uint width = Texture2D_get_width(_self->curt_color_sketch);
    uint height = Texture2D_get_height(_self->curt_color_sketch);
    tmp->x = 1.0f / (float)width;
    tmp->y = 1.0f / (float)height;
    ret.value = tmp;
    return ret;
}

renderer_param_value getLightPosition(Renderer _self)
{
    renderer_param_value ret;
    EFloat3* tmp = Malloc(sizeof(EFloat3));
    *tmp = LightBase2_get_pos(_self->curt_light);
    ret.value = tmp;
    return ret;
}
renderer_param_value getLightDirection(Renderer _self)
{
    renderer_param_value ret;
    EFloat3* tmp = Malloc(sizeof(EFloat3));
    *tmp = LightBase2_get_dir(_self->curt_light);
    ret.value = tmp;
    return ret;
}
renderer_param_value getLightColor(Renderer _self)
{
    renderer_param_value ret;
    EFloat4* tmp = Malloc(sizeof(EFloat4));
    *tmp = LightBase2_get_color(_self->curt_light);
    ret.value = tmp;
    return ret;
}
renderer_param_value getLightAtteCoef(Renderer _self)
{
    renderer_param_value ret;
    float* tmp = Malloc(sizeof(float));
    *tmp = LightBase2_get_atte_coef(_self->curt_light);
    ret.value = tmp;
    return ret;
}
renderer_param_value getLightNumShadowEmitters(Renderer _self)
{
    renderer_param_value ret;
    int* tmp = Malloc(sizeof(int));
    *tmp = LightBase2_get_num_shadow_emitters(_self->curt_light);
    ret.value = tmp;
    return ret;
}

renderer_param_value getLightProjMatrix0(Renderer _self)
{
    renderer_param_value ret;
    Matrix4x4 tmp = NULL;
    Matrix4x4 proj_mat = LightBase2_get_proj_matrix(_self->curt_light, 0);
    if (proj_mat)
    {
        tmp = Matrix4x4_new();
        Matrix4x4_assign(tmp, proj_mat);
    }
    ret.value = tmp;
    return ret;
}

#define DefGetLightProjMatrix(i) \
renderer_param_value getLightProjMatrix##i(Renderer _self) \
{ \
    renderer_param_value ret; \
    Matrix4x4 tmp = NULL; \
    Matrix4x4 proj_mat = LightBase2_get_proj_matrix(_self->curt_light, i); \
    if (proj_mat) { \
        tmp = Matrix4x4_new(); \
        Matrix4x4_assign(tmp, proj_mat); \
    } \
    ret.value = tmp; \
    return ret; \
}

///DefGetLightProjMatrix(0);
DefGetLightProjMatrix(1);
DefGetLightProjMatrix(2);
DefGetLightProjMatrix(3);
DefGetLightProjMatrix(4);
DefGetLightProjMatrix(5);

#define DefGetLightWorldMatrix(i) \
renderer_param_value getLightWorldMatrix##i(Renderer _self) \
{ \
    renderer_param_value ret; \
    Matrix4x4 tmp = NULL; \
    Matrix4x4 world_mat = LightBase2_get_world_matrix(_self->curt_light, i); \
    if (world_mat) { \
        tmp = Matrix4x4_new(); \
        Matrix4x4_assign(tmp, world_mat); \
    } \
    ret.value = tmp; \
    return ret; \
}
DefGetLightWorldMatrix(0);
DefGetLightWorldMatrix(1);
DefGetLightWorldMatrix(2);
DefGetLightWorldMatrix(3);
DefGetLightWorldMatrix(4);
DefGetLightWorldMatrix(5);

#define DefGetShadowMap(i) \
renderer_param_value getShadowMap##i(Renderer _self) \
{ \
    renderer_param_value ret; \
    Texture2D* tmp = NULL; \
    Texture2D depth_map = Renderer_get_shadow_map(_self, i); \
    if (depth_map) { \
        tmp = Malloc(sizeof(Texture2D)); \
        *tmp = depth_map; \
    } \
    ret.value = tmp; \
    return ret; \
}
DefGetShadowMap(0);
DefGetShadowMap(1);
DefGetShadowMap(2);
DefGetShadowMap(3);
DefGetShadowMap(4);
DefGetShadowMap(5);

#define DefGetShadowDir(i) \
renderer_param_value getShadowDir##i(Renderer _self) \
{ \
    renderer_param_value ret; \
    EFloat3* tmp = Malloc(sizeof(EFloat3)); \
    *tmp = LightBase2_get_shadow_dir(_self->curt_light, i); \
    ret.value = tmp; \
    return ret; \
}
DefGetShadowDir(0);
DefGetShadowDir(1);
DefGetShadowDir(2);
DefGetShadowDir(3);
DefGetShadowDir(4);
DefGetShadowDir(5);

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

const char* _get_param_src_str(param_source _src)
{
    switch (_src)
    {
    case EmptySrc:
        return "EmptySrc";
    case VaryPosition:
        return "VaryPosition";
    case VaryTexCoord:
        return "VaryTexCoord";
    case VaryNormal:
        return "VaryNormal";
    case VaryTangent:
        return "VaryTangent";
    case VaryBinormal:
        return "VaryBinormal";
    case CameraPosition:
        return "CameraPosition";
    case CameraDirection:
        return "CameraDirection";
    case CameraPlaneNear:
        return "CameraPlaneNear";
    case CameraPlaneFar:
        return "CameraPlaneFar";
    case CameraPlaneWidth:
        return "CameraPlaneWidth";
    case CameraPlaneHeight:
        return "CameraPlaneHeight";
    case CameraProjectionMatrix:
        return "CameraProjectionMatrix";
    case CameraWorldMatrix:
        return "CameraWorldMatrix";
    case InvertCameraProjectionMatrix:
        return "InvertCameraProjectionMatrix";
    case InvertCameraWorldMatrix:
        return "InvertCameraProjectionMatrix";

    case LightInnerCos:
        return "LightInnerCos";
    case LightOuterCos:
        return "LightOuterCos";

    case LightPosition:
        return "LightPosition";
    case LightDirection:
        return "LightDirection";
    case LightColor:
        return "LightColor";
    case LightAtteCoef:
        return "LightAtteCoef";
    case LightNumEmitters:
        return "LightNumEmitters";
    case LightProjectionMatrix0:
        return "LightProjectionMatrix0";
    case LightProjectionMatrix1:
        return "LightProjectionMatrix1";
    case LightProjectionMatrix2:
        return "LightProjectionMatrix2";
    case LightProjectionMatrix3:
        return "LightProjectionMatrix3";
    case LightProjectionMatrix4:
        return "LightProjectionMatrix4";
    case LightProjectionMatrix5:
        return "LightProjectionMatrix5";
    case LightWorldMatrix0:
        return "LightWorldMatrix0";
    case LightWorldMatrix1:
        return "LightWorldMatrix1";
    case LightWorldMatrix2:
        return "LightWorldMatrix2";
    case LightWorldMatrix3:
        return "LightWorldMatrix3";
    case LightWorldMatrix4:
        return "LightWorldMatrix4";
    case LightWorldMatrix5:
        return "LightWorldMatrix5";
    case ShadowMap0:
        return "ShadowMap0";
    case ShadowMap1:
        return "ShadowMap1";
    case ShadowMap2:
        return "ShadowMap2";
    case ShadowMap3:
        return "ShadowMap3";
    case ShadowMap4:
        return "ShadowMap4";
    case ShadowMap5:
        return "ShadowMap5";

    case ShadowDirection0:
        return "ShadowDirection0";
    case ShadowDirection1:
        return "ShadowDirection1";
    case ShadowDirection2:
        return "ShadowDirection2";
    case ShadowDirection3:
        return "ShadowDirection3";
    case ShadowDirection4:
        return "ShadowDirection4";
    case ShadowDirection5:
        return "ShadowDirection5";

    case RenderableWorldMatrix:
        return "RenderableWorldMatrix";
    case ColorMap0:
        return "ColorMap0";
    case ColorMap1:
        return "ColorMap1";
    case ColorMap2:
        return "ColorMap2";
    case ColorMap3:
        return "ColorMap3";
    case NormalMap0:
        return "NormalMap0";
    case NormalMap1:
        return "NormalMap1";
    case NormalMap2:
        return "NormalMap2";
    case NormalMap3:
        return "NormalMap3";
    case Plaster:
        return "Plaster";
    case ColorSketch:
        return "ColorSketch";
    case NormalSketch:
        return "NormalSketch";
    case MaterialIDSketch:
        return "MaterialIDSketch";
    case LightingSketch:
        return "LightingSketch";
    case DiffuseLightingSketch:
        return "DiffuseLightingSketch";
    case SpecularLightingSketch:
        return "SpecularLightingSketch";
    case ColorSketchPixelSize:
        return "ColorSketchPixelSize";
    case AmbiColor:
        return "AmbiColor";
    case CurrentMaterialID:
        return "CurrentMaterialID";
    default:
        return "UnknownParamSource";
    }
}

void _Renderer_common_Init(renderer* _self)
{
    _self->param_proc_tree = Tree_new(Sint32, Vptr, Ealloc, Efree);
    {
        var key, data;
        key.sint32_var = CameraPosition;
        data.vptr_var = RendererParamEntry_new(getCameraPositionProc, Float32x3_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = CameraDirection;
        data.vptr_var = RendererParamEntry_new(getCameraDirectionProc, Float32x3_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = CameraPlaneNear;
        data.vptr_var = RendererParamEntry_new(getCameraPlaneNearProc, Float32_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = CameraPlaneFar;
        data.vptr_var = RendererParamEntry_new(getCameraPlaneFarProc, Float32_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = CameraPlaneWidth;
        data.vptr_var = RendererParamEntry_new(getCameraPlaneWidthProc, Float32_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = CameraPlaneHeight;
        data.vptr_var = RendererParamEntry_new(getCameraPlaneHeightProc, Float32_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = CameraProjectionMatrix;
        data.vptr_var = RendererParamEntry_new(getCameraProjectionMatrixProc, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = CameraWorldMatrix;
        data.vptr_var = RendererParamEntry_new(getCameraWorldMatrixProc, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = RenderableWorldMatrix;
        data.vptr_var = RendererParamEntry_new(getRenderableWorldMatrixProc, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = InvertCameraProjectionMatrix;
        data.vptr_var = RendererParamEntry_new(getInvertCameraProjectionMatrixProc, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = InvertCameraWorldMatrix;
        data.vptr_var = RendererParamEntry_new(getInvertCameraWorldMatrixProc, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = CurrentMaterialID;
        data.vptr_var = RendererParamEntry_new(getCurrentMaterialIDProc, Float32_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = ColorMap0;
        data.vptr_var = RendererParamEntry_new(getColorMap0Proc, Texture2D_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = NormalMap0;
        data.vptr_var = RendererParamEntry_new(getNormalMap0Proc, Texture2D_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = Plaster;
        data.vptr_var = RendererParamEntry_new(getPlasterProc, Texture2D_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = ColorSketch;
        data.vptr_var = RendererParamEntry_new(getColorSketchProc, Texture2D_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = NormalSketch;
        data.vptr_var = RendererParamEntry_new(getNormalSketchProc, Texture2D_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = MaterialIDSketch;
        data.vptr_var = RendererParamEntry_new(getMaterialIDSketchProc, Texture2D_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightingSketch;
        data.vptr_var = RendererParamEntry_new(getLightingSketchProc, Texture2D_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightInnerCos;
        data.vptr_var = RendererParamEntry_new(getLightInnerCos, Float32_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightOuterCos;
        data.vptr_var = RendererParamEntry_new(getLightOuterCos, Float32_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightPosition;
        data.vptr_var = RendererParamEntry_new(getLightPosition, Float32x3_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightDirection;
        data.vptr_var = RendererParamEntry_new(getLightDirection, Float32x3_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightColor;
        data.vptr_var = RendererParamEntry_new(getLightColor, Float32x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightAtteCoef;
        data.vptr_var = RendererParamEntry_new(getLightAtteCoef, Float32_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightNumEmitters;
        data.vptr_var = RendererParamEntry_new(getLightNumShadowEmitters, Int_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightProjectionMatrix0;
        data.vptr_var = RendererParamEntry_new(getLightProjMatrix0, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightProjectionMatrix1;
        data.vptr_var = RendererParamEntry_new(getLightProjMatrix1, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightProjectionMatrix2;
        data.vptr_var = RendererParamEntry_new(getLightProjMatrix2, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightProjectionMatrix3;
        data.vptr_var = RendererParamEntry_new(getLightProjMatrix3, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightProjectionMatrix4;
        data.vptr_var = RendererParamEntry_new(getLightProjMatrix4, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightProjectionMatrix5;
        data.vptr_var = RendererParamEntry_new(getLightProjMatrix5, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightWorldMatrix0;
        data.vptr_var = RendererParamEntry_new(getLightWorldMatrix0, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightWorldMatrix1;
        data.vptr_var = RendererParamEntry_new(getLightWorldMatrix1, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightWorldMatrix2;
        data.vptr_var = RendererParamEntry_new(getLightWorldMatrix2, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightWorldMatrix3;
        data.vptr_var = RendererParamEntry_new(getLightWorldMatrix3, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightWorldMatrix4;
        data.vptr_var = RendererParamEntry_new(getLightWorldMatrix4, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = LightWorldMatrix5;
        data.vptr_var = RendererParamEntry_new(getLightWorldMatrix5, Matrix4x4_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = ShadowMap0;
        data.vptr_var = RendererParamEntry_new(getShadowMap0, Texture2D_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = ShadowMap1;
        data.vptr_var = RendererParamEntry_new(getShadowMap1, Texture2D_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = ShadowMap2;
        data.vptr_var = RendererParamEntry_new(getShadowMap2, Texture2D_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = ShadowMap3;
        data.vptr_var = RendererParamEntry_new(getShadowMap3, Texture2D_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = ShadowMap4;
        data.vptr_var = RendererParamEntry_new(getShadowMap4, Texture2D_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = ShadowMap5;
        data.vptr_var = RendererParamEntry_new(getShadowMap5, Texture2D_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = ShadowDirection0;
        data.vptr_var = RendererParamEntry_new(getShadowDir0, Float32x3_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = ShadowDirection1;
        data.vptr_var = RendererParamEntry_new(getShadowDir1, Float32x3_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = ShadowDirection2;
        data.vptr_var = RendererParamEntry_new(getShadowDir2, Float32x3_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = ShadowDirection3;
        data.vptr_var = RendererParamEntry_new(getShadowDir3, Float32x3_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = ShadowDirection4;
        data.vptr_var = RendererParamEntry_new(getShadowDir4, Float32x3_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = ShadowDirection5;
        data.vptr_var = RendererParamEntry_new(getShadowDir5, Float32x3_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = DiffuseLightingSketch;
        data.vptr_var = RendererParamEntry_new(getDiffuseLightingSketchProc, Texture2D_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = SpecularLightingSketch;
        data.vptr_var = RendererParamEntry_new(getSpecularLightingSketchProc, Texture2D_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);

        key.sint32_var = ColorSketchPixelSize;
        data.vptr_var = RendererParamEntry_new(getColorSketchPixelSizeProc, Float32x2_Param, 1);
        Tree_insert(_self->param_proc_tree, key, data);
    }

    Iterator ppt_iter = Tree_begin(_self->param_proc_tree);
    elog("%s", "########");
    while (ppt_iter)
    {
        var key = Tree_get_key(ppt_iter);
        const char* key_str = _get_param_src_str(key.sint32_var);
        elog("%s", key_str);
        ppt_iter = Tree_next(ppt_iter);
    }
    elog("%s", "########");
    for (uint i = 0; i < MaxParamSource; i++)
    {
        var key, data;
        key.sint32_var = i;
        if (!Tree_find(_self->param_proc_tree, key, &data))
        {
            const char* key_str = _get_param_src_str(key.sint32_var);
            elog("param source %s cant find", key_str);
        }
        else
        {
            RendererParamEntry rpe = data.vptr_var;
            elog("source %s, proc %x, type %d", _get_param_src_str(key.sint32_var), rpe->get_value_proc, rpe->type, rpe->array_size);
        }
    }
    elog("%s", "########");
    memset(&_self->std_sketch_book, 0, sizeof(_self->std_sketch_book));
    memset(_self->lighting_sketch_book_array, 0, sizeof(_self->lighting_sketch_book_array));

    memset(&_self->depth_sketch_book, 0, sizeof(_self->depth_sketch_book));
    memset(&_self->depth_sketch_cfg, 0, sizeof(_self->depth_sketch_cfg));
    memset(&_self->blur_pass, 0, sizeof(_self->blur_pass));
    ///memset(&_self->blurred_depth_sketch_book, 0, sizeof(_self->blurred_depth_sketch_book));
    ///memset(&_self->blurred_depth_sketch_book_cfg, 0, sizeof(_self->blurred_depth_sketch_book_cfg));
    blurred_sketch_book null_blurred_sketch_book = {NULL, NULL};
    _self->blurred_sketch_books = array_new(blurred_sketch_book, 3, null_blurred_sketch_book);

    _self->dir_light_tree2 = Tree_new(Vptr, Vptr, Ealloc, Efree);
    _self->spot_light_tree2 = Tree_new(Vptr, Vptr, Ealloc, Efree);
    _self->curt_light = NULL;

    VertexDecl pdec = RenderablePlane_get_vertex_declaration(_self->base.render_plane);

    _self->material_table = Tree_new(String, Vptr, Ealloc, Efree);
    var key, data;
    key.str_var = "default_material";
    MaterialPrototype mp;
    {
        SdrNdGen sng = ISdrNdGen.New();
        ISdrNdGen.register_default_nodes(sng);

        SDisplayProc s;
        s.proc = default_material_proc3;
        mp = MaterialPrototype_new(pdec, 2,
                                   s,
                                   Shaded, false);
        ISdrNdGen.Delete(sng);
    }
    data.vptr_var = mp;
    Tree_insert(_self->material_table, key, data);
    _self->material_count = 3;
    _self->dirty_flag = true;
    _self->clear_color_buffer_flag = true;
    _self->clear_depth_buffer_flag = true;

    _self->first_dir_lighting_pass = NULL;
    _self->dir_lighting_pass = NULL;

    _self->curt_rend_world_matrix = NULL;
}

void Renderer_Init_from_prev_renderer(renderer* _self, Renderer prev_renderer, const char* _file, uint _line)
{
    RendererBase_Init_from_prev_renderer_base(&_self->base, &prev_renderer->base, _file, _line);
    _Renderer_common_Init(_self);
    _self->prev_renderer = prev_renderer;
    _self->use_deferred_shading = prev_renderer->use_deferred_shading;
}

void Renderer_Init(renderer* _self, uint x, uint y, uint width, uint height, const char* _file, uint _line)
{
    RendererBase_Init(&_self->base, x, y, width, height, _file, _line);
    _Renderer_common_Init(_self);
    _self->prev_renderer = NULL;
    _self->use_deferred_shading = USE_DS_RENDER;
}

void _Renderer_delete_blurred_sketck_books(renderer* _self)
{
    if (_self->blurred_sketch_books)
    {
        uint n = array_n(_self->blurred_sketch_books);
        for (uint i = 0; i < n; i++)
        {
            blurred_sketch_book bskb = _self->blurred_sketch_books[i];
            SketchBook_delete(bskb.sketch_book);
            SketchBookConfig_delete(bskb.sketch_cfg);
        }
        array_delete(_self->blurred_sketch_books);
        _self->blurred_sketch_books = NULL;
    }
}

void Renderer_Dest(renderer* _self)
{
    RendererBase_Dest(&_self->base);
    if (!_self->prev_renderer)
    {
        if (_self->std_sketch_book)
            SketchBook_delete(_self->std_sketch_book);
        for (uint i = 0; i < NUM_LIGHTING_SKETCH_BOOKS; i++)
        {
            SketchBook skb = _self->lighting_sketch_book_array[i];
            if (skb)
                SketchBook_delete(skb);
        }
        if (_self->depth_sketch_book)
            SketchBook_delete(_self->depth_sketch_book);
        if (_self->depth_sketch_cfg)
            SketchBookConfig_delete(_self->depth_sketch_cfg);
        if (_self->blur_pass)
            Pass_delete(_self->blur_pass);
        /**
        if (_self->blurred_depth_sketch_book)
        SketchBook_delete(_self->blurred_depth_sketch_book);
        if (_self->blurred_depth_sketch_book_cfg)
        SketchBookConfig_delete(_self->blurred_depth_sketch_book_cfg);
        **/
        _Renderer_delete_blurred_sketck_books(_self);
    }

    if (_self->first_dir_lighting_pass)
        Pass_delete(_self->first_dir_lighting_pass);
    if (_self->dir_lighting_pass)
        Pass_delete(_self->dir_lighting_pass);

    if (_self->first_spot_lighting_pass)
        Pass_delete(_self->first_spot_lighting_pass);
    if (_self->spot_lighting_pass)
        Pass_delete(_self->spot_lighting_pass);

    Iterator iter = Tree_begin(_self->material_table);
    while (iter)
    {
        var data = Tree_get_value(iter);
        MaterialPrototype mp = data.vptr_var;
        MaterialPrototype_delete(mp);
        iter = Tree_next(iter);
    }
    Tree_Dest(_self->material_table);

    _light_base_2_tree_dest(_self->dir_light_tree2);
    _light_base_2_tree_dest(_self->spot_light_tree2);

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

LightBase2 Renderer_add_dir_light_2(Renderer _self)
{
    LightBase2 ret = DirLight_new();

    var key, data;
    key.vptr_var = ret;
    data.vptr_var = ret;
    Tree_insert(_self->dir_light_tree2, key, data);
    return ret;
}

LightBase2 Renderer_add_spot_light_2(Renderer _self)
{
    LightBase2 ret = SpotLight_new();

    var key, data;
    key.vptr_var = ret;
    data.vptr_var = ret;
    Tree_insert(_self->spot_light_tree2, key, data);
    return ret;
}

void Renderer_register_material(Renderer _self, const char* mat_name, SDisplayProc disp_proc, draw_mode draw_mode, bool double_sided_flag)
{
    var key, data;
    key.str_var = mat_name;
    Iterator iter = Tree_find(_self->material_table, key, &data);
    if (!iter)
    {
        VertexDecl dec = RenderablePlane_get_vertex_declaration(_self->base.render_plane);
        MaterialPrototype mp;
        {
            SdrNdGen sng = ISdrNdGen.New();
            ISdrNdGen.register_default_nodes(sng);
            mp = MaterialPrototype_new(dec, _self->material_count,
                                       disp_proc,
                                       draw_mode, double_sided_flag);
            ISdrNdGen.Delete(sng);
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

void _Renderer_prepare_sketchbooks(Renderer _self)
{
    if (!_self->prev_renderer)
    {
        elog("%s", "STD SKETCHBOOK INIT BEGIN:");
        {
            if (_self->std_sketch_book)
                SketchBook_delete(_self->std_sketch_book);
            SketchBookConfig cfg = SketchBookConfig_new();
            SketchBookConfig_set_size(cfg, _self->base.width, _self->base.height);

            SketchBookConfig_set_sketch_format(cfg, MATERIAL_ID_INDEX,     MATERIAL_ID_FORMAL);
            SketchBookConfig_set_sketch_format(cfg, BASE_COLOR_INDEX,      BASE_COLOR_FORMAT);
            SketchBookConfig_set_sketch_format(cfg, NORMAL_SPECULAR_INDEX, NORMAL_SPECULAR_FORMAT);

            SketchBookConfig_set_plaster_format(cfg, DEPTH32F);
            SketchBook skb = SketchBook_new(cfg);
            SketchBookConfig_delete(cfg);
            _self->std_sketch_book = skb;

            Texture2D tex_mat_id = SketchBook_get_sketch(skb, MATERIAL_ID_INDEX);
            Texture2D tex_color = SketchBook_get_sketch(skb, BASE_COLOR_INDEX);
            Texture2D tex_normal = SketchBook_get_sketch(skb, NORMAL_SPECULAR_INDEX);
            elog("        MATERIAL ID SKETCH ID %d", Texture2D_get_id(tex_mat_id));
            elog("        BASE COLOR SKETCH ID %d", Texture2D_get_id(tex_color));
            elog("        NORMAL SPECULAR SKETCH ID %d", Texture2D_get_id(tex_normal));
            elog("%s", "STD SKETCHBOOK INIT END:");
        }
        {
            if (_self->depth_sketch_book)
                SketchBook_delete(_self->depth_sketch_book);
            if (_self->depth_sketch_cfg)
                SketchBookConfig_delete(_self->depth_sketch_cfg);
            if (_self->blur_pass)
                Pass_delete(_self->blur_pass);
            /**
            if (_self->blurred_depth_sketch_book)
            SketchBook_delete(_self->blurred_depth_sketch_book);
            if (_self->blurred_depth_sketch_book_cfg)
            SketchBook_delete(_self->blurred_depth_sketch_book_cfg);
            **/
            uint width = 128;
            uint height = 128;

            _Renderer_delete_blurred_sketck_books(_self);
            {
                SketchBookConfig cfg = SketchBookConfig_new();
                SketchBookConfig_set_size(cfg, width, height);
                SketchBookConfig_set_sketch_format(cfg, 0, RG32F);

                ///SketchBookConfig_set_plaster_format(cfg, DEPTH32F);
                SketchBook skb = SketchBook_new(cfg);
                ///SketchBookConfig_delete(cfg);
                _self->depth_sketch_book = skb;
                _self->depth_sketch_cfg = cfg;
            }

            _self->blur_pass = RenderSystem_register_blur_pass(RenderablePlane_get_vertex_declaration(_self->base.render_plane));
            blurred_sketch_book null_blurred_sketch_book = {NULL, NULL};
            _self->blurred_sketch_books = array_new(blurred_sketch_book, 3, null_blurred_sketch_book);

            uint num_blur_passes = 1;
            for (uint i = 0; i < num_blur_passes; i++)
            {
                width *= 2;
                height *= 2;

                SketchBookConfig cfg = SketchBookConfig_new();
                SketchBookConfig_set_size(cfg, width, height);
                SketchBookConfig_set_sketch_format(cfg, 0, RG32F);
                SketchBook skb = SketchBook_new(cfg);

                blurred_sketch_book bskb = {skb, cfg};
                apush(_self->blurred_sketch_books, bskb);
            }
            /**
                        {
                            SketchBookConfig cfg = SketchBookConfig_new();
                            SketchBookConfig_set_size(cfg, 256, 256);
                            SketchBookConfig_set_sketch_format(cfg, 0, RG32F);

                            SketchBook skb = SketchBook_new(cfg);

                            _self->blurred_depth_sketch_book = skb;
                            _self->blurred_depth_sketch_book_cfg = cfg;

                            _self->blur_pass = RenderSystem_register_blur_pass(RenderablePlane_get_vertex_declaration(_self->base.render_plane));
                        }
                        **/
        }

        elog("%s", "LIGHTING SKETCHBOOK INIT BEGIN:");
        for (uint i = 0; i < NUM_LIGHTING_SKETCH_BOOKS; i++)
        {
            SketchBook skb = _self->lighting_sketch_book_array[i];
            if (to_ptr(skb))
                SketchBook_delete(skb);
        }
        for (uint i = 0; i < NUM_LIGHTING_SKETCH_BOOKS; i++)
        {
            elog("    SKETCH BOOK %d", i);
            SketchBookConfig cfg = SketchBookConfig_new();
            SketchBookConfig_set_size(cfg, _self->base.width, _self->base.height);

            SketchBookConfig_set_sketch_format(cfg, DIFFUSE_INDEX,  DIFFUSE_FORMAT);
            SketchBookConfig_set_sketch_format(cfg, SPECULAR_INDEX, SPECULAR_FORMAT);

            SketchBook skb = SketchBook_new(cfg);
            SketchBookConfig_delete(cfg);
            _self->lighting_sketch_book_array[i] = skb;

            Texture2D tex_diffuse = SketchBook_get_sketch(skb, DIFFUSE_INDEX);
            Texture2D tex_specular = SketchBook_get_sketch(skb, SPECULAR_INDEX);
            elog("        DIFFUSE INDEX SKETCH ID %d", Texture2D_get_id(tex_diffuse));
            elog("        SPECULAR INDEX SKETCH ID %d", Texture2D_get_id(tex_specular));
        }
        elog("%s", "LIGHTING SKETCHBOOK INIT END:");
    }
    else
    {
        _self->std_sketch_book = _self->prev_renderer->std_sketch_book;
        for (uint i = 0; i < NUM_LIGHTING_SKETCH_BOOKS; i++)
        {
            _self->lighting_sketch_book_array[i] = _self->prev_renderer->lighting_sketch_book_array[i];
        }
    }
}
void _Renderer_prepare_display_passes(Renderer _self)
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
void _Renderer_prepare_lighting_passes(Renderer _self)
{
    VertexDecl vtx_decl = RenderablePlane_get_vertex_declaration(_self->base.render_plane);
    lighting_decl null_lt_decl = {NULL, -1};
    lighting_decl* decls = array_new(lighting_decl, Tree_count(_self->material_table), null_lt_decl);
    Iterator mat_iter = Tree_begin(_self->material_table);
    while (mat_iter)
    {
        var data = Tree_get_value(mat_iter);
        MaterialPrototype mp = data.vptr_var;
        lighting_decl d = {lighting_template, mp->material_id};
        apush(decls, d);
        mat_iter = Tree_next(mat_iter);
    }
    {
        light_state state = {Direction, DirectionLightingNode, 1};
        _self->first_dir_lighting_pass = create_lighting_pass_ex3(_self, vtx_decl, &state, decls, false);
        _self->dir_lighting_pass = create_lighting_pass_ex3(_self, vtx_decl, &state, decls, true);
    }
    {
        light_state state = {Spot, SpotLightingNode, 1};
        _self->first_spot_lighting_pass = create_lighting_pass_ex3(_self, vtx_decl, &state, decls, false);
        _self->spot_lighting_pass = create_lighting_pass_ex3(_self, vtx_decl, &state, decls, true);
    }
    array_delete(decls);
}
void _Renderer_pre_lighting(Renderer _self)
{
    _self->prev_render_skb = _self->std_sketch_book;
    Pass std_pass = {NULL};

    glViewport(0, 0, _self->base.width, _self->base.height);

    if (_self->use_deferred_shading)
    {
        SketchBook_bind_framebuffer(_self->prev_render_skb);
        SketchBook_bind_renderbuffer();
    }
    uint clear_flag = 0;
    if (_self->clear_color_buffer_flag)
        clear_flag |= GL_COLOR_BUFFER_BIT;
    if (_self->clear_depth_buffer_flag)
        clear_flag |= GL_DEPTH_BUFFER_BIT;
    if (clear_flag)
        glClear(clear_flag);
    glDepthMask(GL_TRUE);
    if (_self->use_deferred_shading)
    {
        SketchBook_unbind_framebuffer();
        SketchBook_unbind_renderbuffer();
    }

    Iterator r_iter = Tree_begin(_self->base.used_renderable_tree);
    uint count = 0;
    while (r_iter)
    {
        var key, data;
        data = Tree_get_value(r_iter);
        Renderable rbl = data.vptr_var;
        if (!rbl->world_matrix)
        {
            throw_exception(RenderableWorldMatrixIsNull, "renderable world matrix is null");
            continue;
        }
        _self->curt_rend_world_matrix = rbl->world_matrix;

        const char* mat_name = MaterialInstance_get_material_name(rbl->material);

        key.str_var = mat_name;
        Iterator iter = Tree_find(_self->material_table, key, &data);
        if (!iter)
        {
            throw_exception(MaterialNotExist, "material not exist");
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

        if (_self->use_deferred_shading)
            SketchBook_draw_begin(_self->prev_render_skb, 3);

        Renderable_prev_render(rbl);
        std_pass = rbl->std_pass;

        _self->curt_mat_proto = mp;
        _self->curt_mat_inst = rbl->material;
        Pass_auto_set_uniform_params(std_pass, _self, false);

        uint face_count = IndexBuffer_get_num_faces(rbl->idx_buf);
        mesh_mode mode = IndexBuffer_get_mesh_mode(rbl->idx_buf);
        if (mode == Triangular)
            Pass_render(std_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 3, mode);
        else
            Pass_render(std_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 2, mode);

        ERROR_PROC;
        if (_self->use_deferred_shading)
        {
            SketchBook_draw_end(_self->prev_render_skb);
            ERROR_PROC;
        }

        count++;
        r_iter = Tree_next(r_iter);
    }
}

void _Renderer_all_lighting_render(Renderer _self, SketchBook curt_lighting_skb, Tree light_tree, Pass first_lighting_pass, Pass lighting_pass, uint* light_count)
{
    void clear_sketch_book(SketchBook curt_skb)
    {
        SketchBook_bind_framebuffer(curt_skb);
        SketchBook_bind_renderbuffer();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClearDepth(1.0);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glDepthMask(GL_TRUE);

        SketchBook_unbind_framebuffer();
        SketchBook_unbind_renderbuffer();
    }
    void depth_map_blur()
    {
        /**
        uint width = SketchBookConfig_get_width(_self->blurred_depth_sketch_book_cfg);
        uint height = SketchBookConfig_get_width(_self->blurred_depth_sketch_book_cfg);

        SketchBook_draw_begin(_self->blurred_depth_sketch_book, 1);
        _self->curt_color_sketch = SketchBook_get_sketch(_self->depth_sketch_book, 0);
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        ERROR_PROC;
        glEnable(GL_STENCIL_TEST);
        ERROR_PROC;
        Pass_auto_set_uniform_params(_self->blur_pass, _self, false);
        Pass_render_plane(_self->blur_pass, _self->base.render_plane);
        SketchBook_draw_end(_self->blurred_depth_sketch_book);
        **/
        SketchBook prev_skb = _self->depth_sketch_book;

        uint num_blur_passes = array_n(_self->blurred_sketch_books);
        for (uint i = 0; i < num_blur_passes; i++)
        {
            _self->curt_color_sketch = SketchBook_get_sketch(prev_skb, 0);
            blurred_sketch_book bskb = _self->blurred_sketch_books[i];
            uint width = SketchBookConfig_get_width(bskb.sketch_cfg);
            uint height = SketchBookConfig_get_height(bskb.sketch_cfg);

            clear_sketch_book(bskb.sketch_book);

            SketchBook_draw_begin(bskb.sketch_book, 1);
            _self->curt_color_sketch = SketchBook_get_sketch(prev_skb, 0);
            glViewport(0, 0, width, height);
            glEnable(GL_DEPTH_TEST);
            ERROR_PROC;
            glEnable(GL_STENCIL_TEST);
            ERROR_PROC;
            Pass_auto_set_uniform_params(_self->blur_pass, _self, false);
            Pass_render_plane(_self->blur_pass, _self->base.render_plane);
            SketchBook_draw_end(bskb.sketch_book);

            prev_skb = bskb.sketch_book;
        }
    }
    void shadow_render(Renderable rbl, SketchBook curt_skb)
    {
        if (!rbl->world_matrix)
        {
            throw_exception(RenderableWorldMatrixIsNull, "renderable world matrix is null");
            return;
        }
        _self->curt_rend_world_matrix = rbl->world_matrix;

        const char* mat_name = MaterialInstance_get_material_name(rbl->material);

        var key, data;
        key.str_var = mat_name;
        Iterator iter = Tree_find(_self->material_table, key, &data);
        if (!iter)
        {
            throw_exception(MaterialNotExist, "material not exist");
            return;
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

        SketchBook_draw_begin(curt_skb, 1);

        ///Renderable_prev_render(rbl);
        ///Pass std_pass = rbl->std_pass;
        Renderable_depth_prev_render(rbl);
        Pass std_pass = rbl->depth_pass;

        _self->curt_mat_proto = mp;
        _self->curt_mat_inst = rbl->material;

        Pass_auto_set_uniform_params(std_pass, _self, false);

        uint face_count = IndexBuffer_get_num_faces(rbl->idx_buf);
        mesh_mode mode = IndexBuffer_get_mesh_mode(rbl->idx_buf);
        if (mode == Triangular)
            Pass_render(std_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 3, mode);
        else
            Pass_render(std_pass, rbl->vtx_buf, rbl->idx_buf, face_count * 2, mode);

        ERROR_PROC;
        SketchBook_draw_end(curt_skb);
        ERROR_PROC;
    }
    void lighting_render(Pass lighting_pass, SketchBook curt_skb)
    {
        SketchBook_draw_begin(curt_skb, 2);

        Pass_auto_set_uniform_params(lighting_pass, _self, false);

        ERROR_PROC;
        Pass_render_plane(lighting_pass, _self->base.render_plane);
        ERROR_PROC;
        SketchBook_draw_end(curt_skb);
        ERROR_PROC;
    }
    Iterator light_iter = Tree_begin(light_tree);
    while (light_iter)
    {
        var data = Tree_get_value(light_iter);
        LightBase2 pl = data.vptr_var;

        _self->curt_light = pl;

        /// here render shadow map
        Camera light_cam = LightBase2_get_camera(pl, 0);
        if (to_ptr(light_cam))
        {
            _self->base.curt_render_cam = light_cam;

            Iterator rbl_iter = Tree_begin(_self->base.used_renderable_tree);
            SketchBook skb = _self->depth_sketch_book;

            clear_sketch_book(skb);
            while (rbl_iter)
            {
                uint width = SketchBookConfig_get_width(_self->depth_sketch_cfg);
                uint height = SketchBookConfig_get_height(_self->depth_sketch_cfg);

                var data = Tree_get_value(rbl_iter);
                Renderable rbl = data.vptr_var;
                glViewport(0, 0, width, height);
                glEnable(GL_DEPTH_TEST);
                ERROR_PROC;
                glEnable(GL_STENCIL_TEST);
                ERROR_PROC;
                shadow_render(rbl, skb);
                rbl_iter = Tree_next(rbl_iter);
            }
            ///clear_sketch_book(_self->blurred_depth_sketch_book);
            /**
            uint num_blur_passes = array_n(_self->blurred_sketch_books);
            for (uint i = 0; i < num_blur_passes; i++)
            {
                blurred_sketch_book bskb = _self->blurred_sketch_books[i];
                clear_sketch_book(bskb.sketch_book);
            }
            **/
            depth_map_blur();
        }

        _self->base.curt_render_cam = _self->base.camera_base;

        glViewport(0, 0, _self->base.width, _self->base.height);
        glDisable(GL_DEPTH_TEST);
        ERROR_PROC;
        glDisable(GL_STENCIL_TEST);
        ERROR_PROC;

        if (*light_count != 0)
        {
            _self->curt_diffuse_lighting_sketch = SketchBook_get_sketch(_self->prev_lighting_skb, 0);
            _self->curt_specular_lighting_sketch = SketchBook_get_sketch(_self->prev_lighting_skb, 1);

            lighting_render(lighting_pass, curt_lighting_skb);
        }
        else
        {
            lighting_render(first_lighting_pass, curt_lighting_skb);
        }

        SketchBook tmp = _self->prev_lighting_skb;
        _self->prev_lighting_skb = curt_lighting_skb;
        curt_lighting_skb = tmp;

        light_iter = Tree_next(light_iter);
        (*light_count)++;

        glDisable(GL_DEPTH_TEST);
        ERROR_PROC;
        glDisable(GL_STENCIL_TEST);
        ERROR_PROC;
    }
}

void _Renderer_lighting(Renderer _self)
{
    _self->curt_normal_sketch = SketchBook_get_sketch(_self->prev_render_skb, NORMAL_SPECULAR_INDEX);
    _self->curt_plaster = SketchBook_get_plaster(_self->prev_render_skb);
    _self->curt_material_id_sketch = SketchBook_get_sketch(_self->prev_render_skb, MATERIAL_ID_INDEX);

    {
        SketchBook curt_lighting_skb = _self->lighting_sketch_book_array[0];
        _self->prev_lighting_skb = _self->lighting_sketch_book_array[1];

        _self->curt_material_id_sketch = SketchBook_get_sketch(_self->prev_render_skb, MATERIAL_ID_INDEX);

        uint32 light_count = 0;

        _Renderer_all_lighting_render(_self, curt_lighting_skb, _self->dir_light_tree2, _self->first_dir_lighting_pass, _self->dir_lighting_pass, &light_count);
        _Renderer_all_lighting_render(_self, curt_lighting_skb, _self->spot_light_tree2, _self->first_spot_lighting_pass, _self->spot_lighting_pass, &light_count);
    }
}
void Renderer_render(Renderer _self)
{
    TRY(0)

    _render_one_frame_begin();

    if (!Tree_count(_self->base.used_renderable_tree))
        return;

    if (_self->dirty_flag)
    {
        _Renderer_prepare_sketchbooks(_self);
        _Renderer_prepare_display_passes(_self);
        _Renderer_prepare_lighting_passes(_self);
    }

    _Renderer_pre_lighting(_self);
    if (_self->use_deferred_shading)
    {
        _Renderer_lighting(_self);
        glViewport(_self->base.x, _self->base.y, _self->base.width, _self->base.height);

        if (!_self->prev_renderer)
        {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClearDepth(1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        _self->curt_mat_proto = NULL;
        _self->curt_material_id_sketch = SketchBook_get_sketch(_self->prev_render_skb, MATERIAL_ID_INDEX);
        _self->curt_color_sketch = SketchBook_get_sketch(_self->prev_render_skb, BASE_COLOR_INDEX);
        _self->curt_normal_sketch = SketchBook_get_sketch(_self->prev_render_skb, NORMAL_SPECULAR_INDEX);
        _self->curt_lighting_sketch = SketchBook_get_sketch(_self->prev_lighting_skb, 0);
        _self->curt_diffuse_lighting_sketch = SketchBook_get_sketch(_self->prev_lighting_skb, 0);
        _self->curt_specular_lighting_sketch = SketchBook_get_sketch(_self->prev_lighting_skb, 1);

        Pass_auto_set_uniform_params(_self->base.display_pass, _self, false);

        Pass_render_plane(_self->base.display_pass, _self->base.render_plane);
    }
    _self->dirty_flag = false;
    CATCH(0)
    {
        _self->dirty_flag = false;
    }
}

void Renderer_clear_sketchbook(Renderer _self)
{
    Pass clear_sketchbook_pass = NULL;
    if (_self->prev_renderer)
        clear_sketchbook_pass = _self->prev_renderer->base.clear_sketchbook_pass;
    else
        clear_sketchbook_pass = _self->base.clear_sketchbook_pass;

    SketchBook render_skb = _self->std_sketch_book;

    SketchBook_draw_begin(render_skb, 3);

    glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0);
    glClearStencil(0);
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_BACK);
    ERROR_PROC;

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    ERROR_PROC;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_FALSE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glViewport(_self->base.x, _self->base.y, _self->base.width, _self->base.height);
    Pass_render_plane(clear_sketchbook_pass, _self->base.render_plane);
    SketchBook_draw_end(render_skb);
}

Renderable _Renderer_new_renderable(Renderer _self, VertexDecl _dec, MaterialInstance _m_inst, mesh_mode _mesh_mode,
                                    const char* _file, uint _line)
{
    const char* mat_name = MaterialInstance_get_material_name(_m_inst);
    var key, data;
    key.str_var = mat_name;
    Iterator iter = Tree_find(_self->material_table, key, &data);
    if (!iter)
    {
        char mbuf[STRING_BUFFER_SIZE];
        snprintf(mbuf, STRING_BUFFER_SIZE - 1, "file %s line %d material not exist", _file, _line);
        throw_exception(MaterialNotExist, mbuf);
    }
    return _RendererBase_new_renderable(&_self->base, _dec, _m_inst, _mesh_mode, _file, _line);
}

void Renderer_use_renderable(Renderer _self, Renderable _rbl)
{
    RendererBase_use_renderable(_self, _rbl);
}

Camera Renderer_get_camera(Renderer _self)
{
    return _self->base.curt_render_cam;
}

Texture2D Renderer_get_shadow_map(Renderer _self, uint i)
{
    if (i == 0)
    {
        ///return SketchBook_get_plaster(_self->depth_sketch_book);
        ///return SketchBook_get_sketch(_self->depth_sketch_book, 0);
        ///return SketchBook_get_sketch(_self->blurred_depth_sketch_book, 0);
        uint num_blur_passes = array_n(_self->blurred_sketch_books);
        if (num_blur_passes > 0)
        {
            blurred_sketch_book bskb = _self->blurred_sketch_books[num_blur_passes - 1];
            return SketchBook_get_sketch(bskb.sketch_book, 0);
        }
        else
            return NULL;
    }
    else
        return NULL;
}

void Renderer_viewport_refresh(Renderer _self, uint x, uint y, uint width, uint height)
{
    Camera cam = _self->base.curt_render_cam;

    Camera_refresh(cam, width, height);

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

void Renderer_get_mouse_ray(Renderer _self, int x, int y, EFloat3* ori, EFloat3* dir)
{
    int window_width, window_height;
    RenderSystem_get_window_width_height(&window_width, &window_height);

    int totel_x = x - (int)_self->base.x;
    int totel_y = y - (int)_self->base.y;

    *ori = Camera_screen_position_to_world_position(_self->base.curt_render_cam, (float)totel_x, (float)totel_y);
    sfloat3 s_ori = SFloat3_assign_from_efloat3(ori);
    EFloat3 cam_pos = Camera_get_position(_self->base.curt_render_cam);
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
    key.sint32_var = _id;
    data.vptr_var = RendererParamEntry_new(_proc, _type, _array_size);
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
