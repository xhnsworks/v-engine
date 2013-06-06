#ifndef GUI_SCROLLBAR_H
#define GUI_SCROLLBAR_H
///**********************************************************************///
///                           include begin                              ///
///**********************************************************************///
#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
#include "gui_bar.h"
#include "gui_simple_panel.h"
///**********************************************************************///
///                           include end                                ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class GUIVertSlider : public GUIVertBar
{
public:
	GUIVertSlider(SpriteRenderer* renderer, const xhn::static_string name)
		: GUIVertBar(renderer, name)
	{}
	virtual void Init(const xhn::static_string configName);
	virtual void OnMouseMove(const SpriteMouseMoveEvent* mouseEvt);
	virtual void OnMouseButtonDown(const SpriteMouseButtonDownEvent* mouseEvt);
	virtual void OnMouseButtonUp(const SpriteMouseButtonUpEvent* mouseEvt);
	virtual void BuildElementsImpl(xhn::list<SpriteElement>& to);
};

class GUIVertSliderFactory : public GUIVertBarFactory
{
public:
	AttributeHandle m_sizeHandle;
public:
	GUIVertSliderFactory(SpriteRenderer* renderer,
		                 const char* cfgName,
		                 AttributeHandle sizeHandle)
		: m_sizeHandle(sizeHandle)
		, GUIVertBarFactory(renderer, cfgName)
	{}
	virtual Sprite* MakeSpriteImpl();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class GUIVertButton : public GUISimplePanel
{
public:
	GUIVertButton(SpriteRenderer* renderer, const xhn::static_string name)
		: GUISimplePanel(renderer, name)
	{}
    virtual void Init(const xhn::static_string configName);
};

class GUIVertScrollbar : public GUIVertBar
{
public:
	GUIVertScrollbar(SpriteRenderer* renderer, const xhn::static_string name)
		: GUIVertBar(renderer, name)
	{}
	virtual void Init(const xhn::static_string configName);
};

///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif