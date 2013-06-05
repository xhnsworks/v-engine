#include "gui_pch.h"
#include "gui_combo_box.h"
#include "render_system.h"
#include "sprite_event_hub.h"
#include "robot.h"
#include "animation.hpp"

ImplementRTTI(GUIComboBox, GUIHoriBar);
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
void GUIComboBox::OnMouseMove(const SpriteMouseMoveEvent* mouseEvt)
{
	SpriteRect mainRect;
	SpriteRect menuRect;
	FourBorders fourBorders;
	GetBackgroundRect(mainRect);
	GetDropDownMenuRect(menuRect);
	mainRect.Merge(menuRect);
	mainRect.GetFourBorders(m_renderer, fourBorders);
	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);
	fourBorders.ApplyTranform(&mat);

	EFloat2 realCrd =
		m_renderer->get_real_position(
                                      (float)mouseEvt->m_curtMouseCoord.x,
		                              (float)mouseEvt->m_curtMouseCoord.y);

	EFloat3 realPt(realCrd.x, realCrd.y, 0.0f);
	sfloat3 pt = SFloat3_assign_from_efloat3(&realPt);

	if (fourBorders.IsInBorders(pt)) {
		SetState(GUIListEntry::Touched);
	}
	else {
		SetState(GUIListEntry::Normal);
		if (m_isShowDropDownMenu) {
			HideDropDownMenu();
		}
	}
}
void GUIComboBox::OnMouseButtonDown(const SpriteMouseButtonDownEvent* mouseEvt)
{
	if (mouseEvt->m_leftButtomDown && 
		GetState() == GUITouchable::Touched) {
		ShowDropDownMenu();
	}
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
GUIComboBox::GUIComboBox(SpriteRenderer* renderer,
						 const xhn::static_string name,
						 GUIListFactory* dropDownMenuFactory,
						 AttributeHandle dropDownMenuSizeHandle)
: m_dropDownMenuFactory(dropDownMenuFactory)
, m_dropDownMenu(NULL)
, m_isShowDropDownMenu(false)
, GUIListEntry(renderer, name, dropDownMenuSizeHandle)
{
}

void GUIComboBox::Init(const xhn::static_string configName)
{
	GUIListEntry::Init(configName);
	SpriteRect rect;
	GetBackgroundRect(rect);
	m_dropDownMenu = m_dropDownMenuFactory->MakeSprite()->DynamicCast<GUIList>();
	m_dropDownMenu->SetCoord(0.0f, rect.size.height);
	AddChild(m_dropDownMenu);
}
void GUIComboBox::BuildDropDownMenu(xhn::list<SpriteElement>& to)
{
	m_dropDownMenu->BuildElementsImpl(to);
}
void GUIComboBox::GetScope(SpriteRect& result)
{
    GetBackgroundRect(result);
}
void GUIComboBox::AddEntry(const xhn::string& str)
{
    m_dropDownMenu->AddEntry(str);
}
void GUIComboBox::ShowDropDownMenu()
{
	if (!m_isShowDropDownMenu) {
		RWBuffer channel =
			RobotManager::Get()->GetChannel("RenderRobot",
			"AnimationRobot");
		if (channel) {
			PlayAnimCommand* pac = ENEW PlayAnimCommand("show");
			RWBuffer_Write(channel, (const euint*)&pac, sizeof(pac));
		}
		m_isShowDropDownMenu = true;
	}
}
void GUIComboBox::HideDropDownMenu()
{
	if (m_isShowDropDownMenu) {
		RWBuffer channel =
			RobotManager::Get()->GetChannel("RenderRobot",
			"AnimationRobot");
		if (channel) {
			PlayAnimCommand* pac = ENEW PlayAnimCommand("hide");
			RWBuffer_Write(channel, (const euint*)&pac, sizeof(pac));
		}
		m_isShowDropDownMenu = false;
	}
}
void GUIComboBox::GetDropDownMenuRect(SpriteRect& rect)
{
    m_dropDownMenu->GetScope(rect);
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
GUIComboBoxFactory::GUIComboBoxFactory(SpriteRenderer* renderer,
									   const char* entryCfgName,
									   const char* menuCfgName)
									   : m_comboBoxCount(0)
									   , GUIListEntryFactory(renderer, entryCfgName)
{
	m_entryFactory = ENEW GUIListEntryFactory(m_renderer,
		entryCfgName,
		m_sizeHandle);
	m_dropDownMenuFactory = ENEW GUIListFactory(m_renderer,
		menuCfgName,
		m_entryFactory,
		m_sizeHandle);
}
Sprite* GUIComboBoxFactory::MakeSpriteImpl()
{
	char mbuf[256];
	snprintf(mbuf, 255, "GUIComboBox_%d", m_horiBarCount);
	m_horiBarCount++;
	GUIComboBox* ret = ENEW GUIComboBox(m_renderer, 
		                                mbuf, 
										m_dropDownMenuFactory, 
										m_sizeHandle);
	ret->Init(m_configName);
	ret->SetText("ggv");
	ret->AddEntry("abc");
	ret->AddEntry("def");
	ret->SetText("bbx");
	return ret;
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
