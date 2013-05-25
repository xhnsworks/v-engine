#include "gui_pch.h"
#include "gui_combo_box.h"

ImplementRTTI(GUIComboBoxEntry, GUIHoriBar);
ImplementRTTI(GUIComboBoxMenuBackground, GUIPanelLayer);
ImplementRTTI(GUIComboBoxDropDownMenu, GUIContainer);
ImplementRTTI(GUIComboBox, GUIHoriBar);

Sprite* GUIComboBoxEntryFactory::MakeSpriteImpl()
{
    char mbuf[256];
	snprintf(mbuf, 255, "GUIComboBoxEntry_%d", m_horiBarCount);
	m_horiBarCount++;
	GUIComboBoxEntry* ret = ENEW GUIComboBoxEntry(m_renderer, mbuf);
	ret->Init(m_configName);
	ret->RegisterPublicEventCallback(&SpriteFrameStartEvent::s_RTTI,
                                     ENEW SpriteFrameStartEventProc(
                                         ret, m_renderer)
                                     );
	return ret;
}

GUIComboBoxDropDownMenu::GUIComboBoxDropDownMenu(SpriteRenderer* renderer)
: GUIContainer(renderer, "drop_down_menu")
{
	m_sizeHandle.m_lock = ENEW xhn::RWLock;
	m_sizeHandle.AttachAttribute<Float2Attr>();
}