
renderer_param_value getCameraPositionProc(RendererBase* _self)
{
    renderer_param_value ret;
    EFloat3* tmp = (EFloat3*)SMalloc(sizeof(EFloat3));
    *tmp = Camera_get_position(_self->curt_render_cam);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraDirectionProc(RendererBase* _self)
{
    renderer_param_value ret;
    EFloat3* tmp = (EFloat3*)SMalloc(sizeof(EFloat3));
    *tmp = Camera_get_direction(_self->curt_render_cam);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraPlaneNearProc(RendererBase* _self)
{
    renderer_param_value ret;
    float* tmp = (float*)SMalloc(sizeof(float));
    *tmp = Camera_get_plane_near(_self->curt_render_cam);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraPlaneFarProc(RendererBase* _self)
{
    renderer_param_value ret;
    float* tmp = (float*)SMalloc(sizeof(float));
    *tmp = Camera_get_plane_far(_self->curt_render_cam);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraPlaneWidthProc(RendererBase* _self)
{
    renderer_param_value ret;
    float* tmp = (float*)SMalloc(sizeof(float));
    *tmp = Camera_get_width(_self->curt_render_cam);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraPlaneHeightProc(RendererBase* _self)
{
    renderer_param_value ret;
    float* tmp = (float*)SMalloc(sizeof(float));
    *tmp = Camera_get_height(_self->curt_render_cam);
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraProjectionMatrixProc(RendererBase* _self)
{
    renderer_param_value ret;
    matrix4x4* tmp = Matrix4x4_new();
    Matrix4x4_assign(tmp, Camera_get_render_matrix(_self->curt_render_cam));
    ret.value = tmp;
    return ret;
}
renderer_param_value getCameraWorldMatrixProc(RendererBase* _self)
{
    renderer_param_value ret;
    matrix4x4* tmp = Matrix4x4_new();
    Matrix4x4_assign(tmp, Camera_get_world_matrix(_self->curt_render_cam));
    ret.value = tmp;
    return ret;
}
renderer_param_value getRenderableWorldMatrixProc(RendererBase* _self)
{
    renderer_param_value ret;
    matrix4x4* tmp = Matrix4x4_new();
    Matrix4x4_assign(tmp, _self->curt_rend_world_matrix);
    Matrix4x4_turn(tmp);
    ret.value = tmp;
    return ret;
}
renderer_param_value getInvertCameraProjectionMatrixProc(RendererBase* _self)
{
    renderer_param_value ret;
    matrix4x4* tmp = Matrix4x4_new();
    Matrix4x4_assign(tmp, Camera_get_inv_render_matrix(_self->curt_render_cam));
    ret.value = tmp;
    return ret;
}
renderer_param_value getInvertCameraWorldMatrixProc(RendererBase* _self)
{
    renderer_param_value ret;
    matrix4x4* tmp = Matrix4x4_new();
    Matrix4x4_assign(tmp, Camera_get_inv_world_matrix(_self->curt_render_cam));
    ret.value = tmp;
    return ret;
}
renderer_param_value getCurrentMaterialIDProc(RendererBase* _self)
{
    renderer_param_value ret;
    float* tmp = (float*)SMalloc(sizeof(float));
    MaterialPrototype mp = _self->curt_mat_proto;
    *tmp = (float)mp->material_id;
    ret.value = tmp;
    return ret;
}
renderer_param_value getColorMap0Proc(RendererBase* _self)
{
    renderer_param_value ret;
    Texture2D** tmp = (Texture2D**)SMalloc(sizeof(Texture2D*));
    *tmp = MaterialInstance_get_color_texture(_self->curt_mat_inst).get();
    ret.value = tmp;
    return ret;
}
renderer_param_value getNormalMap0Proc(RendererBase* _self)
{
    renderer_param_value ret;
    Texture2D** tmp = (Texture2D**)SMalloc(sizeof(Texture2D*));
    *tmp = MaterialInstance_get_normal_texture(_self->curt_mat_inst).get();
    ret.value = tmp;
    return ret;
}
renderer_param_value getPlasterProc(RendererBase* _self)
{
    renderer_param_value ret;
    Texture2D** tmp = (Texture2D**)SMalloc(sizeof(Texture2D*));
    *tmp = _self->curt_plaster.get();
    ret.value = tmp;
    return ret;
}
renderer_param_value getColorSketchProc(RendererBase* _self)
{
    renderer_param_value ret;
    Texture2D** tmp = (Texture2D**)SMalloc(sizeof(Texture2D*));
    *tmp = _self->curt_color_sketch.get();
    ret.value = tmp;
    return ret;
}
renderer_param_value getNormalSketchProc(RendererBase* _self)
{
    renderer_param_value ret;
    Texture2D** tmp = (Texture2D**)SMalloc(sizeof(Texture2D*));
    *tmp = _self->curt_normal_sketch.get();
    ret.value = tmp;
    return ret;
}
renderer_param_value getMaterialIDSketchProc(RendererBase* _self)
{
    renderer_param_value ret;
    Texture2D** tmp = (Texture2D**)SMalloc(sizeof(Texture2D*));
    *tmp = _self->curt_material_id_sketch.get();
    ret.value = tmp;
    return ret;
}
renderer_param_value getLightInnerCos(RendererBase* _self)
{
    renderer_param_value ret;
    float* tmp = (float*)SMalloc(sizeof(float));
    ///*tmp = LightBase2_get_inner_cos(_self->curt_light);
	*tmp = _self->curt_light->get_inner_cos();
    ret.value = tmp;
    return ret;
}
renderer_param_value getLightOuterCos(RendererBase* _self)
{
    renderer_param_value ret;
    float* tmp = (float*)SMalloc(sizeof(float));
    ///*tmp = LightBase2_get_outer_cos(_self->curt_light);
	*tmp = _self->curt_light->get_outer_cos();
    ret.value = tmp;
    return ret;
}
renderer_param_value getLightingSketchProc(RendererBase* _self)
{
    renderer_param_value ret;
    Texture2D** tmp = (Texture2D**)SMalloc(sizeof(Texture2D*));
    *tmp = _self->curt_lighting_sketch.get();
    ret.value = tmp;
    return ret;
}
renderer_param_value getDiffuseLightingSketchProc(RendererBase* _self)
{
    renderer_param_value ret;
    Texture2D** tmp = (Texture2D**)SMalloc(sizeof(Texture2D*));
    *tmp = _self->curt_diffuse_lighting_sketch.get();
    ret.value = tmp;
    return ret;
}
renderer_param_value getSpecularLightingSketchProc(RendererBase* _self)
{
    renderer_param_value ret;
    Texture2D** tmp = (Texture2D**)SMalloc(sizeof(Texture2D*));
    *tmp = _self->curt_specular_lighting_sketch.get();
    ret.value = tmp;
    return ret;
}
renderer_param_value getColorSketchPixelSizeProc(RendererBase* _self)
{
    renderer_param_value ret;
    EFloat2* tmp = (EFloat2*)SMalloc(sizeof(EFloat2));
    uint width = _self->curt_color_sketch->GetWidth();
    uint height = _self->curt_color_sketch->GetHeight();
    tmp->x = 1.0f / (float)width;
    tmp->y = 1.0f / (float)height;
    ret.value = tmp;
    return ret;
}

renderer_param_value getLightPosition(RendererBase* _self)
{
    renderer_param_value ret;
    EFloat3* tmp = (EFloat3*)SMalloc(sizeof(EFloat3));
    ///*tmp = LightBase2_get_pos(_self->curt_light);
	*tmp = _self->curt_light->get_position();
    ret.value = tmp;
    return ret;
}
renderer_param_value getLightDirection(RendererBase* _self)
{
    renderer_param_value ret;
    EFloat3* tmp = (EFloat3*)SMalloc(sizeof(EFloat3));
    ///*tmp = LightBase2_get_dir(_self->curt_light);
	*tmp = _self->curt_light->get_direction();
    ret.value = tmp;
    return ret;
}
renderer_param_value getLightColor(RendererBase* _self)
{
    renderer_param_value ret;
    EFloat4* tmp = (EFloat4*)SMalloc(sizeof(EFloat4));
    ///*tmp = LightBase2_get_color(_self->curt_light);
	*tmp = _self->curt_light->get_color();
    ret.value = tmp;
    return ret;
}
renderer_param_value getLightAtteCoef(RendererBase* _self)
{
    renderer_param_value ret;
    float* tmp = (float*)SMalloc(sizeof(float));
    ///*tmp = LightBase2_get_atte_coef(_self->curt_light);
	*tmp = _self->curt_light->get_atte_coef();
    ret.value = tmp;
    return ret;
}
renderer_param_value getLightNumShadowEmitters(RendererBase* _self)
{
    renderer_param_value ret;
    int* tmp = (int*)SMalloc(sizeof(int));
    ///*tmp = LightBase2_get_num_shadow_emitters(_self->curt_light);
	*tmp = _self->curt_light->get_num_shadow_emitters();
    ret.value = tmp;
    return ret;
}

renderer_param_value getLightProjMatrix0(RendererBase* _self)
{
    renderer_param_value ret;
    matrix4x4* tmp = NULL;
    ///matrix4x4* proj_mat = LightBase2_get_proj_matrix(_self->curt_light, 0);
	matrix4x4* proj_mat = _self->curt_light->get_proj_matrix(0);
    if (proj_mat)
    {
        tmp = Matrix4x4_new();
        Matrix4x4_assign(tmp, proj_mat);
    }
    ret.value = tmp;
    return ret;
}

#define DefGetLightProjMatrix(i) \
renderer_param_value getLightProjMatrix##i(RendererBase* _self) \
{ \
    renderer_param_value ret; \
    matrix4x4* tmp = NULL; \
    matrix4x4* proj_mat = _self->curt_light->get_proj_matrix(i); \
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
renderer_param_value getLightWorldMatrix##i(RendererBase* _self) \
{ \
    renderer_param_value ret; \
    matrix4x4* tmp = NULL; \
    matrix4x4* world_mat = _self->curt_light->get_world_matrix(i); \
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
renderer_param_value getShadowMap##i(RendererBase* _self) \
{ \
    renderer_param_value ret; \
    Texture2D** tmp = NULL; \
    Texture2D* depth_map = _self->get_shadow_map(i).get(); \
    if (depth_map) { \
        tmp = (Texture2D**)SMalloc(sizeof(Texture2D*)); \
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

renderer_param_value getShadowCubeMap(RendererBase* _self)
{
    renderer_param_value ret;
    TextureCube* tmp = NULL;
    TextureCube depth_map = _self->get_shadow_cube_map();
    if (depth_map) {
        tmp = (TextureCube*)SMalloc(sizeof(TextureCube));
        *tmp = depth_map;
    }
    ret.value = tmp;
    return ret;
}

renderer_param_value getLightInfluence(RendererBase* _self)
{
    renderer_param_value ret;
    float* tmp = NULL;
    tmp = (float*)SMalloc(sizeof(float));
    *tmp = _self->curt_light->get_influence();
    ret.value = tmp;
    return ret;
}

#define DefGetShadowDir(i) \
renderer_param_value getShadowDir##i(RendererBase* _self) \
{ \
    renderer_param_value ret; \
	EFloat3* tmp = (EFloat3*)SMalloc( sizeof(EFloat3) ); \
    *tmp = _self->curt_light->get_shadow_dir(i); \
    ret.value = tmp; \
    return ret; \
}
DefGetShadowDir(0);
DefGetShadowDir(1);
DefGetShadowDir(2);
DefGetShadowDir(3);
DefGetShadowDir(4);
DefGetShadowDir(5);
