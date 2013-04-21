#include "gui_pch.h"
#include "gui_panel.h"
#include "xml_resource.h"
#include "render_system.h"
#include "sprite_event_hub.h"
#include "sprite_renderer.h"
#include "sfloat3.h"
ImplementRTTI(GUIPanelLayer, SpriteNormalLayer);
ImplementRTTI(GUIPanel, Sprite);

GUIPanelLayer::GUIPanelLayer()
: SpriteNormalLayer("base")
{
}
void GUIPanelLayer::ApplyTransform(const matrix4x4* trans)
{
	xhn::map<xhn::static_string, SpriteElement>::iterator iter = m_elementBuffer.begin();
	for (; iter != m_elementBuffer.end(); iter++)
	{
		SpriteElement& ele = iter->second;
		ele.ApplyTransform(trans);
	}
	/**
	xhn::map<xhn::static_string, SpriteElement>::iterator iterLowerLeft = m_elementBuffer.find("lower_left_corner");
	xhn::map<xhn::static_string, SpriteElement>::iterator iterUpperLeft = m_elementBuffer.find("upper_left_corner");
	xhn::map<xhn::static_string, SpriteElement>::iterator iterUpperRight = m_elementBuffer.find("upper_right_corner");
	xhn::map<xhn::static_string, SpriteElement>::iterator iterLowerRight = m_elementBuffer.find("lower_right_corner");
	SpriteElement tmpUpperLeft = iterUpperLeft->second;
	SpriteElement tmpLowerRight = iterLowerRight->second;
	
    tmpUpperLeft.ApplyTransform(trans);
	tmpLowerRight.ApplyTransform(trans);
	float left = tmpUpperLeft.m_left;
	float top = tmpUpperLeft.m_top;
	float right = tmpLowerRight.m_left + tmpLowerRight.m_width;
	float bottom = tmpLowerRight.m_top + tmpLowerRight.m_height;

	SpriteElement& lowerLeft = iterLowerLeft->second;
	SpriteElement& upperLeft = iterUpperLeft->second;
	SpriteElement& upperRight = iterUpperRight->second;
	SpriteElement& lowerRight = iterLowerRight->second;
	**/
/**
	lowerLeft.m_left = left;
	lowerLeft.m_top = bottom - lowerLeft.m_height;

	upperLeft.m_left = left;
	upperLeft.m_top = top;

	upperRight.m_left = right - upperRight.m_width;
	upperRight.m_top = top;

	lowerRight.m_left = right - lowerRight.m_width;
	lowerRight.m_top = bottom - lowerRight.m_height;
	**/
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
		SpriteLayerPtr layer = ENEW GUIPanelLayer;
		layer->LoadConfig(baselayer);
		m_children.push_back(layer);
	}
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
