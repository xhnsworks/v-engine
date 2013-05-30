#include "gui_pch.h"
#include "gui_panel.h"
#include "xml_resource.h"
#include "render_system.h"
#include "sprite_event_hub.h"
#include "sprite_renderer.h"
#include "sfloat3.h"
ImplementRTTI(GUIPanelLayer, SpriteNormalLayer);
ImplementRTTI(GUIPanel, Sprite);
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
void GUIPanelLayer::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
    matrix4x4 transform;
    GetMatrix(&transform);
    
	SpriteElementMap::iterator iterLowerLeft =
    m_elementBuffer.find("lower_left_corner");
	SpriteElementMap::iterator iterUpperLeft =
    m_elementBuffer.find("upper_left_corner");
	SpriteElementMap::iterator iterUpperRight =
    m_elementBuffer.find("upper_right_corner");
	SpriteElementMap::iterator iterLowerRight =
    m_elementBuffer.find("lower_right_corner");

	SpriteElementMap::iterator iterLeft = m_elementBuffer.find("left");
	SpriteElementMap::iterator iterTop = m_elementBuffer.find("top");
	SpriteElementMap::iterator iterRight = m_elementBuffer.find("right");
	SpriteElementMap::iterator iterBottom = m_elementBuffer.find("bottom");

	SpriteElementMap::iterator iterCenter = m_elementBuffer.find("center");

	SpriteElement tmpLowerLeft = iterLowerLeft->second;
	SpriteElement tmpUpperLeft = iterUpperLeft->second;
	SpriteElement tmpUpperRight = iterUpperRight->second;
	SpriteElement tmpLowerRight = iterLowerRight->second;

	SpriteElement tmpLeft = iterLeft->second;
	SpriteElement tmpTop = iterTop->second;
	SpriteElement tmpRight = iterRight->second;
	SpriteElement tmpBottom = iterBottom->second;

	SpriteElement tmpCenter = iterCenter->second;

	Float2Attr pivot;
	Float2Attr size;
	{
		/**
		xhn::RWLock::Instance inst = m_pivotHandle.GetReadLock();
		pivot = *(m_pivotHandle.GetAttribute<Float2Attr>());
		**/
		m_pivotHandle.GetAttribute(&pivot);
	}
	{
		/**
		xhn::RWLock::Instance inst = m_sizeHandle.GetReadLock();
		size = *(m_sizeHandle.GetAttribute<Float2Attr>());
		**/
		m_sizeHandle.GetAttribute(&size);
	}

	float left = tmpUpperLeft.m_rect.left - pivot.x;
	float top = tmpUpperLeft.m_rect.top - pivot.y;
	float right =
    tmpLowerRight.m_rect.left + tmpLowerRight.m_rect.size.width - pivot.x;
	float bottom =
    tmpLowerRight.m_rect.top + tmpLowerRight.m_rect.size.height - pivot.y;

	float realWidth = right - left;
	float realHeight = bottom - top;

	float scaleX = size.x / realWidth;
	float scaleY = size.y / realHeight;

	left *= scaleX;
	top *= scaleY;
	right *= scaleX;
	bottom *= scaleY;

	tmpLowerLeft.m_rect.left = left;
	tmpLowerLeft.m_rect.top = bottom - tmpLowerLeft.m_rect.size.height;

    tmpUpperLeft.m_rect.left = left;
	tmpUpperLeft.m_rect.top = top;

    tmpUpperRight.m_rect.left = right - tmpUpperRight.m_rect.size.width;
	tmpUpperRight.m_rect.top = top;

    tmpLowerRight.m_rect.left = right - tmpLowerRight.m_rect.size.width;
	tmpLowerRight.m_rect.top = bottom - tmpLowerRight.m_rect.size.height;

	tmpLeft.m_rect.left = left;
	tmpLeft.m_rect.top = top + tmpUpperLeft.m_rect.size.height;
	tmpLeft.m_rect.size.height =
    tmpLowerLeft.m_rect.top -
    tmpUpperLeft.m_rect.top -
    tmpUpperLeft.m_rect.size.height;

	tmpTop.m_rect.left = left + tmpUpperLeft.m_rect.size.width;
	tmpTop.m_rect.top = top;
	tmpTop.m_rect.size.width =
    tmpUpperRight.m_rect.left -
    tmpUpperLeft.m_rect.left -
    tmpUpperLeft.m_rect.size.width;

	tmpRight.m_rect.left = right - tmpRight.m_rect.size.width;
    
	tmpRight.m_rect.top =
    tmpUpperRight.m_rect.top +
    tmpUpperRight.m_rect.size.height;
    
	tmpRight.m_rect.size.height =
    tmpLowerRight.m_rect.top -
    tmpUpperRight.m_rect.top -
    tmpUpperRight.m_rect.size.height;

	tmpBottom.m_rect.left = left + tmpLowerLeft.m_rect.size.width;
	tmpBottom.m_rect.top = tmpLowerLeft.m_rect.top;
	tmpBottom.m_rect.size.width =
    tmpUpperRight.m_rect.left -
    tmpLowerLeft.m_rect.left -
    tmpLowerLeft.m_rect.size.width;

    tmpCenter.m_rect.left = left + tmpLeft.m_rect.size.width;
	tmpCenter.m_rect.top = top + tmpTop.m_rect.size.height;
	tmpCenter.m_rect.size.width = tmpTop.m_rect.size.width;
	tmpCenter.m_rect.size.height = tmpRight.m_rect.size.height;
    
    tmpCenter.SetTransform(&transform);
    tmpLeft.SetTransform(&transform);
    tmpTop.SetTransform(&transform);
    tmpRight.SetTransform(&transform);
    tmpBottom.SetTransform(&transform);
    tmpLowerLeft.SetTransform(&transform);
    tmpLowerRight.SetTransform(&transform);
    tmpUpperLeft.SetTransform(&transform);
    tmpUpperRight.SetTransform(&transform);

	to.push_back(tmpCenter);

	to.push_back(tmpLeft);
	to.push_back(tmpTop);
	to.push_back(tmpRight);
	to.push_back(tmpBottom);
	
	to.push_back(tmpLowerLeft);
	to.push_back(tmpLowerRight);
	to.push_back(tmpUpperLeft);
	to.push_back(tmpUpperRight);
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
void GUIPanelLayer::GetScopeImpl(SpriteRect& result)
{
	SpriteElementMap::iterator iterLowerLeft =
    m_elementBuffer.find("lower_left_corner");
	SpriteElementMap::iterator iterUpperLeft =
    m_elementBuffer.find("upper_left_corner");
	SpriteElementMap::iterator iterUpperRight =
    m_elementBuffer.find("upper_right_corner");
	SpriteElementMap::iterator iterLowerRight =
    m_elementBuffer.find("lower_right_corner");

	SpriteElementMap::iterator iterLeft = m_elementBuffer.find("left");
	SpriteElementMap::iterator iterTop = m_elementBuffer.find("top");
	SpriteElementMap::iterator iterRight = m_elementBuffer.find("right");
	SpriteElementMap::iterator iterBottom = m_elementBuffer.find("bottom");

	SpriteElementMap::iterator iterCenter = m_elementBuffer.find("center");

	SpriteElement tmpLowerLeft = iterLowerLeft->second;
	SpriteElement tmpUpperLeft = iterUpperLeft->second;
	SpriteElement tmpUpperRight = iterUpperRight->second;
	SpriteElement tmpLowerRight = iterLowerRight->second;

	SpriteElement tmpLeft = iterLeft->second;
	SpriteElement tmpTop = iterTop->second;
	SpriteElement tmpRight = iterRight->second;
	SpriteElement tmpBottom = iterBottom->second;

	SpriteElement tmpCenter = iterCenter->second;

	Float2Attr pivot;
	Float2Attr size;
	{
		/**
		xhn::RWLock::Instance inst = m_pivotHandle.GetReadLock();
		pivot = *(m_pivotHandle.GetAttribute<Float2Attr>());
		**/
		m_pivotHandle.GetAttribute(&pivot);
	}
	{
		/**
		xhn::RWLock::Instance inst = m_sizeHandle.GetReadLock();
		size = *(m_sizeHandle.GetAttribute<Float2Attr>());
		**/
		m_sizeHandle.GetAttribute(&size);
	}

	float left = tmpUpperLeft.m_rect.left - pivot.x;
	float top = tmpUpperLeft.m_rect.top - pivot.y;
	float right =
    tmpLowerRight.m_rect.left +
    tmpLowerRight.m_rect.size.width -
    pivot.x;
	float bottom =
    tmpLowerRight.m_rect.top +
    tmpLowerRight.m_rect.size.height -
    pivot.y;

	float realWidth = right - left;
	float realHeight = bottom - top;

	float scaleX = size.x / realWidth;
	float scaleY = size.y / realHeight;

	left *= scaleX;
	top *= scaleY;
	right *= scaleX;
	bottom *= scaleY;

	result.left = left;
	result.top = top;
	result.size.width = right - left;
	result.size.height = bottom - top;
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
GUIPanel::GUIPanel(SpriteRenderer* renderer, const xhn::static_string name)
: Sprite(renderer, name)
{
	///m_sizeHandle.m_attr = &m_size;
	m_sizeHandle.m_lock = ENEW xhn::RWLock;
	m_sizeHandle.AttachAttribute<Float2Attr>();
}

void GUIPanel::Init(const xhn::static_string configName)
{
	XMLResourcePtr cfg = RenderSystem_load_gui_config(configName);
	if (TestResourcePtr(cfg)) {
		pugi::xml_document& doc = cfg->GetDocument();
		pugi::xml_node root = doc.child("root");
		pugi::xml_node layers = root.child("layers");
		pugi::xml_node baselayer = layers.child("base");
		if (!baselayer)
			return;
		SpriteLayerPtr layer =
        ENEW GUIPanelLayer("base", m_pivotHandle, m_sizeHandle);
		layer->LoadConfigImpl(baselayer);
		///m_children.push_back(layer);
        AddChild(layer);
	}
}

void GUIPanel::SetSize(float x, float y)
{
	/**
    xhn::RWLock::Instance inst = m_sizeHandle.GetWriteLock();
	Float2Attr* size = m_sizeHandle.GetAttribute<Float2Attr>();
	size->x = x;
	size->y = y;
	**/
	Float2Attr size(x, y);
	m_sizeHandle.SetAttribute(&size);
}

Sprite* GUIPanelFactory::MakeSpriteImpl()
{
	char mbuf[256];
	snprintf(mbuf, 255, "GUIPanel_%d", m_panelCount);
	m_panelCount++;
	GUIPanel* ret = ENEW GUIPanel(m_renderer, mbuf);
	ret->Init(m_configName);
	ret->RegisterPublicEventCallback(&SpriteFrameStartEvent::s_RTTI,
                                     ENEW SpriteFrameStartEventProc(ret, m_renderer));
	return ret;
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///
///**********************************************************************///
///                       class implement begin                          ///
///**********************************************************************///
void GUIPanelFactory::CreateSheetConfig(const char* cfgName,
                                        const char* sheetName,
                                        const char* textureName,
										const SpriteRect& panelRect,
                                        const SpriteSize& cornerSize,
                                        const SpriteRect& areaRect,
                                        const SpriteSize& areaCornerSize)
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
	pugi::xml_node elements = sheet.append_child("elements");
	elements.append_attribute("num_elements").set_value(9);

	pugi::xml_node lowerLeftCorner = elements.append_child("element");
    pugi::xml_node upperLeftCorner = elements.append_child("element");
	pugi::xml_node upperRightCorner = elements.append_child("element");
	pugi::xml_node lowerRightCorner = elements.append_child("element");

	pugi::xml_node left = elements.append_child("element");
	pugi::xml_node top = elements.append_child("element");
	pugi::xml_node right = elements.append_child("element");
	pugi::xml_node bottom = elements.append_child("element");

	pugi::xml_node center = elements.append_child("element");

	lowerLeftCorner.append_attribute("filename").set_value(textureName);
	upperLeftCorner.append_attribute("filename").set_value(textureName);
	upperRightCorner.append_attribute("filename").set_value(textureName);
	lowerRightCorner.append_attribute("filename").set_value(textureName);

	left.append_attribute("filename").set_value(textureName);
	top.append_attribute("filename").set_value(textureName);
	right.append_attribute("filename").set_value(textureName);
	bottom.append_attribute("filename").set_value(textureName);

	center.append_attribute("filename").set_value(textureName);

	float panelLeft = panelRect.left;
	float panelTop = panelRect.top;
	float panelRight = panelRect.left + panelRect.size.width;
	float panelBottom = panelRect.top + panelRect.size.height;

	float areaLeft = areaRect.left;
	float areaTop = areaRect.top;
	float areaRight = areaRect.left + areaRect.size.width;
	float areaBottom = areaRect.top + areaRect.size.height;

    
    lowerLeftCorner.append_attribute("name").set_value("lower_left_corner");
    upperLeftCorner.append_attribute("name").set_value("upper_left_corner");
    upperRightCorner.append_attribute("name").set_value("upper_right_corner");
    lowerRightCorner.append_attribute("name").set_value("lower_right_corner");
    
    lowerLeftCorner.append_attribute("left").set_value(panelLeft);
    upperLeftCorner.append_attribute("left").set_value(panelLeft);
    upperRightCorner.append_attribute("left").set_value(panelRight -
                                                        cornerSize.width);
    lowerRightCorner.append_attribute("left").set_value(panelRight -
                                                        cornerSize.width);
    
    lowerLeftCorner.append_attribute("top").set_value(panelBottom -
                                                      cornerSize.height);
    upperLeftCorner.append_attribute("top").set_value(panelTop);
    upperRightCorner.append_attribute("top").set_value(panelTop);
    lowerRightCorner.append_attribute("top").set_value(panelBottom -
                                                       cornerSize.height);
    
    lowerLeftCorner.append_attribute("width").set_value(cornerSize.width);
    upperLeftCorner.append_attribute("width").set_value(cornerSize.width);
    upperRightCorner.append_attribute("width").set_value(cornerSize.width);
    lowerRightCorner.append_attribute("width").set_value(cornerSize.width);
    
    lowerLeftCorner.append_attribute("height").set_value(cornerSize.height);
    upperLeftCorner.append_attribute("height").set_value(cornerSize.height);
    upperRightCorner.append_attribute("height").set_value(cornerSize.height);
    lowerRightCorner.append_attribute("height").set_value(cornerSize.height);
    
    lowerLeftCorner.append_attribute("area_x0").set_value(areaLeft);
    upperLeftCorner.append_attribute("area_x0").set_value(areaLeft);
    upperRightCorner.append_attribute("area_x0").set_value(areaRight -
                                                           areaCornerSize.width);
    lowerRightCorner.append_attribute("area_x0").set_value(areaRight -
                                                           areaCornerSize.width);
    
    lowerLeftCorner.append_attribute("area_x1").set_value(areaLeft +
                                                          areaCornerSize.width);
    upperLeftCorner.append_attribute("area_x1").set_value(areaLeft +
                                                          areaCornerSize.width);
    upperRightCorner.append_attribute("area_x1").set_value(areaRight);
    lowerRightCorner.append_attribute("area_x1").set_value(areaRight);
    
    lowerLeftCorner.append_attribute("area_y0").set_value(areaBottom -
                                                          areaCornerSize.height);
    upperLeftCorner.append_attribute("area_y0").set_value(areaTop);
    upperRightCorner.append_attribute("area_y0").set_value(areaTop);
    lowerRightCorner.append_attribute("area_y0").set_value(areaBottom -
                                                           areaCornerSize.height);
    
    lowerLeftCorner.append_attribute("area_y1").set_value(areaBottom);
    upperLeftCorner.append_attribute("area_y1").set_value(areaTop +
                                                          areaCornerSize.height);
    upperRightCorner.append_attribute("area_y1").set_value(areaTop +
                                                           areaCornerSize.height);
    lowerRightCorner.append_attribute("area_y1").set_value(areaBottom);
    
    lowerLeftCorner.append_attribute("transparent").set_value(1.0f);
    upperLeftCorner.append_attribute("transparent").set_value(1.0f);
    upperRightCorner.append_attribute("transparent").set_value(1.0f);
    lowerRightCorner.append_attribute("transparent").set_value(1.0f);
	
    left.append_attribute("name").set_value("left");
    top.append_attribute("name").set_value("top");
    right.append_attribute("name").set_value("right");
    bottom.append_attribute("name").set_value("bottom");
    
    left.append_attribute("left").set_value(panelLeft);
    top.append_attribute("left").set_value(panelLeft +
                                           cornerSize.width);
    right.append_attribute("left").set_value(panelRight -
                                             cornerSize.width);
    bottom.append_attribute("left").set_value(panelLeft +
                                              cornerSize.width);
    
    left.append_attribute("top").set_value(panelTop +
                                           cornerSize.height);
    top.append_attribute("top").set_value(panelTop);
    right.append_attribute("top").set_value(panelTop +
                                            cornerSize.height);
    bottom.append_attribute("top").set_value(panelBottom -
                                             cornerSize.height);
    
    left.append_attribute("width").set_value(cornerSize.width);
    top.append_attribute("width").set_value(panelRect.size.width -
                                            cornerSize.width -
                                            cornerSize.width);
    right.append_attribute("width").set_value(cornerSize.width);
    bottom.append_attribute("width").set_value(panelRect.size.width -
                                               cornerSize.width -
                                               cornerSize.width);
    
    left.append_attribute("height").set_value(panelRect.size.height -
                                              cornerSize.height -
                                              cornerSize.height);
    top.append_attribute("height").set_value(cornerSize.height);
    right.append_attribute("height").set_value(panelRect.size.height -
                                               cornerSize.height -
                                               cornerSize.height);
    bottom.append_attribute("height").set_value(cornerSize.height);
    
    left.append_attribute("area_x0").set_value(areaLeft);
    top.append_attribute("area_x0").set_value(areaLeft +
                                              areaCornerSize.width);
    right.append_attribute("area_x0").set_value(areaRight -
                                                areaCornerSize.width);
    bottom.append_attribute("area_x0").set_value(areaLeft +
                                                 areaCornerSize.width);
    
    left.append_attribute("area_x1").set_value(areaLeft +
                                               areaCornerSize.width);
    top.append_attribute("area_x1").set_value(areaRight -
                                              areaCornerSize.width);
    right.append_attribute("area_x1").set_value(areaRight);
    bottom.append_attribute("area_x1").set_value(areaRight -
                                                 areaCornerSize.width);
    
    left.append_attribute("area_y0").set_value(areaTop +
                                               areaCornerSize.height);
    top.append_attribute("area_y0").set_value(areaTop);
    right.append_attribute("area_y0").set_value(areaTop +
                                                areaCornerSize.height);
    bottom.append_attribute("area_y0").set_value(areaBottom -
                                                 areaCornerSize.height);
    
    left.append_attribute("area_y1").set_value(areaBottom -
                                               areaCornerSize.height);
    top.append_attribute("area_y1").set_value(areaTop +
                                              areaCornerSize.height);
    right.append_attribute("area_y1").set_value(areaBottom -
                                                areaCornerSize.height);
    bottom.append_attribute("area_y1").set_value(areaBottom);
    
    left.append_attribute("transparent").set_value(1.0f);
    top.append_attribute("transparent").set_value(1.0f);
    right.append_attribute("transparent").set_value(1.0f);
    bottom.append_attribute("transparent").set_value(1.0f);
	
    center.append_attribute("name").set_value("center");
    
    center.append_attribute("left").set_value(panelLeft + cornerSize.width);
    center.append_attribute("top").set_value(panelTop + cornerSize.height);
    center.append_attribute("width").set_value(panelRect.size.width -
                                               cornerSize.width -
                                               cornerSize.width);
    center.append_attribute("height").set_value(panelRect.size.height -
                                                cornerSize.height -
                                                cornerSize.height);
    
    center.append_attribute("area_x0").set_value(areaLeft +
                                                 areaCornerSize.width);
    center.append_attribute("area_x1").set_value(areaRight -
                                                 areaCornerSize.width);
    center.append_attribute("area_y0").set_value(areaTop +
                                                 areaCornerSize.height);
    center.append_attribute("area_y1").set_value(areaBottom -
                                                 areaCornerSize.height);
    
    center.append_attribute("transparent").set_value(1.0f);
}
///**********************************************************************///
///                       class implement end                            ///
///**********************************************************************///