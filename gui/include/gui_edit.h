#ifndef GUI_EDIT_H
#define GUI_EDIT_H
#include "xhn_static_string.hpp"
#include "xhn_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
#include "gui_panel.h"
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class SpriteRenderer;
class GUIEdit;
class GUIEditBackgroundLayer : public GUIPanelLayer
{
public:
	GUIEditBackgroundLayer(AttributeHandle pivotHandle,
                           AttributeHandle sizeHandle);
};
class GUIEditTextLayer : public SpriteTextLayer
{
public:
	GUIEditTextLayer();
};
class GUIEdit : public GUIPanel
{
public:
	xhn::string m_text;

	class SetTexEventProc : public SpriteEventProc
	{
	public:
		GUIEdit* m_edit;
	public:
		SetTexEventProc(GUIEdit* edit) : m_edit(edit) {}
		~SetTexEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};
public:
	GUIEdit(SpriteRenderer* renderer, const xhn::static_string name)
		: GUIPanel(renderer, name)
	{}
	virtual void Init(const xhn::static_string configName);
	virtual ProcGroup NewProcGroup();
};

class GUIEditFactory : public SpriteFactory<GUIEdit>
{
public:
	int m_editCount;
public:
	GUIEditFactory(SpriteRenderer* renderer, const char* cfgName)
		: m_editCount(0)
		, SpriteFactory(renderer, cfgName)
	{}
	virtual Sprite* MakeSpriteImpl();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif