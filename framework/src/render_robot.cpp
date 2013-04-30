#include "render_robot.h"
#include "camera_utility.h"
#include "sfloat3.h"
#include "collada_loader.h"
#include "render_system.h"
#include "gl_utility.h"
#include "sprite_event_hub.h"
#include "locator.h"
#include "animation.hpp"

#define BASE_DIR "D:\\"

ImplementRTTI(ModifyAttrCommand, RobotCommand);


bool ModifyAttrCommand::Test(Robot* exeRob)
{
	RenderRobot* rr = exeRob->DynamicCast<RenderRobot>();
	if (!rr)
		return false;
	return SpriteFactory::TestAnimAttr(m_attr);
}

ModifyAttrCommand::~ModifyAttrCommand()
{
	if (m_newValue) {
		delete m_newValue;
		m_newValue = NULL;
	}
}

void ModifyAttrCommand::Do(Robot* exeRob, xhn::static_string sender)
{
	switch (m_attrType)
	{
	case Attribute::Float:
		*((EFloat*)m_attr) = *((EFloat*)m_newValue);
		break;
	case Attribute::Float2:
		*((EFloat2*)m_attr) = *((EFloat2*)m_newValue);
		break;
	case Attribute::Float3:
		*((EFloat3*)m_attr) = *((EFloat3*)m_newValue);
		break;
	case Attribute::Float4:
		*((EFloat4*)m_attr) = *((EFloat4*)m_newValue);
		break;
	}
}

void RendererChain::Init()
{
	ViewportPtr view = ENEW Viewport;
	view->x = 500;
	view->y = 0;
	view->width = W_Width - 500;
	view->height = W_Height;

	m_viewport = view;

	AddRenderer("MainRenderer");
	AddRenderer("CoverRenderer");

	gl_Init();
	ERROR_PROC;
}

void GUIRendererChain::Init()
{
	ViewportPtr parent = ENEW Viewport;
	parent->width = W_Width;
	parent->height = W_Height;
	ViewportPtr view = ENEW Viewport;
	view->x = 0;
	view->y = 0;
	view->width = 500;
	view->height = W_Height;
	view->parent = parent;
	m_viewport = view;
	AddRenderer("GUIRenderer");
}

ShaderNode pure_color_material_proc(PxlSdrBuf _psb, int _id)
{
    ShaderNode psn = ShaderNode_new();
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "PixelProc%d", _id);
    ShaderNode_set_name(psn, mbuf);

    ShaderNode_set_function(psn,
                            "{\n"
#if GLSL_MAIN_VERSION >= 1 && GLSL_SUB_VERSION > 2
                            "    vec3 cmap = texture(ColorMap, vTexCoord).rgb;\n"
#else
							"    vec3 cmap = texture2D(ColorMap, vTexCoord).rgb;\n"
#endif
                            "    gl_FragData[0] = vec4( cmap, 1.0 );"
                            "}\n");
    return psn;
}

ShaderNode pure_lighting_material_proc(PxlSdrBuf _psb, int _id)
{
    ShaderBuffer sb = to_ShaderBuffer(_psb);
    ShaderNode psn = ShaderNode_new();
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "PixelProc%d", _id);
    ShaderNode_set_name(psn, mbuf);

    ShaderNode_set_function(psn,
                            "{\n"
#if GLSL_MAIN_VERSION >= 1 && GLSL_SUB_VERSION > 2
                            "    vec3 lmap = texture(DiffuseLightingMap, vTexCoord).rgb;\n"
#else
							"    vec3 lmap = texture2D(DiffuseLightingMap, vTexCoord).rgb;\n"
#endif
                            "    gl_FragData[0] = vec4( lmap, 1.0 );"
                            "}\n");

    ShaderBuffer_add_prototype_node(sb, psn);
    return psn;
}

ShaderNode pure_normal_material_proc(PxlSdrBuf _psb, int _id)
{
    ShaderBuffer sb = to_ShaderBuffer(_psb);
    ShaderNode psn = ShaderNode_new();
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "PixelProc%d", _id);
    ShaderNode_set_name(psn, mbuf);

    ShaderNode_set_function(psn,
                            "{\n"
#if GLSL_MAIN_VERSION >= 1 && GLSL_SUB_VERSION > 2
                            "    vec4 lmap = texture(NormalTangentMap, vTexCoord).rgba;\n"
#else
							"    vec4 lmap = texture2D(NormalTangentMap, vTexCoord).rgba;\n"
#endif
							"    vec3 ret;"
                            ///"    NormalDecode(lmap, ret);\n"
							"{\n"
							///"    ret.xy = Enc.xy * 2.0 - 1.0;\n"
							///"    ret.z = sqrt(1.0 - dot(ret.xy, ret.xy));\n"
							"    vec2 fenc = lmap.xy * 2.0 - 1.0;\n"
							"    ret.z = -(dot(fenc, fenc) * 2.0 - 1.0);\n"
							"    ret.xy = normalize(fenc) * sqrt(1.0 - ret.z * ret.z);\n"
							"}\n"
                            "    gl_FragData[0] = vec4( ret, 1.0 );"
                            "}\n");

    ShaderBuffer_add_prototype_node(sb, psn);
    return psn;
}

ShaderNode pure_tangent_material_proc(PxlSdrBuf _psb, int _id)
{
    ShaderBuffer sb = to_ShaderBuffer(_psb);
    ShaderNode psn = ShaderNode_new();
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "PixelProc%d", _id);
    ShaderNode_set_name(psn, mbuf);

    ShaderNode_set_function(psn,
                            "{\n"
#if GLSL_MAIN_VERSION >= 1 && GLSL_SUB_VERSION > 2
                            "    vec4 lmap = texture(NormalTangentMap, vTexCoord).rgba;\n"
#else
							"    vec4 lmap = texture2D(NormalTangentMap, vTexCoord).rgba;\n"
#endif
                            "    lmap.xy = lmap.zw;\n"
                            "    vec3 ret;"
							///"    NormalDecode(lmap, ret);\n"
							"{\n"
							///"    ret.xy = Enc.xy * 2.0 - 1.0;\n"
							///"    ret.z = sqrt(1.0 - dot(ret.xy, ret.xy));\n"
							"    vec2 fenc = lmap.xy * 2.0 - 1.0;\n"
							"    ret.z = -(dot(fenc, fenc) * 2.0 - 1.0);\n"
							"    ret.xy = normalize(fenc) * sqrt(1.0 - ret.z * ret.z);\n"
							"}\n"
                            "    gl_FragData[0] = vec4( ret, 1.0 );"
                            "}\n");

    ShaderBuffer_add_prototype_node(sb, psn);
    return psn;
}

ShaderNode red_material_proc(PxlSdrBuf _psb, int _id)
{
    ShaderNode psn = ShaderNode_new();
    char mbuf[STRING_BUFFER_SIZE];
    snprintf(mbuf, STRING_BUFFER_SIZE - 1, "RedPixelProc%d", _id);
    ShaderNode_set_name(psn, mbuf);

    ShaderNode_set_function(psn,
                            "{\n"
                            "    gl_FragData[0] = vec4( 1.0, 0.0, 0.0, 1.0 );"
                            "}\n");
    return psn;
}

class DefaultKeyboardListener2 : public InputListener
{
	DeclareRTTI;
public:
	bool m_left_alt_key_down;
	DefaultKeyboardListener2()
		: m_left_alt_key_down(false)
	{}
	virtual void ListenImpl(const input_event& event);
};

class DefaultMouseListener2 : public InputListener
{
	DeclareRTTI;
public:
	DefaultKeyboardListener2* m_keyboardListener;
	CameraUtility m_cameraUtil;
	SpriteRenderer* m_guiRenderer;
	int m_mouseX;
	int m_mouseY;
	bool m_leftButtonDown;
	bool m_rightButtonDown;
	bool m_middleButtonDown;
	DefaultMouseListener2(DefaultKeyboardListener2* list, RendererChain* rendererChain, GUIRendererChain* guiRenderChain);
	~DefaultMouseListener2();
	virtual void ListenImpl(const input_event& event);
};

ImplementRTTI(DefaultKeyboardListener2, InputListener);
ImplementRTTI(DefaultMouseListener2, InputListener);

void DefaultKeyboardListener2::ListenImpl(const input_event& event)
{
	if (event.type == KeyDownEvent)
	{
		printf("key down %d\n", event.info.key_info);
		if (event.info.key_info == 56)
		{
			m_left_alt_key_down = true;
		}
	}
	else if (event.type == KeyUpEvent)
	{
		printf("key up %d\n", event.info.key_info);
		if (event.info.key_info == 56)
		{
			m_left_alt_key_down = false;
		}
	}
}

DefaultMouseListener2::DefaultMouseListener2(DefaultKeyboardListener2* list, RendererChain* rendererChain, GUIRendererChain* guiRenderChain)
: m_keyboardListener(list)
, m_mouseX(0)
, m_mouseY(0)
, m_leftButtonDown(false)
, m_rightButtonDown(false)
, m_middleButtonDown(false)
{
    Renderer* mainRdr = rendererChain->GetRenderer("MainRenderer");
	m_cameraUtil = CameraUtility_new(mainRdr->get_camera());
	m_guiRenderer = guiRenderChain->GetRenderer("GUIRenderer");
}

DefaultMouseListener2::~DefaultMouseListener2()
{
	if (m_cameraUtil) {
		CameraUtility_delete(m_cameraUtil);
	}
}

void DefaultMouseListener2::ListenImpl(const input_event& event)
{
	if (event.type == MouseMoveEvent)
	{	
		if (m_keyboardListener->m_left_alt_key_down && m_rightButtonDown)
		{
			sfloat3 vec = SFloat3( (float)event.info.mouse_info.mouse_move_info.x,
				(float)event.info.mouse_info.mouse_move_info.y,
				0.0f );
			float dist = SFloat3_length(vec);

			if (event.info.mouse_info.mouse_move_info.x < 0)
				dist = -dist;
			CameraUtility_dolly_shot(m_cameraUtil, dist);
		}
		else if (m_keyboardListener->m_left_alt_key_down && m_middleButtonDown)
		{
			CameraUtility_pan(m_cameraUtil, (float)event.info.mouse_info.mouse_move_info.x, (float)event.info.mouse_info.mouse_move_info.y);
		}
		else if (m_keyboardListener->m_left_alt_key_down && m_leftButtonDown)
		{
			CameraUtility_rotate(m_cameraUtil, (float)-event.info.mouse_info.mouse_move_info.x, 0.0f);
		}
	}
	else if (event.type == MouseAbsolutePositionEvent)
	{
		m_mouseX = event.info.mouse_info.mouse_abs_pos.x;
		m_mouseY = event.info.mouse_info.mouse_abs_pos.y + 35;
		///printf("mouse x %d, y %d\n", g_mouse_x, g_mouse_y);
		SpriteMouseMoveEvent sptEvt;
		sptEvt.m_curtMousePos.x = m_mouseX;
		sptEvt.m_curtMousePos.y = m_mouseY;
		///m_guiRenderer->get_mouse_ray(m_mouseX, m_mouseY, &guiEvt.m_mouseRay.origin, &guiEvt.m_mouseRay.direction);
		SpriteEventHub::Get()->BroadcastEvent(sptEvt, SpriteEventHub::Get()->GetAllReceivers());
	}
	else if (event.type == MouseButtonDownEvent)
	{
		if (event.info.mouse_info.mouse_button_info == LeftButton) {
			m_leftButtonDown = true;
			SpriteMouseButtonDownEvent sptEvt;
			sptEvt.m_leftButtomDown = true;
			SpriteEventHub::Get()->BroadcastEvent(sptEvt, SpriteEventHub::Get()->GetAllReceivers());
		}
		else if (event.info.mouse_info.mouse_button_info == RightButton)
			m_rightButtonDown = true;
		else if (event.info.mouse_info.mouse_button_info == MiddleButton)
			m_middleButtonDown = true;
	}
	else if (event.type == MouseButtonUpEvent)
	{
		if (event.info.mouse_info.mouse_button_info == LeftButton) {
			m_leftButtonDown = false;
			m_leftButtonDown = true;
			SpriteMouseButtonUpEvent sptEvt;
			sptEvt.m_leftButtomUp = true;
			SpriteEventHub::Get()->BroadcastEvent(sptEvt, SpriteEventHub::Get()->GetAllReceivers());
		}
		else if (event.info.mouse_info.mouse_button_info == RightButton)
			m_rightButtonDown = false;
		else if (event.info.mouse_info.mouse_button_info == MiddleButton)
			m_middleButtonDown = false;
	}
}

ImplementRTTI(ResourceAction, Action);
ImplementRTTI(InputAction, Action);
ImplementRTTI(LogicAction, Action);
ImplementRTTI(RenderAction, Action);
ImplementRTTI(SwapBuffersAction, Action);

void ResourceAction::DoImpl()
{
	if (!m_isInited)
	{
		m_isInited = true;

		HWND hwnd = (HWND)m_window;
		HDC hdc;
		HGLRC hglrc;
		EnableOpenGL(hwnd, &hdc, &hglrc);

		RenderSystem_Init(hwnd);

		m_rendererChain->Init();
		m_guiRendererChain->Init();

		m_inputSys->Init(hwnd);

		m_swpAct->Init(hdc);
		m_inputAct->Init();

		Renderer* mainRdr = m_rendererChain->GetRenderer("MainRenderer");
		Renderer* coverRdr = m_rendererChain->GetRenderer("CoverRenderer");

		SpriteRenderer* guiRdr = m_guiRendererChain->GetRenderer("GUIRenderer");

		SpriteEventHub::Init();
		m_buttonFactory = ENEW GUIButtonFactory(guiRdr, "button2.xml");
		m_cursorFactory = ENEW GUICursorFactory(guiRdr, "cursor.xml");
		m_panelFactory = ENEW GUIPanelFactory(guiRdr, "panel2.xml");

        m_guiButton = m_buttonFactory->MakeSprite()->DynamicCast<GUIButton>();
		m_guiCursor = m_cursorFactory->MakeSprite()->DynamicCast<GUICursor>();
		m_guiPanel = m_panelFactory->MakeSprite()->DynamicCast<GUIPanel>();
		m_guiPanel->SetCoord(30.0f, 50.0f);
		m_guiPanel->SetScale(1.0f, 1.0f);
		m_guiPanel->SetSize(100.0f, 100.0f);

		m_guiButton->SetCoord(50.0f, 150.0f);
		///m_guiButton->SetRotate(0.5f);
		m_guiCursor->SetCoord(10.0f, 30.0f);

		///
		m_guiPanel->AddChild(m_guiButton);
		m_guiPanel->SetRotate(0.2f);
		///

		SDisplayProc s;
		if (mainRdr) {
			s.proc = red_material_proc;
			mainRdr->register_material("red_material", s, Shaded, false);
			s.proc = pure_color_material_proc;
			mainRdr->register_material("pure_color", s, Shaded, false);
			s.proc = pure_lighting_material_proc;
			mainRdr->register_material("pure_lighting", s, Shaded, false);
			s.proc = pure_normal_material_proc;
			mainRdr->register_material("pure_normal", s, Shaded, false);
			s.proc = pure_tangent_material_proc;
			mainRdr->register_material("pure_tangent", s, Shaded, false);
		}
		if (coverRdr) {
			s.proc = red_material_proc;
			coverRdr->register_material("red_material", s, Shaded, true);

			m_lineDrawer = ILnDwr.New(coverRdr);
		}
		m_defaultVtxDec = VertexDecl_new_from_string(
			"pf3"
            "tf2"
            "nf3"
            "Tf3"
            "Bf3");
		{
			m_coverMat = MaterialInstance_new("red_material", NULL, NULL, "Texture");
			m_locator = coverRdr->new_renderable(m_defaultVtxDec, m_coverMat, Segment);
            coverRdr->use_renderable(m_locator);
		}
		{
		    MeshPtr m;
            {
                ColladaState colSt = ColladaState_new();
                ColladaState_load_mesh(colSt, "D:\\test_scene\\test4.dae");
                ColladaState_log(colSt);
                m = ColladaState_create_mesh(colSt);
            }

            char path[MAX_PATH];
            snprintf(path, MAX_PATH, "%s%s", BASE_DIR, "test_scene\\mesh_pos_unit1.ogl");

			m_mat0 = MaterialInstance_new("pure_lighting", NULL, NULL, "Texture");
			Renderable r = mainRdr->new_renderable(m_defaultVtxDec, m_mat0, Triangular);
            Renderable_add_mesh(r, m);
            mainRdr->use_renderable(r);
        }
        {
		    MeshPtr m;
            {
                ColladaState colSt = ColladaState_new();
                ColladaState_load_mesh(colSt, "D:\\test_scene\\test5.dae");
                ColladaState_log(colSt);
                m = ColladaState_create_mesh(colSt);
            }
		    m_mat1 = MaterialInstance_new("default_material", "GAZER789_Diffuse.png", "GAZER789_Normal-1.png", "Texture");

		    Renderable r = mainRdr->new_renderable(m_defaultVtxDec, m_mat1, Triangular);
            Renderable_add_mesh(r, m);
            mainRdr->use_renderable(r);

            matrix4x4* mat = Renderable_get_world_matrix(r);

            matrix4x4* scale_mat = Matrix4x4_new();
            matrix4x4* tran_mat = Matrix4x4_new();

            float s = 0.2f;
            Matrix4x4_set_scale(scale_mat, s, s, s);
            Matrix4x4_set_as_translate(tran_mat, 0.0f, -16.5f, -25.0f);

            Matrix4x4_mul_matrix4(mat, tran_mat, mat);
            Matrix4x4_mul_matrix4(mat, scale_mat, mat);

            Matrix4x4_delete(scale_mat);
            Matrix4x4_delete(tran_mat);
        }

        p0 = SFloat4(-2.2f, 0.0f, 1.5f, 1.0f);
        p1 = SFloat4(1.0f, 0.0f, 1.5f, 1.0f);
        p2 = SFloat4(-2.5f, 0.0f, 0.1f, 1.0f);
        p3 = SFloat4(1.8f, 0.0f, 0.0f, 1.0f);

        p4 = SFloat4(1.0f, 0.0f, -1.6f, 1.0f);
        p5 = SFloat4(1.0f, 0.0f, 1.7f, 1.0f);
        p6 = SFloat4(1.8f, 0.0f, 0.9f, 1.0f);
        p7 = SFloat4(-1.0f, 0.0f, 0.3f, 1.0f);
	    m_lightMatrix = Matrix4x4_new();
	 

		{
		    ///==============create default render data================///
            matrix4x4* scl = Matrix4x4_new();
            Matrix4x4_set_scale(scl, 1.5, 1.5, 1.5);
            p0 = Matrix4x4_mul_float4(scl, p0);
            p1 = Matrix4x4_mul_float4(scl, p1);
            p2 = Matrix4x4_mul_float4(scl, p2);
            p3 = Matrix4x4_mul_float4(scl, p3);

            p4 = Matrix4x4_mul_float4(scl, p4);
            p5 = Matrix4x4_mul_float4(scl, p5);
            p6 = Matrix4x4_mul_float4(scl, p6);
            p7 = Matrix4x4_mul_float4(scl, p7);

            Matrix4x4_delete(scl);

            Matrix4x4_set_y_axis_rotate(m_lightMatrix, 0.01f);

            p0 = Matrix4x4_mul_float4(m_lightMatrix, p0);
            p1 = Matrix4x4_mul_float4(m_lightMatrix, p1);
            p2 = Matrix4x4_mul_float4(m_lightMatrix, p2);
            p3 = Matrix4x4_mul_float4(m_lightMatrix, p3);

            p4 = Matrix4x4_mul_float4(m_lightMatrix, p4);
            p5 = Matrix4x4_mul_float4(m_lightMatrix, p5);
            p6 = Matrix4x4_mul_float4(m_lightMatrix, p6);
            p7 = Matrix4x4_mul_float4(m_lightMatrix, p7);

        
            EFloat3 lt_look_at(0.0f, 0.0f, 0.0f);
            EFloat4 lt_color(0.15f, 0.13f, 0.15f, 1.0f);

	        LightBase2 lt = mainRdr->add_dir_light_2();
			lt->set_position(m_lightPos);
            lt->look_at(lt_look_at);

            lt->set_light_color(lt_color);

            lt = mainRdr->add_spot_light_2();
            m_lightPos.x = -5.0f;
            m_lightPos.z = 2.0f;
            lt_color.x = 0.1f;
            lt_color.y = 0.13f;
            lt_color.z = 0.13f;
            lt_look_at.z = -3.0f;
	        lt->set_position(m_lightPos);
	        lt->look_at(lt_look_at);

	        lt->set_light_color(lt_color);

            lt = mainRdr->add_point_light_2();
            lt_color.x = 0.8f;
            lt_color.y = 0.75f;
            lt_color.z = 0.65f;
            lt->set_light_color(lt_color);
			m_light = lt;
	    }
	}
	static bool s_isCreateAnimed = false;
	{
		if (!RobotManager::Get()->GetChannel("RenderRobot", "AnimationRobot"))
			RobotManager::Get()->MakeChannel("RenderRobot", "AnimationRobot");
		if (!RobotManager::Get()->GetChannel("AnimationRobot", "RenderRobot"))
			RobotManager::Get()->MakeChannel("AnimationRobot", "RenderRobot");

		if (!s_isCreateAnimed) {
			RWBuffer channel = RobotManager::Get()->GetChannel("RenderRobot", "AnimationRobot");
			if (channel) {
				{
					CreateAnimCommand* cac = ENEW CreateAnimCommand(m_guiButton->GetScaleHandle(), Attribute::Float2);
					cac->m_animFileName = "anim.xml";
					cac->m_animName = "scale";
					RWBuffer_Write(channel, (const euint*)&cac, sizeof(cac));
				}
				{
					CreateAnimCommand* cac = ENEW CreateAnimCommand(m_guiButton->GetPivotHandle(), Attribute::Float2);
					cac->m_animFileName = "anim.xml";
					cac->m_animName = "offset";
					RWBuffer_Write(channel, (const euint*)&cac, sizeof(cac));
				}
				s_isCreateAnimed = true;
			}
		}
	}
}

ResourceAction::~ResourceAction()
{
	DestroyWindow((HWND)m_window);
}

InputAction::InputAction(InputSystem* inputSys, RendererChain* rendererChain, GUIRendererChain* guiRendererChain)
	: m_inputSys(inputSys)
	, m_rendererChain(rendererChain)
	, m_guiRendererChain(guiRendererChain)
	, m_mouseListener(NULL)
	, m_keyboardListener(NULL)
{
}

InputAction::~InputAction()
{
	delete m_keyboardListener;
    delete m_mouseListener;
}
void InputAction::Init()
{
	DefaultKeyboardListener2* list = ENEW DefaultKeyboardListener2;
	m_keyboardListener = list;
	list->Init();
	m_mouseListener = ENEW DefaultMouseListener2(list, m_rendererChain, m_guiRendererChain);
	m_mouseListener->Init();
	m_inputSys->register_input_listener(m_keyboardListener);
	m_inputSys->register_input_listener(m_mouseListener);
}
void InputAction::DoImpl()
{
	m_inputSys->update();
    m_keyboardListener->Listen();
	m_mouseListener->Listen();
}
static float s_rotate = 0.0f;
void LogicAction::DoImpl()
{
	Renderer* rdr = m_resAct->m_rendererChain->GetRenderer("MainRenderer");
	SpriteRenderer* guiRdr = m_resAct->m_guiRendererChain->GetRenderer("GUIRenderer");

	m_resAct->p0 = Matrix4x4_mul_float4(m_resAct->m_lightMatrix, m_resAct->p0);

    EFloat3 lt_pos = SFloat3_convert(SFloat4_xyz(m_resAct->p0));
    m_resAct->m_light->set_position(lt_pos);
	
	guiRdr->GetGeomBuffer()->Clear();

	m_resAct->m_guiPanel->SetRotate(s_rotate);
	///s_rotate += 0.01f;

	TimeCheckpoint checkPoint = TimeCheckpoint::Tick();
	double elapsedTime = TimeCheckpoint::CaleElapsedTime(m_prevCheckpoint, checkPoint);
	elapsedTime = us_to_s(elapsedTime);
	m_prevCheckpoint = checkPoint;
	SpriteFrameStartEvent evt(elapsedTime);
	SpriteEventHub::Get()->BroadcastFrameStartEvent(evt, SpriteEventHub::Get()->GetAllReceivers());

	guiRdr->clear_depth();
	
	ray mouseRay;
	DefaultMouseListener2* list = m_inputAct->m_mouseListener->DynamicCast<DefaultMouseListener2>();
	rdr->get_mouse_ray(list->m_mouseX, list->m_mouseY, &mouseRay.origin, &mouseRay.direction);

	Renderable_clear(m_resAct->m_locator);

    sfloat3 ori = SFloat3_assign_from_efloat3(&mouseRay.origin);
    sfloat3 dir = SFloat3_assign_from_efloat3(&mouseRay.direction);
    dir = SFloat3_mul_float(-2.5f, dir);
    ori = SFloat3_add(ori, dir);
    EFloat3 center = SFloat3_convert(ori);
    MeshPtr m = create_locator(&center, 0.2f);
    Renderable_add_mesh(m_resAct->m_locator, m);
	
	guiRdr->get_mouse_ray(list->m_mouseX, list->m_mouseY, &mouseRay.origin, &mouseRay.direction);
	///Ray_log(&mouseRay);

	ILnDwr.update(m_resAct->m_lineDrawer);
}

void RenderAction::DoImpl()
{
	m_rendererChain->Render();
	m_guiRendererChain->Render();
}

RenderAction::~RenderAction()
{
}

void SwapBuffersAction::DoImpl()
{
	///glFinish();
    SwapBuffers((HDC)m_renderDevice);
	m_fps++;
}

ImplementRTTI(RenderRobot, Robot);

RenderRobot::RenderRobot(vptr window)
: m_window(window)
{
	InputSystem* inputSys = ENEW InputSystem;

	SwapBuffersAction* swpAct = ENEW SwapBuffersAction();
	InputAction* inputAct = ENEW InputAction(inputSys, &m_rendererChain, &m_guiRendererChain);
	ActionPtr swpActPtr = swpAct;
	ActionPtr inputActPtr = inputAct;
	ResourceAction* resAct = ENEW ResourceAction(&m_rendererChain, &m_guiRendererChain, swpAct, inputAct, window, inputSys);
	ActionPtr resActPtr = resAct;
	ActionPtr logicActPtr = ENEW LogicAction(resAct, inputAct);
	ActionPtr renderActPtr = ENEW RenderAction(&m_rendererChain, &m_guiRendererChain);
	
	AddAction(resActPtr);
	AddAction(inputActPtr);
	AddAction(logicActPtr);
	AddAction(renderActPtr);
	AddAction(swpActPtr);
	m_swpAct = swpAct;
}

RenderRobot::~RenderRobot()
{
}

void RenderRobot::InitChannels()
{
	RobotManager::Get()->MakeChannel("RenderRobot", "AnimationRobot");
	RobotManager::Get()->MakeChannel("AnimationRobot", "RenderRobot");
}

xhn::static_string RenderRobot::GetName()
{
    return "RenderRobot";
}

void RenderRobot::CommandProcImpl(xhn::static_string sender, RobotCommand* command)
{
    ModifyAttrCommand* aacmd = command->DynamicCast<ModifyAttrCommand>();
	if (aacmd) {
        aacmd->Do(this, sender);
	}
}

void RenderRobot::CommandReceiptProcImpl(xhn::static_string sender, RobotCommandReceipt* receipt)
{
	AnimStatusChangeReceipt* animStatusChangeRec = receipt->DynamicCast<AnimStatusChangeReceipt>();
	if (animStatusChangeRec) {
		m_attrStatusMap.insert(xhn::make_pair(animStatusChangeRec->m_animInfo.animID, animStatusChangeRec->m_animInfo.cureStatus));
	}
}