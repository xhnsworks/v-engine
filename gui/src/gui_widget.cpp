#include "gui_pch.h"
#include "gui_widget.h"

void GUIWidget::MouseMoveEventProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseMoveEvent* mouseEvt = evt->DynamicCast<SpriteMouseMoveEvent>();
	if (mouseEvt) {
		m_widget->OnMouseMove(mouseEvt);
	}
}
void GUIWidget::MouseButtonDownEventProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseButtonDownEvent* mouseEvt = evt->DynamicCast<SpriteMouseButtonDownEvent>();
	if (mouseEvt) {
		m_widget->OnMouseButtonDown(mouseEvt);
	}
}
void GUIWidget::MouseButtonUpEventProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseButtonUpEvent* mouseEvt = evt->DynamicCast<SpriteMouseButtonUpEvent>();
	if (mouseEvt) {
		m_widget->OnMouseButtonUp(mouseEvt);
	}
}
GUIWidget::GUIWidget(SpriteRenderer* renderer, const xhn::static_string name)
: m_curtState(Normal)
, Sprite(renderer, name)
{
}

GUIWidget::~GUIWidget()
{
}

void GUIWidget::Init()
{
	ProcGroup procGroup = NewProcGroup();
	if (procGroup.mouseMoveProc.get()) {
		RegisterPublicEventCallback(&SpriteMouseMoveEvent::s_RTTI,
			procGroup.mouseMoveProc);
	}
	if (procGroup.mouseButtonDownProc.get()) {
		RegisterPublicEventCallback(&SpriteMouseButtonDownEvent::s_RTTI,
			procGroup.mouseButtonDownProc);
	}
	if (procGroup.mouseButtonUpProc.get()) {
		RegisterPublicEventCallback(&SpriteMouseButtonUpEvent::s_RTTI,
			procGroup.mouseButtonUpProc);
	}
	RegisterPublicEventCallback(&SpriteFrameStartEvent::s_RTTI,
		ENEW SpriteFrameStartEventProc(this, m_renderer));
}

void GUIWidget::BuildBackgroundLayer(xhn::list<SpriteElement>& to)
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

void GUIWidget::BuildTextLayer(xhn::list<SpriteElement>& to)
{
	SpriteLayerPtr layerPtr = GetLayer("text");
	if (layerPtr.get())
		layerPtr->BuildElementsImpl(to);
}

SpriteTextLayer* GUIWidget::GetTextLayer()
{
	SpriteLayerPtr layerPtr = GetLayer("text");
	if (layerPtr.get() && layerPtr->IsTextLayer())
		return static_cast<SpriteTextLayer*>(layerPtr.get());
	else
		return NULL;
}