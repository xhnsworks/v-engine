#include "gui_pch.h"
#include "gui_button.h"
#include "xml_resource.h"
#include "render_system.h"
#include "sprite_event_hub.h"
#include "sprite_renderer.h"
#include "sfloat3.h"
ImplementRTTI(GUIButtonLayer, SpriteNormalLayer);
ImplementRTTI(GUIButtonTextLayer, SpriteTextLayer);
ImplementRTTI(GUIButton, Sprite);

GUIButtonLayer::GUIButtonLayer()
: SpriteNormalLayer("base")
{
}

GUIButtonTextLayer::GUIButtonTextLayer(ComposingStick* cs)
: SpriteTextLayer("text", cs)
{
}
void GUIButtonTextLayer::GetScopeImpl(SpriteRect& result)
{
	result.left = 0.0f;
	result.top = 0.0f;
	result.width = 0.0f;
	result.height = 0.0f;
}
void GUIButton::Init(const xhn::static_string configName)
{
	XMLResourcePtr cfg = RenderSystem_load_gui_config(configName);
	if (TestResourcePtr(cfg)) {
		pugi::xml_document& doc = cfg->GetDocument();
		pugi::xml_node root = doc.child("root");
		pugi::xml_node layers = root.child("layers");
		pugi::xml_node baselayer = layers.child("base");
		if (!baselayer)
			return;
		SpriteLayerPtr layer = ENEW GUIButtonLayer;
		layer->LoadConfig(baselayer);
		m_children.push_back(layer);

		pugi::xml_node textlayer = layers.child("text");
		if (!textlayer)
			return;
		FontRenderer* fr = ENEW FontRenderer("..\\test_scene\\Earthbound-Condensed-Bold.otf");
		fr->set_font_size(Pixel30);
		ComposingStick* cs = ENEW ComposingStick(fr, 256);
		layer = ENEW GUIButtonTextLayer(cs);
		layer->LoadConfig(textlayer);
		m_children.push_back(layer);
	}
}
void GUIButton::GetScopeImpl(SpriteRect& result)
{
	result.left = 0.0f;
	result.top = 0.0f;
	result.width = 0.0f;
	result.height = 0.0f;
}

void GUIButtonMouseEventProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseMoveEvent* mouseEvt = evt->DynamicCast<SpriteMouseMoveEvent>();
	SpriteLayerPtr layer = m_button->GetLayer(0);
	if (layer.get()) {
		///elog("mouse pos %d %d\n", mouseEvt->m_curtMousePos.x, mouseEvt->m_curtMousePos.y);
		SpriteNormalLayer* norLayer = layer->DynamicCast<SpriteNormalLayer>();
		SpriteElement* unpressed = norLayer->GetElement("unpressed");
		SpriteElement* pressed = norLayer->GetElement("pressed");
		FourBorders borders;
		SpriteRect rect;
		m_button->GetScope(rect);
		rect.GetFourBorders(m_button->m_renderer, borders);
		///unpressed->GetFourBorders(m_button->m_renderer, borders);
		borders.ApplyTranform(&unpressed->m_transform);
		EFloat2 realCrd = m_button->m_renderer->get_real_position((float)mouseEvt->m_curtMousePos.x, (float)mouseEvt->m_curtMousePos.y);
		EFloat3 realPt(realCrd.x, realCrd.y, 0.0f);
		sfloat3 pt = SFloat3_assign_from_efloat3(&realPt);
		///pt = Matrix4x4_mul_float3(&unpressed->m_transform, pt);
		if (borders.IsInBorders(pt)) {
			unpressed->SetTransparent(0.0f);
			pressed->SetTransparent(1.0f);
		}
		else {
			unpressed->SetTransparent(1.0f);
			pressed->SetTransparent(0.0f);
		}
	}
}

Sprite* GUIButtonFactory::MakeSpriteImpl()
{
	char mbuf[256];
	snprintf(mbuf, 255, "GUIButton_%d", m_buttonCount);
	m_buttonCount++;
    GUIButton* ret = ENEW GUIButton(m_renderer, mbuf);
	ret->Init(m_configName);
	ret->RegisterEventCallback(&SpriteMouseMoveEvent::s_RTTI, ENEW GUIButtonMouseEventProc(ret));
	ret->RegisterEventCallback(&SpriteFrameStartEvent::s_RTTI, ENEW SpriteFrameStartEventProc(ret, m_renderer));
	return ret;
}