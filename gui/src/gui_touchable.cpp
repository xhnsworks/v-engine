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

void GUITouchable::BuildBackgroundLayer(xhn::list<SpriteElement>& to)
{
	switch (GetState())
	{
	case Touched:
		{
			SpriteLayerPtr layerPtr = GetLayer("touched");
			if (layerPtr.get())
				layerPtr->BuildElementsImpl(to);
		}
		break;
	case Selected:
		{
			SpriteLayerPtr layerPtr = GetLayer("selected");
			if (layerPtr.get())
				layerPtr->BuildElementsImpl(to);
		}
		break;
	case Pressed:
		{
			SpriteLayerPtr layerPtr = GetLayer("pressed");
			if (layerPtr.get())
				layerPtr->BuildElementsImpl(to);
		}
		break;
	case Normal:
	default:
		{
			SpriteLayerPtr layerPtr = GetLayer("normal");
			if (layerPtr.get())
				layerPtr->BuildElementsImpl(to);
		}
		break;
	}
}

void GUITouchable::BuildTextLayer(xhn::list<SpriteElement>& to)
{
	SpriteLayerPtr layerPtr = GetLayer("text");
	if (layerPtr.get())
		layerPtr->BuildElementsImpl(to);
}

SpriteTextLayer* GUITouchable::GetTextLayer()
{
    SpriteLayerPtr layerPtr = GetLayer("text");
	if (layerPtr.get())
		return layerPtr->DynamicCast<SpriteTextLayer>();
	else
		return NULL;
}