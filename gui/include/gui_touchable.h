#ifndef GUI_TOUCHABLE_H
#define GUI_TOUCHABLE_H
///**********************************************************************///
///                           include begin                              ///
///**********************************************************************///
#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_event_hub.h"
///**********************************************************************///
///                           include end                                ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class GUITouchable : public Sprite
{
	DeclareRTTI;
public:
	enum State
	{
		Normal,
		Touched,
		Selected,
		Pressed,
	};
	class MouseMoveEventProc : public SpriteEventProc
	{
	public:
		GUITouchable* m_touchable;
	public:
		MouseMoveEventProc(GUITouchable* touchable) : m_touchable(touchable) {}
		~MouseMoveEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};

	class MouseButtonDownEventProc : public SpriteEventProc
	{
	public:
		GUITouchable* m_touchable;
	public:
		MouseButtonDownEventProc(GUITouchable* touchable) : m_touchable(touchable) {}
		~MouseButtonDownEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};

	class MouseButtonUpEventProc : public SpriteEventProc
	{
	public:
		GUITouchable* m_touchable;
	public:
		MouseButtonUpEventProc(GUITouchable* touchable) : m_touchable(touchable) {}
		~MouseButtonUpEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};
private:
    State m_curtState;
public:
    GUITouchable(SpriteRenderer* renderer, const xhn::static_string name);
	virtual ~GUITouchable();
	void Dest();
	inline State GetState() {
		return m_curtState;
	}
	inline void SetState(State s) {
        m_curtState = s;
	}
	virtual void OnMouseMove(const SpriteMouseMoveEvent* mouseEvt) = 0;
	virtual void OnMouseButtonDown(const SpriteMouseButtonDownEvent* mouseEvt) = 0;
	virtual void OnMouseButtonUp(const SpriteMouseButtonUpEvent* mouseEvt) = 0;
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif