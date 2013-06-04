#include "gui_pch.h"
#include "gui_touchable.h"

ImplementRTTI(GUITouchable, Sprite);
void GUITouchable::MouseMoveEventProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseMoveEvent* mouseEvt = evt->DynamicCast<SpriteMouseMoveEvent>();
	if (mouseEvt) {
		m_touchable->OnMouseMove(mouseEvt);
	}
}
void GUITouchable::MouseButtonDownEventProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseButtonDownEvent* mouseEvt = evt->DynamicCast<SpriteMouseButtonDownEvent>();
	if (mouseEvt) {
		m_touchable->OnMouseButtonDown(mouseEvt);
	}
}
void GUITouchable::MouseButtonUpEventProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseButtonUpEvent* mouseEvt = evt->DynamicCast<SpriteMouseButtonUpEvent>();
	if (mouseEvt) {
		m_touchable->OnMouseButtonUp(mouseEvt);
	}
}
GUITouchable::GUITouchable(SpriteRenderer* renderer, const xhn::static_string name)
: m_curtState(Normal)
, Sprite(renderer, name)
{
    RegisterPublicEventCallback(&SpriteMouseMoveEvent::s_RTTI,
		ENEW MouseMoveEventProc(this));
	RegisterPublicEventCallback(&SpriteMouseButtonDownEvent::s_RTTI,
		ENEW MouseButtonDownEventProc(this));
	RegisterPublicEventCallback(&SpriteMouseButtonUpEvent::s_RTTI,
		ENEW MouseButtonUpEventProc(this));
	RegisterPublicEventCallback(&SpriteFrameStartEvent::s_RTTI,
		ENEW SpriteFrameStartEventProc(this, renderer));
}

GUITouchable::~GUITouchable()
{
}

void GUITouchable::Dest()
{
}