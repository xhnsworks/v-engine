//
//  gui_bar.cpp
//  ecg
//
//  Created by 徐 海宁 on 13-5-9.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//
#include "gui_pch.h"
#include "gui_bar.h"

#include "xml_resource.h"
#include "render_system.h"
#include "sprite_event_hub.h"
#include "sprite_renderer.h"
#include "sfloat3.h"
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
ImplementRTTI(GUIHoriBarLayer, SpriteNormalLayer);
ImplementRTTI(GUIHoriBar, Sprite);

void GUIHoriBarLayer::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
    matrix4x4 transform;
    GetMatrix(&transform);
    
	SpriteElementMap::iterator iterLeft = m_elementBuffer.find("left");
	SpriteElementMap::iterator iterRight = m_elementBuffer.find("right");
    
	SpriteElementMap::iterator iterCenter = m_elementBuffer.find("center");
    
	SpriteElement tmpLeft = iterLeft->second;
	SpriteElement tmpRight = iterRight->second;
    
	SpriteElement tmpCenter = iterCenter->second;
    
	Float2Attr pivot;
	FloatAttr size;
	{
		xhn::RWLock::Instance inst = m_pivotHandle.GetReadLock();
		pivot = *(m_pivotHandle.GetAttribute<Float2Attr>());
	}
	{
		xhn::RWLock::Instance inst = m_sizeHandle.GetReadLock();
		size = *(m_sizeHandle.GetAttribute<FloatAttr>());
	}
    
	float left = tmpLeft.m_rect.left - pivot.x;
	float top = tmpCenter.m_rect.top - pivot.y;
	float right =
    tmpRight.m_rect.left + tmpRight.m_rect.size.width - pivot.x;
    
	float realWidth = right - left;
    
	float scaleX = size.x / realWidth;

	left *= scaleX;
	right *= scaleX;
    
	tmpLeft.m_rect.left = left;
	tmpLeft.m_rect.top = top;
    
	tmpRight.m_rect.left = right - tmpRight.m_rect.size.width;
	tmpRight.m_rect.top = top;
    
    tmpCenter.m_rect.left = left + tmpLeft.m_rect.size.width;
	tmpCenter.m_rect.top = top;
	tmpCenter.m_rect.size.width =
    right - left - tmpRight.m_rect.size.width - tmpLeft.m_rect.size.width;
    
    tmpCenter.SetTransform(&transform);
    tmpLeft.SetTransform(&transform);
    tmpRight.SetTransform(&transform);

	to.push_back(tmpCenter);
    to.push_back(tmpLeft);
    to.push_back(tmpRight);
}

void GUIHoriBarLayer::GetScopeImpl(SpriteRect& result)
{    
	SpriteElementMap::iterator iterLeft = m_elementBuffer.find("left");
	SpriteElementMap::iterator iterRight = m_elementBuffer.find("right");
    
	SpriteElementMap::iterator iterCenter = m_elementBuffer.find("center");
    
	SpriteElement tmpLeft = iterLeft->second;
	SpriteElement tmpRight = iterRight->second;
    
	SpriteElement tmpCenter = iterCenter->second;
    
	Float2Attr pivot;
	FloatAttr size;
	{
		xhn::RWLock::Instance inst = m_pivotHandle.GetReadLock();
		pivot = *(m_pivotHandle.GetAttribute<Float2Attr>());
	}
	{
		xhn::RWLock::Instance inst = m_sizeHandle.GetReadLock();
		size = *(m_sizeHandle.GetAttribute<FloatAttr>());
	}
    
	float left = tmpLeft.m_rect.left - pivot.x;
	float top = tmpCenter.m_rect.top - pivot.y;
	float right = tmpRight.m_rect.left + tmpRight.m_rect.size.width - pivot.x;
    float bottom = tmpCenter.m_rect.top + tmpCenter.m_rect.size.height;
    
	float realWidth = right - left;
    
	float scaleX = size.x / realWidth;
    
	left *= scaleX;
	right *= scaleX;

	result.left = left;
	result.top = top;
	result.size.width = right - left;
	result.size.height = bottom - top;
}

GUIHoriBar::GUIHoriBar(SpriteRenderer* renderer,
                       const xhn::static_string name)
: Sprite(renderer, name)
{
	m_sizeHandle.m_lock = ENEW xhn::RWLock;
	m_sizeHandle.AttachAttribute<FloatAttr>();
}

void GUIHoriBar::Init(const xhn::static_string configName)
{
	XMLResourcePtr cfg = RenderSystem_load_gui_config(configName);
	if (TestResourcePtr(cfg)) {
		pugi::xml_document& doc = cfg->GetDocument();
		pugi::xml_node root = doc.child("root");
		pugi::xml_node layers = root.child("layers");
		pugi::xml_node baselayer = layers.child("base");
		if (!baselayer)
			return;
		SpriteLayerPtr layer = ENEW GUIHoriBarLayer("base",
                                                    m_pivotHandle,
                                                    m_sizeHandle);
		layer->LoadConfigImpl(baselayer);
        AddChild(layer);
		SetSize(100.0f);
	}
}

void GUIHoriBar::SetSize(float x)
{
    xhn::RWLock::Instance inst = m_sizeHandle.GetWriteLock();
	FloatAttr* size = m_sizeHandle.GetAttribute<FloatAttr>();
	size->x = x;
}

void GUIHoriBar::GetScopeImpl(SpriteRect& result)
{
	result.left = 0.0f;
	result.top = 0.0f;
	result.size.width = 0.0f;
	result.size.height = 0.0f;
}

Sprite* GUIHoriBarFactory::MakeSpriteImpl()
{
	char mbuf[256];
	snprintf(mbuf, 255, "GUIHoriBar_%d", m_horiBarCount);
	m_horiBarCount++;
	GUIHoriBar* ret = ENEW GUIHoriBar(m_renderer, mbuf);
	ret->Init(m_configName);
	ret->RegisterPublicEventCallback(&SpriteFrameStartEvent::s_RTTI,
                                     ENEW SpriteFrameStartEventProc(
                                         ret, m_renderer)
                                     );
	return ret;
}

void GUIHoriBarFactory::CreateSheetConfig(const char* cfgName,
                                          const char* sheetName,
                                          const char* textureName,
										  const SpriteRect& panelRect,
                                          float cornerSize,
                                          const SpriteRect& areaRect,
                                          float areaCornerSize)
{
    XMLResourcePtr xmlRes = RenderSystem_new_gui_config(cfgName);
	pugi::xml_document& doc = xmlRes->GetDocument();
	pugi::xml_node root = doc.child("root");
	if (!root)
	    root = doc.append_child("root");
	pugi::xml_node layers = root.child("layers");
	if (!layers)
		layers = root.append_child("layers");
	pugi::xml_node sheet = layers.append_child(sheetName);
	pugi::xml_node elements = sheet.append_child("elements");
	elements.append_attribute("num_elements").set_value(9);
    
	pugi::xml_node left = elements.append_child("element");
	pugi::xml_node right = elements.append_child("element");
    
	pugi::xml_node center = elements.append_child("element");
    
	left.append_attribute("filename").set_value(textureName);
	right.append_attribute("filename").set_value(textureName);
	    
	center.append_attribute("filename").set_value(textureName);
    
	float panelLeft = panelRect.left;
	float panelTop = panelRect.top;
	float panelRight = panelRect.left + panelRect.size.width;
    
	float areaLeft = areaRect.left;
	float areaTop = areaRect.top;
	float areaRight = areaRect.left + areaRect.size.width;
	float areaBottom = areaRect.top + areaRect.size.height;
    
	left.append_attribute("name").set_value("left");
    right.append_attribute("name").set_value("right");
    
    left.append_attribute("left").set_value(panelLeft);
    right.append_attribute("left").set_value(panelRight - cornerSize);
    
    left.append_attribute("top").set_value(panelTop);
    right.append_attribute("top").set_value(panelTop);
    
    left.append_attribute("width").set_value(cornerSize);
    right.append_attribute("width").set_value(cornerSize);
    
    left.append_attribute("height").set_value(panelRect.size.height);
    right.append_attribute("height").set_value(panelRect.size.height);
    
    left.append_attribute("area_x0").set_value(areaLeft);
    right.append_attribute("area_x0").set_value(areaRight -
                                                areaCornerSize);
    
    left.append_attribute("area_x1").set_value(areaLeft +
                                               areaCornerSize);
    right.append_attribute("area_x1").set_value(areaRight);
    
    left.append_attribute("area_y0").set_value(areaTop);
    right.append_attribute("area_y0").set_value(areaTop);
    
    left.append_attribute("area_y1").set_value(areaBottom);
    right.append_attribute("area_y1").set_value(areaBottom);
    
    left.append_attribute("transparent").set_value(1.0f);
    right.append_attribute("transparent").set_value(1.0f);
    
	center.append_attribute("name").set_value("center");
    
    center.append_attribute("left").set_value(panelLeft + cornerSize);
    center.append_attribute("top").set_value(panelTop);
    center.append_attribute("width").set_value(panelRect.size.width -
                                               cornerSize -
                                               cornerSize);
    center.append_attribute("height").set_value(panelRect.size.height);
    
    center.append_attribute("area_x0").set_value(areaLeft +
                                                 areaCornerSize);
    center.append_attribute("area_x1").set_value(areaRight -
                                                 areaCornerSize);
    center.append_attribute("area_y0").set_value(areaTop);
    center.append_attribute("area_y1").set_value(areaBottom);
    
    center.append_attribute("transparent").set_value(1.0f);
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
ImplementRTTI(GUIVertBarLayer, SpriteNormalLayer);
ImplementRTTI(GUIVertBar, Sprite);

void GUIVertBarLayer::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
    matrix4x4 transform;
    GetMatrix(&transform);
    
	SpriteElementMap::iterator iterTop = m_elementBuffer.find("top");
	SpriteElementMap::iterator iterBottom = m_elementBuffer.find("bottom");

	SpriteElementMap::iterator iterCenter = m_elementBuffer.find("center");

	SpriteElement tmpTop = iterTop->second;
	SpriteElement tmpBottom = iterBottom->second;

	SpriteElement tmpCenter = iterCenter->second;

	Float2Attr pivot;
	FloatAttr size;
	{
		xhn::RWLock::Instance inst = m_pivotHandle.GetReadLock();
		pivot = *(m_pivotHandle.GetAttribute<Float2Attr>());
	}
	{
		xhn::RWLock::Instance inst = m_sizeHandle.GetReadLock();
		size = *(m_sizeHandle.GetAttribute<FloatAttr>());
	}

	float left = tmpCenter.m_rect.left - pivot.x;
	float top = tmpTop.m_rect.top - pivot.y;
	float bottom =
    tmpBottom.m_rect.top + tmpBottom.m_rect.size.height - pivot.y;

	float realHeight = bottom - top;

	float scaleY = size.x / realHeight;

	top *= scaleY;
	bottom *= scaleY;

	tmpTop.m_rect.left = left;
	tmpTop.m_rect.top = top;

	tmpBottom.m_rect.left = left;
	tmpBottom.m_rect.top = bottom - tmpBottom.m_rect.size.height;

	tmpCenter.m_rect.left = left;
	tmpCenter.m_rect.top = top + tmpTop.m_rect.size.height;
	tmpCenter.m_rect.size.height =
    bottom - top - tmpTop.m_rect.size.height - tmpBottom.m_rect.size.height;
    
    tmpCenter.SetTransform(&transform);
    tmpTop.SetTransform(&transform);
    tmpBottom.SetTransform(&transform);

	to.push_back(tmpCenter);

	to.push_back(tmpTop);
	to.push_back(tmpBottom);
}

void GUIVertBarLayer::GetScopeImpl(SpriteRect& result)
{    
	SpriteElementMap::iterator iterTop = m_elementBuffer.find("top");
	SpriteElementMap::iterator iterBottom = m_elementBuffer.find("bottom");

	SpriteElementMap::iterator iterCenter = m_elementBuffer.find("center");
    
	SpriteElement tmpTop = iterTop->second;
	SpriteElement tmpBottom = iterBottom->second;

	SpriteElement tmpCenter = iterCenter->second;
    
	Float2Attr pivot;
	FloatAttr size;
	{
		xhn::RWLock::Instance inst = m_pivotHandle.GetReadLock();
		pivot = *(m_pivotHandle.GetAttribute<Float2Attr>());
	}
	{
		xhn::RWLock::Instance inst = m_sizeHandle.GetReadLock();
		size = *(m_sizeHandle.GetAttribute<FloatAttr>());
	}
   
	float left = tmpCenter.m_rect.left - pivot.x;
	float top = tmpTop.m_rect.top - pivot.y;
	float right = tmpCenter.m_rect.left + tmpCenter.m_rect.size.width - pivot.x;
	float bottom = tmpBottom.m_rect.top + tmpBottom.m_rect.size.height;

	float realHeight = bottom - top;

	float scaleY = size.x / realHeight;

	top *= scaleY;
	bottom *= scaleY;

	result.left = left;
	result.top = top;
	result.size.width = right - left;
	result.size.height = bottom - top;
}

GUIVertBar::GUIVertBar(SpriteRenderer* renderer,
                       const xhn::static_string name)
: Sprite(renderer, name)
{
    m_sizeHandle.m_lock = ENEW xhn::RWLock;
	m_sizeHandle.AttachAttribute<FloatAttr>();
}

void GUIVertBar::Init(const xhn::static_string configName)
{
	XMLResourcePtr cfg = RenderSystem_load_gui_config(configName);
	if (TestResourcePtr(cfg)) {
		pugi::xml_document& doc = cfg->GetDocument();
		pugi::xml_node root = doc.child("root");
		pugi::xml_node layers = root.child("layers");
		pugi::xml_node baselayer = layers.child("base");
		if (!baselayer)
			return;
		SpriteLayerPtr layer = ENEW GUIVertBarLayer("base",
                                                    m_pivotHandle,
                                                    m_sizeHandle);
		layer->LoadConfigImpl(baselayer);
        AddChild(layer);
		SetSize(100.0f);
	}
}

void GUIVertBar::SetSize(float x)
{
	xhn::RWLock::Instance inst = m_sizeHandle.GetWriteLock();
	FloatAttr* size = m_sizeHandle.GetAttribute<FloatAttr>();
	size->x = x;
}

void GUIVertBar::GetScopeImpl(SpriteRect& result)
{
	result.left = 0.0f;
	result.top = 0.0f;
	result.size.width = 0.0f;
	result.size.height = 0.0f;
}

Sprite* GUIVertBarFactory::MakeSpriteImpl()
{
	char mbuf[256];
	snprintf(mbuf, 255, "GUIVertBar_%d", m_vertBarCount);
	m_vertBarCount++;
	GUIVertBar* ret = ENEW GUIVertBar(m_renderer, mbuf);
	ret->Init(m_configName);
	ret->RegisterPublicEventCallback(&SpriteFrameStartEvent::s_RTTI,
                                     ENEW SpriteFrameStartEventProc(
                                         ret, m_renderer)
                                     );
	return ret;
}

void GUIVertBarFactory::CreateSheetConfig(const char* cfgName,
                                          const char* sheetName,
                                          const char* textureName,
										  const SpriteRect& panelRect,
                                          float cornerSize,
                                          const SpriteRect& areaRect,
                                          float areaCornerSize)
{
	
	XMLResourcePtr xmlRes = RenderSystem_new_gui_config(cfgName);
	pugi::xml_document& doc = xmlRes->GetDocument();
	pugi::xml_node root = doc.child("root");
	if (!root)
		root = doc.append_child("root");
	pugi::xml_node layers = root.child("layers");
	if (!layers)
		layers = root.append_child("layers");
	pugi::xml_node sheet = layers.append_child(sheetName);
	pugi::xml_node elements = sheet.append_child("elements");
	elements.append_attribute("num_elements").set_value(9);

	pugi::xml_node top = elements.append_child("element");
	pugi::xml_node bottom = elements.append_child("element");

	pugi::xml_node center = elements.append_child("element");

	top.append_attribute("filename").set_value(textureName);
	bottom.append_attribute("filename").set_value(textureName);

	center.append_attribute("filename").set_value(textureName);

	float panelLeft = panelRect.left;
	float panelTop = panelRect.top;
	float panelBottom = panelRect.top + panelRect.size.height;

	float areaLeft = areaRect.left;
	float areaTop = areaRect.top;
	float areaRight = areaRect.left + areaRect.size.width;
	float areaBottom = areaRect.top + areaRect.size.height;

	top.append_attribute("name").set_value("top");
    bottom.append_attribute("name").set_value("bottom");
    
    top.append_attribute("left").set_value(panelLeft);
    bottom.append_attribute("left").set_value(panelLeft);
    
    top.append_attribute("top").set_value(panelTop);
    bottom.append_attribute("top").set_value(panelBottom - cornerSize);
    
    top.append_attribute("width").set_value(panelRect.size.width);
    bottom.append_attribute("width").set_value(panelRect.size.width);
    
    top.append_attribute("height").set_value(cornerSize);
    bottom.append_attribute("height").set_value(cornerSize);
    
    top.append_attribute("area_x0").set_value(areaLeft);
    bottom.append_attribute("area_x0").set_value(areaLeft);
    
    top.append_attribute("area_x1").set_value(areaRight);
    bottom.append_attribute("area_x1").set_value(areaRight);
    
    top.append_attribute("area_y0").set_value(areaTop);
    bottom.append_attribute("area_y0").set_value(areaBottom -
                                                 areaCornerSize);
    
    top.append_attribute("area_y1").set_value(areaTop + areaCornerSize);
    bottom.append_attribute("area_y1").set_value(areaBottom);
    
    top.append_attribute("transparent").set_value(1.0f);
    bottom.append_attribute("transparent").set_value(1.0f);

	center.append_attribute("name").set_value("center");
    
    center.append_attribute("left").set_value(panelLeft);
    center.append_attribute("top").set_value(panelTop + cornerSize);
    center.append_attribute("width").set_value(panelRect.size.width);
    center.append_attribute("height").set_value(panelRect.size.height -
                                                cornerSize -
                                                cornerSize);
    
    center.append_attribute("area_x0").set_value(areaLeft);
    center.append_attribute("area_x1").set_value(areaRight);
    center.append_attribute("area_y0").set_value(areaTop +
                                                 areaCornerSize);
    center.append_attribute("area_y1").set_value(areaBottom -
                                                 areaCornerSize);
    
    center.append_attribute("transparent").set_value(1.0f);
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///