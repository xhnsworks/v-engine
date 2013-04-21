#include "input_win32.h"
#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

LPDIRECTINPUT8       g_pDI       = NULL;
LPDIRECTINPUTDEVICE8 g_pMouse    = NULL;
LPDIRECTINPUTDEVICE8 g_pKeyboard = NULL;

HWND                 g_hwnd      = NULL;

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#define DINPUT_BUFFERSIZE 256

bool input_Init(HWND h)
{
    RECT rc;
    if (!GetWindowRect(h, &rc))
        return false;

    g_hwnd = h;

    SetCursorPos(rc.left, rc.top);

    HRESULT hr;
    DWORD   dwCoopFlags;
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION,
                                         IID_IDirectInput8, (VOID**)&g_pDI, NULL ) ) )
    {
        return false;
    }
    if( FAILED( hr = g_pDI->CreateDevice( GUID_SysMouse, &g_pMouse, NULL ) ) )
    {
        return false;
    }

    if( FAILED( hr = g_pDI->CreateDevice( GUID_SysKeyboard, &g_pKeyboard, NULL ) ) )
    {
        return false;
    }
    //Sets the data format for the Microsoft DirectInput device
    if( FAILED( hr = g_pMouse->SetDataFormat( &c_dfDIMouse ) ) )
    {
        return false;
    }
    if( FAILED( hr = g_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
    {
        return false;
    }
///#define DINPUT_BUFFERSIZE 16
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;

    dipdw.dwData = DINPUT_BUFFERSIZE ;
    g_pMouse->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
    g_pKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
    /**
    	dipdw.dwData = DIPROPAXISMODE_ABS ;
        g_pMouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
        **/
    //检测设备是否可以独占使用
    dwCoopFlags = DISCL_NONEXCLUSIVE|DISCL_BACKGROUND;
    //dwCoopFlags = DISCL_EXCLUSIVE|DISCL_BACKGROUND;
    hr = g_pMouse->SetCooperativeLevel( h, dwCoopFlags );
    if (DI_OK!=hr)
    {
        return false;
    }
    hr = g_pKeyboard->SetCooperativeLevel( h, dwCoopFlags );
    if (DI_OK!=hr)
    {
        return false;
    }
    //Obtains access to the input device.

    g_pMouse->Acquire();
    g_pKeyboard->Acquire();
    return true;
}

void input_Dest()
{
    // Unacquire the device one last time just in case
    // the app tried to exit while the device is still acquired.
    if( g_pMouse )
        g_pMouse->Unacquire();

    if ( g_pKeyboard )
        g_pKeyboard->Unacquire();

    // Release any DirectInput objects.
    SAFE_RELEASE( g_pMouse );
    SAFE_RELEASE( g_pKeyboard );
    SAFE_RELEASE( g_pDI );
}

void input_Proc(const xhn::vector<input_buffer>& buffer)
{
    if(g_pMouse != NULL)
    {
        DIDEVICEOBJECTDATA didod[DINPUT_BUFFERSIZE]; // Receives buffered data
        DWORD               dwElements;
        HRESULT             hr;

        hr = DIERR_INPUTLOST;

        memset(didod, 0, sizeof(didod));
        dwElements = DINPUT_BUFFERSIZE;
        hr = g_pMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0);
        if (hr != DI_OK)
        {
            hr = g_pMouse->Acquire();
            if(FAILED(hr))
                return;
            hr = g_pMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0);
        }

        if(FAILED(hr))
        {
            if (hr == (HRESULT)DIERR_INPUTLOST)
                printf("DIERR_INPUTLOST\n");
            if (hr == (HRESULT)DIERR_INVALIDPARAM)
                printf("DIERR_INVALIDPARAM\n");
            if (hr == (HRESULT)DIERR_NOTACQUIRED)
                printf("DIERR_NOTACQUIRED\n");
            if (hr == (HRESULT)DIERR_NOTBUFFERED)
                printf("DIERR_NOTBUFFERED\n");
            if (hr == (HRESULT)DIERR_NOTINITIALIZED)
                printf("DIERR_NOTINITIALIZED\n");

            return;
        }
        else
        {
            for(DWORD i=0; i<dwElements; i++)
            {
                input_event event;
                memset(&event, 0, sizeof(event));

                event.time_stamp = didod[i].dwTimeStamp;
                if (didod[i].dwOfs == (DWORD)DIMOFS_X)
                {
                    int disp = didod[i].dwData;
                    event.info.mouse_info.mouse_move_info.x = disp;
                    event.type = MouseMoveEvent;
                }
                else if (didod[i].dwOfs == (DWORD)DIMOFS_Y)
                {
                    int disp = didod[i].dwData;
                    event.info.mouse_info.mouse_move_info.y = disp;
                    event.type = MouseMoveEvent;
                }
                else if (didod[i].dwOfs == (DWORD)DIMOFS_BUTTON0)
                {
                    if (didod[i].dwData & 0x80)
                        event.type = MouseButtonDownEvent;
                    else
                        event.type = MouseButtonUpEvent;
                    event.info.mouse_info.mouse_button_info = LeftButton;
                }
                else if (didod[i].dwOfs == (DWORD)DIMOFS_BUTTON1)
                {
                    if (didod[i].dwData & 0x80)
                        event.type = MouseButtonDownEvent;
                    else
                        event.type = MouseButtonUpEvent;
                    event.info.mouse_info.mouse_button_info = RightButton;
                }
                else if (didod[i].dwOfs == (DWORD)DIMOFS_BUTTON2)
                {
                    if (didod[i].dwData & 0x80)
                        event.type = MouseButtonDownEvent;
                    else
                        event.type = MouseButtonUpEvent;
                    event.info.mouse_info.mouse_button_info = MiddleButton;
                }
                else
                    continue;
				for (uint i = 0; i < buffer.size(); i++) {
					RWBuffer input_buffer = buffer[i].input_buffer;
					InterRWBuffer inter_input_buffer = buffer[i].inter_input_buffer;
					inter_input_buffer.write(input_buffer, (const uint*)&event, sizeof(event));
				}
            }
            POINT curPos;
            GetCursorPos(&curPos);
            ScreenToClient(g_hwnd, &curPos);
            input_event event;
            event.type = MouseAbsolutePositionEvent;
            event.info.mouse_info.mouse_abs_pos.x = curPos.x;
            event.info.mouse_info.mouse_abs_pos.y = curPos.y;
            ///inter_input_buffer.write(input_buffer, (const uint*)&event, sizeof(event));
			for (uint i = 0; i < buffer.size(); i++) {
				RWBuffer input_buffer = buffer[i].input_buffer;
				InterRWBuffer inter_input_buffer = buffer[i].inter_input_buffer;
				inter_input_buffer.write(input_buffer, (const uint*)&event, sizeof(event));
			}
        }

    }
    if(g_pKeyboard != NULL)
    {
        DIDEVICEOBJECTDATA didod[DINPUT_BUFFERSIZE]; // Receives buffered data
        DWORD               dwElements;
        HRESULT             hr;

        hr = DIERR_INPUTLOST;

        memset(didod, 0, sizeof(didod));
        dwElements = DINPUT_BUFFERSIZE;
        hr = g_pKeyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0);
        if (hr != DI_OK)
        {
            hr = g_pKeyboard->Acquire();
            if(FAILED(hr))
                return;
            hr = g_pKeyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0);
        }

        if(FAILED(hr))
        {
            if (hr == (HRESULT)DIERR_INPUTLOST)
                printf("DIERR_INPUTLOST\n");
            if (hr == (HRESULT)DIERR_INVALIDPARAM)
                printf("DIERR_INVALIDPARAM\n");
            if (hr == (HRESULT)DIERR_NOTACQUIRED)
                printf("DIERR_NOTACQUIRED\n");
            if (hr == (HRESULT)DIERR_NOTBUFFERED)
                printf("DIERR_NOTBUFFERED\n");
            if (hr == (HRESULT)DIERR_NOTINITIALIZED)
                printf("DIERR_NOTINITIALIZED\n");

            return;
        }
        else
        {
            for(DWORD i=0; i<dwElements; i++)
            {
                input_event event;
                memset(&event, 0, sizeof(event));

                event.time_stamp = didod[i].dwTimeStamp;
                if (didod[i].dwData & 0x80)
                    event.type = KeyDownEvent;
                else
                    event.type = KeyUpEvent;
                event.info.key_info = didod[i].dwOfs & 0xff;
                ///inter_input_buffer.write(input_buffer, (const uint*)&event, sizeof(event));
				for (uint i = 0; i < buffer.size(); i++) {
					RWBuffer input_buffer = buffer[i].input_buffer;
					InterRWBuffer inter_input_buffer = buffer[i].inter_input_buffer;
					inter_input_buffer.write(input_buffer, (const uint*)&event, sizeof(event));
				}
            }
        }

    }
}
