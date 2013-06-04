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
	DeclareRTTI;
public:
	GUIVertSlider(SpriteRenderer* renderer, const xhn::static_string name)
		: GUIVertBar(renderer, name)
	{}
	virtual void Init(const xhn::static_string configName);
};

class GUIVertButton : public GUISimplePanel
{
	DeclareRTTI;
public:
	GUIVertButton(SpriteRenderer* renderer, const xhn::static_string name)
		: GUISimplePanel(renderer, name)
	{}
    virtual void Init(const xhn::static_string configName);
};

class GUIVertScrollbar : public GUIVertBar
{
	DeclareRTTI;
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