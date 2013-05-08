#include "gui_pch.h"
#include "gui_panel.h"
#include "xml_resource.h"
#include "render_system.h"
#include "sprite_event_hub.h"
#include "sprite_renderer.h"
#include "sfloat3.h"
ImplementRTTI(GUIPanelLayer, SpriteNormalLayer);
ImplementRTTI(GUIPanel, Sprite);

void GUIPanelLayer::BuildElements(xhn::list<SpriteElement>& to)
{
	xhn::map<xhn::static_string, SpriteElement>::iterator iterLowerLeft = m_elementBuffer.find("lower_left_corner");
	xhn::map<xhn::static_string, SpriteElement>::iterator iterUpperLeft = m_elementBuffer.find("upper_left_corner");
	xhn::map<xhn::static_string, SpriteElement>::iterator iterUpperRight = m_elementBuffer.find("upper_right_corner");
	xhn::map<xhn::static_string, SpriteElement>::iterator iterLowerRight = m_elementBuffer.find("lower_right_corner");

	xhn::map<xhn::static_string, SpriteElement>::iterator iterLeft = m_elementBuffer.find("left");
	xhn::map<xhn::static_string, SpriteElement>::iterator iterTop = m_elementBuffer.find("top");
	xhn::map<xhn::static_string, SpriteElement>::iterator iterRight = m_elementBuffer.find("right");
	xhn::map<xhn::static_string, SpriteElement>::iterator iterBottom = m_elementBuffer.find("bottom");

	xhn::map<xhn::static_string, SpriteElement>::iterator iterCenter = m_elementBuffer.find("center");

	SpriteElement tmpLowerLeft = iterLowerLeft->second;
	SpriteElement tmpUpperLeft = iterUpperLeft->second;
	SpriteElement tmpUpperRight = iterUpperRight->second;
	SpriteElement tmpLowerRight = iterLowerRight->second;

	SpriteElement tmpLeft = iterLeft->second;
	SpriteElement tmpTop = iterTop->second;
	SpriteElement tmpRight = iterRight->second;
	SpriteElement tmpBottom = iterBottom->second;

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

	float left = tmpUpperLeft.m_rect.left - pivot.x;
	float top = tmpUpperLeft.m_rect.top - pivot.y;
	float right = tmpLowerRight.m_rect.left + tmpLowerRight.m_rect.width - pivot.x;
	float bottom = tmpLowerRight.m_rect.top + tmpLowerRight.m_rect.height - pivot.y;

	float realWidth = right - left;
	float realHeight = bottom - top;

	float scaleX = size.x / realWidth;
	float scaleY = size.y / realHeight;

	left *= scaleX;
	top *= scaleY;
	right *= scaleX;
	bottom *= scaleY;

	tmpLowerLeft.m_rect.left = left;
	tmpLowerLeft.m_rect.top = bottom - tmpLowerLeft.m_rect.height;

    tmpUpperLeft.m_rect.left = left;
	tmpUpperLeft.m_rect.top = top;

    tmpUpperRight.m_rect.left = right - tmpUpperRight.m_rect.width;
	tmpUpperRight.m_rect.top = top;

    tmpLowerRight.m_rect.left = right - tmpLowerRight.m_rect.width;
	tmpLowerRight.m_rect.top = bottom - tmpLowerRight.m_rect.height;

	tmpLeft.m_rect.left = left;
	tmpLeft.m_rect.top = top + tmpUpperLeft.m_rect.height;
	tmpLeft.m_rect.height = tmpLowerLeft.m_rect.top - tmpUpperLeft.m_rect.top - tmpUpperLeft.m_rect.height;

	tmpTop.m_rect.left = left + tmpUpperLeft.m_rect.width;
	tmpTop.m_rect.top = top;
	tmpTop.m_rect.width = tmpUpperRight.m_rect.left - tmpUpperLeft.m_rect.left - tmpUpperLeft.m_rect.width;

	tmpRight.m_rect.left = right - tmpRight.m_rect.width;
	tmpRight.m_rect.top = tmpUpperRight.m_rect.top + tmpUpperRight.m_rect.height;
	tmpRight.m_rect.height = tmpLowerRight.m_rect.top - tmpUpperRight.m_rect.top - tmpUpperRight.m_rect.height;

	tmpBottom.m_rect.left = left + tmpLowerLeft.m_rect.width;
	tmpBottom.m_rect.top = tmpLowerLeft.m_rect.top;
	tmpBottom.m_rect.width = right - tmpLowerLeft.m_rect.left - tmpLowerLeft.m_rect.width;

    tmpCenter.m_rect.left = left + tmpLeft.m_rect.width;
	tmpCenter.m_rect.top = top + tmpTop.m_rect.height;
	tmpCenter.m_rect.width = tmpTop.m_rect.width;
	tmpCenter.m_rect.height = tmpRight.m_rect.height;

	to.push_back(tmpCenter);

	to.push_back(tmpLowerLeft);
	to.push_back(tmpLowerRight);
	to.push_back(tmpUpperLeft);
	to.push_back(tmpUpperRight);

	to.push_back(tmpLeft);
	to.push_back(tmpTop);
	to.push_back(tmpRight);
	to.push_back(tmpBottom);
}

void GUIPanelLayer::GetScopeImpl(SpriteRect& result)
{
	xhn::map<xhn::static_string, SpriteElement>::iterator iterLowerLeft = m_elementBuffer.find("lower_left_corner");
	xhn::map<xhn::static_string, SpriteElement>::iterator iterUpperLeft = m_elementBuffer.find("upper_left_corner");
	xhn::map<xhn::static_string, SpriteElement>::iterator iterUpperRight = m_elementBuffer.find("upper_right_corner");
	xhn::map<xhn::static_string, SpriteElement>::iterator iterLowerRight = m_elementBuffer.find("lower_right_corner");

	xhn::map<xhn::static_string, SpriteElement>::iterator iterLeft = m_elementBuffer.find("left");
	xhn::map<xhn::static_string, SpriteElement>::iterator iterTop = m_elementBuffer.find("top");
	xhn::map<xhn::static_string, SpriteElement>::iterator iterRight = m_elementBuffer.find("right");
	xhn::map<xhn::static_string, SpriteElement>::iterator iterBottom = m_elementBuffer.find("bottom");

	xhn::map<xhn::static_string, SpriteElement>::iterator iterCenter = m_elementBuffer.find("center");

	SpriteElement tmpLowerLeft = iterLowerLeft->second;
	SpriteElement tmpUpperLeft = iterUpperLeft->second;
	SpriteElement tmpUpperRight = iterUpperRight->second;
	SpriteElement tmpLowerRight = iterLowerRight->second;

	SpriteElement tmpLeft = iterLeft->second;
	SpriteElement tmpTop = iterTop->second;
	SpriteElement tmpRight = iterRight->second;
	SpriteElement tmpBottom = iterBottom->second;

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

	float left = tmpUpperLeft.m_rect.left - pivot.x;
	float top = tmpUpperLeft.m_rect.top - pivot.y;
	float right = tmpLowerRight.m_rect.left + tmpLowerRight.m_rect.width - pivot.x;
	float bottom = tmpLowerRight.m_rect.top + tmpLowerRight.m_rect.height - pivot.y;

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
	result.width = right - left;
	result.height = bottom - top;
}

GUIPanel::GUIPanel(SpriteRenderer* renderer, const xhn::static_string name)
: Sprite(renderer, name)
{
	///m_sizeHandle.m_attr = &m_size;
	m_sizeHandle.m_lock = ENEW xhn::RWLock;
	m_sizeHandle.AttachAttribute<EFloat2>();
}

void GUIPanel::Init(const xhn::static_string configName)
{
	XMLResourcePtr cfg = RenderSystem_load_gui_config(configName);
	if (TestResourcePtr(cfg)) {
		pugi::xml_document& doc = cfg->GetDocument();
		pugi::xml_node root = doc.child("root");
		pugi::xml_node layers = root.child("layers");
		pugi::xml_node baselayer = layers.child("base");
		if (!baselayer)
			return;
		SpriteLayerPtr layer = ENEW GUIPanelLayer("base", m_pivotHandle, m_sizeHandle);
		layer->LoadConfig(baselayer);
		m_children.push_back(layer);
	}
}

void GUIPanel::SetSize(float x, float y)
{
    xhn::RWLock::Instance inst = m_sizeHandle.GetWriteLock();
	EFloat2* size = (EFloat2*)m_sizeHandle.GetAttribute();
	size->x = x;
	size->y = y;
}

void GUIPanel::GetScopeImpl(SpriteRect& result)
{
	result.left = 0.0f;
	result.top = 0.0f;
	result.width = 0.0f;
	result.height = 0.0f;
}

Sprite* GUIPanelFactory::MakeSpriteImpl()
{
	char mbuf[256];
	snprintf(mbuf, 255, "GUIPanel_%d", m_panelCount);
	m_panelCount++;
	GUIPanel* ret = ENEW GUIPanel(m_renderer, mbuf);
	ret->Init(m_configName);
	ret->RegisterPublicEventCallback(&SpriteFrameStartEvent::s_RTTI, ENEW SpriteFrameStartEventProc(ret, m_renderer));
	return ret;
}
