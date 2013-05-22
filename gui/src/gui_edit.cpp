#include "gui_pch.h"
#include "gui_edit.h"
#include "xml_resource.h"
#include "render_system.h"
#include "sprite_event_hub.h"
#include "sprite_renderer.h"
#include "sfloat3.h"
ImplementRTTI(GUIEditBackgroundLayer, GUIPanelLayer);
ImplementRTTI(GUIEditTextLayer, SpriteTextLayer);
ImplementRTTI(GUIEdit, GUIPanel);

GUIEditBackgroundLayer::GUIEditBackgroundLayer(AttributeHandle pivotHandle, AttributeHandle sizeHandle)
: GUIPanelLayer("background", pivotHandle, sizeHandle)
{
}

GUIEditTextLayer::GUIEditTextLayer()
: SpriteTextLayer("text")
{
}

void GUIEdit::Init(const xhn::static_string configName)
{
	XMLResourcePtr cfg = RenderSystem_load_gui_config(configName);
	if (TestResourcePtr(cfg)) {
		pugi::xml_document& doc = cfg->GetDocument();
		pugi::xml_node root = doc.child("root");
		pugi::xml_node layers = root.child("layers");

		{
			xhn::RWLock::Instance inst = m_sizeHandle.GetWriteLock();
			EFloat2* size = (EFloat2*)m_sizeHandle.GetAttribute();
			size->x = 100.0f;
			size->y = 50.0f;
		}

		{
			pugi::xml_node textlayer = layers.child("text");
			if (textlayer) {
                SpriteLayerPtr layer = ENEW GUIEditTextLayer();
                layer->LoadConfigImpl(textlayer);
                AddChild(layer);
            }
		}

		{
			pugi::xml_node baselayer = layers.child("background");
			if (baselayer) {
                SpriteLayerPtr layer = ENEW GUIEditBackgroundLayer(m_pivotHandle, m_sizeHandle);
                layer->LoadConfigImpl(baselayer);
                AddChild(layer);
            }
		}
	}
}
void GUIEdit::GetScopeImpl(SpriteRect& result)
{
	result.left = 0.0f;
	result.top = 0.0f;
	result.size.width = 0.0f;
	result.size.height = 0.0f;
}

Sprite* GUIEditFactory::MakeSpriteImpl()
{
	char mbuf[256];
	snprintf(mbuf, 255, "GUIEdit_%d", m_editCount);
	m_editCount++;
	GUIEdit* ret = ENEW GUIEdit(m_renderer, mbuf);
	ret->Init(m_configName);
	ret->RegisterPublicEventCallback(&SpriteFrameStartEvent::s_RTTI, ENEW SpriteFrameStartEventProc(ret, m_renderer));
	return ret;
}