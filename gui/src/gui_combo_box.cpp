#include "gui_pch.h"
#include "gui_combo_box.h"
#include "render_system.h"
#include "sprite_event_hub.h"

ImplementRTTI(GUIComboBoxEntry, GUIHoriBar);
ImplementRTTI(GUIDropDownMenu, GUIPanel);
ImplementRTTI(GUIComboBox, GUIHoriBar);

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
		SetSize(100.0f);
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

void GUIComboBoxEntry::Build()
{
	m_elements.clear();
	BuildTextLayer(m_elements);
	BuildBackgroundLayer(m_elements);
	
	SpriteRect rect;
	GetScope(rect);
	rect.GetFourBorders(m_renderer, m_fourBorders);
    
	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);
    
	m_fourBorders.ApplyTranform(&mat);
}

void GUIComboBoxEntry::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
	BuildTextLayer(to);
	BuildBackgroundLayer(to);
	
	SpriteRect rect;
	GetScope(rect);
	rect.GetFourBorders(m_renderer, m_fourBorders);
    
	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);
    
	m_fourBorders.ApplyTranform(&mat);
}

GUIDropDownMenu::GUIDropDownMenu(SpriteRenderer* renderer)
: GUIPanel(renderer, "drop_down_menu")
, m_entryCount(0)
{
	m_sizeHandle.m_lock = ENEW xhn::RWLock;
	m_sizeHandle.AttachAttribute<Float2Attr>();

	SpriteRect panelRect;

	panelRect.size.width = 100.0f;
	panelRect.size.height = 25.0f;
	EFloat2 areaSize(247.0f - 204.0f, 184.0f - 174.0f);
	EFloat2 normalCoord(204.0f, 174.0f);
	EFloat2 touchedCoord(204.0f, 188.0f);
	EFloat2 selectedCoord(204.0f, 202.0f);
	GUIComboBoxEntryFactory::CreateSheetConfig(
		"combo_box_entry.xml",
		"BlackOrangeSkins.png",
		panelRect,
		8,
		areaSize,
		8,
		normalCoord,
		touchedCoord,
		selectedCoord);
    m_entryFactory = ENEW GUIComboBoxEntryFactory(renderer, "combo_box_entry.xml", m_sizeHandle);
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

Sprite* GUIDropDownMenuFactory::MakeSpriteImpl()
{
	GUIDropDownMenu* ret = ENEW GUIDropDownMenu(m_renderer);
	ret->Init(m_configName);
	ret->RegisterPublicEventCallback(&SpriteFrameStartEvent::s_RTTI,
		ENEW SpriteFrameStartEventProc(ret, m_renderer));
	return ret;
}