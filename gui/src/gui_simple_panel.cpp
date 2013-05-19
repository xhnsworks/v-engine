//
//  gui_simple_panel.cpp
//  ecg
//
//  Created by 徐 海宁 on 13-5-19.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//

#include "gui_pch.h"
#include "gui_simple_panel.h"
#include "xml_resource.h"
#include "render_system.h"
#include "sprite_event_hub.h"
#include "sprite_renderer.h"
#include "sfloat3.h"
ImplementRTTI(GUISimplePanelLayer, SpriteNormalLayer);
ImplementRTTI(GUISimplePanel, Sprite);
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
void GUISimplePanelLayer::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
    matrix4x4 transform;
    GetMatrix(&transform);
    
	SpriteElementMap::iterator iterCenter = m_elementBuffer.find("center");
    
	SpriteElement tmpCenter = iterCenter->second;
    
	EFloat2 pivot;
	EFloat2 size;
	{
		xhn::RWLock::Instance inst = m_pivotHandle.GetReadLock();
		pivot = *((EFloat2*)m_pivotHandle.GetAttribute());
	}
	{
		xhn::RWLock::Instance inst = m_sizeHandle.GetReadLock();
		size = *((EFloat2*)m_sizeHandle.GetAttribute());
	}
    
	float left = tmpCenter.m_rect.left - pivot.x;
	float top = tmpCenter.m_rect.top - pivot.y;
	float right =
    tmpCenter.m_rect.left + tmpCenter.m_rect.size.width - pivot.x;
	float bottom =
    tmpCenter.m_rect.top + tmpCenter.m_rect.size.height - pivot.y;
    
	float realWidth = right - left;
	float realHeight = bottom - top;
    
	float scaleX = size.x / realWidth;
	float scaleY = size.y / realHeight;
    
	left *= scaleX;
	top *= scaleY;
	right *= scaleX;
	bottom *= scaleY;
    
    tmpCenter.m_rect.left = left;
	tmpCenter.m_rect.top = top;
	tmpCenter.m_rect.size.width = right - left;
	tmpCenter.m_rect.size.height = bottom - top;
    
    tmpCenter.ApplyTransform(&transform);

	to.push_back(tmpCenter);
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
void GUISimplePanelLayer::GetScopeImpl(SpriteRect& result)
{
	SpriteElementMap::iterator iterCenter = m_elementBuffer.find("center");
    
	SpriteElement tmpCenter = iterCenter->second;
    
	EFloat2 pivot;
	EFloat2 size;
	{
		xhn::RWLock::Instance inst = m_pivotHandle.GetReadLock();
		pivot = *((EFloat2*)m_pivotHandle.GetAttribute());
	}
	{
		xhn::RWLock::Instance inst = m_sizeHandle.GetReadLock();
		size = *((EFloat2*)m_sizeHandle.GetAttribute());
	}
    
	float left = tmpCenter.m_rect.left - pivot.x;
	float top = tmpCenter.m_rect.top - pivot.y;
	float right =
    tmpCenter.m_rect.left +
    tmpCenter.m_rect.size.width -
    pivot.x;
	float bottom =
    tmpCenter.m_rect.top +
    tmpCenter.m_rect.size.height -
    pivot.y;
    
	float realWidth = right - left;
	float realHeight = bottom - top;
    
	float scaleX = size.x / realWidth;
	float scaleY = size.y / realHeight;
    
	left *= scaleX;
	top *= scaleY;
	right *= scaleX;
	bottom *= scaleY;
    
	result.left = left;
	result.top = top;
	result.size.width = right - left;
	result.size.height = bottom - top;
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
GUISimplePanel::GUISimplePanel(SpriteRenderer* renderer, const xhn::static_string name)
: Sprite(renderer, name)
{
	///m_sizeHandle.m_attr = &m_size;
	m_sizeHandle.m_lock = ENEW xhn::RWLock;
	m_sizeHandle.AttachAttribute<EFloat2>();
}

void GUISimplePanel::Init(const xhn::static_string configName)
{
	XMLResourcePtr cfg = RenderSystem_load_gui_config(configName);
	if (TestResourcePtr(cfg)) {
		pugi::xml_document& doc = cfg->GetDocument();
		pugi::xml_node root = doc.child("root");
		pugi::xml_node layers = root.child("layers");
		pugi::xml_node baselayer = layers.child("base");
		if (!baselayer)
			return;
		SpriteLayerPtr layer =
        ENEW GUISimplePanelLayer("base", m_pivotHandle, m_sizeHandle);
		layer->LoadConfigImpl(baselayer);
		///m_children.push_back(layer);
        AddChild(layer);
	}
}

void GUISimplePanel::SetSize(float x, float y)
{
    xhn::RWLock::Instance inst = m_sizeHandle.GetWriteLock();
	EFloat2* size = (EFloat2*)m_sizeHandle.GetAttribute();
	size->x = x;
	size->y = y;
}

void GUISimplePanel::GetScopeImpl(SpriteRect& result)
{
	result.left = 0.0f;
	result.top = 0.0f;
	result.size.width = 0.0f;
	result.size.height = 0.0f;
}

Sprite* GUISimplePanelFactory::MakeSpriteImpl()
{
	char mbuf[256];
	snprintf(mbuf, 255, "GUISimplePanel_%d", m_simplePanelCount);
	m_simplePanelCount++;
	GUISimplePanel* ret = ENEW GUISimplePanel(m_renderer, mbuf);
	ret->Init(m_configName);
	ret->RegisterPublicEventCallback(&SpriteFrameStartEvent::s_RTTI,
                                     ENEW SpriteFrameStartEventProc(ret, m_renderer));
	return ret;
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
void GUISimplePanelFactory::CreateSheetConfig(const char* cfgName,
                                              const char* sheetName,
                                              const char* textureName,
										      const SpriteRect& panelRect,
                                              const SpriteSize& cornerSize,
                                              const SpriteRect& areaRect,
                                              const SpriteSize& areaCornerSize)
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
	elements.append_attribute("num_elements").set_value(1);
    
	pugi::xml_node center = elements.append_child("element");
    
	center.append_attribute("filename").set_value(textureName);
    
	float panelLeft = panelRect.left;
	float panelTop = panelRect.top;
	///float panelRight = panelRect.left + panelRect.size.width;
	///float panelBottom = panelRect.top + panelRect.size.height;
    
	float areaLeft = areaRect.left;
	float areaTop = areaRect.top;
	float areaRight = areaRect.left + areaRect.size.width;
	float areaBottom = areaRect.top + areaRect.size.height;
	
    center.append_attribute("name").set_value("center");
    
    center.append_attribute("left").set_value(panelLeft + cornerSize.width);
    center.append_attribute("top").set_value(panelTop + cornerSize.height);
    center.append_attribute("width").set_value(panelRect.size.width -
                                               cornerSize.width -
                                               cornerSize.width);
    center.append_attribute("height").set_value(panelRect.size.height -
                                                cornerSize.height -
                                                cornerSize.height);
    
    center.append_attribute("area_x0").set_value(areaLeft +
                                                 areaCornerSize.width);
    center.append_attribute("area_x1").set_value(areaRight -
                                                 areaCornerSize.width);
    center.append_attribute("area_y0").set_value(areaTop +
                                                 areaCornerSize.height);
    center.append_attribute("area_y1").set_value(areaBottom -
                                                 areaCornerSize.height);
    
    center.append_attribute("transparent").set_value(1.0f);
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
