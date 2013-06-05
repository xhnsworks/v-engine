#include "gui_pch.h"
#include "gui_scrollbar.h"
#include "render_system.h"
ImplementRTTI(GUIVertSlider, GUIVertBar);
ImplementRTTI(GUIVertButton, GUISimplePanel);
ImplementRTTI(GUIVertScrollbar, GUIVertBar);

void GUIVertSlider::Init(const xhn::static_string configName)
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
		SetState(GUITouchable::Normal);
	}
}
void GUIVertSlider::OnMouseMove(const SpriteMouseMoveEvent* mouseEvt)
{
}
void GUIVertSlider::OnMouseButtonDown(const SpriteMouseButtonDownEvent* mouseEvt)
{
}
void GUIVertSlider::OnMouseButtonUp(const SpriteMouseButtonUpEvent* mouseEvt)
{
}
void GUIVertSlider::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
	BuildTextLayer(to);
	BuildBackgroundLayer(to);

	SpriteRect rect;
	GetScope(rect);
	rect.GetFourBorders(m_renderer, m_fourBorders);

	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);

	m_fourBorders.ApplyTranform(&mat);
}