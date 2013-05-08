/*
* Copyright (c) 2013, 徐海宁
* All rights reserved.
*
* 文件名称：material_instance.h
* 文件描述：材质实例
*
* 当前版本：0.1
* 作    者：徐海宁
* 完成日期：2013年1月27日
*/
#ifndef MATERIAL_INSTANCE_H
#define MATERIAL_INSTANCE_H
#include "common.h"
#include "etypes.h"
#include "vertex_declaration.h"
#include "matrix4x4.h"
#include "texture2d.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader_object.h"
#include "pixel_shader_buffer.h"
#include "pass.h"
#include "texture2d.h"
#include "xhn_static_string.hpp"
#include "xhn_smart_ptr.hpp"
class MaterialInstance : public RefObject
{
private:
	Texture2DPtr m_colTex;
	Texture2DPtr m_norTex;
	xhn::static_string m_matName;
public:
	MaterialInstance(const char* matName, const char* colTexFile, const char* norTexFile, const char* resGrp);
	MaterialInstance(const MaterialInstance& matInst)
		: m_colTex(matInst.m_colTex)
		, m_norTex(matInst.m_norTex)
		, m_matName(matInst.m_matName)
	{}
	MaterialInstance(const MaterialInstance* matInst)
		: m_colTex(matInst->m_colTex)
		, m_norTex(matInst->m_norTex)
		, m_matName(matInst->m_matName)
	{}
	MaterialInstance(const xhn::SmartPtr<MaterialInstance>& matInst)
		: m_colTex(matInst->m_colTex)
		, m_norTex(matInst->m_norTex)
		, m_matName(matInst->m_matName)
	{}
	~MaterialInstance() {}
	inline Texture2DPtr GetColorTexture() {
		return m_colTex;
	}
	inline Texture2DPtr GetNormalTexture() {
		return m_norTex;
	}
	inline bool HasColorTexture() {
		return m_colTex.get() != NULL;
	}
	inline bool HasNormalTexture() {
		return m_norTex.get() != NULL;
	}
	inline xhn::static_string GetMaterialName() {
		return m_matName;
	}
	MaterialInstance* Clone();
};
typedef xhn::SmartPtr<MaterialInstance> MaterialInstancePtr;
#endif
