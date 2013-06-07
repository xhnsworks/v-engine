#ifndef GUI_PROC_GROUP_H
#define GUI_PROC_GROUP_H
#include "gui_widget.h"
class EmptyMouseMoveProc : public GUIWidget::MouseMoveEventProc
{
public:
	EmptyMouseMoveProc(GUIWidget* widget)
		: GUIWidget::MouseMoveEventProc(widget)
	{}
	virtual void Proc(const SpriteEvent* evt) {}
};
class EmptyMouseButtonDownProc : public GUIWidget::MouseButtonDownEventProc
{
public:
	EmptyMouseButtonDownProc(GUIWidget* widget)
		: GUIWidget::MouseButtonDownEventProc(widget)
	{}
	virtual void Proc(const SpriteEvent* evt) {}
};
class EmptyMouseButtonUpProc : public GUIWidget::MouseButtonUpEventProc
{
public:
	EmptyMouseButtonUpProc(GUIWidget* widget)
		: GUIWidget::MouseButtonUpEventProc(widget)
	{}
	virtual void Proc(const SpriteEvent* evt) {}
};

class TouchableMouseMoveProc : public GUIWidget::MouseMoveEventProc
{
public:
	TouchableMouseMoveProc(GUIWidget* widget)
		: GUIWidget::MouseMoveEventProc(widget)
	{}
	virtual void Proc(const SpriteEvent* evt);
};

class PressableMouseMoveProc : public GUIWidget::MouseMoveEventProc
{
public:
	PressableMouseMoveProc(GUIWidget* widget)
		: GUIWidget::MouseMoveEventProc(widget)
	{}
	virtual void Proc(const SpriteEvent* evt);
};

class LeaveableMouseMoveProc : public GUIWidget::MouseMoveEventProc
{
public:
	int m_leaveCount;
	LeaveableMouseMoveProc(GUIWidget* widget)
		: m_leaveCount(0)
	    , GUIWidget::MouseMoveEventProc(widget)
	{}
	virtual void Proc(const SpriteEvent* evt);
};

class PressableMouseButtonDownProc : public GUIWidget::MouseButtonDownEventProc
{
public:
	PressableMouseButtonDownProc(GUIWidget* widget)
		: GUIWidget::MouseButtonDownEventProc(widget)
	{}
	virtual void Proc(const SpriteEvent* evt);
};

class DraggableMouseMoveProc : public TouchableMouseMoveProc
{
public:
	SInt2 m_prevMouseCoord;
public:
	DraggableMouseMoveProc(GUIWidget* widget)
		: TouchableMouseMoveProc(widget)
	{}
	virtual void Proc(const SpriteEvent* evt);
};

class DraggableMouseButtonDownProc : public GUIWidget::MouseButtonDownEventProc
{
public:
	DraggableMouseMoveProc* m_mouseMoveProc;
public:
	DraggableMouseButtonDownProc(GUIWidget* widget, DraggableMouseMoveProc* mouseMoveProc)
		: m_mouseMoveProc(mouseMoveProc)
	    , GUIWidget::MouseButtonDownEventProc(widget)
	{}
	virtual void Proc(const SpriteEvent* evt);
};

#endif