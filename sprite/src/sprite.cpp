#include "sprite_pch.h"
#include "sprite.h"
#include "sprite_geom_buffer.h"
#include "sprite_renderer.h"
#include "eassert.h"
#include "sfloat3.h"
#include "render_system.h"
#include "pugixml.hpp"
#include "clamp.h"

void SpriteRect::ApplyTransform(const matrix4x4* transform)
{
	/**
	sfloat4 ft4Pos = SFloat4(left, top, 0.0f, 1.0f);
    sfloat4 ft4Size = SFloat4(size.width, size.height, 0.0f, 1.0f);
	**/
	sfloat4 pos0 = SFloat4(left, top, 0.0f, 1.0f);
	sfloat4 pos1 = SFloat4(left, top + size.height, 0.0f, 1.0f);
	sfloat4 pos2 = SFloat4(left + size.width, top + size.height, 0.0f, 1.0f);
	sfloat4 pos3 = SFloat4(left + size.width, top, 0.0f, 1.0f);
	pos0 = Matrix4x4_mul_float4(transform, pos0);
	pos1 = Matrix4x4_mul_float4(transform, pos1);
	pos2 = Matrix4x4_mul_float4(transform, pos2);
	pos3 = Matrix4x4_mul_float4(transform, pos3);
	sfloat4 maxCoord = SFloat4_max(pos0, pos1);
	maxCoord = SFloat4_max(maxCoord, pos2);
	maxCoord = SFloat4_max(maxCoord, pos3);

	sfloat4 minCoord = SFloat4_min(pos0, pos1);
	minCoord = SFloat4_min(minCoord, pos2);
	minCoord = SFloat4_min(minCoord, pos3);

	left = SFloat4_get_x(&minCoord);
	top = SFloat4_get_y(&minCoord);
	size.width = SFloat4_get_x(&maxCoord) - SFloat4_get_x(&minCoord);
	size.height = SFloat4_get_y(&maxCoord) - SFloat4_get_y(&minCoord);
}

void SpriteRect::GetFourBorders(SpriteRenderer* renderer, FourBorders& borders)
{
	renderer->get_four_borders(left, top, size.width, size.height, borders);
}

void SpriteElement::SetTransform(const matrix4x4* transform)
{
    Matrix4x4_assign(&m_transform, transform);
}
Mesh SpriteElement::Build(SpriteRenderer* sprite_renderer) const
{
	if (m_transparency > 0.0f) {
		float u0, u1, v0, v1;
		u0 = u1 = v0 = v1 = 0.0f;
		Texture2DPtr tex = RenderSystem_get_texture2d(m_filename, "Texture");
		if (tex.get()) {
			float w = (float)tex->GetWidth();
			float h = (float)tex->GetHeight();
            /// adjust texcoord area, removal edge
			float area_x0 = clamp(m_area_x0 + 0.5f, 0.0f, w);
			float area_x1 = clamp(m_area_x1 - 0.5f, 0.0f, w);
			float area_y0 = clamp(m_area_y0 + 0.5f, 0.0f, h);
			float area_y1 = clamp(m_area_y1 - 0.5f, 0.0f, h);
			u0 = area_x0 / w;
			u1 = area_x1 / w;
			v0 = area_y0 / h;
			v1 = area_y1 / h;
		}
		EColor u0v0 = m_color_u0v0;
		EColor u1v0 = m_color_u1v0;
		EColor u1v1 = m_color_u1v1;
		EColor u0v1 = m_color_u0v1;
		u0v0.alpha *= m_transparency;
		u1v0.alpha *= m_transparency;
		u1v1.alpha *= m_transparency;
		u0v1.alpha *= m_transparency;
		Mesh ret = sprite_renderer->new_widget_mesh(m_rect.left, m_rect.top, m_rect.size.width, m_rect.size.height, u0, u1, v0, v1, 
			u0v0, u1v0, u1v1, u0v1);
		Mesh_apply_transform(ret, &m_transform);
		return ret;
	}
	else {
        Mesh ret = Mesh_new();
		return ret;
	}
}
bool SpriteElement::Trim(const SpriteRect& rect)
{
	float left = m_rect.left;
	float top = m_rect.top;
    float right = m_rect.left + m_rect.size.width;
	float bottom = m_rect.top + m_rect.size.height;
	float rectLeft = rect.left;
	float rectTop = rect.top;
	float rectRight = rect.left + rect.size.width;
	float rectBottom = rect.top + rect.size.height;
	float area_x0 = m_area_x0;
	float area_x1 = m_area_x1;
	float area_y0 = m_area_y0;
	float area_y1 = m_area_y1;
	EColor color_u0v0 = m_color_u0v0;
	EColor color_u1v0 = m_color_u1v0;
	EColor color_u1v1 = m_color_u1v1;
	EColor color_u0v1 = m_color_u0v1;
	if (right < rectLeft || top > rectBottom || left > rectRight || bottom < rectTop)
		return false;
	if (rectLeft > left) {
		m_area_x0 += ((rectLeft - left) / m_rect.size.width) * (area_x1 - area_x0);
		m_color_u0v0 += (color_u1v0 - color_u0v0) * ((rectLeft - left) / m_rect.size.width);
		m_color_u0v1 += (color_u1v1 - color_u0v1) * ((rectLeft - left) / m_rect.size.width);
		m_rect.left = rectLeft;
	}
	if (rectRight < right) {
		m_area_x1 -= ((right - rectRight) / m_rect.size.width) * (area_x1 - area_x0);
		m_color_u1v0 -= (color_u1v0 - color_u0v0) * ((rectLeft - left) / m_rect.size.width);
		m_color_u1v1 -= (color_u1v1 - color_u0v1) * ((rectLeft - left) / m_rect.size.width);
		m_rect.size.width = rectRight - m_rect.left;
	}
	if (rectTop > top) {
		m_area_y0 += ((rectTop - top) / m_rect.size.height) * (area_y1 - area_y0);
		m_color_u0v0 += (color_u0v1 - color_u0v0) * ((rectTop - top) / m_rect.size.height);
		m_color_u1v0 += (color_u1v1 - color_u1v0) * ((rectTop - top) / m_rect.size.height);
		m_rect.top = rectTop;
	}
	if (rectBottom < bottom) {
		m_area_y1 -= ((bottom - rectBottom) / m_rect.size.height) * (area_y1 - area_y0);
		m_color_u0v1 -= (color_u0v1 - color_u0v0) * ((rectBottom - bottom) / m_rect.size.height);
		m_color_u1v1 -= (color_u1v1 - color_u1v0) * ((rectBottom - bottom) / m_rect.size.height);
		m_rect.size.height = rectBottom - m_rect.top;
	}
	return true;
}

void SpriteElement::Test()
{
    SpriteElement ele(10.0f, 20.0f, 32.0f, 32.0f);
	ele.m_area_x0 = 10.0f;
	ele.m_area_x1 = 42.0f;
	ele.m_area_y0 = 20.0f;
	ele.m_area_y1 = 52.0f;
	ele.m_color_u0v0 = EColor(0.0f, 0.0f, 0.0f, 0.0f);
	ele.m_color_u1v0 = EColor(1.0f, 0.0f, 0.0f, 0.0f);
	ele.m_color_u0v1 = EColor(0.0f, 1.0f, 0.0f, 0.0f);
	ele.m_color_u1v1 = EColor(0.0f, 0.0f, 1.0f, 0.0f);
    SpriteRect rect0(5.0f, 30.0f, 45.0f, 10.0f);
	ele.Trim(rect0);
}

ImplementRootRTTI(SpriteLayer);
ImplementRTTI(SpriteNormalLayer, SpriteLayer);
ImplementRTTI(SpriteTextLayer, SpriteLayer);
ImplementRTTI(Sprite, SpriteLayer);

SpriteLayer::SpriteLayer(const xhn::static_string& name)
: m_name(name)
, m_horizontalAlignmentMode(NotHorizontalAligned)
, m_verticalAlignmentMode(NotVerticalAligned)
, m_parent(NULL)
{
	///m_transparentHandle.m_lock = ENEW xhn::RWLock;
	///m_transparentHandle.AttachAttribute<FloatAttr>();
	///FloatAttr* t = m_transparentHandle.GetAttribute<FloatAttr>();
	///t->x = 1.0f;
	FloatAttr t(1.0f);
	m_transparencyHandle.SetAttribute(&t);
}

void SpriteLayer::LoadConfig(const pugi::xml_node& from)
{
	SpriteLayerList::iterator iter = m_children.begin();
	for (; iter != m_children.end(); iter++)
	{
		SpriteLayerPtr layerPtr = *iter;
		const xhn::static_string& name = layerPtr->GetName();
		pugi::xml_node node = from.child(name.c_str());
		if (node) {
			layerPtr->LoadConfigImpl(node);
		}
	}
}
void SpriteLayer::SaveConfig(pugi::xml_node& to)
{
	/// nothing
}

void SpriteLayer::BuildElements(xhn::list<SpriteElement>& to)
{
    SpriteLayerList::iterator iter = m_children.begin();
    SpriteLayerList::iterator end = m_children.end();
    for (; iter != end; iter++) {
		xhn::list<SpriteElement> buf;
        SpriteLayerPtr& sptLayer = *iter;
        sptLayer->BuildElementsImpl(to);
    }
}

void SpriteLayer::SetTransparency(float t)
{
    ///xhn::RWLock::Instance inst = m_transparentHandle.GetWriteLock();
	///FloatAttr* transparent = m_transparentHandle.GetAttribute<FloatAttr>();
	///transparent->x = t;
	FloatAttr transparency(t);
    m_transparencyHandle.SetAttribute(&transparency);
}

void SpriteLayer::GetScope(SpriteRect& result)
{
	GetScopeImpl(result);
	SpriteLayerList::iterator iter = m_children.begin();
	SpriteLayerList::iterator end = m_children.end();
	///euint size = m_children.size();
	for (; iter != end; iter++) {
		SpriteRect rc;
		(*iter)->GetScope(rc);
		result.Merge(rc);
	}
}
void SpriteLayer::BroadcastEventToBrothers(const SpriteEvent* evt)
{
	if (m_parent) {
		SpriteLayerList::iterator iter = m_parent->m_children.begin();
		SpriteLayerList::iterator end = m_parent->m_children.end();
		for (; iter != end; iter++) {
			Sprite* sp = (*iter)->DynamicCast<Sprite>();
			if (sp && sp != this) {
				EventProcMap& epm = sp->GetPrivateEventProcMap();
				const RTTI* rtti = evt->GetRTTI();
				xhn::map< const RTTI*, xhn::set<SpriteEventProcPtr> >::iterator epmIter = epm.find(rtti);
				if (epmIter != epm.end()) {
					xhn::set<SpriteEventProcPtr>& sepSet = epmIter->second;
					xhn::set<SpriteEventProcPtr>::iterator i = sepSet.begin();
					xhn::set<SpriteEventProcPtr>::iterator e = sepSet.end();
					for (; i != e; i++) {
						SpriteEventProcPtr sep = *i;
						sep->Proc(evt);
					}
				}
			}
		}
	}
}

void SpriteLayer::GetMatrix(matrix4x4* result)
{
    if (m_parent) {
        m_parent->GetMatrix(result);
    }
    else {
        Matrix4x4_set_one(result);
    }

	if ((m_horizontalAlignmentMode != NotHorizontalAligned ||
		m_verticalAlignmentMode != NotVerticalAligned) &&
		m_parent) {
		/// calculate the coordinates of the center
		SpriteRect parentScope;
		m_parent->GetScope(parentScope);

		SpriteRect scope;
		GetScope(scope);
		float x = 0.0f;
		float y = 0.0f;

		if (m_horizontalAlignmentMode == CenterHorizontalAligned) {
			x = -scope.size.width * 0.5f + parentScope.size.width * 0.5f;
		}
		else if (m_horizontalAlignmentMode == LeftHorizontalAligned) {
			x = 0.0f;
		}
		else if (m_horizontalAlignmentMode == RightHorizontalAligned) {
			x = parentScope.size.width - scope.size.width;
		}
		else if (m_horizontalAlignmentMode == NotHorizontalAligned) {
			x = 0.0f;
		}

		if (m_verticalAlignmentMode == CenterVerticalAligned) {
			y = -scope.size.height * 0.5f + parentScope.size.height * 0.5f;
		}
		else if (m_verticalAlignmentMode == TopVerticalAligned) {
			y = 0.0f;
		}
		else if (m_verticalAlignmentMode == BottomVerticalAligned) {
			y = parentScope.size.height - scope.size.height;
		}
		else if (m_verticalAlignmentMode == NotVerticalAligned) {
			y = 0.0f;
		}
		matrix4x4 tran;
		Matrix4x4_set_as_translate(&tran, x, y, 0.0f);
		Matrix4x4_mul_matrix4(&tran, result, result);
	}
}
void SpriteLayer::RemoveChild(SpriteLayerPtr spriteLayer)
{
	SpriteLayerList::iterator iter = m_children.begin();
	SpriteLayerList::iterator end = m_children.end();
	for (; iter != end; iter++) {
		SpriteLayerPtr& sptLayerPtr = *iter;
		if (sptLayerPtr == spriteLayer) {
			m_children.remove(iter);
			return;
		}
	}
}
SpriteLayerPtr SpriteLayer::GetLayer(euint index) 
{
	SpriteLayerPtr ret;
	if (index < m_children.size()) {
		SpriteLayerList::iterator iter = m_children.begin();
		SpriteLayerList::iterator end = m_children.end();
		for (euint count = 0; iter != end; iter++, count++) {
			if (count == index)
				return *iter;
		}
	}
	return ret;
}
SpriteLayerPtr SpriteLayer::GetLayer(xhn::static_string layerName) 
{
	SpriteLayerPtr ret;
	SpriteLayerList::iterator iter = m_children.begin();
	SpriteLayerList::iterator end = m_children.end();
	for (; iter != end; iter++) {
		SpriteLayerPtr& sptLayerPtr = *iter;
		if (sptLayerPtr->GetName() == layerName)
			return sptLayerPtr;
	}
	return ret;
}
void SpriteLayer::AlwaysOnTop(SpriteLayerPtr layer)
{
	SpriteLayerList::iterator iter = m_children.begin();
	SpriteLayerList::iterator end = m_children.end();
	for (; iter != end; iter++) {
		SpriteLayerPtr& sptLayerPtr = *iter;
		if (sptLayerPtr == layer) {
			m_children.throw_front(iter);
			return;
		}
	}
}

void SpriteLayer::Tick(double elapsedTime)
{
    SpriteLayerList::iterator iter = m_children.begin();
	for (; iter != m_children.end(); iter++)
	{
		SpriteLayerPtr layerPtr = *iter;
		layerPtr->TickImpl(elapsedTime);
	}
}
void SpriteLayer::Tock()
{
    SpriteLayerList::iterator iter = m_children.begin();
	for (; iter != m_children.end(); iter++)
	{
		SpriteLayerPtr layerPtr = *iter;
		layerPtr->TockImpl();
	}
}

euint _ToUint(char c)
{
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	else if (c >= 'a' && c <= 'f')
	{
		return c - 'a' + 10;
	}
	else if (c >= 'A' && c <= 'F')
	{
		return c - 'A' + 10;
	}
	else
		return 0;
}
euint _ToUint(const xhn::string& str)
{
	euint ret = 0;
	int i = (int)(str.size() - 1);
	euint weight = 1;
	for (; i >= 0; i--)
	{
		char c = str[(euint)i];
		euint t = _ToUint(c);
		ret += t * weight;
		weight *= 16;
	}
	return ret;
}
EColor _ToColor(const xhn::string& str)
{
	EColor ret;
	xhn::string compStr[4];
	int i = (int)(str.size() - 1);
	euint j = 0;
	for (; i >= 0 && j < 8; i--, j++)
	{
		euint compIdx = j / 2;
		compStr[compIdx] += str[(euint)i];
	}
	ret.alpha = (float)(_ToUint(compStr[0])) / 255.0f;
	ret.blue = (float)(_ToUint(compStr[1])) / 255.0f;
	ret.green = (float)(_ToUint(compStr[2])) / 255.0f;
	ret.red = (float)(_ToUint(compStr[3])) / 255.0f;
	return ret;
}

void SpriteNormalLayer::LoadConfigImpl(const pugi::xml_node& from)
{
	Clear();
	pugi::xml_node eles = from.child("elements");
	pugi::xml_node::iterator iter = eles.begin();
	pugi::xml_node::iterator end = eles.end();
	for (; iter != end; iter++)
	{
		pugi::xml_node node = *iter;
		pugi::xml_attribute name = node.attribute("name");
		pugi::xml_attribute filename = node.attribute("filename");
		pugi::xml_attribute left = node.attribute("left");
		pugi::xml_attribute top = node.attribute("top");
		pugi::xml_attribute width = node.attribute("width");
		pugi::xml_attribute height = node.attribute("height");
		pugi::xml_attribute area_x0 = node.attribute("area_x0");
		pugi::xml_attribute area_x1 = node.attribute("area_x1");
		pugi::xml_attribute area_y0 = node.attribute("area_y0");
		pugi::xml_attribute area_y1 = node.attribute("area_y1");
		pugi::xml_attribute transparency = node.attribute("transparency");

		pugi::xml_attribute color = node.attribute("color");
		EColor eleColor;

		if (color) {
			xhn::string colorStr = color.value();
			eleColor = _ToColor(colorStr);
		}
		else {
			eleColor.red = 1.0f;
			eleColor.green = 1.0f;
			eleColor.blue = 1.0f;
			eleColor.alpha = 1.0f;
		}
		
		SpriteElement sprite_ele;
		sprite_ele.SetFilename(xhn::static_string(filename.value()));
		sprite_ele.m_rect.left = left.as_float();
		sprite_ele.m_rect.top = top.as_float();
		sprite_ele.m_rect.size.width = width.as_float();
		sprite_ele.m_rect.size.height = height.as_float();
		sprite_ele.m_area_x0 = area_x0.as_float();
		sprite_ele.m_area_x1 = area_x1.as_float();
		sprite_ele.m_area_y0 = area_y0.as_float();
		sprite_ele.m_area_y1 = area_y1.as_float();
		sprite_ele.m_color_u0v0 = eleColor;
		sprite_ele.m_color_u0v1 = eleColor;
		sprite_ele.m_color_u1v0 = eleColor;
		sprite_ele.m_color_u1v1 = eleColor;
		sprite_ele.m_transparency = transparency.as_float();
		m_elementBuffer.insert(xhn::make_pair(xhn::static_string(name.value()), sprite_ele));
	}
}

void SpriteNormalLayer::SaveConfigImpl(pugi::xml_node& to)
{
	pugi::xml_node eles = to.append_child("elements");
	pugi::xml_attribute num_elements = eles.append_attribute("num_elements");
	num_elements.set_value((unsigned int)m_elementBuffer.size());
	xhn::map<xhn::static_string, SpriteElement>::iterator iter = m_elementBuffer.begin();
	for (; iter != m_elementBuffer.end(); iter++)
	{
		pugi::xml_node ele = eles.append_child("element");
		///mxml_node_t* ele = mxmlNewElement(eles, "element");
		SpriteElement& sprite_ele = iter->second;
		ele.append_attribute("name").set_value(iter->first.c_str());
		ele.append_attribute("filename").set_value(sprite_ele.m_filename.c_str());
		ele.append_attribute("left").set_value(sprite_ele.m_rect.left);
		ele.append_attribute("top").set_value(sprite_ele.m_rect.top);
		ele.append_attribute("width").set_value(sprite_ele.m_rect.size.width);
		ele.append_attribute("height").set_value(sprite_ele.m_rect.size.height);
		ele.append_attribute("area_x0").set_value(sprite_ele.m_area_x0);
		ele.append_attribute("area_x1").set_value(sprite_ele.m_area_x1);
		ele.append_attribute("area_y0").set_value(sprite_ele.m_area_y0);
		ele.append_attribute("area_y1").set_value(sprite_ele.m_area_y1);
		ele.append_attribute("transparency").set_value(sprite_ele.m_transparency);
	}
}

void SpriteNormalLayer::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
    matrix4x4 transform;
    GetMatrix(&transform);
    xhn::map<xhn::static_string, SpriteElement>::iterator iter = m_elementBuffer.begin();
	for (; iter != m_elementBuffer.end(); iter++)
	{
		SpriteElement& ele = iter->second;
        ele.SetTransform(&transform);
		to.push_back(ele);
	}
}

void SpriteNormalLayer::Clear()
{
    m_elementBuffer.clear();
}

void SpriteNormalLayer::GetScopeImpl(SpriteRect& result)
{
	xhn::map<xhn::static_string, SpriteElement>::iterator iter = m_elementBuffer.begin();
	xhn::map<xhn::static_string, SpriteElement>::iterator end = m_elementBuffer.end();
	if (iter != end) {
		result = iter->second.m_rect;
		iter++;
	}
	for (; iter != end; iter++) {
        result.Merge(iter->second.m_rect);
	}
}

SpriteTextLayer::~SpriteTextLayer()
{
    Clear();
}

void SpriteTextLayer::SetText(const xhn::string& text, 
							  const EColor& color,
							  float interval,
							  float transparency,
							  PixelSize pixelSize)
{
	Clear();
	float x = 0.0f;
	float y = 0.0f;

	float maxHeight = 0.0f;

	float width = 0.0f;
	float height = 0.0f;
	m_text = text;
	xhn::wstring wtext = ComposingStick::Convert(m_text);

	for (euint i = 0; i < wtext.size(); i++)
	{
		wchar_t ch = wtext[i];
		if (ch == (wchar_t)'\n')
		{
			x = 0.0f;
			y += maxHeight;
			y += interval;
			height += maxHeight;
			continue;
		}
		else if (ch == (wchar_t)'\r')
			continue;
		else if (ch == (wchar_t)'\\') {
			if (i + 1 < wtext.size()) {
				if (wtext[i + 1] == (wchar_t)'r') {
					i++;
					continue;
				}
				else if (wtext[i + 1] == (wchar_t)'n') {
					i++;
					x = 0.0f;
					y += maxHeight;
					y += interval;
					height += maxHeight;
					continue;
				}
			}
		}

		ComposingStick::GlyphHandle handle = ComposingStickManager::Get()->AllocGlyph(ch, pixelSize);
		m_letters.push_back(handle);

		SpriteElement sprite_ele;
		sprite_ele.SetFilename(handle.GetFilename());
		sprite_ele.m_rect.left = x;
		sprite_ele.m_rect.top = y;
		sprite_ele.m_rect.size.width = (float)handle.GetGlyph()->width;
		sprite_ele.m_rect.size.height = (float)handle.GetGlyph()->height;

		if ((float)handle.GetGlyph()->height > maxHeight)
		{
			maxHeight = (float)handle.GetGlyph()->height;
		}

		sprite_ele.m_area_x0 = (float)handle.GetGlyph()->x;
		sprite_ele.m_area_x1 = (float)(handle.GetGlyph()->x + handle.GetGlyph()->width);
		sprite_ele.m_area_y0 = (float)handle.GetGlyph()->y;
		sprite_ele.m_area_y1 = (float)(handle.GetGlyph()->y + handle.GetGlyph()->height);
		sprite_ele.m_color_u0v0 = color;
		sprite_ele.m_color_u0v1 = color;
		sprite_ele.m_color_u1v0 = color;
		sprite_ele.m_color_u1v1 = color;
		sprite_ele.m_transparency = transparency;

		m_elementBuffer.push_back(sprite_ele);

		x += (float)handle.GetWidth();
		x += interval;

		if (x > width)
			width = x;
	}
	m_size.x = width;
	m_size.y = height + maxHeight;
}

void SpriteTextLayer::LoadConfigImpl(const pugi::xml_node& from)
{
	pugi::xml_node styles = from.child("styles");
	pugi::xml_node::iterator iter = styles.begin();
	pugi::xml_node::iterator end = styles.end();
	///for (; iter != end; iter++)
	if (iter != end)
	{
		pugi::xml_node node = *iter;
		///pugi::xml_attribute name = node.attribute("name");
		pugi::xml_attribute text = node.attribute("text");
		pugi::xml_attribute interval = node.attribute("interval");
		pugi::xml_attribute color = node.attribute("color");
		pugi::xml_attribute size = node.attribute("size");
		pugi::xml_attribute transparency = node.attribute("transparency");

		PixelSize pixelSize = Pixel30;
		if (size) {
			pixelSize = (PixelSize)size.as_uint();
		}

		xhn::string colorStr = color.value();
		EColor textColor = _ToColor(colorStr);

		xhn::string t = text.value();

		SetText(t, textColor, interval.as_float(), transparency.as_float(), pixelSize);
	}
}

void SpriteTextLayer::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
    matrix4x4 transform;
    GetMatrix(&transform);
	xhn::vector<SpriteElement>::iterator iter = m_elementBuffer.begin();
	for (; iter != m_elementBuffer.end(); iter++)
	{
		SpriteElement& ele = *iter;
        ele.SetTransform(&transform);
		to.push_back(ele);
	}
}

void SpriteTextLayer::Clear()
{
	m_letters.clear();
	m_elementBuffer.clear();
}

void SpriteTextLayer::GetScopeImpl(SpriteRect& result)
{
	result.left = 0.0f;
	result.top = 0.0f;
	result.size.width = m_size.x;
	result.size.height = m_size.y;
}

Sprite::Sprite(SpriteRenderer* renderer, const xhn::static_string name)
: m_renderer(renderer)
, SpriteLayer(name)
{
	//m_pivotHandle.m_attr = &m_pivot;
	m_pivotHandle.m_lock = ENEW xhn::RWLock;
	m_pivotHandle.AttachAttribute<Float2Attr>();
	///Float2Attr* pivot = m_pivotHandle.GetAttribute<Float2Attr>();
	///pivot->x = 0.0f;
	///pivot->y = 0.0f;
	Float2Attr pivot(0.0f, 0.0f);
	m_pivotHandle.SetAttribute(&pivot);

	///m_coordinateHandle.m_attr = &m_coordinate;
	m_coordinateHandle.m_lock = ENEW xhn::RWLock;
	m_coordinateHandle.AttachAttribute<Float2Attr>();
	///Float2Attr* coordinate = m_coordinateHandle.GetAttribute<Float2Attr>();
	///coordinate->x = 0.0f;
	///coordinate->y = 0.0f;
	Float2Attr coordinate(0.0f, 0.0f);
	m_coordinateHandle.SetAttribute(&coordinate);

	///m_rotationHandle.m_attr = &m_rotation;
	m_rotationHandle.m_lock = ENEW xhn::RWLock;
	m_rotationHandle.AttachAttribute<FloatAttr>();
	///FloatAttr* rotation = m_rotationHandle.GetAttribute<FloatAttr>();
	///rotation->x = 0.0f;
	FloatAttr rotation(0.0f);
	m_rotationHandle.SetAttribute(&rotation);

	///m_scaleHandle.m_attr = &m_scale;
	m_scaleHandle.m_lock = ENEW xhn::RWLock;
	m_scaleHandle.AttachAttribute<Float2Attr>();
	///Float2Attr* scale = m_scaleHandle.GetAttribute<Float2Attr>();
	///scale->x = 1.0f;
	///scale->y = 1.0f;
	Float2Attr scale(1.0f, 1.0f);
	m_scaleHandle.SetAttribute(&scale);
}

void Sprite::RegisterPublicEventCallback(const RTTI* type, SpriteEventProcPtr proc)
{
    m_publicEventProcs[type].insert(proc);
}
void Sprite::UnregisterPublicEventCallback(const RTTI* type, SpriteEventProcPtr proc)
{
	m_publicEventProcs[type].erase(proc);
}

void Sprite::PublicEventCallback(const SpriteEvent* evt)
{
	const RTTI* rtti = evt->GetRTTI();
	EventProcMap::iterator procSetIter = m_publicEventProcs.find(rtti);
	if (procSetIter != m_publicEventProcs.end())
	{
		xhn::set<SpriteEventProcPtr>& procs = procSetIter->second;
		xhn::set<SpriteEventProcPtr>::iterator procIter = procs.begin();
		for (; procIter != procs.end(); procIter++)
		{
			SpriteEventProcPtr proc = *procIter;
			proc->Proc(evt);
		}
	}
}

void Sprite::AttachToGeomBuffer(SpriteGeomBufferPtr buffer)
{
	ElementList::iterator iter = m_elements.begin();
	for (; iter != m_elements.end(); iter++)
	{
		SpriteElement& ele = *iter;
		Mesh mesh = ele.Build(m_renderer);
		buffer->Attach(ele.m_filename, mesh, ele.m_fourBorders);
	}
}

void Sprite::SetCoord(float x, float y)
{
	/**
	xhn::RWLock::Instance inst = m_coordinateHandle.m_lock->GetWriteLock();
	///m_coordinate.x = x;
	///m_coordinate.y = y;
	Float2Attr* coord = m_coordinateHandle.GetAttribute<Float2Attr>();
	coord->x = x;
	coord->y = y;
	**/
    Float2Attr coord(x, y);
	m_coordinateHandle.SetAttribute(&coord);
}

void Sprite::SetScale(float x, float y)
{
	/**
	xhn::RWLock::Instance inst = m_scaleHandle.m_lock->GetWriteLock();
	///m_scale.x = x;
	///m_scale.y = y;
	Float2Attr* scale = m_scaleHandle.GetAttribute<Float2Attr>();
	scale->x = x;
	scale->y = y;
	**/
    Float2Attr scale(x, y);
	m_scaleHandle.SetAttribute(&scale);
}

void Sprite::SetRotate(float rad)
{
	/**
	xhn::RWLock::Instance inst = m_rotationHandle.m_lock->GetWriteLock();
	///m_rotation = rad;
	FloatAttr* rotation = m_rotationHandle.GetAttribute<FloatAttr>();
	rotation->x = rad;
	**/
	FloatAttr rotation(rad);
	m_rotationHandle.SetAttribute(&rotation);
}

void Sprite::GetScopeImpl(SpriteRect& result)
{
    result.left = FLT_MIN;
    result.top = FLT_MIN;
    result.size.width = FLT_MIN;
    result.size.height = FLT_MIN;
}

void Sprite::Build()
{
	m_elements.clear();
	BuildElements(m_elements);
	BuildFourBorders();
}
void Sprite::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
    BuildElements(to);
	BuildFourBorders();
}

void Sprite::BuildFourBorders()
{
	SpriteRect rect;
	GetScope(rect);
	rect.GetFourBorders(m_renderer, m_fourBorders);
	matrix4x4 mat;
	Matrix4x4_set_one(&mat);
	GetMatrix(&mat);
	m_fourBorders.ApplyTranform(&mat);
}

void Sprite::Clear()
{
    /// nothing
}

void Sprite::LoadConfig(const char* configName)
{
	XMLResourcePtr res = RenderSystem_load_gui_config(configName);
	if (!res) {
		return;
	}
	pugi::xml_document& doc = res->GetDocument();
	pugi::xml_node root = doc.child("root");
	pugi::xml_node layers = root.child("layers");
    
    SpriteLayer::LoadConfig(layers);
}

void Sprite::SaveConfig(const char* configName)
{
    /// nothing
}

void Sprite::LoadConfigImpl(const pugi::xml_node& from)
{
    SpriteLayer::LoadConfig(from);
}
void Sprite::SaveConfigImpl(pugi::xml_node& to)
{
    SpriteLayer::SaveConfig(to);
}

void Sprite::GetMatrix(matrix4x4* result)
{
	matrix4x4 parentMatrix;
	Matrix4x4_set_one(&parentMatrix);
	if (m_parent) {
		m_parent->GetMatrix(&parentMatrix);
	}
	matrix4x4 offs;
	matrix4x4 inv_offs;
	matrix4x4 scal;
	matrix4x4 rota;
	matrix4x4 tran;
    matrix4x4 tmp;
	EFloat2 p = m_renderer->get_real_position(0.0f, 0.0f);
	{
		///xhn::RWLock::Instance inst = m_pivotHandle.m_lock->GetReadLock();
		///p = p + m_pivot;
		///Float2Attr* pivot = m_pivotHandle.GetAttribute<Float2Attr>();
		///p = p + (pivot->ToEFloat2());
		Float2Attr pivot;
		m_pivotHandle.GetAttribute(&pivot);
        p = p + (pivot.ToEFloat2());
	}
	sfloat3 axis = SFloat3(0.0f, 0.0f, 1.0f);
	Matrix4x4_set_as_translate(&offs, -p.x, -p.y, 0.0f);
	Matrix4x4_set_as_translate(&inv_offs, p.x, p.y, 0.0f);
	{
		/**
		xhn::RWLock::Instance inst = m_rotationHandle.m_lock->GetReadLock();
		FloatAttr* rotation = m_rotationHandle.GetAttribute<FloatAttr>();
		
		Matrix4x4_from_axis_angle(&rota, axis, rotation->Get());
		**/
        FloatAttr rotation;
		m_rotationHandle.GetAttribute(&rotation);
		Matrix4x4_from_axis_angle(&rota, axis, rotation.Get());
	}
	{
		/**
		xhn::RWLock::Instance inst = m_coordinateHandle.m_lock->GetReadLock();
		Float2Attr* coord = m_coordinateHandle.GetAttribute<Float2Attr>();
		
		Matrix4x4_set_as_translate(&tran, coord->x, coord->y, 0.0f);
		**/
		Float2Attr coord;
		m_coordinateHandle.GetAttribute(&coord);
		Matrix4x4_set_as_translate(&tran, coord.x, coord.y, 0.0f);
	}
	{
		/**
		xhn::RWLock::Instance inst = m_scaleHandle.m_lock->GetReadLock();
		Float2Attr* scale = m_scaleHandle.GetAttribute<Float2Attr>();
		
		Matrix4x4_set_scale(&scal, scale->x, scale->y, 1.0f);
		**/
        Float2Attr scale;
		m_scaleHandle.GetAttribute(&scale);
        Matrix4x4_set_scale(&scal, scale.x, scale.y, 1.0f);
	}
	Matrix4x4_mul_matrix4(&offs, &rota, &tmp);
	Matrix4x4_mul_matrix4(&tmp, &scal, &tmp);
	Matrix4x4_mul_matrix4(&tmp, &inv_offs, &tmp);
	Matrix4x4_mul_matrix4(&tmp, &tran, result);
    
    if ((m_horizontalAlignmentMode != NotHorizontalAligned ||
		 m_verticalAlignmentMode != NotVerticalAligned) &&
        m_parent) {
		/// calculate the coordinates of the center
		SpriteRect parentScope;
		m_parent->GetScopeImpl(parentScope);
        
        SpriteRect scope;
        GetScope(scope);
        scope.ApplyTransform(result);
		sfloat4 zero = SFloat4(0.0f, 0.0f, 0.0f, 1.0f);
		zero = Matrix4x4_mul_float4(result, zero);
		float x = 0.0f;
		float y = 0.0f;

		if (m_horizontalAlignmentMode == CenterHorizontalAligned) {
			x = -scope.size.width * 0.5f + parentScope.size.width * 0.5f +
            (SFloat4_get_x(&zero) - scope.left);
		}
		else if (m_horizontalAlignmentMode == LeftHorizontalAligned) {
			x = SFloat4_get_x(&zero) - scope.left;
		}
		else if (m_horizontalAlignmentMode == RightHorizontalAligned) {
			x = parentScope.size.width - scope.size.width +
            (SFloat4_get_x(&zero) - scope.left);
		}
		else if (m_horizontalAlignmentMode == NotHorizontalAligned) {
			/**
			xhn::RWLock::Instance inst = m_coordinateHandle.m_lock->GetReadLock();
			Float2Attr* coord = m_coordinateHandle.GetAttribute<Float2Attr>();
			x = coord->x;
			**/
			Float2Attr coord;
			m_coordinateHandle.GetAttribute(&coord);
			x = coord.x;
		}

		if (m_verticalAlignmentMode == CenterVerticalAligned) {
            y = -scope.size.height * 0.5f + parentScope.size.height * 0.5f +
            (SFloat4_get_y(&zero) - scope.top);
		}
		else if (m_verticalAlignmentMode == TopVerticalAligned) {
			y = SFloat4_get_y(&zero) - scope.top;
		}
		else if (m_verticalAlignmentMode == BottomVerticalAligned) {
			y = parentScope.size.height - scope.size.height +
            (SFloat4_get_y(&zero) - scope.top);
		}
		else if (m_verticalAlignmentMode == NotVerticalAligned) {
			/**
			xhn::RWLock::Instance inst = m_coordinateHandle.m_lock->GetReadLock();
			Float2Attr* coord = m_coordinateHandle.GetAttribute<Float2Attr>();
			y = coord->y;
			**/
			Float2Attr coord;
			m_coordinateHandle.GetAttribute(&coord);
			y = coord.y;
		}
        Matrix4x4_set_as_translate(&tran, x, y, 0.0f);
		Matrix4x4_mul_matrix4(&tmp, &tran, result);
    }
    
    Matrix4x4_mul_matrix4(result, &parentMatrix, result);
}

void Sprite::RegisterAnimAttrs(SpriteFactory::SpriteLayerAnimAttrMap& slaaMap, SpriteFactory::AnimAttrSpriteLayerMap& aaslMap)
{
	SpriteFactory::SpriteLayerAnimAttrMap::bucket* b = slaaMap.find_bucket(this);
	if (b)
		return;
	/**
	SpriteFactory::AnimAttrArray array;
	slaaMap.insert(this, array);
    b = slaaMap.find_bucket(this);
	{
		xhn::RWLock::Instance inst = b->get_write_lock();
		SpriteFactory::AnimAttrArray* a = b->find_unlock(this);
		a->push_back(m_pivotHandle.GetAttribute<Attribute>());
		a->push_back(m_coordinateHandle.GetAttribute<Attribute>());
		a->push_back(m_rotationHandle.GetAttribute<Attribute>());
		a->push_back(m_scaleHandle.GetAttribute<Attribute>());
	}

	aaslMap.insert(m_pivotHandle.GetAttribute<Attribute>(), this);
	aaslMap.insert(m_coordinateHandle.GetAttribute<Attribute>(), this);
	aaslMap.insert(m_rotationHandle.GetAttribute<Attribute>(), this);
	aaslMap.insert(m_scaleHandle.GetAttribute<Attribute>(), this);
	**/
}
