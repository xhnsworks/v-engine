#include "gui_pch.h"
#include "gui_container.h"
#include "xml_resource.h"
#include "render_system.h"
#include "sprite_event_hub.h"
#include "sprite_renderer.h"
#include "sfloat3.h"

GUIContainer::GUIContainer(SpriteRenderer* renderer, const xhn::static_string name)
: m_simplePanel(NULL)
, m_simplePanelFactory(NULL)
, Sprite(renderer, name)
{
	m_rectHandle.m_lock = ENEW xhn::RWLock;
	m_rectHandle.AttachAttribute<Float4Attr>();
}

GUIContainer::~GUIContainer()
{
	if (m_simplePanelFactory) {
		delete m_simplePanelFactory;
	}
}

void GUIContainer::Init(const xhn::static_string configName)
{
	XMLResourcePtr cfg = RenderSystem_load_gui_config(configName);
	if (TestResourcePtr(cfg)) {
		pugi::xml_document& doc = cfg->GetDocument();
		pugi::xml_node root = doc.child("root");
		pugi::xml_node layers = root.child("layers");
		pugi::xml_node rect = layers.child("rect");
		pugi::xml_attribute left = rect.attribute("left");
		pugi::xml_attribute top = rect.attribute("top");
		pugi::xml_attribute width = rect.attribute("width");
		pugi::xml_attribute height = rect.attribute("height");
		SetRect(left.as_float(),
                top.as_float(),
                width.as_float(),
                height.as_float());
		pugi::xml_attribute isDebugMode = rect.attribute("debug_mode");
		if (isDebugMode.as_bool()) {
			///
			SpriteRect rect(left.as_float(),
				            top.as_float(),
							width.as_float(),
							height.as_float());
			SpriteRect areaRect(0.0f, 0.0f, 32.0f, 32.0f);

			xhn::string simplePanelConfigName = "background_";
			simplePanelConfigName += configName.c_str();
			GUISimplePanelFactory<GUISimplePanel>::CreateSheetConfig(simplePanelConfigName.c_str(), 
				                                                     "base", 
													                 "default",
													                 rect,
													                 areaRect);
			m_simplePanelFactory = 
				ENEW GUISimplePanelFactory<GUISimplePanel>(m_renderer, 
				                                           simplePanelConfigName.c_str());
			m_simplePanel = m_simplePanelFactory->MakeSprite();
			AddChild(m_simplePanel);
		}
	}
}

void GUIContainer::SetRect(float left,
                           float top,
                           float width,
                           float height)
{
    ///xhn::RWLock::Instance inst = m_rectHandle.GetWriteLock();
	///Float4Attr* rect = m_rectHandle.GetAttribute<Float4Attr>();
	Float4Attr rect;
	rect.x = left;
	rect.y = top;
	rect.z = width;
	rect.w = height;
	m_rectHandle.SetAttribute(&rect);
}

void GUIContainer::GetScopeImpl(SpriteRect& result)
{
	///xhn::RWLock::Instance inst = m_rectHandle.GetReadLock();
	///Float4Attr* rect = m_rectHandle.GetAttribute<Float4Attr>();

	Float4Attr rect;
	m_rectHandle.GetAttribute(&rect);
	result.left = rect.x;
	result.top = rect.y;
	result.size.width = rect.z;
    result.size.height = rect.w;
}
void GUIContainer::Build()
{
	m_elements.clear();
	BuildElements(m_elements);

	SpriteRect rect;
	GetScope(rect);
	rect.GetFourBorders(m_renderer, m_fourBorders);

	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);

	m_fourBorders.ApplyTranform(&mat);
}

Sprite* GUIContainerFactory::MakeSpriteImpl()
{
	char mbuf[256];
	snprintf(mbuf, 255, "GUIContainer_%d", m_containerCount);
	m_containerCount++;
	GUIContainer* ret = ENEW GUIContainer(m_renderer, mbuf);
	ret->Init(m_configName);
	return ret;
}

void GUIContainerFactory::CreateSheetConfig(const char* cfgName,
                                            const char* sheetName,
                                            const SpriteRect& panelRect,
											bool isDebugMode)
{
    XMLResourcePtr xmlRes = RenderSystem_new_gui_config(cfgName);
	pugi::xml_document& doc = xmlRes->GetDocument();
	pugi::xml_node root = doc.child("root");
	if (!root)
	    root = doc.append_child("root");
	pugi::xml_node layers = root.child("layers");
	if (!layers)
		layers = root.append_child("layers");
	pugi::xml_node sheet = layers.append_child(sheetName);
	sheet.append_attribute("left").set_value(panelRect.left);
	sheet.append_attribute("top").set_value(panelRect.top);
	sheet.append_attribute("width").set_value(panelRect.size.width);
	sheet.append_attribute("height").set_value(panelRect.size.height);
	sheet.append_attribute("debug_mode").set_value(isDebugMode);
}