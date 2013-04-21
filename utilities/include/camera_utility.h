/*
* Copyright (c) 2013, �캣��
* All rights reserved.
*
* �ļ����ƣ�camera_utility.h
* �ļ����������������
*
* ��ǰ�汾��0.1
* ��    �ߣ��캣��
* ������ڣ�2013��1��27��
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
