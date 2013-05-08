#include "gui_pch.h"
#include "gui_button.h"
#include "xml_resource.h"
#include "render_system.h"
#include "sprite_event_hub.h"
#include "sprite_renderer.h"
#include "sfloat3.h"
ImplementRTTI(GUIButtonLayer, GUIPanelLayer);
ImplementRTTI(GUIButtonTextLayer, SpriteTextLayer);
ImplementRTTI(GUIButton, GUIPanel);

GUIButtonLayer::GUIButtonLayer(const xhn::static_string name, AttributeHandle pivotHandle, AttributeHandle sizeHandle)
: GUIPanelLayer(name, pivotHandle, sizeHandle)
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

		{
			xhn::RWLock::Instance inst = m_sizeHandle.GetWriteLock();
            EFloat2* size = (EFloat2*)m_sizeHandle.GetAttribute();
			size->x = 100.0f;
			size->y = 50.0f;
		}
		
		{
			pugi::xml_node baselayer = layers.child("normal");
			if (!baselayer)
				return;
			SpriteLayerPtr layer = ENEW GUIButtonLayer("normal", m_pivotHandle, m_sizeHandle);
			layer->LoadConfig(baselayer);
			AddChild(layer);
		}
		{
			pugi::xml_node baselayer = layers.child("selected");
			if (!baselayer)
				return;
			SpriteLayerPtr layer = ENEW GUIButtonLayer("selected", m_pivotHandle, m_sizeHandle);
			layer->LoadConfig(baselayer);
			AddChild(layer);
		}
		{
			pugi::xml_node baselayer = layers.child("pressed");
			if (!baselayer)
				return;
			SpriteLayerPtr layer = ENEW GUIButtonLayer("pressed", m_pivotHandle, m_sizeHandle);
			layer->LoadConfig(baselayer);
			AddChild(layer);
		}
		{
			pugi::xml_node textlayer = layers.child("text");
			if (!textlayer)
				return;
#if defined(_WIN32) || defined(_WIN64)
			FontRenderer* fr = ENEW FontRenderer("..\\test_scene\\Earthbound-Condensed-Bold.otf");
#else
            FontRenderer* fr = ENEW FontRenderer("/Users/joumining/v-engine/test_scene/Earthbound-Condensed-Bold.otf");
#endif
			fr->set_font_size(Pixel30);
			ComposingStick* cs = ENEW ComposingStick(fr, 256);
			SpriteLayerPtr layer = ENEW GUIButtonTextLayer(cs);
			layer->LoadConfig(textlayer);
			m_children.push_back(layer);
		}
	}
}
void GUIButton::GetScopeImpl(SpriteRect& result)
{
	result.left = 0.0f;
	result.top = 0.0f;
	result.width = 0.0f;
	result.height = 0.0f;
}

void GUIButton::BuildBackgroundLayer(const matrix4x4& transform)
{
	switch (m_curtState)
	{
	case Normal:
		{
			SpriteLayerArray::iterator iter = m_children.begin();
			for (; iter != m_children.end(); iter++)
			{
				SpriteLayerPtr layerPtr = *iter;
				if (layerPtr->GetName() == "normal") {
					layerPtr->ApplyTransform(&transform);
					layerPtr->BuildElements(m_elements);
				}
			}
		}
		break;
	case Selected:
		{
			SpriteLayerArray::iterator iter = m_children.begin();
			for (; iter != m_children.end(); iter++)
			{
				SpriteLayerPtr layerPtr = *iter;
				if (layerPtr->GetName() == "selected") {
					layerPtr->ApplyTransform(&transform);
					layerPtr->BuildElements(m_elements);
				}
			}
		}
		break;
	case Pressed:
	default:
		{
			SpriteLayerArray::iterator iter = m_children.begin();
			for (; iter != m_children.end(); iter++)
			{
				SpriteLayerPtr layerPtr = *iter;
				if (layerPtr->GetName() == "pressed") {
					layerPtr->ApplyTransform(&transform);
					layerPtr->BuildElements(m_elements);
				}
			}
		}
		break;
	}
}
void GUIButton::BuildTextLayer(const matrix4x4& transform)
{
	SpriteLayerArray::iterator iter = m_children.begin();
	for (; iter != m_children.end(); iter++)
	{
		SpriteLayerPtr layerPtr = *iter;
		if (layerPtr->GetName() == "text") {
			layerPtr->ApplyTransform(&transform);
			layerPtr->BuildElements(m_elements);
		}
	}
}

void GUIButton::Build()
{
	m_elements.clear();
	matrix4x4 transform;
	GetMatrix(&transform);
    
	BuildTextLayer(transform);
	BuildBackgroundLayer(transform);
}

void GUIButton::Tick(double elapsedTime)
{
	if (m_curtState == Pressed) {
		m_releaseTimer -= elapsedTime;
		if (m_releaseTimer < 0.0) {
			m_releaseTimer = 0.0;
			m_curtState = Normal;
		}
	}
}

void GUIButton::MouseMoveEventProc::Proc(const SpriteEvent* evt)
{
	if (m_button->m_curtState == GUIButton::Pressed)
		return;
	const SpriteMouseMoveEvent* mouseEvt = evt->DynamicCast<SpriteMouseMoveEvent>();
	FourBorders borders;
	SpriteRect rect;
	m_button->GetScope(rect);
	rect.GetFourBorders(m_button->m_renderer, borders);

	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	m_button->GetMatrix(&mat);

	borders.ApplyTranform(&mat);
	EFloat2 realCrd = m_button->m_renderer->get_real_position((float)mouseEvt->m_curtMousePos.x, (float)mouseEvt->m_curtMousePos.y);
	EFloat3 realPt(realCrd.x, realCrd.y, 0.0f);
	sfloat3 pt = SFloat3_assign_from_efloat3(&realPt);

	if (borders.IsInBorders(pt)) {
		m_button->SetState(GUIButton::Selected);
	}
	else {
		m_button->SetState(GUIButton::Normal);
	}
}

void GUIButton::MouseButtonDownEventProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseButtonDownEvent* mouseEvt = evt->DynamicCast<SpriteMouseButtonDownEvent>();
	if (mouseEvt->m_leftButtomDown && m_button->m_curtState == GUIButton::Selected) {
		m_button->SetState(GUIButton::Pressed);
		m_button->m_releaseTimer = m_button->m_releaseDelay;
	}
}

void GUIButton::MouseButtonUpEventProc::Proc(const SpriteEvent* evt)
{
	///const SpriteMouseButtonUpEvent* mouseEvt = evt->DynamicCast<SpriteMouseButtonUpEvent>();
}

Sprite* GUIButtonFactory::MakeSpriteImpl()
{
	char mbuf[256];
	snprintf(mbuf, 255, "GUIButton_%d", m_buttonCount);
	m_buttonCount++;
    GUIButton* ret = ENEW GUIButton(m_renderer, mbuf);
	ret->Init(m_configName);
	ret->RegisterPublicEventCallback(&SpriteMouseMoveEvent::s_RTTI, ENEW GUIButton::MouseMoveEventProc(ret));
	ret->RegisterPublicEventCallback(&SpriteMouseButtonDownEvent::s_RTTI, ENEW GUIButton::MouseButtonDownEventProc(ret));
	ret->RegisterPublicEventCallback(&SpriteMouseButtonUpEvent::s_RTTI, ENEW GUIButton::MouseButtonUpEventProc(ret));
	ret->RegisterPublicEventCallback(&SpriteFrameStartEvent::s_RTTI, ENEW SpriteFrameStartEventProc(ret, m_renderer));
	return ret;
}