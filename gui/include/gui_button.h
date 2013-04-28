#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H
#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
#include "gui_panel.h"
class SpriteRenderer;
class GUIButton;
class GUIButtonLayer : public GUIPanelLayer
{
	DeclareRTTI;
public:
	GUIButtonLayer(const xhn::static_string name, AttributeHandle pivotHandle, AttributeHandle sizeHandle);
};
class GUIButtonTextLayer : public SpriteTextLayer
{
    DeclareRTTI;
public:
    GUIButtonTextLayer(ComposingStick* cs);
	virtual void GetScopeImpl(SpriteRect& result);
};
class GUIButton : public GUIPanel
{
	DeclareRTTI;
public:
	enum ButtonState
	{
		Normal,
		Selected,
		Pressed,
	};
public:
	double m_releaseDelay;
	double m_releaseTimer;
	ButtonState m_curtState;

	class MouseMoveEventProc : public SpriteEventProc
	{
	public:
		GUIButton* m_button;
	public:
		MouseMoveEventProc(GUIButton* button) : m_button(button) {}
		~MouseMoveEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};

	class MouseButtonDownEventProc : public SpriteEventProc
	{
	public:
		GUIButton* m_button;
	public:
		MouseButtonDownEventProc(GUIButton* button) : m_button(button) {}
		~MouseButtonDownEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};

	class MouseButtonUpEventProc : public SpriteEventProc
	{
	public:
		GUIButton* m_button;
	public:
		MouseButtonUpEventProc(GUIButton* button) : m_button(button) {}
		~MouseButtonUpEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};
public:
	GUIButton(SpriteRenderer* renderer, const xhn::static_string name)
	: m_releaseDelay(1.0)
	, m_releaseTimer(0.0f)
	, m_curtState(Normal)
	, GUIPanel(renderer, name)
	{}
	virtual void Init(const xhn::static_string configName);
	virtual void GetScopeImpl(SpriteRect& result);
	virtual void Build();
	virtual void Tick(double elapsedTime);
	inline void SetState(ButtonState state) {
		m_curtState = state;
	}
};

class GUIButtonFactory : public SpriteFactory
{
public:
	int m_buttonCount;
public:
	GUIButtonFactory(SpriteRenderer* renderer, const char* cfgName)
		: m_buttonCount(0)
		, SpriteFactory(renderer, cfgName)
	{}
    virtual Sprite* MakeSpriteImpl();
};
#endif