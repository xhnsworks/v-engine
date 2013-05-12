#include "sprite_pch.h"
#include "sprite.h"
#include "sprite_geom_buffer.h"
#include "sprite_renderer.h"
#include "eassert.h"
#include "sfloat3.h"
#include "render_system.h"
#include "pugixml.hpp"
#include "clamp.h"

void SpriteRect::GetFourBorders(SpriteRenderer* renderer, FourBorders& borders)
{
	renderer->get_four_borders(left, top, size.width, size.height, borders);
}

void SpriteElement::ApplyTransform(const matrix4x4* transform)
{
    Matrix4x4_assign(&m_transform, transform);
}
Mesh SpriteElement::Build(SpriteRenderer* sprite_renderer) const
{
	if (m_transparent > 0.0f) {
		float u0, u1, v0, v1;
		u0 = u1 = v0 = v1 = 0.0f;
		Texture2DPtr tex = RenderSystem_get_texture2d(m_filename, "Texture");
		if (tex.get()) {
			float w = (float)tex->GetWidth();
			float h = (float)tex->GetHeight();
			float area_x0 = clamp(m_area_x0, 0.0f, w);
			float area_x1 = clamp(m_area_x1, 0.0f, w);
			float area_y0 = clamp(m_area_y0, 0.0f, h);
			float area_y1 = clamp(m_area_y1, 0.0f, h);
			u0 = area_x0 / w;
			u1 = area_x1 / w;
			v0 = area_y0 / h;
			v1 = area_y1 / h;
		}
		EColor u0v0 = m_color_u0v0;
		EColor u1v0 = m_color_u1v0;
		EColor u1v1 = m_color_u1v1;
		EColor u0v1 = m_color_u0v1;
		u0v0.alpha *= m_transparent;
		u1v0.alpha *= m_transparent;
		u1v1.alpha *= m_transparent;
		u0v1.alpha *= m_transparent;
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

ImplementRootRTTI(SpriteLayer);
ImplementRTTI(SpriteNormalLayer, SpriteLayer);
ImplementRTTI(SpriteTextLayer, SpriteLayer);
ImplementRTTI(Sprite, SpriteLayer);

SpriteLayer::SpriteLayer(const xhn::static_string& name)
: m_name(name)
, m_parent(NULL)
{
	m_transparentHandle.m_lock = ENEW xhn::RWLock;
	m_transparentHandle.AttachAttribute<EFloat>();
	EFloat* t = (EFloat*)m_transparentHandle.GetAttribute();
	t->x = 1.0f;
}

void SpriteLayer::LoadConfig(const pugi::xml_node& from)
{
	xhn::vector<SpriteLayerPtr>::iterator iter = m_children.begin();
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
    SpriteList::iterator iter = m_children.begin();
    SpriteList::iterator end = m_children.end();
    for (; iter != end; iter++) {
        SpriteLayerPtr& sptLayer = *iter;
        sptLayer->BuildElementsImpl(to);
    }
}

void SpriteLayer::SetTransparent(float t)
{
    xhn::RWLock::Instance inst = m_transparentHandle.GetWriteLock();
	EFloat* transparent = (EFloat*)m_transparentHandle.GetAttribute();
	transparent->x = t;
}

void SpriteLayer::GetScope(SpriteRect& result)
{
	GetScopeImpl(result);
	xhn::vector< xhn::SmartPtr< SpriteLayer, FSpriteDestProc> >::iterator iter = m_children.begin();
	xhn::vector< xhn::SmartPtr< SpriteLayer, FSpriteDestProc> >::iterator end = m_children.end();
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
		SpriteList::iterator iter = m_parent->m_children.begin();
		SpriteList::iterator end = m_parent->m_children.end();
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
}

void SpriteLayer::Tick(double elapsedTime)
{
    SpriteLayerArray::iterator iter = m_children.begin();
	for (; iter != m_children.end(); iter++)
	{
		SpriteLayerPtr layerPtr = *iter;
		layerPtr->TickImpl(elapsedTime);
	}
}
void SpriteLayer::Tock()
{
    SpriteLayerArray::iterator iter = m_children.begin();
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
		pugi::xml_attribute transparent = node.attribute("transparent");

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
		sprite_ele.m_transparent = transparent.as_float();
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
		ele.append_attribute("transparent").set_value(sprite_ele.m_transparent);
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
        ele.ApplyTransform(&transform);
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

void SpriteTextLayer::LoadConfigImpl(const pugi::xml_node& from)
{
	Clear();
	pugi::xml_node styles = from.child("styles");
	pugi::xml_node::iterator iter = styles.begin();
	pugi::xml_node::iterator end = styles.end();
	for (; iter != end; iter++)
	{
		pugi::xml_node node = *iter;
		///pugi::xml_attribute name = node.attribute("name");
		pugi::xml_attribute text = node.attribute("text");
		pugi::xml_attribute interval = node.attribute("interval");
		pugi::xml_attribute color = node.attribute("color");
		pugi::xml_attribute transparent = node.attribute("transparent");

		xhn::string colorStr = color.value();
		EColor textColor = _ToColor(colorStr);

		m_text = text.value();
		xhn::wstring wtext = ComposingStick::Convert(m_text);
		float x = 0.0f;
		float y = 0.0f;
		float maxHeight = 0.0f;
		for (euint i = 0; i < wtext.size(); i++)
		{
			wchar_t ch = wtext[i];
			if (ch == (wchar_t)'\n')
			{
				x = 0.0f;
				y += maxHeight;
				y += interval.as_float();
				continue;
			}
			else if (ch == (wchar_t)'\r')
				continue;

			ComposingStick::GlyphHandle handle = m_composingStick->AllocGlyph(ch);
			m_letters.push_back(handle);

			SpriteElement sprite_ele;
			sprite_ele.SetFilename(m_composingStick->GetFilename());
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
			sprite_ele.m_color_u0v0 = textColor;
			sprite_ele.m_color_u0v1 = textColor;
			sprite_ele.m_color_u1v0 = textColor;
			sprite_ele.m_color_u1v1 = textColor;
			sprite_ele.m_transparent = transparent.as_float();
			
			m_elementBuffer.push_back(sprite_ele);

			x += (float)handle.GetWidth();
			x += interval.as_float();
		}
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
        ele.ApplyTransform(&transform);
		to.push_back(ele);
	}
}

void SpriteTextLayer::Clear()
{
	m_letters.clear();
	m_elementBuffer.clear();
}

Sprite::Sprite(SpriteRenderer* renderer, const xhn::static_string name)
: m_renderer(renderer)
, SpriteLayer(name)
{
	//m_pivotHandle.m_attr = &m_pivot;
	m_pivotHandle.m_lock = ENEW xhn::RWLock;
	m_pivotHandle.AttachAttribute<EFloat2>();
	EFloat2* pivot = (EFloat2*)m_pivotHandle.GetAttribute();
	pivot->x = 0.0f;
	pivot->y = 0.0f;

	///m_coordinateHandle.m_attr = &m_coordinate;
	m_coordinateHandle.m_lock = ENEW xhn::RWLock;
	m_coordinateHandle.AttachAttribute<EFloat2>();
	EFloat2* coordinate = (EFloat2*)m_coordinateHandle.GetAttribute();
	coordinate->x = 0.0f;
	coordinate->y = 0.0f;

	///m_rotationHandle.m_attr = &m_rotation;
	m_rotationHandle.m_lock = ENEW xhn::RWLock;
	m_rotationHandle.AttachAttribute<EFloat>();
	EFloat* rotation = (EFloat*)m_rotationHandle.GetAttribute();
	rotation->x = 0.0f;

	///m_scaleHandle.m_attr = &m_scale;
	m_scaleHandle.m_lock = ENEW xhn::RWLock;
	m_scaleHandle.AttachAttribute<EFloat2>();
	EFloat2* scale = (EFloat2*)m_scaleHandle.GetAttribute();
	scale->x = 1.0f;
	scale->y = 1.0f;
}

void Sprite::RegisterPublicEventCallback(const RTTI* type, SpriteEventProcPtr proc)
{
    m_publicEventProcs[type].insert(proc);
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
void Sprite::Build()
{
	m_elements.clear();
    BuildElements(m_elements);
}
void Sprite::AttachToGeomBuffer(SpriteGeomBufferPtr buffer)
{
	ElementList::iterator iter = m_elements.begin();
	for (; iter != m_elements.end(); iter++)
	{
		SpriteElement& ele = *iter;
		Mesh mesh = ele.Build(m_renderer);
		buffer->Attach(ele.m_filename, mesh);
	}
}

void Sprite::SetCoord(float x, float y)
{
	xhn::RWLock::Instance inst = m_coordinateHandle.m_lock->GetWriteLock();
	///m_coordinate.x = x;
	///m_coordinate.y = y;
	EFloat2* coord = (EFloat2*)m_coordinateHandle.GetAttribute();
	coord->x = x;
	coord->y = y;
}

void Sprite::SetScale(float x, float y)
{
	xhn::RWLock::Instance inst = m_scaleHandle.m_lock->GetWriteLock();
	///m_scale.x = x;
	///m_scale.y = y;
	EFloat2* scale = (EFloat2*)m_scaleHandle.GetAttribute();
	scale->x = x;
	scale->y = y;
}

void Sprite::SetRotate(float rad)
{
	xhn::RWLock::Instance inst = m_rotationHandle.m_lock->GetWriteLock();
	///m_rotation = rad;
	EFloat* rotation = (EFloat*)m_rotationHandle.GetAttribute();
	rotation->x = rad;
}


void Sprite::BuildElementsImpl(xhn::list<SpriteElement>& to)
{
    BuildElements(to);
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
	EFloat2 p = m_renderer->get_real_position(0.0f, 0.0f);
	{
		xhn::RWLock::Instance inst = m_pivotHandle.m_lock->GetReadLock();
		///p = p + m_pivot;
		EFloat2* pivot = (EFloat2*)m_pivotHandle.GetAttribute();
		p = p + (*pivot);
	}
	sfloat3 axis = SFloat3(0.0f, 0.0f, 1.0f);
	Matrix4x4_set_as_translate(&offs, -p.x, -p.y, 0.0f);
	Matrix4x4_set_as_translate(&inv_offs, p.x, p.y, 0.0f);
	{
		xhn::RWLock::Instance inst = m_rotationHandle.m_lock->GetReadLock();
		EFloat* rotation = (EFloat*)m_rotationHandle.GetAttribute();
		///Matrix4x4_from_axis_angle(&rota, axis, m_rotation.Get());
		Matrix4x4_from_axis_angle(&rota, axis, rotation->Get());
	}
	{
		xhn::RWLock::Instance inst = m_coordinateHandle.m_lock->GetReadLock();
		EFloat2* coord = (EFloat2*)m_coordinateHandle.GetAttribute();
		///Matrix4x4_set_as_translate(&tran, m_coordinate.x, m_coordinate.y, 0.0f);
		Matrix4x4_set_as_translate(&tran, coord->x, coord->y, 0.0f);
	}
	{
		xhn::RWLock::Instance inst = m_scaleHandle.m_lock->GetReadLock();
		EFloat2* scale = (EFloat2*)m_scaleHandle.GetAttribute();
		///Matrix4x4_set_scale(&scal, m_scale.x, m_scale.y, 1.0f);
		Matrix4x4_set_scale(&scal, scale->x, scale->y, 1.0f);
	}
	Matrix4x4_mul_matrix4(&offs, &rota, result);
	Matrix4x4_mul_matrix4(result, &scal, result);
	Matrix4x4_mul_matrix4(result, &inv_offs, result);
	Matrix4x4_mul_matrix4(result, &tran, result);
	
	Matrix4x4_mul_matrix4(result, &parentMatrix, result);
}

void Sprite::RegisterAnimAttrs(SpriteFactory::SpriteLayerAnimAttrMap& slaaMap, SpriteFactory::AnimAttrSpriteLayerMap& aaslMap)
{
	SpriteFactory::SpriteLayerAnimAttrMap::bucket* b = slaaMap.find_bucket(this);
	if (b)
		return;
	SpriteFactory::AnimAttrArray array;
	slaaMap.insert(this, array);
    b = slaaMap.find_bucket(this);
	{
		xhn::RWLock::Instance inst = b->get_write_lock();
		SpriteFactory::AnimAttrArray* a = b->find_unlock(this);
		a->push_back(m_pivotHandle.GetAttribute());
		a->push_back(m_coordinateHandle.GetAttribute());
		a->push_back(m_rotationHandle.GetAttribute());
		a->push_back(m_scaleHandle.GetAttribute());
	}

	aaslMap.insert(m_pivotHandle.GetAttribute(), this);
	aaslMap.insert(m_coordinateHandle.GetAttribute(), this);
	aaslMap.insert(m_rotationHandle.GetAttribute(), this);
	aaslMap.insert(m_scaleHandle.GetAttribute(), this);
}
