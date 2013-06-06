#include "gui_pch.h"
#include "gui_edit.h"
#include "xml_resource.h"
#include "render_system.h"
#include "sprite_event_hub.h"
#include "sprite_renderer.h"
#include "sfloat3.h"
#include "gui_proc_group.h"

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
			///xhn::RWLock::Instance inst = m_sizeHandle.GetWriteLock();
			///Float2Attr* size = m_sizeHandle.GetAttribute<Float2Attr>();
			Float2Attr size;
			size.x = 100.0f;
			size.y = 50.0f;
			m_sizeHandle.SetAttribute(&size);
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

ProcGroup GUIEdit::NewProcGroup()
{
	ProcGroup pg;
	pg.mouseMoveProc = ENEW EmptyMouseMoveProc(this);
	pg.mouseButtonDownProc = ENEW EmptyMouseButtonDownProc(this);
	pg.mouseButtonUpProc = ENEW EmptyMouseButtonUpProc(this);
	return pg;
}

Sprite* GUIEditFactory::MakeSpriteImpl()
{
	char mbuf[256];
	snprintf(mbuf, 255, "GUIEdit_%d", m_editCount);
	m_editCount++;
	GUIEdit* ret = ENEW GUIEdit(m_renderer, mbuf);
	ret->Init(m_configName);
	return ret;
}