#include "gui_pch.h"
#include "gui_combo_box.h"
#include "render_system.h"
#include "sprite_event_hub.h"
#include "robot.h"
#include "animation.hpp"

ImplementRTTI(GUIComboBoxEntry, GUIHoriBar);
ImplementRTTI(GUIDropDownMenu, GUIPanel);
ImplementRTTI(GUIComboBox, GUIHoriBar);
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
GUIComboBoxEntryFactory::GUIComboBoxEntryFactory(SpriteRenderer* renderer,
						                         const char* cfgName)
: GUIHoriBarFactory(renderer, cfgName)
{
	Float2Attr size(100.0f, 0.0f);
	m_sizeHandle.m_lock = ENEW xhn::RWLock;
	m_sizeHandle.AttachAttribute<Float2Attr>();
	m_sizeHandle.SetAttribute(&size);
}

Sprite* GUIComboBoxEntryFactory::MakeSpriteImpl()
{
    char mbuf[256];
	snprintf(mbuf, 255, "GUIComboBoxEntry_%d", m_horiBarCount);
	m_horiBarCount++;
	GUIComboBoxEntry* ret = ENEW GUIComboBoxEntry(m_renderer,
                                                  mbuf,
                                                  m_sizeHandle);
	ret->Init(m_configName);
	ret->RegisterPublicEventCallback(&SpriteFrameStartEvent::s_RTTI,
                                     ENEW SpriteFrameStartEventProc(
                                         ret, m_renderer)
                                     );
    ret->RegisterPublicEventCallback(&SpriteMouseMoveEvent::s_RTTI,
                                     ENEW GUIComboBoxEntry::MouseMoveEventProc(
                                         ret)
                                     );
	ret->RegisterPublicEventCallback(&SpriteMouseButtonDownEvent::s_RTTI,
		                             ENEW GUIComboBoxEntry::MouseButtonDownEventProc(
									     ret)
									 );
	return ret;
}

void CreateLayer(pugi::xml_node sheet,
                 const char* textureName,
                 const SpriteRect& panelRect,
                 float cornerSize,
                 const EFloat2& areaSize,
                 float areaCornerSize,
                 const EFloat2& areaCoord)
{
    float panelLeft = panelRect.left;
	float panelTop = panelRect.top;
	float panelRight = panelRect.left + panelRect.size.width;
    
    float areaLeft = areaCoord.x;
    float areaTop = areaCoord.y;
    float areaRight = areaCoord.x + areaSize.x;
    float areaBottom = areaCoord.y + areaSize.y;
    
    pugi::xml_node elements = sheet.append_child("elements");
	elements.append_attribute("num_elements").set_value(3);
    
    pugi::xml_node left = elements.append_child("element");
	pugi::xml_node right = elements.append_child("element");
    
	pugi::xml_node center = elements.append_child("element");
    
	left.append_attribute("filename").set_value(textureName);
	right.append_attribute("filename").set_value(textureName);
    
	center.append_attribute("filename").set_value(textureName);
    
	left.append_attribute("name").set_value("left");
    right.append_attribute("name").set_value("right");
    
    left.append_attribute("left").set_value(panelLeft);
    right.append_attribute("left").set_value(panelRight - cornerSize);
    
    left.append_attribute("top").set_value(panelTop);
    right.append_attribute("top").set_value(panelTop);
    
    left.append_attribute("width").set_value(cornerSize);
    right.append_attribute("width").set_value(cornerSize);
    
    left.append_attribute("height").set_value(panelRect.size.height);
    right.append_attribute("height").set_value(panelRect.size.height);
    
    left.append_attribute("area_x0").set_value(areaLeft);
    right.append_attribute("area_x0").set_value(areaRight -
                                                areaCornerSize);
    
    left.append_attribute("area_x1").set_value(areaLeft +
                                               areaCornerSize);
    right.append_attribute("area_x1").set_value(areaRight);
    
    left.append_attribute("area_y0").set_value(areaTop);
    right.append_attribute("area_y0").set_value(areaTop);
    
    left.append_attribute("area_y1").set_value(areaBottom);
    right.append_attribute("area_y1").set_value(areaBottom);
    
    left.append_attribute("transparency").set_value(1.0f);
    right.append_attribute("transparency").set_value(1.0f);
    
	center.append_attribute("name").set_value("center");
    
    center.append_attribute("left").set_value(panelLeft + cornerSize);
    center.append_attribute("top").set_value(panelTop);
    center.append_attribute("width").set_value(panelRect.size.width -
                                               cornerSize -
                                               cornerSize);
    center.append_attribute("height").set_value(panelRect.size.height);
    
    center.append_attribute("area_x0").set_value(areaLeft +
                                                 areaCornerSize);
    center.append_attribute("area_x1").set_value(areaRight -
                                                 areaCornerSize);
    center.append_attribute("area_y0").set_value(areaTop);
    center.append_attribute("area_y1").set_value(areaBottom);
    
    center.append_attribute("transparency").set_value(1.0f);
}

void GUIComboBoxEntryFactory::CreateSheetConfig(
                            const char* cfgName,
                            const char* textureName,
                            const SpriteRect& panelRect,
                            float cornerSize,
                            const EFloat2& areaSize,
                            float areaCornerSize,
                            const EFloat2& areaCoordNormal,
                            const EFloat2& areaCoordTouched,
                            const EFloat2& areaCoorfSelected)
{
    XMLResourcePtr xmlRes = RenderSystem_new_gui_config(cfgName);
	pugi::xml_document& doc = xmlRes->GetDocument();
	pugi::xml_node root = doc.child("root");
	if (!root)
		root = doc.append_child("root");
	pugi::xml_node layers = root.child("layers");
	if (!layers)
		layers = root.append_child("layers");
	
    {
        pugi::xml_node sheet = layers.append_child("normal");
        CreateLayer(sheet, textureName, panelRect, cornerSize, areaSize, areaCornerSize, areaCoordNormal);
    }
    {
        pugi::xml_node sheet = layers.append_child("touched");
        CreateLayer(sheet, textureName, panelRect, cornerSize, areaSize, areaCornerSize, areaCoordTouched);
    }
    {
        pugi::xml_node sheet = layers.append_child("selected");
        CreateLayer(sheet, textureName, panelRect, cornerSize, areaSize, areaCornerSize, areaCoorfSelected);
    }
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
void GUIComboBoxEntry::MouseMoveEventProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseMoveEvent* mouseEvt =
		evt->DynamicCast<SpriteMouseMoveEvent>();

	const FourBorders& borders = m_entry->GetFourBorders();
	EFloat2 realCrd =
		m_entry->m_renderer->get_real_position((float)mouseEvt->m_curtMousePos.x,
		(float)mouseEvt->m_curtMousePos.y);
	EFloat3 realPt(realCrd.x, realCrd.y, 0.0f);
	sfloat3 pt = SFloat3_assign_from_efloat3(&realPt);

	if (borders.IsInBorders(pt)) {
		m_entry->SetState(GUIComboBoxEntry::Touched);
	}
	else {
		m_entry->SetState(GUIComboBoxEntry::Normal);
	}
}

void GUIComboBoxEntry::MouseButtonDownEventProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseButtonDownEvent* mouseEvt =
		evt->DynamicCast<SpriteMouseButtonDownEvent>();

	if (m_entry->m_curtState == GUIComboBoxEntry::Touched) {
		SpriteLayer* parent = m_entry->GetParent();
		if (parent) {
			parent = parent->GetParent();
			if (parent) {
				GUIComboBox* comboxBox = parent->DynamicCast<GUIComboBox>();
				if (comboxBox) {
					xhn::string text = m_entry->GetText();
					comboxBox->SetText(text);
				}
			}
		}
	}
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
GUIComboBoxEntry::~GUIComboBoxEntry()
{
}

void GUIComboBoxEntry::Init(const xhn::static_string configName)
{
    XMLResourcePtr cfg = RenderSystem_load_gui_config(configName);
	if (TestResourcePtr(cfg)) {
		pugi::xml_document& doc = cfg->GetDocument();
		pugi::xml_node root = doc.child("root");
		pugi::xml_node layers = root.child("layers");
		pugi::xml_node normallayer = layers.child("normal");
        pugi::xml_node touchedlayer = layers.child("touched");
        pugi::xml_node selectedlayer = layers.child("selected");
		if (!normallayer ||
            !touchedlayer ||
            !selectedlayer)
			return;
        {
            SpriteLayerPtr layer = ENEW GUIHoriBarLayer("normal",
                                                        m_pivotHandle,
                                                        m_sizeHandle);
            layer->LoadConfigImpl(normallayer);
            AddChild(layer);
        }
		{
            SpriteLayerPtr layer = ENEW GUIHoriBarLayer("touched",
                                                        m_pivotHandle,
                                                        m_sizeHandle);
            layer->LoadConfigImpl(touchedlayer);
            AddChild(layer);
        }
        {
            SpriteLayerPtr layer = ENEW GUIHoriBarLayer("selected",
                                                        m_pivotHandle,
                                                        m_sizeHandle);
            layer->LoadConfigImpl(selectedlayer);
            AddChild(layer);
        }
		{
			SpriteLayerPtr layer = ENEW SpriteTextLayer("text");
			layer->m_horizontalAlignmentMode = SpriteLayer::CenterHorizontalAligned;
			layer->m_verticalAlignmentMode = SpriteLayer::CenterVerticalAligned;
            AddChild(layer);
		}
        m_curtState = Normal;
	}
}

void GUIComboBoxEntry::BuildBackgroundLayer(xhn::list<SpriteElement>& to)
{
    switch (m_curtState)
	{
        case Normal:
		{
            SpriteLayerPtr layerPtr = GetLayer("normal");
            if (layerPtr.get())
                layerPtr->BuildElementsImpl(to);
		}
            break;
        case Touched:
		{
            SpriteLayerPtr layerPtr = GetLayer("touched");
            if (layerPtr.get())
                layerPtr->BuildElementsImpl(to);
		}
            break;
        case Selected:
        default:
		{
            SpriteLayerPtr layerPtr = GetLayer("selected");
            if (layerPtr.get())
                layerPtr->BuildElementsImpl(to);
		}
            break;
	}
}

void GUIComboBoxEntry::BuildTextLayer(xhn::list<SpriteElement>& to)
{
	SpriteLayerPtr layerPtr = GetLayer("text");
	if (layerPtr.get())
		layerPtr->BuildElementsImpl(to);
}

void GUIComboBoxEntry::SetText(const xhn::string& text)
{
	SpriteLayerPtr layerPtr = GetLayer("text");
	if (layerPtr.get()) {
		SpriteTextLayer* textLayer = layerPtr->DynamicCast<SpriteTextLayer>();
		EColor color(1.0f, 1.0f, 1.0f, 1.0f);
		textLayer->SetText(text, color, 2.0f, 1.0f, Pixel16);
	}
}

xhn::string GUIComboBoxEntry::GetText()
{
	SpriteLayerPtr layerPtr = GetLayer("text");
	if (layerPtr.get()) {
		SpriteTextLayer* textLayer = layerPtr->DynamicCast<SpriteTextLayer>();
		EColor color(1.0f, 1.0f, 1.0f, 1.0f);
		return textLayer->GetText();
	}
	return "";
}

void GUIComboBoxEntry::GetBackgroundRect(SpriteRect& rect)
{
	SpriteLayerPtr layerPtr = GetLayer("normal");
	if (layerPtr.get()) {
		layerPtr->GetScope(rect);
	}
}

void GUIComboBoxEntry::Build()
{
	m_elements.clear();
}

void GUIComboBoxEntry::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
	BuildTextLayer(to);
	BuildBackgroundLayer(to);
	BuildDropDownMenu(to);
	
	SpriteRect rect;
	GetScope(rect);
	rect.GetFourBorders(m_renderer, m_fourBorders);
    
	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);
    
	m_fourBorders.ApplyTranform(&mat);
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
GUIDropDownMenu::GUIDropDownMenu(SpriteRenderer* renderer, 
				                 GUIComboBoxEntryFactory* entryFactory,
				                 AttributeHandle sizeHandle)
: m_entryFactory(entryFactory)
, GUIPanel(renderer, "drop_down_menu", sizeHandle)
, m_entryCount(0)
{
	Float2Attr size(100.0f, 0.0f);
	m_sizeHandle.SetAttribute(&size);
}

void GUIDropDownMenu::AddEntry(const xhn::string& str)
{
    GUIComboBoxEntry* entry = m_entryFactory->MakeSprite()->DynamicCast<GUIComboBoxEntry>();
	entry->SetText(str);
	SpriteRect rect;
	entry->GetScope(rect);
	entry->SetCoord(0.0f, rect.size.height * (float)m_entryCount);
	AddChild(entry);
	AlwaysOnTop(entry);
	m_entryCount++;
}

void GUIDropDownMenu::RemoveAllEntries()
{
	SpriteLayerList::iterator iter = m_children.begin();
	SpriteLayerList::iterator end = m_children.end();
	for (; iter != end; ) {
		SpriteLayerPtr sptLayerPtr = *iter;
		if (sptLayerPtr->DynamicCast<GUIComboBoxEntry>() != NULL) {
            iter = m_children.remove(iter);
		}
		else
			iter++;
	}
	m_entryCount = 0;
}

void GUIDropDownMenu::RemoveBackground()
{
	SpriteLayerList::iterator iter = m_children.begin();
	SpriteLayerList::iterator end = m_children.end();
	for (; iter != end; ) {
		SpriteLayerPtr sptLayerPtr = *iter;
		if (sptLayerPtr->DynamicCast<GUIComboBoxEntry>() == NULL) {
			iter = m_children.remove(iter);
		}
		else
			iter++;
	}
}

void GUIDropDownMenu::GetBackgroundRect(SpriteRect& rect)
{
	SpriteLayerList::iterator iter = m_children.begin();
	SpriteLayerList::iterator end = m_children.end();
	for (; iter != end; iter++) {
		SpriteLayerPtr sptLayerPtr = *iter;

		GUIPanelLayer* background = sptLayerPtr->DynamicCast<GUIPanelLayer>();
		if (background) {
            background->GetScope(rect);
			return;
		}
	}
}

void GUIDropDownMenu::BuildFourBorders()
{
	SpriteRect rect;
	GetBackgroundRect(rect);
	rect.GetFourBorders(m_renderer, m_fourBorders);
	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);
	m_fourBorders.ApplyTranform(&mat);
}
void GUIDropDownMenu::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
	xhn::list<SpriteElement> buffer;
	BuildElements(buffer);
	BuildFourBorders();

	xhn::list<SpriteElement>::iterator iter = buffer.begin();
	xhn::list<SpriteElement>::iterator end = buffer.end();
	for (; iter != end; iter++) {
		SpriteElement& ele = *iter;
		ele.m_fourBorders = &GetFourBorders();
		to.push_back(ele);
	}
}
void GUIDropDownMenu::Build()
{
	m_elements.clear();
	BuildElements(m_elements);
	BuildFourBorders();

	xhn::list<SpriteElement>::iterator iter = m_elements.begin();
	xhn::list<SpriteElement>::iterator end = m_elements.end();
	for (; iter != end; iter++) {
		SpriteElement& ele = *iter;
		ele.m_fourBorders = &GetFourBorders();
	}
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
GUIDropDownMenuFactory::GUIDropDownMenuFactory(SpriteRenderer* renderer,
											   const char* cfgName,
											   GUIComboBoxEntryFactory* entryFactory,
											   AttributeHandle dropDownMenuSizeHandle)
: m_entryFactory(entryFactory)
, m_dropDownMenuSizeHandle(dropDownMenuSizeHandle)
, GUIPanelFactory(renderer, cfgName)
{
}

Sprite* GUIDropDownMenuFactory::MakeSpriteImpl()
{
	GUIDropDownMenu* ret = ENEW GUIDropDownMenu(m_renderer, 
		                                        m_entryFactory, 
												m_dropDownMenuSizeHandle);
	ret->Init(m_configName);
	ret->RegisterPublicEventCallback(&SpriteFrameStartEvent::s_RTTI,
		ENEW SpriteFrameStartEventProc(ret, m_renderer));
	return ret;
}

void GUIDropDownMenuFactory::CreateAnimationConfig(const char* cfgName,
								  const char* showAnimName,
								  const char* hideAnimName,
                                  float width,
								  float maxHeight)
{
    XMLResourcePtr xmlRes = RenderSystem_new_animation_config(cfgName);
    pugi::xml_document& doc = xmlRes->GetDocument();
	pugi::xml_node root = doc.append_child("root");
    pugi::xml_node animations = root.append_child("animations");
    pugi::xml_node showAnim = animations.append_child(showAnimName);
    pugi::xml_node hideAnim = animations.append_child(hideAnimName);
    {
        showAnim.append_attribute("num_timelines").set_value(1);
        pugi::xml_node timeLine = showAnim.append_child("timeline");
        timeLine.append_attribute("name").set_value("show");
        timeLine.append_attribute("type").set_value("Float2");
        timeLine.append_attribute("num_keyframes").set_value(2);
        timeLine.append_attribute("is_looped").set_value(false);
        pugi::xml_node keyframe0 = timeLine.append_child("keyframe");
        pugi::xml_node keyframe1 = timeLine.append_child("keyframe");
        keyframe0.append_attribute("time").set_value(0.0f);
        keyframe0.append_attribute("value_x").set_value(width);
        keyframe0.append_attribute("value_y").set_value(0.0f);
        keyframe1.append_attribute("time").set_value(0.25f);
        keyframe1.append_attribute("value_x").set_value(width);
        keyframe1.append_attribute("value_y").set_value(maxHeight);
    }
    {
		hideAnim.append_attribute("num_timelines").set_value(1);
		pugi::xml_node timeLine = hideAnim.append_child("timeline");
		timeLine.append_attribute("name").set_value("show");
		timeLine.append_attribute("type").set_value("Float2");
		timeLine.append_attribute("num_keyframes").set_value(2);
		timeLine.append_attribute("is_looped").set_value(false);
		pugi::xml_node keyframe0 = timeLine.append_child("keyframe");
		pugi::xml_node keyframe1 = timeLine.append_child("keyframe");
		keyframe0.append_attribute("time").set_value(0.0f);
		keyframe0.append_attribute("value_x").set_value(width);
		keyframe0.append_attribute("value_y").set_value(maxHeight);
		keyframe1.append_attribute("time").set_value(0.25f);
		keyframe1.append_attribute("value_x").set_value(width);
		keyframe1.append_attribute("value_y").set_value(0.0f);
    }
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
void GUIComboBox::MouseMoveEventProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseMoveEvent* mouseEvt =
		evt->DynamicCast<SpriteMouseMoveEvent>();

	SpriteRect mainRect;
	SpriteRect menuRect;
	FourBorders fourBorders;
	m_comboBox->GetBackgroundRect(mainRect);
	m_comboBox->GetDropDownMenuRect(menuRect);
	mainRect.Merge(menuRect);
	mainRect.GetFourBorders(m_comboBox->m_renderer, fourBorders);
	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	m_comboBox->GetMatrix(&mat);
	fourBorders.ApplyTranform(&mat);

	EFloat2 realCrd =
		m_comboBox->m_renderer->get_real_position((float)mouseEvt->m_curtMousePos.x,
		(float)mouseEvt->m_curtMousePos.y);

	EFloat3 realPt(realCrd.x, realCrd.y, 0.0f);
	sfloat3 pt = SFloat3_assign_from_efloat3(&realPt);

	if (fourBorders.IsInBorders(pt)) {
		m_comboBox->SetState(GUIComboBoxEntry::Touched);
	}
	else {
		m_comboBox->SetState(GUIComboBoxEntry::Normal);
		if (m_comboBox->m_isShowDropDownMenu) {
			m_comboBox->HideDropDownMenu();
		}
	}
}
void GUIComboBox::MouseButtonDownEventProc::Proc(const SpriteEvent* evt)
{
	const SpriteMouseButtonDownEvent* mouseEvt =
		evt->DynamicCast<SpriteMouseButtonDownEvent>();

	if (mouseEvt->m_leftButtomDown && 
		m_comboBox->m_curtState == GUIComboBoxEntry::Touched) {
		m_comboBox->ShowDropDownMenu();
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
						 GUIDropDownMenuFactory* dropDownMenuFactory,
						 AttributeHandle dropDownMenuSizeHandle)
: m_dropDownMenuFactory(dropDownMenuFactory)
, m_dropDownMenu(NULL)
, m_isShowDropDownMenu(false)
, GUIComboBoxEntry(renderer, name, dropDownMenuSizeHandle)
{
}

void GUIComboBox::Init(const xhn::static_string configName)
{
	GUIComboBoxEntry::Init(configName);
	SpriteRect rect;
	GetBackgroundRect(rect);
	m_dropDownMenu = m_dropDownMenuFactory->MakeSprite()->DynamicCast<GUIDropDownMenu>();
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
									   , GUIComboBoxEntryFactory(renderer, entryCfgName)
{
	m_entryFactory = ENEW GUIComboBoxEntryFactory(m_renderer,
		entryCfgName,
		m_sizeHandle);
	m_dropDownMenuFactory = ENEW GUIDropDownMenuFactory(m_renderer,
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
	ret->RegisterPublicEventCallback(&SpriteFrameStartEvent::s_RTTI,
		ENEW SpriteFrameStartEventProc(
		ret, m_renderer)
		);
	ret->RegisterPublicEventCallback(&SpriteMouseMoveEvent::s_RTTI,
		ENEW GUIComboBox::MouseMoveEventProc(
		ret)
		);
	ret->RegisterPublicEventCallback(&SpriteMouseButtonDownEvent::s_RTTI,
		ENEW GUIComboBox::MouseButtonDownEventProc(
		ret)
		);
	ret->SetText("ggv");
	ret->AddEntry("abc");
	ret->AddEntry("def");
	ret->SetText("bbx");
	return ret;
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
