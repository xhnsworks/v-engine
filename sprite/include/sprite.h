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
#include "sprite_renderer.h"
namespace pugi
{
    class xml_node;
}
///**********************************************************************///
///                       strcut define begin                            ///
///**********************************************************************///
class SpriteRenderer;
struct FourBorders;
struct SpriteSize
{
    float width;
	float height;
	SpriteSize()
		: width(32.0f)
		, height(32.0f)
	{}
    SpriteSize(float w, float h)
		: width(w)
		, height(h)
	{}
	SpriteSize(const SpriteSize& size)
		: width(size.width)
		, height(size.height)
	{}
	inline SpriteSize& operator = (const SpriteSize& size) {
		width = size.width;
		height = size.height;
		return *this;
	}
};
struct SpriteRect
{
	float left;
	float top;
	SpriteSize size;
	SpriteRect()
		: left(0.0f)
		, top(0.0f)
	{}
	SpriteRect(float x, float y, float w, float h)
		: left(x)
		, top(y)
		, size(w, h)
	{}
	SpriteRect(const SpriteRect& rc)
		: left(rc.left)
		, top(rc.top)
		, size(rc.size)
	{}
	inline SpriteRect& operator = (const SpriteRect& rc) {
		left = rc.left;
		top = rc.top;
		size = rc.size;
		return *this;
	}
	inline void Merge(const SpriteRect& rc) {
		float right = left + size.width;
		float bottom = top + size.height;
		float rcRight = rc.left + rc.size.width;
		float rcBottom = rc.top + rc.size.height;

		left = left < rc.left ? left : rc.left;
		top = top < rc.top ? top : rc.top;
		right = right > rcRight ? right : rcRight;
		bottom = bottom > rcBottom ? bottom : rcBottom;

		size.width = right - left;
		size.height = bottom - top;
	}
	void ApplyTransform(const matrix4x4* transform);
	void GetFourBorders(SpriteRenderer* renderer, FourBorders& borders);
};
///**********************************************************************///
///                       struct define end                              ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
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
	float m_transparency;
	matrix4x4 m_transform;
	const FourBorders* m_fourBorders;
	SpriteElement()
        : m_filename("default")
        , m_area_x0(0.0f)
        , m_area_x1(32.0f)
        , m_area_y0(0.0f)
        , m_area_y1(32.0f)
		, m_transparency(1.0f)
		, m_fourBorders(NULL)
    {}
    SpriteElement(float x, float y, float w, float h)
        : m_filename("default")
        , m_rect(x, y, w, h)
		, m_area_x0(0.0f)
		, m_area_x1(32.0f)
		, m_area_y0(0.0f)
		, m_area_y1(32.0f)
		, m_transparency(1.0f)
		, m_fourBorders(NULL)
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
		, m_transparency(e.m_transparency)
		, m_fourBorders(e.m_fourBorders)
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
		m_transparency = e.m_transparency;
		m_fourBorders = e.m_fourBorders;
        return *this;
    }
    inline void SetFilename(const xhn::static_string& filename)
    {
        m_filename = filename;
    }
	inline void SetTransparency(float t)
	{
		m_transparency = t;
	}
	void SetTransform(const matrix4x4* transform);
    Mesh Build(SpriteRenderer* sprite_renderer) const;
    bool Trim(const SpriteRect& rect);
	static void Test();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class SpriteLayer;
typedef xhn::SmartPtr<SpriteLayer, FSpriteDestProc> SpriteLayerPtr;
typedef xhn::list< SpriteLayerPtr > SpriteLayerList;
class SpriteLayer : public RefObject
{
	DeclareRootRTTI;
    friend struct FSpriteDestProc;
public:
	AttributeHandle m_transparencyHandle;
    SpriteLayer* m_parent;
protected:
	/// perhaps loop reference, must to check
	SpriteLayerList m_children;
private:
	xhn::static_string m_name;
public:
	enum HorizontalAlignmentMode
	{
		NotHorizontalAligned,
		LeftHorizontalAligned,
		RightHorizontalAligned,
		CenterHorizontalAligned,
	};
	enum VerticalAlignmentMode
	{
		NotVerticalAligned,
		TopVerticalAligned,
		BottomVerticalAligned,
		CenterVerticalAligned,
	};
public:
	HorizontalAlignmentMode m_horizontalAlignmentMode;
	VerticalAlignmentMode m_verticalAlignmentMode;
public:
	SpriteLayer(const xhn::static_string& name);
	virtual ~SpriteLayer();
    void LoadConfig(const pugi::xml_node& from);
	void SaveConfig(pugi::xml_node& to);
    void BuildElements(xhn::list<SpriteElement>& to);
    void Tick(double elapsedTime);
    void Tock();
    void BroadcastEventToBrothers(const SpriteEvent* evt);
    void SetTransparency(float t);
	virtual void GetScope(SpriteRect& result);
    virtual void GetMatrix(matrix4x4* result);
	
    inline const xhn::static_string& GetName() const {
		return m_name;
	}
    inline void AddChild(SpriteLayerPtr spriteLayer) {
        m_children.push_back(spriteLayer);
		spriteLayer->m_parent = this;
	}
	inline void RemoveAllChildren() {
		m_children.clear();
	}
	inline SpriteLayer* GetParent() {
		return m_parent;
	}
	void RemoveChild(SpriteLayerPtr spriteLayer);
    SpriteLayerPtr GetLayer(euint index);
    SpriteLayerPtr GetLayer(xhn::static_string layerName);
	void AlwaysOnTop(SpriteLayerPtr layer);
    
    virtual void BuildElementsImpl(xhn::list<SpriteElement>& to) = 0;
    virtual void GetScopeImpl(SpriteRect& result) = 0;
    virtual void TickImpl(double elapsedTime) = 0;
    virtual void TockImpl() = 0;
    virtual void LoadConfigImpl(const pugi::xml_node& from) = 0;
	virtual void SaveConfigImpl(pugi::xml_node& to) = 0;
    virtual void Build() = 0;
	virtual void Clear() = 0;
    virtual void RegisterAnimAttrs(SpriteFactory::SpriteLayerAnimAttrMap& slaaMap,
                                   SpriteFactory::AnimAttrSpriteLayerMap& aaslMap) = 0;
};

///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
typedef xhn::map<xhn::static_string, SpriteElement> SpriteElementMap;
class SpriteNormalLayer : public SpriteLayer
{
	DeclareRTTI;
protected:
	SpriteElementMap m_elementBuffer;
public:
	SpriteNormalLayer(const xhn::static_string name)
		: SpriteLayer(name)
	{}
	virtual void Clear();
	inline SpriteElement* NewElement(const char* name) {
		SpriteElement ele;
        xhn::static_string staticName = name;
		SpriteElementMap::iterator iter =
        m_elementBuffer.insert(xhn::make_pair(staticName, ele));
		if (iter != m_elementBuffer.end())
			return &iter->second;
		else
			return NULL;
	}
	inline SpriteElement* GetElement(const xhn::static_string& name) {
		SpriteElementMap::iterator iter = m_elementBuffer.find(name);
		if (iter != m_elementBuffer.end())
			return &iter->second;
		else
			return NULL;
	}
    virtual void LoadConfigImpl(const pugi::xml_node& from);
	virtual void SaveConfigImpl(pugi::xml_node& to);
	virtual void BuildElementsImpl(xhn::list<SpriteElement>& to);
    virtual void TickImpl(double elapsedTime) {}
    virtual void TockImpl() {}
	virtual void RegisterAnimAttrs(SpriteFactory::SpriteLayerAnimAttrMap& slaaMap,
                                   SpriteFactory::AnimAttrSpriteLayerMap& aaslMap)
    {}
	virtual void GetScopeImpl(SpriteRect& result);
    virtual void Build() {}
};

typedef xhn::map< const RTTI*, xhn::set<SpriteEventProcPtr> > EventProcMap;
typedef xhn::list< SpriteElement > ElementList;
typedef xhn::set< const RTTI* > ReceiverSet;

class SpriteTextLayer : public SpriteLayer
{
	DeclareRTTI;
public:
	xhn::string m_text;
	xhn::vector<ComposingStick::GlyphHandle> m_letters;
	xhn::vector<SpriteElement> m_elementBuffer;
	EFloat2 m_size;
public:
	SpriteTextLayer(const xhn::static_string& name)
		: SpriteLayer(name)
	{}
    virtual ~SpriteTextLayer();
	virtual void LoadConfigImpl(const pugi::xml_node& from);
	virtual void SaveConfigImpl(pugi::xml_node& to) {}
	void SetText(const xhn::string& text, 
		const EColor& color,
		float interval,
		float transparency,
		PixelSize pixelSize);
	inline xhn::string GetText() {
		return m_text;
	}
	virtual void BuildElementsImpl(xhn::list<SpriteElement>& to);
	virtual void Clear();
    virtual void TickImpl(double elapsedTime) {}
    virtual void TockImpl() {}
	virtual void GetScopeImpl(SpriteRect& result);
	virtual void RegisterAnimAttrs(SpriteFactory::SpriteLayerAnimAttrMap& slaaMap,
                                   SpriteFactory::AnimAttrSpriteLayerMap& aaslMap)
    {}
    virtual void Build() {}
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class Sprite : public SpriteLayer
{
	DeclareRTTI;
	friend class SpriteFactory;
protected:
	virtual ~Sprite() {}
public:
    ElementList m_elements;
    EventProcMap m_publicEventProcs;
	EventProcMap m_privateEventProcs;
protected:
	AttributeHandle m_pivotHandle;
	AttributeHandle m_coordinateHandle;
	AttributeHandle m_rotationHandle;
	AttributeHandle m_scaleHandle;
	FourBorders m_fourBorders;
public:
    SpriteRenderer* m_renderer;
public:
	Sprite(SpriteRenderer* renderer, const xhn::static_string name);
    virtual void Init() {}
    void LoadConfig(const char* configName);
    void SaveConfig(const char* configName);
    void RegisterPublicEventCallback(const RTTI* type,
                                     SpriteEventProcPtr proc);
	void UnregisterPublicEventCallback(const RTTI* type,
		                               SpriteEventProcPtr proc);
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
	inline const FourBorders& GetFourBorders() {
		return m_fourBorders;
	}
public:
    virtual void GetScopeImpl(SpriteRect& result);
	virtual void BuildElementsImpl(xhn::list<SpriteElement>& to);
    virtual void LoadConfigImpl(const pugi::xml_node& from);
	virtual void SaveConfigImpl(pugi::xml_node& to);
	virtual void BuildFourBorders();
    virtual void Build();
	virtual void Clear();
	virtual void GetMatrix(matrix4x4* result);
	virtual void RegisterAnimAttrs(SpriteFactory::SpriteLayerAnimAttrMap& slaaMap,
                                   SpriteFactory::AnimAttrSpriteLayerMap& aaslMap);
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif
