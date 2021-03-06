#ifndef RENDER_ROBOT_H
#define RENDER_ROBOT_H
///**********************************************************************///
///                           include begin                              ///
///**********************************************************************///
#include "common.h"
#include "etypes.h"
#include "robot.h"
#include "renderer.h"
#include "sprite_renderer.h"
#include "xhn_list.hpp"
#include "xhn_map.hpp"
#include "xhn_static_string.hpp"
#include "input_system.h"
#include "ray.h"
#include "line_utility.h"
#include "sfloat4.h"
#include "gui_panel.h"
#include "gui_button.h"
#include "gui_cursor.h"
#include "gui_panel.h"
#include "gui_edit.h"
#include "gui_bar.h"
#include "gui_container.h"
#include "gui_combo_box.h"
#include "input_robot.h"
///**********************************************************************///
///                           include end                                ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class ModifyAttrCommand : public RobotCommand
{
	DeclareRTTI;
public:
	Attribute* m_attr;
	Attribute* m_newValue;
	Attribute::Type m_attrType;
	ModifyAttrCommand(Attribute* attr,
                      Attribute* newValue,
                      Attribute::Type attrType)
		: m_attr(attr)
		, m_newValue(newValue)
		, m_attrType(attrType)
	{}
	~ModifyAttrCommand();
	/// test whether the robot rendering robot, and test attribute are valid
	virtual bool Test(Robot* rob);
	virtual void Do(Robot* rob, xhn::static_string sender);
};

///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       marco define begin                             ///
///**********************************************************************///
#define W_Width  1280
#define W_Height 720
///**********************************************************************///
///                       marco define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
template <typename RENDERER>
class RendererChainBase
{
protected:
	ViewportPtr m_viewport;
    typedef xhn::map<xhn::static_string, RENDERER*> RendererMap;
    typedef xhn::list<RENDERER*> RendererList;
	RendererMap m_rendererMap;
	RendererList m_rendererChain;
public:
	RendererChainBase()
	{}
	~RendererChainBase()
	{
		typename RendererList::iterator iter = m_rendererChain.begin();
		typename RendererList::iterator end = m_rendererChain.end();
		for (; iter != end; iter++)
		{
			RENDERER* rdr = *iter;
			delete rdr;
		}
	}
	virtual void Init() = 0;
	void AddRenderer(const xhn::static_string name)
	{
		if (m_rendererMap.find(name) == m_rendererMap.end()) {
			if (m_rendererChain.empty()) {
				RENDERER* rdr = ENEW RENDERER(m_viewport);
				m_rendererChain.push_back(rdr);
				m_rendererMap.insert(xhn::make_pair(name, rdr));
			}
			else {
				RENDERER* prevRdr = m_rendererChain.back();
				RENDERER* rdr = ENEW RENDERER(prevRdr);
				m_rendererChain.push_back(rdr);
				m_rendererMap.insert(xhn::make_pair(name, rdr));
			}
		}
	}
	void Render()
	{
		typename RendererList::iterator iter = m_rendererChain.begin();
		typename RendererList::iterator end = m_rendererChain.end();
		for (; iter != end; iter++)
		{
			RENDERER* rdr = *iter;
			rdr->render();
		}
	}
	RENDERER* GetRenderer(const xhn::static_string name)
	{
		typename RendererMap::iterator iter = m_rendererMap.find(name);
		if (iter != m_rendererMap.end()) {
			return iter->second;
		}
		else {
			return NULL;
		}
	}
};

class RendererChain : public RendererChainBase<Renderer>
{
public:
    virtual void Init();
};

class GUIRendererChain : public RendererChainBase<SpriteRenderer>
{
public:
    virtual void Init();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class SwapBuffersAction;
class InputAction;
class ResourceAction : public Action
{
	friend class LogicAction;
	DeclareRTTI;
private:
	sfloat4 p0;
    sfloat4 p1;
    sfloat4 p2;
    sfloat4 p3;

    sfloat4 p4;
    sfloat4 p5;
    sfloat4 p6;
    sfloat4 p7;

	Ray m_mouseRay;
    MaterialInstance* m_mat0;
    MaterialInstance* m_mat1;

    MaterialInstance* m_pureLightingMat;
    MaterialInstance* m_coverMat;

	Renderable m_locator;

	LineDrawer m_lineDrawer;

    MaterialInstance* m_guiMat;
    VertexDecl m_defaultVtxDec;
	RendererChain* m_rendererChain;
	GUIRendererChain* m_guiRendererChain;

	Matrix4x4 m_lightMatrix;
	EFloat3 m_lightPos;
	LightBase2 m_light;

    InputAction* m_inputAct;
#if defined(_WIN32) ||defined(_WIN64)
	HWND m_window;
    SwapBuffersAction* m_swpAct;
#endif
    
	GUIButtonFactory* m_buttonFactory;
	GUICursorFactory* m_cursorFactory;
	///GUIPanelFactory* m_panelFactory;
	GUIEditFactory* m_editFactory;
	///GUIHoriBarFactory* m_horiBarFactory;
	///GUIVertBarFactory* m_vertBarFactory;
    GUIContainerFactory* m_containerFactory;
	GUIComboBoxFactory* m_comboxBoxFactory;

	GUIButton* m_guiButton;
	GUICursor* m_guiCursor;
	///GUIPanel* m_guiPanel;
	GUIEdit* m_guiEdit;
	///GUIHoriBar* m_guiHoriBar;
    ///GUIVertBar* m_guiVertBar;
    GUIContainer* m_guiContainer;
	GUIComboBox* m_guiComboxBox;

	bool m_isInited;
public:
#if defined(_WIN32) || defined(_WIN64)
	ResourceAction(RendererChain* rdrChain,
                   GUIRendererChain* guiRdrChain,
                   SwapBuffersAction* swpAct,
                   HWND window)
    : m_mouseRay(NULL)
    , m_mat0(NULL)
    , m_mat1(NULL)
    , m_pureLightingMat(NULL)
    , m_coverMat(NULL)
    , m_locator(NULL)
    , m_lineDrawer(NULL)
    , m_guiMat(NULL)
    , m_defaultVtxDec(NULL)
    , m_rendererChain(rdrChain)
    , m_guiRendererChain(guiRdrChain)
    , m_lightMatrix(NULL)
    , m_lightPos(2.0f, 0.0f, 2.0f)
    , m_light(NULL)
    , m_window(window)
    , m_swpAct(swpAct)
    , m_buttonFactory(NULL)
    , m_cursorFactory(NULL)
    ///, m_panelFactory(NULL)
    , m_editFactory(NULL)
    ///, m_horiBarFactory(NULL)
    ///, m_vertBarFactory(NULL)
    , m_containerFactory(NULL)
	///, m_dropDownMenuFactory(NULL)
	, m_comboxBoxFactory(NULL)
    , m_guiButton(NULL)
    , m_guiCursor(NULL)
    ///, m_guiPanel(NULL)
    , m_guiEdit(NULL)
    ///, m_guiHoriBar(NULL)
    ///, m_guiVertBar(NULL)
    , m_guiContainer(NULL)
	///, m_guiDropDownMenu(NULL)
	, m_guiComboxBox(NULL)
    , m_isInited(false)
	{}
#else
    ResourceAction(RendererChain* rdrChain, GUIRendererChain* guiRdrChain)
    : m_mouseRay(NULL)
    , m_mat0(NULL)
    , m_mat1(NULL)
    , m_pureLightingMat(NULL)
    , m_coverMat(NULL)
    , m_locator(NULL)
	, m_lineDrawer(NULL)
    , m_guiMat(NULL)
    , m_defaultVtxDec(NULL)
    , m_rendererChain(rdrChain)
    , m_guiRendererChain(guiRdrChain)
    , m_lightMatrix(NULL)
    , m_lightPos(2.0f, 0.0f, 2.0f)
    , m_light(NULL)
    , m_buttonFactory(NULL)
    , m_cursorFactory(NULL)
	///, m_panelFactory(NULL)
	///, m_horiBarFactory(NULL)
	///, m_vertBarFactory(NULL)
    , m_containerFactory(NULL)
	///, m_dropDownMenuFactory(NULL)
	, m_comboxBoxFactory(NULL)
    , m_guiButton(NULL)
    , m_guiCursor(NULL)
    ///, m_guiPanel(NULL)
	, m_guiEdit(NULL)
	///, m_guiHoriBar(NULL)
    ///, m_guiVertBar(NULL)
    , m_guiContainer(NULL)
	///, m_guiDropDownMenu(NULL)
	, m_guiComboxBox(NULL)
    , m_isInited(false)
	{}
#endif
	~ResourceAction();
	virtual void DoImpl();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class LogicAction : public Action
{
	DeclareRTTI;
private:
	ResourceAction* m_resAct;
	TimeCheckpoint m_prevCheckpoint;
public:
	LogicAction(ResourceAction* resAct)
		: m_resAct(resAct)
	{
	}
	~LogicAction()
	{
	}
	virtual void DoImpl();
};
///**********************************************************************///
///                       class define end                             ///
///**********************************************************************///
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class RenderRobot;
class RenderAction : public Action
{
	DeclareRTTI;
public:
	RendererChain* m_rendererChain;
	GUIRendererChain* m_guiRendererChain;
public:
	RenderAction(RendererChain* renderChain, GUIRendererChain* guiRenderChain)
		: m_rendererChain(renderChain)
		, m_guiRendererChain(guiRenderChain)
	{}
	~RenderAction();
    virtual void DoImpl();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#if defined(_WIN32) || defined(_WIN64)
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class SwapBuffersAction : public Action
{
	DeclareRTTI;
public:
	int m_fps;
	vptr m_renderDevice;
	SwapBuffersAction()
		: m_fps(0)
	    , m_renderDevice(NULL)
	{}
	inline void Init(vptr dev) {
		m_renderDevice = dev;
	}
	virtual void DoImpl();
};
///**********************************************************************///
///                       class define end                               ///
///**********************************************************************///
#endif
///**********************************************************************///
///                       class define begin                             ///
///**********************************************************************///
class RenderRobot : public Robot
{
	DeclareRTTI;
public:
    RendererChain m_rendererChain;
	GUIRendererChain m_guiRendererChain;
	xhn::map<xhn::static_string, AnimationStatus> m_attrStatusMap;
#if defined(_WIN32) || defined(_WIN64)
    SwapBuffersAction* m_swpAct;
    RenderRobot(HWND window);
    inline int GetAndClearFPS() {
		int fps = m_swpAct->m_fps;
		m_swpAct->m_fps = 0;
		return fps;
	}
#else
    RenderRobot();
#endif
	~RenderRobot();
public:
	virtual void InitChannels();
	virtual xhn::static_string GetName();
	virtual void CommandProcImpl(xhn::static_string sender, RobotCommand* command);
	virtual void CommandReceiptProcImpl(xhn::static_string sender,
                                        RobotCommandReceipt* receipt);
    Camera GetMainCamera();
    SpriteRenderer* GetGUIRenderer();
};
///**********************************************************************///
///                       class define end                             ///
///**********************************************************************///
#endif
