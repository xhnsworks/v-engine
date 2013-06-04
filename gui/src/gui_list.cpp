//
//  gui_list.cpp
//  ecg
//
//  Created by 徐 海宁 on 13-6-3.
//  Copyright (c) 2013年 徐 海宁. All rights reserved.
//
#include "gui_pch.h"
#include "gui_list.h"
#include "render_system.h"
#include "sprite_event_hub.h"
#include "robot.h"
#include "animation.hpp"

ImplementRTTI(GUIListEntry, GUIHoriBar);
ImplementRTTI(GUIList, GUIPanel);
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
GUIListEntryFactory::GUIListEntryFactory(SpriteRenderer* renderer,
                                         const char* cfgName)
: GUIHoriBarFactory(renderer, cfgName)
{
	Float2Attr size(100.0f, 0.0f);
	m_sizeHandle.m_lock = ENEW xhn::RWLock;
	m_sizeHandle.AttachAttribute<Float2Attr>();
	m_sizeHandle.SetAttribute(&size);
}

Sprite* GUIListEntryFactory::MakeSpriteImpl()
{
    char mbuf[256];
	snprintf(mbuf, 255, "GUIListEntry_%d", m_horiBarCount);
	m_horiBarCount++;
	GUIListEntry* ret = ENEW GUIListEntry(m_renderer,
                                          mbuf,
                                          m_sizeHandle);
	ret->Init(m_configName);
	return ret;
}

void CreateEntryLayer(pugi::xml_node sheet,
                      const char* textureName,
                      const SpriteRect& panelRect,
                      float cornerSize,
                      const EFloat2& areaSize,
                      float areaCornerSize,
                      const EFloat2& areaCoord)
{
    float panelLeft = panelRect.left;
	float panelTop = panelRect.top;
	float panelRight = panelRect.left + panelRect.size.width;
    
    float areaLeft = areaCoord.x;
    float areaTop = areaCoord.y;
    float areaRight = areaCoord.x + areaSize.x;
    float areaBottom = areaCoord.y + areaSize.y;
    
    pugi::xml_node elements = sheet.append_child("elements");
	elements.append_attribute("num_elements").set_value(3);
    
    pugi::xml_node left = elements.append_child("element");
	pugi::xml_node right = elements.append_child("element");
    
	pugi::xml_node center = elements.append_child("element");
    
	left.append_attribute("filename").set_value(textureName);
	right.append_attribute("filename").set_value(textureName);
    
	center.append_attribute("filename").set_value(textureName);
    
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
    
    left.append_attribute("transparency").set_value(1.0f);
    right.append_attribute("transparency").set_value(1.0f);
    
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
    
    center.append_attribute("transparency").set_value(1.0f);
}

void GUIListEntryFactory::CreateSheetConfig(
                                            const char* cfgName,
                                            const char* textureName,
                                            const SpriteRect& panelRect,
                                            float cornerSize,
                                            const EFloat2& areaSize,
                                            float areaCornerSize,
                                            const EFloat2& areaCoordNormal,
                                            const EFloat2& areaCoordTouched,
                                            const EFloat2& areaCoorfSelected)
{
    XMLResourcePtr xmlRes = RenderSystem_new_gui_config(cfgName);
	pugi::xml_document& doc = xmlRes->GetDocument();
	pugi::xml_node root = doc.child("root");
	if (!root)
		root = doc.append_child("root");
	pugi::xml_node layers = root.child("layers");
	if (!layers)
		layers = root.append_child("layers");
	
    {
        pugi::xml_node sheet = layers.append_child("normal");
        CreateEntryLayer(sheet,
                         textureName,
                         panelRect,
                         cornerSize,
                         areaSize,
                         areaCornerSize,
                         areaCoordNormal);
    }
    {
        pugi::xml_node sheet = layers.append_child("touched");
        CreateEntryLayer(sheet,
                         textureName,
                         panelRect,
                         cornerSize,
                         areaSize,
                         areaCornerSize,
                         areaCoordTouched);
    }
    {
        pugi::xml_node sheet = layers.append_child("selected");
        CreateEntryLayer(sheet,
                         textureName,
                         panelRect,
                         cornerSize,
                         areaSize,
                         areaCornerSize,
                         areaCoorfSelected);
    }
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
void GUIListEntry::OnMouseMove(const SpriteMouseMoveEvent* mouseEvt)
{
	const FourBorders& borders = GetFourBorders();
	EFloat2 realCrd =
    m_renderer->get_real_position((float)mouseEvt->m_curtMousePos.x,
                                  (float)mouseEvt->m_curtMousePos.y);
	EFloat3 realPt(realCrd.x, realCrd.y, 0.0f);
	sfloat3 pt = SFloat3_assign_from_efloat3(&realPt);
    
	if (borders.IsInBorders(pt)) {
		SetState(GUITouchable::Touched);
	}
	else {
		SetState(GUITouchable::Normal);
	}
}
void GUIListEntry::OnMouseButtonDown(const SpriteMouseButtonDownEvent* mouseEvt)
{
    if (mouseEvt->m_leftButtomDown) {
        if (GetState() == GUITouchable::Touched) {
            SpriteLayer* parent = GetParent();
            if (parent) {
                parent = parent->GetParent();
                if (parent) {
                    GUIComboBox* comboxBox = parent->DynamicCast<GUIComboBox>();
                    if (comboxBox) {
                        xhn::string text = GetText();
                        comboxBox->SetText(text);
                    }
                }
            }
        }
    }
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
GUIListEntry::~GUIListEntry()
{
}

void GUIListEntry::Init(const xhn::static_string configName)
{
    XMLResourcePtr cfg = RenderSystem_load_gui_config(configName);
	if (TestResourcePtr(cfg)) {
		pugi::xml_document& doc = cfg->GetDocument();
		pugi::xml_node root = doc.child("root");
		pugi::xml_node layers = root.child("layers");
		pugi::xml_node normallayer = layers.child("normal");
        pugi::xml_node touchedlayer = layers.child("touched");
        pugi::xml_node selectedlayer = layers.child("selected");
		if (!normallayer ||
            !touchedlayer ||
            !selectedlayer)
			return;
        {
            SpriteLayerPtr layer = ENEW GUIHoriBarLayer("normal",
                                                        m_pivotHandle,
                                                        m_sizeHandle);
            layer->LoadConfigImpl(normallayer);
            AddChild(layer);
        }
		{
            SpriteLayerPtr layer = ENEW GUIHoriBarLayer("touched",
                                                        m_pivotHandle,
                                                        m_sizeHandle);
            layer->LoadConfigImpl(touchedlayer);
            AddChild(layer);
        }
        {
            SpriteLayerPtr layer = ENEW GUIHoriBarLayer("selected",
                                                        m_pivotHandle,
                                                        m_sizeHandle);
            layer->LoadConfigImpl(selectedlayer);
            AddChild(layer);
        }
		{
			SpriteLayerPtr layer = ENEW SpriteTextLayer("text");
			layer->m_horizontalAlignmentMode = SpriteLayer::CenterHorizontalAligned;
			layer->m_verticalAlignmentMode = SpriteLayer::CenterVerticalAligned;
            AddChild(layer);
		}
        SetState(GUITouchable::Normal);
	}
}

void GUIListEntry::BuildBackgroundLayer(xhn::list<SpriteElement>& to)
{
    switch (GetState())
	{
        case Normal:
		{
            SpriteLayerPtr layerPtr = GetLayer("normal");
            if (layerPtr.get())
                layerPtr->BuildElementsImpl(to);
		}
            break;
        case Touched:
		{
            SpriteLayerPtr layerPtr = GetLayer("touched");
            if (layerPtr.get())
                layerPtr->BuildElementsImpl(to);
		}
            break;
        case Selected:
        default:
		{
            SpriteLayerPtr layerPtr = GetLayer("selected");
            if (layerPtr.get())
                layerPtr->BuildElementsImpl(to);
		}
            break;
	}
}

void GUIListEntry::BuildTextLayer(xhn::list<SpriteElement>& to)
{
	SpriteLayerPtr layerPtr = GetLayer("text");
	if (layerPtr.get())
		layerPtr->BuildElementsImpl(to);
}

void GUIListEntry::SetText(const xhn::string& text)
{
	SpriteLayerPtr layerPtr = GetLayer("text");
	if (layerPtr.get()) {
		SpriteTextLayer* textLayer = layerPtr->DynamicCast<SpriteTextLayer>();
		EColor color(1.0f, 1.0f, 1.0f, 1.0f);
		textLayer->SetText(text, color, 2.0f, 1.0f, Pixel16);
	}
}

xhn::string GUIListEntry::GetText()
{
	SpriteLayerPtr layerPtr = GetLayer("text");
	if (layerPtr.get()) {
		SpriteTextLayer* textLayer = layerPtr->DynamicCast<SpriteTextLayer>();
		EColor color(1.0f, 1.0f, 1.0f, 1.0f);
		return textLayer->GetText();
	}
	return "";
}

void GUIListEntry::GetBackgroundRect(SpriteRect& rect)
{
	SpriteLayerPtr layerPtr = GetLayer("normal");
	if (layerPtr.get()) {
		layerPtr->GetScope(rect);
	}
}

void GUIListEntry::Build()
{
	m_elements.clear();
}

void GUIListEntry::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
	BuildTextLayer(to);
	BuildBackgroundLayer(to);
	BuildDropDownMenu(to);
	
	SpriteRect rect;
	GetScope(rect);
	rect.GetFourBorders(m_renderer, m_fourBorders);
    
	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);
    
	m_fourBorders.ApplyTranform(&mat);
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
GUIList::GUIList(SpriteRenderer* renderer,
                 GUIListEntryFactory* entryFactory,
                 AttributeHandle sizeHandle)
: m_entryFactory(entryFactory)
, GUIPanel(renderer, "list", sizeHandle)
, m_entryCount(0)
{
	Float2Attr size(100.0f, 0.0f);
	m_sizeHandle.SetAttribute(&size);
}

void GUIList::AddEntry(const xhn::string& str)
{
    GUIListEntry* entry = m_entryFactory->MakeSprite()->DynamicCast<GUIListEntry>();
	entry->SetText(str);
	SpriteRect rect;
	entry->GetScope(rect);
	entry->SetCoord(0.0f, rect.size.height * (float)m_entryCount);
	AddChild(entry);
	AlwaysOnTop(entry);
	m_entryCount++;
}

void GUIList::RemoveAllEntries()
{
	SpriteLayerList::iterator iter = m_children.begin();
	SpriteLayerList::iterator end = m_children.end();
	for (; iter != end; ) {
		SpriteLayerPtr sptLayerPtr = *iter;
		if (sptLayerPtr->DynamicCast<GUIListEntry>() != NULL) {
            iter = m_children.remove(iter);
		}
		else
			iter++;
	}
	m_entryCount = 0;
}

void GUIList::RemoveBackground()
{
	SpriteLayerList::iterator iter = m_children.begin();
	SpriteLayerList::iterator end = m_children.end();
	for (; iter != end; ) {
		SpriteLayerPtr sptLayerPtr = *iter;
		if (sptLayerPtr->DynamicCast<GUIListEntry>() == NULL) {
			iter = m_children.remove(iter);
		}
		else
			iter++;
	}
}

void GUIList::GetBackgroundRect(SpriteRect& rect)
{
	SpriteLayerList::iterator iter = m_children.begin();
	SpriteLayerList::iterator end = m_children.end();
	for (; iter != end; iter++) {
		SpriteLayerPtr sptLayerPtr = *iter;
        
		GUIPanelLayer* background = sptLayerPtr->DynamicCast<GUIPanelLayer>();
		if (background) {
            background->GetScope(rect);
			return;
		}
	}
}

void GUIList::BuildFourBorders()
{
	SpriteRect rect;
	GetBackgroundRect(rect);
	rect.GetFourBorders(m_renderer, m_fourBorders);
	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);
	m_fourBorders.ApplyTranform(&mat);
}
void GUIList::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
	xhn::list<SpriteElement> buffer;
	BuildElements(buffer);
	BuildFourBorders();
    
	xhn::list<SpriteElement>::iterator iter = buffer.begin();
	xhn::list<SpriteElement>::iterator end = buffer.end();
	for (; iter != end; iter++) {
		SpriteElement& ele = *iter;
		ele.m_fourBorders = &GetFourBorders();
		to.push_back(ele);
	}
}
void GUIList::Build()
{
	m_elements.clear();
	BuildElements(m_elements);
	BuildFourBorders();
    
	xhn::list<SpriteElement>::iterator iter = m_elements.begin();
	xhn::list<SpriteElement>::iterator end = m_elements.end();
	for (; iter != end; iter++) {
		SpriteElement& ele = *iter;
		ele.m_fourBorders = &GetFourBorders();
	}
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
GUIListFactory::GUIListFactory(SpriteRenderer* renderer,
                               const char* cfgName,
                               GUIListEntryFactory* entryFactory,
                               AttributeHandle listSizeHandle)
: m_entryFactory(entryFactory)
, m_listSizeHandle(listSizeHandle)
, GUIPanelFactory(renderer, cfgName)
{
}

Sprite* GUIListFactory::MakeSpriteImpl()
{
	GUIList* ret = ENEW GUIList(m_renderer,
                                m_entryFactory,
                                m_listSizeHandle);
	ret->Init(m_configName);
	return ret;
}

void GUIListFactory::CreateAnimationConfig(const char* cfgName,
                                           const char* showAnimName,
                                           const char* hideAnimName,
                                           float width,
                                           float maxHeight)
{
    XMLResourcePtr xmlRes = RenderSystem_new_animation_config(cfgName);
    pugi::xml_document& doc = xmlRes->GetDocument();
	pugi::xml_node root = doc.append_child("root");
    pugi::xml_node animations = root.append_child("animations");
    pugi::xml_node showAnim = animations.append_child(showAnimName);
    pugi::xml_node hideAnim = animations.append_child(hideAnimName);
    {
        showAnim.append_attribute("num_timelines").set_value(1);
        pugi::xml_node timeLine = showAnim.append_child("timeline");
        timeLine.append_attribute("name").set_value("show");
        timeLine.append_attribute("type").set_value("Float2");
        timeLine.append_attribute("num_keyframes").set_value(2);
        timeLine.append_attribute("is_looped").set_value(false);
        pugi::xml_node keyframe0 = timeLine.append_child("keyframe");
        pugi::xml_node keyframe1 = timeLine.append_child("keyframe");
        keyframe0.append_attribute("time").set_value(0.0f);
        keyframe0.append_attribute("value_x").set_value(width);
        keyframe0.append_attribute("value_y").set_value(0.0f);
        keyframe1.append_attribute("time").set_value(0.25f);
        keyframe1.append_attribute("value_x").set_value(width);
        keyframe1.append_attribute("value_y").set_value(maxHeight);
    }
    {
		hideAnim.append_attribute("num_timelines").set_value(1);
		pugi::xml_node timeLine = hideAnim.append_child("timeline");
		timeLine.append_attribute("name").set_value("show");
		timeLine.append_attribute("type").set_value("Float2");
		timeLine.append_attribute("num_keyframes").set_value(2);
		timeLine.append_attribute("is_looped").set_value(false);
		pugi::xml_node keyframe0 = timeLine.append_child("keyframe");
		pugi::xml_node keyframe1 = timeLine.append_child("keyframe");
		keyframe0.append_attribute("time").set_value(0.0f);
		keyframe0.append_attribute("value_x").set_value(width);
		keyframe0.append_attribute("value_y").set_value(maxHeight);
		keyframe1.append_attribute("time").set_value(0.25f);
		keyframe1.append_attribute("value_x").set_value(width);
		keyframe1.append_attribute("value_y").set_value(0.0f);
    }
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///