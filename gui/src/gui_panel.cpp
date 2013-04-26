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
		pivot = *((EFloat2*)m_pivotHandle.m_attr);
	}
	{
		xhn::RWLock::Instance inst = m_sizeHandle.GetReadLock();
		size = *((EFloat2*)m_sizeHandle.m_attr);
	}

	float left = tmpUpperLeft.m_left - pivot.x;
	float top = tmpUpperLeft.m_top - pivot.y;
	float right = tmpLowerRight.m_left + tmpLowerRight.m_width - pivot.x;
	float bottom = tmpLowerRight.m_top + tmpLowerRight.m_height - pivot.y;

	float realWidth = right - left;
	float realHeight = bottom - top;

	float scaleX = size.x / realWidth;
	float scaleY = size.y / realHeight;

	left *= scaleX;
	top *= scaleY;
	right *= scaleX;
	bottom *= scaleY;

	tmpLowerLeft.m_left = left;
	tmpLowerLeft.m_top = bottom - tmpLowerLeft.m_height;

    tmpUpperLeft.m_left = left;
	tmpUpperLeft.m_top = top;

    tmpUpperRight.m_left = right - tmpUpperRight.m_width;
	tmpUpperRight.m_top = top;

    tmpLowerRight.m_left = right - tmpLowerRight.m_width;
	tmpLowerRight.m_top = bottom - tmpLowerRight.m_height;

	tmpLeft.m_left = left;
	tmpLeft.m_top = top + tmpUpperLeft.m_height;
	tmpLeft.m_height = tmpLowerLeft.m_top - tmpUpperLeft.m_top - tmpUpperLeft.m_height;

	tmpTop.m_left = left + tmpUpperLeft.m_width;
	tmpTop.m_top = top;
	tmpTop.m_width = tmpUpperRight.m_left - tmpUpperLeft.m_left - tmpUpperLeft.m_width;

	tmpRight.m_left = right - tmpRight.m_width;
	tmpRight.m_top = tmpUpperRight.m_top + tmpUpperRight.m_height;
	tmpRight.m_height = tmpLowerRight.m_top - tmpUpperRight.m_top - tmpUpperRight.m_height;

	tmpBottom.m_left = left + tmpLowerLeft.m_width;
	tmpBottom.m_top = tmpLowerLeft.m_top;
	tmpBottom.m_width = tmpLowerRight.m_top - tmpLowerLeft.m_left - tmpLowerLeft.m_width;

    tmpCenter.m_left = left + tmpLeft.m_width;
	tmpCenter.m_top = top + tmpTop.m_height;
	tmpCenter.m_width = tmpTop.m_width;
	tmpCenter.m_height = tmpRight.m_height;

	to.push_back(tmpCenter);

	to.push_back(tmpLeft);
	to.push_back(tmpTop);
	to.push_back(tmpRight);
	to.push_back(tmpBottom);

	to.push_back(tmpLowerLeft);
	to.push_back(tmpLowerRight);
	to.push_back(tmpUpperLeft);
	to.push_back(tmpUpperRight);
}

GUIPanel::GUIPanel(SpriteRenderer* renderer, const xhn::static_string name)
: Sprite(renderer, name)
{
	m_sizeHandle.m_attr = &m_size;
	m_sizeHandle.m_lock = ENEW xhn::RWLock;
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
		SpriteLayerPtr layer = ENEW GUIPanelLayer(m_pivotHandle, m_sizeHandle);
		layer->LoadConfig(baselayer);
		m_children.push_back(layer);
	}
}

void GUIPanel::SetSize(float x, float y)
{
    xhn::RWLock::Instance inst = m_sizeHandle.GetWriteLock();
	EFloat2* size = (EFloat2*)m_sizeHandle.m_attr;
	size->x = x;
	size->y = y;
}

Sprite* GUIPanelFactory::MakeSpriteImpl()
{
	char mbuf[256];
	snprintf(mbuf, 255, "GUIPanel_%d", m_panelCount);
	m_panelCount++;
	GUIPanel* ret = ENEW GUIPanel(m_renderer, mbuf);
	ret->Init(m_configName);
	ret->RegisterEventCallback(&SpriteFrameStartEvent::s_RTTI, ENEW SpriteFrameStartEventProc(ret, m_renderer));
	return ret;
}
