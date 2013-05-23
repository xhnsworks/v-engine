#include "gui_pch.h"
#include "gui_combo_box.h"
ImplementRTTI(GUIComboBoxLayer, GUIHoriBarLayer);
ImplementRTTI(GUIComboBoxTextLayer, SpriteTextLayer);
ImplementRTTI(GUIComboBoxEntry, GUIContainer);
ImplementRTTI(GUIComboBoxMenuBackground, GUIPanelLayer);
ImplementRTTI(GUIComboBoxDropDownMenu, GUIContainer);
ImplementRTTI(GUIComboBox, GUIHoriBar);

GUIComboBoxEntry::GUIComboBoxEntry(euint32 id,
				 AttributeHandle pivotHandle,
				 AttributeHandle sizeHandle)
{
	char mbuf[256];
	snprintf("combox_entry_%d", id);
	xhn::static_string name = mbuf;
	SpriteLayerPtr panel = ENEW GUIComboBoxLayer(name, pivotHandle, sizeHandle);
	SpriteLayerPtr text = ENEW GUIComboBoxTextLayer;
	AddChild(panel);
	AddChild(text);
}
GUIComboBoxEntry::GUIComboBoxEntry(
				 AttributeHandle pivotHandle,
				 AttributeHandle sizeHandle)
{
	SpriteLayerPtr panel = ENEW GUIComboBoxLayer("base", pivotHandle, sizeHandle);
	SpriteLayerPtr text = ENEW GUIComboBoxTextLayer;
	AddChild(panel);
	AddChild(text);
}

GUIComboBoxDropDownMenu::GUIComboBoxDropDownMenu()
{
	m_sizeHandle.m_lock = ENEW xhn::RWLock;
	m_sizeHandle.AttachAttribute<EFloat2>();
}