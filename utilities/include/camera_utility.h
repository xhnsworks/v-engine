/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：camera_utility.h
* 文件描述：摄像机工具
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef CAMERA_UTILITY_H
#define CAMERA_UTILITY_H
#include "common.h"
#include "etypes.h"
#include "camera.h"
struct _camera_utility;
typedef struct _camera_utility* CameraUtility;
API_EXPORT CameraUtility CameraUtility_new(Camera cam);
API_EXPORT void CameraUtility_delete(CameraUtility self);
API_EXPORT void CameraUtility_dolly_shot(CameraUtility self, float dist);
API_EXPORT void CameraUtility_pan(CameraUtility self, float x, float y);
API_EXPORT void CameraUtility_rotate(CameraUtility self, float x, float y);
#endif
