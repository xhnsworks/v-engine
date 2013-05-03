#ifndef SPRITE_H
#define SPRITE_H
#include "common.h"
#include "etypes.h"
#include "mesh.h"
#include "xhn_list.hpp"
#include "xhn_map.hpp"
#include "xhn_set.hpp"
#include "xhn_vector.hpp"
#include "xhn_static_string.hpp"
#include "xhn_smart_ptr.hpp"
#include "sprite_geom_buffer.h"
#include "rtti.hpp"
///#include "mxml.h"
#include "matrix4x4.h"
#include "sprite_event.h"
#include "sprite_factory.h"
#include "font_renderer.h"
#include "color.h"
namespace pugi
{
    class xml_node;
}

class SpriteRenderer;
struct FourBorders;
struct SpriteRect
{
	float left;
	float top;
	float width;
	float height;
	SpriteRect()
		: left(0.0f)
		, top(0.0f)
		, width(32.0f)
		, height(32.0f)
	{}
	SpriteRect(float x, float y, float w, float h)
		: left(x)
		, top(y)
		, width(w)
		, height(h)
	{}
	SpriteRect(const SpriteRect& rc)
		: left(rc.left)
		, top(rc.top)
		, width(rc.width)
		, height(rc.height)
	{}
	inline SpriteRect& operator = (const SpriteRect& rc) {
		left = rc.left;
		top = rc.top;
		width = rc.width;
		height = rc.height;
		return *this;
	}
	inline void Merge(const SpriteRect& rc) {
		float right = left + width;
		float bottom = top + height;
		float rcRight = rc.left + rc.width;
		float rcBottom = rc.top + rc.height;

		left = left < rc.left ? left : rc.left;
		top = top < rc.top ? top : rc.top;
		right = right > rcRight ? right : rcRight;
		bottom = bottom > rcBottom ? bottom : rcBottom;

		width = right - left;
		height = bottom - top;
	}
	void GetFourBorders(SpriteRenderer* renderer, FourBorders& borders);
};
class SpriteElement
{
public:
    xhn::static_string m_filename;
    SpriteRect m_rect;
    float m_area_x0;
    float m_area_x1;
    float m_area_y0;
    float m_area_y1;
	EColor m_color_u0v0;
	EColor m_color_u1v0;
	EColor m_color_u1v1;
	EColor m_color_u0v1;
	float m_transparent;
	matrix4x4 m_transform;
	SpriteElement()
        : m_filename("default")
        , m_area_x0(0.0f)
        , m_area_x1(32.0f)
        , m_area_y0(0.0f)
        , m_area_y1(32.0f)
		, m_transparent(1.0f)
    {}
    SpriteElement(float x, float y, float w, float h)
        : m_filename("default")
        , m_rect(x, y, w, h)
		, m_area_x0(0.0f)
		, m_area_x1(32.0f)
		, m_area_y0(0.0f)
		, m_area_y1(32.0f)
		, m_transparent(1.0f)
    {}
    SpriteElement(const SpriteElement& e)
        : m_filename(e.m_filename)
        , m_rect(e.m_rect)
		, m_area_x0(e.m_area_x0)
		, m_area_x1(e.m_area_x1)
		, m_area_y0(e.m_area_y0)
		, m_area_y1(e.m_area_y1)
		, m_color_u0v0(e.m_color_u0v0)
		, m_color_u1v0(e.m_color_u1v0)
		, m_color_u1v1(e.m_color_u1v1)
		, m_color_u0v1(e.m_color_u0v1)
		, m_transform(e.m_transform)
		, m_transparent(e.m_transparent)
    {}
    inline SpriteElement& operator = (const SpriteElement& e)
    {
        m_filename = e.m_filename;
        m_rect = e.m_rect;
        m_area_x0 = e.m_area_x0;
        m_area_x1 = e.m_area_x1;
        m_area_y0 = e.m_area_y0;
        m_area_y1 = e.m_area_y1;
		m_color_u0v0 = e.m_color_u0v0;
		m_color_u1v0 = e.m_color_u1v0;
		m_color_u1v1 = e.m_color_u1v1;
	    m_color_u0v1 = e.m_color_u0v1;
		m_transform = e.m_transform;
		m_transparent = e.m_transparent;
        return *this;
    }
    inline void SetFilename(const xhn::static_string& filename)
    {
        m_filename = filename;
    }
	inline void SetTransparent(float t)
	{
		m_transparent = t;
	}
	void ApplyTransform(const matrix4x4* transform);
    Mesh Build(SpriteRenderer* sprite_renderer) const;
	///void GetFourBorders(SpriteRenderer* renderer, FourBorders& borders);
};

class SpriteLayer : public RefObject
{
	DeclareRootRTTI;
public:
	/// perhaps loop reference, must to check
	xhn::vector< xhn::SmartPtr< SpriteLayer, FSpriteDestProc> > m_children;
	SpriteLayer* m_parent;
	AttributeHandle m_transparentHandle;
private:
	xhn::static_string m_name;
public:
	SpriteLayer(const xhn::static_string& name);
	virtual ~SpriteLayer() {}
	virtual void LoadConfig(const pugi::xml_node& from) = 0;
	virtual void SaveConfig(pugi::xml_node& to) = 0;
	virtual void ApplyTransform(const matrix4x4* trans) = 0;
	virtual void BuildElements(xhn::list<SpriteElement>& to) = 0;
	virtual void Clear() = 0;
	inline const xhn::static_string& GetName() const {
		return m_name;
	}
	virtual void GetMatrix(matrix4x4* result) = 0;
	virtual void RegisterAnimAttrs(SpriteFactory::SpriteLayerAnimAttrMap& slaaMap, SpriteFactory::AnimAttrSpriteLayerMap& aaslMap) = 0;
	void SetTransparent(float t);
	void GetScope(SpriteRect& result);
	virtual void GetScopeImpl(SpriteRect& result) = 0;
};
typedef xhn::SmartPtr<SpriteLayer, FSpriteDestProc> SpriteLayerPtr;
class SpriteNormalLayer : public SpriteLayer
{
	DeclareRTTI;
protected:
	xhn::map<xhn::static_string, SpriteElement> m_elementBuffer;
public:
	SpriteNormalLayer(const xhn::static_string name)
		: SpriteLayer(name)
	{}
	virtual void LoadConfig(const pugi::xml_node& from);
	virtual void SaveConfig(pugi::xml_node& to);
	virtual void ApplyTransform(const matrix4x4* trans);
	virtual void BuildElements(xhn::list<SpriteElement>& to);
	virtual void Clear();
	inline void ClearBuffer() {
		m_elementBuffer.clear();
	}
	inline SpriteElement* NewElement(const char* name) {
		SpriteElement ele;
		xhn::pair<xhn::static_string, SpriteElement> p = xhn::make_pair(xhn::static_string(name), ele);
		xhn::map<xhn::static_string, SpriteElement>::iterator iter = m_elementBuffer.insert(p);
		if (iter != m_elementBuffer.end())
			return &iter->second;
		else
			return NULL;
	}
	inline SpriteElement* GetElement(const xhn::static_string& name) {
		xhn::map<xhn::static_string, SpriteElement>::iterator iter = m_elementBuffer.find(name);
		if (iter != m_elementBuffer.end())
			return &iter->second;
		else
			return NULL;
	}
	virtual void GetMatrix(matrix4x4* result) {
	}
	virtual void RegisterAnimAttrs(SpriteFactory::SpriteLayerAnimAttrMap& slaaMap, SpriteFactory::AnimAttrSpriteLayerMap& aaslMap) {}
	virtual void GetScopeImpl(SpriteRect& result);
};

typedef xhn::map< const RTTI*, xhn::set<SpriteEventProcPtr> > EventProcMap;
typedef xhn::list< SpriteElement > ElementList;
typedef xhn::vector< SpriteLayerPtr > SpriteLayerArray;
typedef xhn::set< const RTTI* > ReceiverSet;

class SpriteTextLayer : public SpriteLayer
{
	DeclareRTTI;
public:
	ComposingStick* m_composingStick;
	xhn::string m_text;
	xhn::vector<ComposingStick::GlyphHandle> m_letters;
	xhn::vector<SpriteElement> m_elementBuffer;
public:
	SpriteTextLayer(const xhn::static_string& name, ComposingStick* cs)
		: SpriteLayer(name)
		, m_composingStick(cs)
	{}
	virtual void LoadConfig(const pugi::xml_node& from);
	virtual void SaveConfig(pugi::xml_node& to) {}
	virtual void ApplyTransform(const matrix4x4* trans);
	virtual void BuildElements(xhn::list<SpriteElement>& to);
	virtual void Clear();
	virtual void GetMatrix(matrix4x4* result) {
	}
	virtual void RegisterAnimAttrs(SpriteFactory::SpriteLayerAnimAttrMap& slaaMap, SpriteFactory::AnimAttrSpriteLayerMap& aaslMap) {}
};

class Sprite : public SpriteLayer
{
	DeclareRTTI;
public:
    ElementList m_elements;
    EventProcMap m_publicEventProcs;
	EventProcMap m_privateEventProcs;
protected:
	AttributeHandle m_pivotHandle;
	AttributeHandle m_coordinateHandle;
	AttributeHandle m_rotationHandle;
	AttributeHandle m_scaleHandle;
public:
    SpriteRenderer* m_renderer;
public:
	Sprite(SpriteRenderer* renderer, const xhn::static_string name);
    virtual void Init() {}
    void LoadConfig(const char* configName);
    void SaveConfig(const char* configName);
    void RegisterPublicEventCallback(const RTTI* type, SpriteEventProcPtr proc);
    void PublicEventCallback(const SpriteEvent* evt);
    void AttachToGeomBuffer(SpriteGeomBufferPtr buffer);
	void SetCoord(float x, float y);
	void SetScale(float x, float y);
	void SetRotate(float rad);
	void BroadcastEventToBrothers(const SpriteEvent* evt);
	inline const EventProcMap& GetPublicEventProcMap() {
		return m_publicEventProcs;
	}
	inline EventProcMap& GetPrivateEventProcMap() {
		return m_privateEventProcs;
	}
	inline SpriteLayerPtr GetLayer(euint index) {
		SpriteLayerPtr ret;
		if (index < m_children.size()) {
			ret = m_children[index];
		}
		return ret;
	}
	inline void AddChild(SpriteLayerPtr spriteLayer) {
        m_children.push_back(spriteLayer);
		spriteLayer->m_parent = this;
	}
	inline AttributeHandle GetPivotHandle() {
		return m_pivotHandle;
	}
	inline AttributeHandle GetCoordinateHandle() {
		return m_coordinateHandle;
	}
	inline AttributeHandle GetRotationHandle() {
		return m_rotationHandle;
	}
	inline AttributeHandle GetScaleHandle() {
		return m_scaleHandle;
	}
public:
	virtual void LoadConfig(const pugi::xml_node& from);
	virtual void SaveConfig(pugi::xml_node& to);
	virtual void ApplyTransform(const matrix4x4* trans);
	virtual void BuildElements(xhn::list<SpriteElement>& to);
	virtual void Clear();
	virtual void GetMatrix(matrix4x4* result);
	virtual void Build();
	virtual void RegisterAnimAttrs(SpriteFactory::SpriteLayerAnimAttrMap& slaaMap, SpriteFactory::AnimAttrSpriteLayerMap& aaslMap);
	virtual void Tick(double elapsedTime) {}
	virtual void Tock() {}
};

#endif
