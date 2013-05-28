
///#include <omp.h>
#include "common.h"
///#include <GL/gl.h>

#include "xhn_vector.hpp"
#include "xhn_map.hpp"
#include "xhn_pair.hpp"
#include "xhn_static_string.hpp"

#include "common.h"
#include "etypes.h"
#include "emem.h"
#include "stack.h"
#include "elog.h"

#include "vertex_element.h"
#include "vertex_declaration.h"
#include "vertex_buffer.h"
#include "index_buffer.h"

///#include "list_test.h"
///#include "tree_test.h"

#include "matrix4x4.h"
#include "gl_utility.h"
#include "sketch.h"
#include "texture2d.h"

#include "pass_console.h"
#include "sfloat3.h"
#include "sfloat4.h"

#include "shader_object.h"
#include "shader_node.h"
#include "shader_buffer.h"
#include "vertex_shader_buffer.h"
#include "pixel_shader_buffer.h"

#include "material_prototype.h"
#include "material_instance.h"
#include "renderable.h"
#include "renderer.h"

#include "lighting_pass.h"
#include "std_pass.h"
#include "display_pass.h"
#include "array.h"

#include "render_system.h"

#include "cylinder.h"
#include "locator.h"

#include "plane.h"
#include "triangle.h"

#include "mesh_triangle.h"

#include "branch_node.h"

#include "input_system.h"
#include "camera_utility.h"
#include "exception.h"

#include "octree.h"

#include "collada_loader.h"

#include "plane_mesh.h"
#include "cone.h"
#include <math.h>

#include "line_utility.h"

#include "sprite_renderer.h"

#include "gc_test.h"
#include "vlang_test.h"
#include "sprite.h"
#include "gui_panel.h"
#include "rtti.hpp"

#include "render_robot.h"
#include "animation.hpp"
#include "input_robot.h"
#include "robot_thread.h"

#include "shader_log.h"

#include "xhn_test.hpp"

#include "glsl_lex.h"

#include "sprite_event_hub.h"

#include "font_renderer.h"

Mesh g_mesh0 = {NULL};
Mesh g_mesh1 = {NULL};


///GUIRenderer* g_gui_renderer = NULL;

LineDrawer g_line_drawer = NULL;

static GUIPanel* g_panel = NULL;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

static InputSystem* g_input_system = NULL;
///////////////////////////////////////////////////////////////////////////////

HHOOK g_hhkLowLevelKybd = NULL;

static RenderRobot* g_robot = NULL;
static AnimationRobot* g_anim_robot = NULL;
static InputRobot* g_input_robot = NULL;
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    MSG msg;
    BOOL bQuit = FALSE;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          W_Width,
                          W_Height,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, 1);
    SetTimer(hwnd, 1,1000,NULL);

	MInit();
	ELog_Init();
	ShaderLog_Init();

	VectorTest();

	GLSL::test();

	TRY(0)

	SpriteElement::Test();

	RobotManager::Init();
	RobotThreadManager::Init();
	SpriteEventHub::Init();
/**	
	g_anim_robot = RobotManager::Get()->StartRobot<AnimationRobot>();
	g_robot = RobotManager::Get()->StartRobot<RenderRobot, vptr>((vptr)hwnd);
**/
	g_robot = RobotManager::Get()->AddRobot<RenderRobot, HWND>(hwnd);
	g_anim_robot = RobotManager::Get()->AddRobot<AnimationRobot>();
	g_input_robot = RobotManager::Get()->AddRobot<InputRobot, HWND>(hwnd);
	
	RobotManager::Get()->Remove("RenderRobot");
	RobotThreadManager::Get()->AddRobotThread();
	///RobotThreadManager::Get()->AddRobotThread();
	ComposingStickManager::Init();

    while (!bQuit)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
		    g_robot->RunOnce();
        }
    }

    CATCH(0)
    {
        /**
        printf("exce %s\n", GET_EXCE);
        getchar();
        exit(1);
        **/
		exit(1);
    }

    DestroyWindow(hwnd);

    return NULL;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_DESTROY:
		///RobotManager::Get()->StopRobot(g_robot->GetName());
		///RobotManager::Get()->StopRobot(g_anim_robot->GetName());
		RobotThreadManager::Get()->StopAllRobotThread();
        Mlog();

        ///UnhookWindowsHookEx(g_hhkLowLevelKybd);
        return 0;

    case WM_TIMER:

		{
			int fps = g_robot->GetAndClearFPS();
            elog("##FPS %d", fps);
		}
        break;

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
    }
    break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
