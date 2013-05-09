#include "gui_pch.h"
#include "gui_cursor.h"
#include "xml_resource.h"
#include "render_system.h"
#include "sprite_event_hub.h"
ImplementRTTI(GUICursorLayer, SpriteNormalLayer);
ImplementRTTI(GUICursor, Sprite);

GUICursorLayer::GUICursorLayer()
: SpriteNormalLayer("base")
{
}

void GUICursor::Init(const xhn::static_string configName)
{
	XMLResourcePtr cfg = RenderSystem_load_gui_config(configName);
	if (TestResourcePtr(cfg)) {
		pugi::xml_document& doc = cfg->GetDocument();
		pugi::xml_node root = doc.child("root");
		pugi::xml_node layers = root.child("layers");
		pugi::xml_node baselayer = layers.child("base");
		if (!baselayer)
			return;
		SpriteLayerPtr layer = ENEW GUICursorLayer;
		layer->LoadConfig(baselayer);
		m_children.push_back(layer);
	}
}

void GUICursor::GetScopeImpl(SpriteRect& result)
{
	result.left = 0.0f;
	result.top = 0.0f;
	result.size.width = 0.0f;
	result.size.height = 0.0f;
}

void GUICursorMouseEventProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseMoveEvent* mouseEvt = evt->DynamicCast<SpriteMouseMoveEvent>();
	m_cursor->SetCoord((float)mouseEvt->m_curtMousePos.x, (float)mouseEvt->m_curtMousePos.y);
	///Ray_log((const Ray)&mouseEvt->m_mouseRay);
}

Sprite* GUICursorFactory::MakeSpriteImpl()
{
	char mbuf[256];
	snprintf(mbuf, 255, "GUICursor_%d", m_cursorCount);
	m_cursorCount++;
	GUICursor* ret = ENEW GUICursor(m_renderer, mbuf);
	ret->Init(m_configName);
	ret->RegisterPublicEventCallback(&SpriteMouseMoveEvent::s_RTTI, ENEW GUICursorMouseEventProc(ret));
	ret->RegisterPublicEventCallback(&SpriteFrameStartEvent::s_RTTI, ENEW SpriteFrameStartEventProc(ret, m_renderer));
	return ret;
}