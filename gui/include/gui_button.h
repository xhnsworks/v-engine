#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H
#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
class SpriteRenderer;
class GUIButtonLayer : public SpriteNormalLayer
{
	DeclareRTTI;
public:
	GUIButtonLayer();
};
class GUIButtonTextLayer : public SpriteTextLayer
{
    DeclareRTTI;
public:
    GUIButtonTextLayer(ComposingStick* cs);
	virtual void GetScopeImpl(SpriteRect& result);
};
class GUIButton : public Sprite
{
	DeclareRTTI;
public:
	GUIButton(SpriteRenderer* renderer, const xhn::static_string name)
		: Sprite(renderer, name)
	{}
	virtual void Init(const xhn::static_string configName);
	virtual void GetScopeImpl(SpriteRect& result);
};

class GUIButtonMouseEventProc : public SpriteEventProc
{
public:
	GUIButton* m_button;
public:
	GUIButtonMouseEventProc(GUIButton* button)
		: m_button(button)
	{}
	~GUIButtonMouseEventProc() {}
public:
	virtual void Proc(const SpriteEvent* evt);
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