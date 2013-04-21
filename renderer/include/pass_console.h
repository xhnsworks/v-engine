/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：pass_console.h
* 文件描述：pass控制台，包含pass生成相关的常用函数和宏定义
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef PASS_CONSOLE_H
#define PASS_CONSOLE_H
#include "common.h"
#include "etypes.h"
#include "shader.h"
#include "pass.h"
#include "shader_node.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"
#include "vertex_declaration.h"
///#include "material_instance.h"

#define CAMERA_POSITION       "UnifCameraPosition"
#define CAMERA_DIRECTION      "UnifCameraDirection"

#define MATERIAL_ID           "MaterialID"

#define LIGHT_ORIGIN          "LightOrigin"
#define LIGHT_DIRECTION       "LightDirection"


#define COLOR_MAP             "ColorMap"
#define PIXEL_SIZE            "PixelSize"
///#define LIGHTING_MAP          "LightingMap"


#define COLOR_OUT             "gl_FragData[0]"

#define NORMAL_MAP                      "NormalMap"
#define DEPTH_MAP                       "DepthMap"
#define TEXCOORD                        "vTexCoord"
#define LIGHT_POSITION                  "LightPosition"
#define LIGHT_DIRECTION                 "LightDirection"
#define LIGHT_COLOR                     "LightColor"
#define LIGHT_ATTE_COEF                 "LightAtteCoef"
#define DIFFUSE_LIGHTING_MAP            "DiffuseLightingMap"
#define SPECULAR_LIGHTING_MAP           "SpecularLightingMap"
#define CAMERA_PLANE_WIDTH              "CamWidth"
#define CAMERA_PLANE_HEIGHT             "CamHeight"
#define CAMERA_PLANE_NEAR               "CamNearPlane"
#define CAMERA_PLANE_FAR                "CamFarPlane"
#define INVERT_CAMERA_WORLD_MATRIX      "InvCamWorldMatrix"
#define INVERT_CAMERA_PROJECTION_MATRIX "InvCamProjMatrix"
#define CAMERA_WORLD_MATRIX             "CamWorldMatrix"
#define CAMERA_PROJECTION_MATRIX        "CamProjMatrix"
///#define CAMERA_POSITION                 "CamPos"

#define LIGHT_INNER_COS                 "LightInnerCos"
#define LIGHT_OUTER_COS                 "LightOuterCos"

#define SHADOW_MAP                      "ShadowMap"
#define SHADOW_CUBE_MAP                 "ShadowCubeMap"
#define LIGHT_INFLUENCE                 "LightInfluence"
#define LIGHT_WORLD_MATRIX              "LightWorldMatrix"
#define LIGHT_PROJ_MATRIX               "LightProjMatrix"
#define SHADOW_DIRECTION                "ShadowDirection"

///#define DISPLAY_POSITION

API_EXPORT Shader create_shader_from_file(const char* _file_name, shader_type _type);
API_EXPORT Pass create_pass_from_shader(Shader _vtx_sdr, Shader _pxl_sdr);

API_EXPORT ShaderNode create_map_sample_node();
API_EXPORT ShaderNode create_map_sample_node_rgba();
API_EXPORT ShaderNode create_clear_lighting_value_node();
#endif
