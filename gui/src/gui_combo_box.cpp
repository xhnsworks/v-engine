#include "gui_pch.h"
#include "gui_combo_box.h"
#include "render_system.h"
#include "sprite_event_hub.h"
#include "robot.h"
#include "animation.hpp"
#include "gui_proc_group.h"

///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
const FourBorders& GUIComboBox::GetFourBorders()
{
	SpriteRect mainRect;
	SpriteRect menuRect;
	GetBackgroundRect(mainRect);
	GetDropDownMenuRect(menuRect);
	mainRect.Merge(menuRect);
	mainRect.GetFourBorders(m_renderer, m_fourBorders);
	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);
	m_fourBorders.ApplyTranform(&mat);
	return m_fourBorders;
}

void GUIComboBox::OnLeave()
{
	if (m_isShowDropDownMenu) {
        HideDropDownMenu();
		m_isShowDropDownMenu = false;
	}
}

void GUIComboBox::OnPress()
{
	SetState(GUIWidget::Touched);
    ShowDropDownMenu();
	m_isShowDropDownMenu = true;
}

ProcGroup GUIComboBox::NewProcGroup()
{
    ProcGroup pg;
	pg.mouseMoveProc = ENEW LeaveableMouseMoveProc(this);
	pg.mouseButtonDownProc = ENEW PressableMouseButtonDownProc(this);
	pg.mouseButtonUpProc = ENEW EmptyMouseButtonUpProc(this);
	return pg;
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
	m_dropDownMenu = static_cast<GUIList *>(m_dropDownMenuFactory->MakeSprite());
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
