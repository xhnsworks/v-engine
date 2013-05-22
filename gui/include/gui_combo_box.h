#ifndef GUI_COMBO_BOX_H
#define GUI_COMBO_BOX_H
///**********************************************************************///
///                           include begin                              ///
///**********************************************************************///
#include "xhn_static_string.hpp"
#include "sprite.h"
#include "sprite_factory.h"
#include "gui_bar.h"
#include "gui_container.h"
///**********************************************************************///
///                           include end                                ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class SpriteRenderer;
class GUIComboBox;
class GUIComboBoxLayer : public GUIHoriBarLayer
{
	DeclareRTTI;
public:
	GUIComboBoxLayer(const xhn::static_string name,
		AttributeHandle pivotHandle,
		AttributeHandle sizeHandle)
		: GUIHoriBarLayer(name, pivotHandle, sizeHandle)
	{}
};
class GUIComboBoxTextLayer : public SpriteTextLayer
{
	DeclareRTTI;
public:
	GUIComboBoxTextLayer();
};
class GUIComboBoxEntry : public GUIContainer
{
	DeclareRTTI;
public:
	enum EntryState
	{
		Normal,
		Touched,
		Selected,
	};
public:
	EntryState m_curtState;
public:
	GUIComboBoxEntry(euint32 id,
		AttributeHandle pivotHandle,
		AttributeHandle sizeHandle);
	GUIComboBoxEntry(
		AttributeHandle pivotHandle,
		AttributeHandle sizeHandle);
};
class GUIComboBoxDropDownMenu : public GUIContainer
{
	DeclareRTTI;
public:
	GUIComboBoxDropDownMenu();
};
class GUIComboBox : public GUIHoriBar
{
	DeclareRTTI;
	friend class GUIComboBoxFactory;
protected:
	~GUIComboBox() {}
public:
	class MouseMoveEventProc : public SpriteEventProc
	{
	public:
		GUIComboBox* m_comboBox;
	public:
		MouseMoveEventProc(GUIComboBox* comboBox) 
			: m_comboBox(comboBox) {}
		~MouseMoveEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};

	class MouseButtonDownEventProc : public SpriteEventProc
	{
	public:
		GUIComboBox* m_comboBox;
	public:
		MouseButtonDownEventProc(GUIComboBox* comboBox) 
			: m_comboBox(comboBox) {}
		~MouseButtonDownEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};

	class MouseButtonUpEventProc : public SpriteEventProc
	{
	public:
		GUIComboBox* m_comboBox;
	public:
		MouseButtonUpEventProc(GUIComboBox* comboBox) 
			: m_comboBox(comboBox) {}
		~MouseButtonUpEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};
public:
	GUIComboBox(SpriteRenderer* renderer, const xhn::static_string name)
		: GUIHoriBar(renderer, name)
	{}
	virtual void Init(const xhn::static_string configName);
	virtual void GetScopeImpl(SpriteRect& result);
	void BuildBackgroundLayer(xhn::list<SpriteElement>& to);
	void BuildTextLayer(xhn::list<SpriteElement>& to);
	virtual void Build();
	virtual void BuildElementsImpl(xhn::list<SpriteElement>& to);
	virtual void TickImpl(double elapsedTime);
	virtual void TockImpl() {}
};

class GUIComboBoxFactory : public SpriteFactory
{
public:
	int m_comboBoxCount;
public:
	GUIComboBoxFactory(SpriteRenderer* renderer,
		const char* cfgName)
		: m_comboBoxCount(0)
		, SpriteFactory(renderer, cfgName)
	{}
	virtual Sprite* MakeSpriteImpl();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif