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
#include "gui_list.h"
///**********************************************************************///
///                           include end                                ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class GUIComboBox : public GUIListEntry
{
	DeclareRTTI;
public:
	GUIListFactory* m_dropDownMenuFactory;
	GUIList* m_dropDownMenu;
	bool m_isShowDropDownMenu;
public:
	GUIComboBox(SpriteRenderer* renderer,
                const xhn::static_string name,
				GUIListFactory* dropDownMenuFactory,
                AttributeHandle dropDownMenuSizeHandle);
	virtual void Init(const xhn::static_string configName);
	virtual void BuildDropDownMenu(xhn::list<SpriteElement>& to);
	virtual void GetScope(SpriteRect& result);
	void AddEntry(const xhn::string& str);
	void ShowDropDownMenu();
	void HideDropDownMenu();
	void GetDropDownMenuRect(SpriteRect& rect);
	inline AttributeHandle GetDropDownMenuSizeHandle() {
		return m_dropDownMenuFactory->GetListSizeHandle();
	}
	virtual void OnMouseMove(const SpriteMouseMoveEvent* mouseEvt);
	virtual void OnMouseButtonDown(const SpriteMouseButtonDownEvent* mouseEvt);
};

class GUIComboBoxFactory : public GUIListEntryFactory
{
public:
	int m_comboBoxCount;
	GUIListEntryFactory* m_entryFactory;
    GUIListFactory* m_dropDownMenuFactory;
public:
	GUIComboBoxFactory(SpriteRenderer* renderer,
		               const char* entryCfgName,
                       const char* menuCfgName);
	virtual Sprite* MakeSpriteImpl();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif