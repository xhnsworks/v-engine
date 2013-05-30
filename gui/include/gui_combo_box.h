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
#include "gui_panel.h"
///**********************************************************************///
///                           include end                                ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class SpriteRenderer;
class GUIComboBox;

class GUIComboBoxEntry : public GUIHoriBar
{
	DeclareRTTI;
    friend class GUIComboBoxEntryFactory;
protected:
	~GUIComboBoxEntry() {}
public:
	enum EntryState
	{
		Normal,
		Touched,
		Selected,
	};
    class MouseMoveEventProc : public SpriteEventProc
	{
	public:
		GUIComboBoxEntry* m_entry;
	public:
		MouseMoveEventProc(GUIComboBoxEntry* entry) : m_entry(entry) {}
		~MouseMoveEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};
    
	class MouseButtonDownEventProc : public SpriteEventProc
	{
	public:
		GUIComboBoxEntry* m_entry;
	public:
		MouseButtonDownEventProc(GUIComboBoxEntry* entry) : m_entry(entry) {}
		~MouseButtonDownEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};
    
	class MouseButtonUpEventProc : public SpriteEventProc
	{
	public:
		GUIComboBoxEntry* m_entry;
	public:
		MouseButtonUpEventProc(GUIComboBoxEntry* entry) : m_entry(entry) {}
		~MouseButtonUpEventProc() {}
	public:
		virtual void Proc(const SpriteEvent* evt);
	};

public:
	EntryState m_curtState;
public:
    GUIComboBoxEntry(SpriteRenderer* renderer,
                     const xhn::static_string name,
                     AttributeHandle sizeHandle)
    : GUIHoriBar(renderer, name, sizeHandle)
    {}
    virtual void Init(const xhn::static_string configName);
    inline void SetState(EntryState state) {
        m_curtState = state;
    }
    ///virtual void GetScopeImpl(SpriteRect& result);
    virtual void Build();
    virtual void BuildElementsImpl(xhn::list<SpriteElement>& to);
    void BuildBackgroundLayer(xhn::list<SpriteElement>& to);
};

class GUIComboBoxEntryFactory : public GUIHoriBarFactory
{
public:
    AttributeHandle m_sizeHandle;
public:
    GUIComboBoxEntryFactory(SpriteRenderer* renderer,
                            const char* cfgName,
                            AttributeHandle sizeHandle)
    : m_sizeHandle(sizeHandle)
    , GUIHoriBarFactory(renderer, cfgName)
    {}
    virtual Sprite* MakeSpriteImpl();
    static void CreateSheetConfig(const char* cfgName,
                                  const char* textureName,
                                  const SpriteRect& panelRect,
                                  float cornerSize,
                                  const EFloat2& areaSize,
                                  float areaCornerSize,
                                  const EFloat2& areaCoordNormal,
                                  const EFloat2& areaCoordTouched,
                                  const EFloat2& areaCoorfSelected);
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class GUIDropDownMenu : public GUIPanel
{
	DeclareRTTI;
	friend class GUIDropDownMenuFactory;
public:
	AttributeHandle m_sizeHandle;
    GUIComboBoxEntryFactory* m_entryFactory;
public:
	GUIDropDownMenu(SpriteRenderer* renderer);
	void AddEntry();
};

class GUIDropDownMenuFactory : public GUIPanelFactory
{
public:
	GUIDropDownMenuFactory(SpriteRenderer* renderer, const char* cfgName)
	: GUIPanelFactory(renderer, cfgName)
	{}
	virtual Sprite* MakeSpriteImpl();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
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