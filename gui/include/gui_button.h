#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H
#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
#include "gui_panel.h"
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class SpriteRenderer;
class GUIButton;

class GUIButton : public GUIPanel
{
	DeclareRTTI;
public:
	double m_releaseDelay;
	double m_releaseTimer;
public:
	GUIButton(SpriteRenderer* renderer, const xhn::static_string name)
	: m_releaseDelay(0.25)
	, m_releaseTimer(0.0f)
	, GUIPanel(renderer, name)
	{}
	virtual void Init(const xhn::static_string configName);
	virtual void Build();
    virtual void BuildElementsImpl(xhn::list<SpriteElement>& to);
	virtual void TickImpl(double elapsedTime);
    virtual void TockImpl() {}
	virtual void OnMouseMove(const SpriteMouseMoveEvent* mouseEvt);
	virtual void OnMouseButtonDown(const SpriteMouseButtonDownEvent* mouseEvt);
	virtual void OnMouseButtonUp(const SpriteMouseButtonUpEvent* mouseEvt) {}
};

class GUIButtonFactory : public SpriteFactory
{
public:
	int m_buttonCount;
public:
	GUIButtonFactory(SpriteRenderer* renderer,
                     const char* cfgName)
		: m_buttonCount(0)
		, SpriteFactory(renderer, cfgName)
	{}
    virtual Sprite* MakeSpriteImpl();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif