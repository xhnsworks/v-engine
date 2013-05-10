#ifndef GUI_EDIT_H
#define GUI_EDIT_H
#include "xhn_static_string.hpp"
#include "xhn_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
#include "gui_panel.h"
class SpriteRenderer;
class GUIEdit;
class GUIEditBackgroundLayer : public GUIPanelLayer
{
	DeclareRTTI;
public:
	GUIEditBackgroundLayer(AttributeHandle pivotHandle, AttributeHandle sizeHandle);
};
class GUIEditTextLayer : public SpriteTextLayer
{
	DeclareRTTI;
public:
	GUIEditTextLayer(ComposingStick* cs);
	virtual void GetScopeImpl(SpriteRect& result);
};
class GUIEdit : public GUIPanel
{
	DeclareRTTI;
	friend class GUIEditFactory;
protected:
	~GUIEdit() {}
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
	virtual void GetScopeImpl(SpriteRect& result);
};

class GUIEditFactory : public SpriteFactory
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
#endif