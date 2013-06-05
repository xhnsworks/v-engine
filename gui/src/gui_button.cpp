#include "gui_pch.h"
#include "gui_button.h"
#include "xml_resource.h"
#include "render_system.h"
#include "sprite_event_hub.h"
#include "sprite_renderer.h"
#include "sfloat3.h"
ImplementRTTI(GUIButton, GUIPanel);
void GUIButton::Init(const xhn::static_string configName)
{	
	Float2Attr size(100.0f, 50.0f);
	m_sizeHandle.SetAttribute(&size);
	InitImpl<GUIPanelLayer>(configName, m_sizeHandle);
	SpriteTextLayer* layer = GetTextLayer();
	if (layer) {
		XMLResourcePtr cfg = RenderSystem_load_gui_config(configName);
		if (TestResourcePtr(cfg)) {
			pugi::xml_document& doc = cfg->GetDocument();
			pugi::xml_node root = doc.child("root");
			pugi::xml_node layers = root.child("layers");
			pugi::xml_node textlayer = layers.child("text");
			if (textlayer) {
				layer->LoadConfigImpl(textlayer);
			}
		}
	}
}

void GUIButton::Build()
{
	m_elements.clear();
	BuildTextLayer(m_elements);
	BuildBackgroundLayer(m_elements);

	SpriteRect rect;
	GetScope(rect);
	rect.GetFourBorders(m_renderer, m_fourBorders);

	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);

	m_fourBorders.ApplyTranform(&mat);
}

void GUIButton::BuildElementsImpl(xhn::list<SpriteElement>& to)
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

void GUIButton::TickImpl(double elapsedTime)
{
	if (GetState() == Pressed) {
		m_releaseTimer -= elapsedTime;
		if (m_releaseTimer < 0.0) {
			m_releaseTimer = 0.0;
			SetState(Normal);
		}
	}
}

void GUIButton::OnMouseMove(const SpriteMouseMoveEvent* mouseEvt)
{
	if (GetState() == GUITouchable::Pressed)
		return;

	const FourBorders& borders = GetFourBorders();
	EFloat2 realCrd =
    m_renderer->get_real_position((float)mouseEvt->m_curtMouseCoord.x,
                                  (float)mouseEvt->m_curtMouseCoord.y);
	EFloat3 realPt(realCrd.x, realCrd.y, 0.0f);
	sfloat3 pt = SFloat3_assign_from_efloat3(&realPt);

	if (borders.IsInBorders(pt)) {
		SetState(GUITouchable::Selected);
	}
	else {
		SetState(GUITouchable::Normal);
	}
}

void GUIButton::OnMouseButtonDown(const SpriteMouseButtonDownEvent* mouseEvt)
{
	if (mouseEvt->m_leftButtomDown && GetState() ==
        GUITouchable::Selected) {
		SetState(GUITouchable::Pressed);
		m_releaseTimer = m_releaseDelay;
	}
}

Sprite* GUIButtonFactory::MakeSpriteImpl()
{
	char mbuf[256];
	snprintf(mbuf, 255, "GUIButton_%d", m_buttonCount);
	m_buttonCount++;
    GUIButton* ret = ENEW GUIButton(m_renderer, mbuf);
	ret->Init(m_configName);
	return ret;
}