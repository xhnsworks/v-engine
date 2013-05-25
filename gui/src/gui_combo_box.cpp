#include "gui_pch.h"
#include "gui_combo_box.h"
#include "render_system.h"

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

void CreateLayer(pugi::xml_node sheet,
                 const char* textureName,
                 const SpriteRect& panelRect,
                 float cornerSize,
                 const EFloat2& areaSize,
                 float areaCornerSize,
                 const EFloat2& areaCoord)
{
    pugi::xml_node elements = sheet.append_child("elements");
    elements.append_attribute("num_elements").set_value(3);
    
    pugi::xml_node top = elements.append_child("element");
    pugi::xml_node bottom = elements.append_child("element");
    
    pugi::xml_node center = elements.append_child("element");
    
    top.append_attribute("filename").set_value(textureName);
    bottom.append_attribute("filename").set_value(textureName);
    
    center.append_attribute("filename").set_value(textureName);
    
    float panelLeft = panelRect.left;
	float panelTop = panelRect.top;
	float panelBottom = panelRect.top + panelRect.size.height;
    
    float areaLeft = areaCoord.x;
    float areaTop = areaCoord.y;
    float areaRight = areaCoord.x + areaSize.x;
    float areaBottom = areaCoord.y + areaSize.y;
    
    top.append_attribute("name").set_value("top");
    bottom.append_attribute("name").set_value("bottom");
    
    top.append_attribute("left").set_value(panelLeft);
    bottom.append_attribute("left").set_value(panelLeft);
    
    top.append_attribute("top").set_value(panelTop);
    bottom.append_attribute("top").set_value(panelBottom - cornerSize);
    
    top.append_attribute("width").set_value(panelRect.size.width);
    bottom.append_attribute("width").set_value(panelRect.size.width);
    
    top.append_attribute("height").set_value(cornerSize);
    bottom.append_attribute("height").set_value(cornerSize);
    
    top.append_attribute("area_x0").set_value(areaLeft);
    bottom.append_attribute("area_x0").set_value(areaLeft);
    
    top.append_attribute("area_x1").set_value(areaRight);
    bottom.append_attribute("area_x1").set_value(areaRight);
    
    top.append_attribute("area_y0").set_value(areaTop);
    bottom.append_attribute("area_y0").set_value(areaBottom -
                                                 areaCornerSize);
    
    top.append_attribute("area_y1").set_value(areaTop + areaCornerSize);
    bottom.append_attribute("area_y1").set_value(areaBottom);
    
    top.append_attribute("transparent").set_value(1.0f);
    bottom.append_attribute("transparent").set_value(1.0f);
    
    center.append_attribute("name").set_value("center");
    
    center.append_attribute("left").set_value(panelLeft);
    center.append_attribute("top").set_value(panelTop + cornerSize);
    center.append_attribute("width").set_value(panelRect.size.width);
    center.append_attribute("height").set_value(panelRect.size.height -
                                                cornerSize -
                                                cornerSize);
    
    center.append_attribute("area_x0").set_value(areaLeft);
    center.append_attribute("area_x1").set_value(areaRight);
    center.append_attribute("area_y0").set_value(areaTop +
                                                 areaCornerSize);
    center.append_attribute("area_y1").set_value(areaBottom -
                                                 areaCornerSize);
    
    center.append_attribute("transparent").set_value(1.0f);
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
		SetSize(100.0f);
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
/**
void GUIComboBoxEntry::GetScopeImpl(SpriteRect& result)
{
    result.left = 0.0f;
    result.top = 0.0f;
    result.size.width = 0.0f;
    result.size.height = 0.0f;
}
**/
void GUIComboBoxEntry::Build()
{
	m_elements.clear();
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
	BuildBackgroundLayer(to);
    
	SpriteRect rect;
	GetScope(rect);
	rect.GetFourBorders(m_renderer, m_fourBorders);
    
	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);
    
	m_fourBorders.ApplyTranform(&mat);
}

GUIComboBoxDropDownMenu::GUIComboBoxDropDownMenu(SpriteRenderer* renderer)
: GUIContainer(renderer, "drop_down_menu")
{
	m_sizeHandle.m_lock = ENEW xhn::RWLock;
	m_sizeHandle.AttachAttribute<Float2Attr>();
}