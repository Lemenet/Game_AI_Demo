#include <windows.h>
#include <time.h>

#include "constants.h"
#include "misc/utils.h"
#include "time/PrecisionTimer.h"
#include "GameWorld.h"
#include "misc/Cgdi.h"
#include "ParamLoader.h"
#include "resource.h"
#include "misc/WindowUtils.h"

//--------------------------------- Globals ------------------------------
//
//------------------------------------------------------------------------

char* g_szApplicationName = "Steering Behaviors - Another Big Shoal";
char*	g_szWindowClassName = "MyWindowClass";

GameWorld* g_GameWorld;

//---------------------------- WindowProc ---------------------------------
//	
//	This is the callback function which handles all the windows messages
//-------------------------------------------------------------------------

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//This hold the dimensions of the client window area
	static int cxClient, cyClient;

	//Used to create the back buffer
	static HDC hdcBackBuffer;
	static HBITMAP hBitmap;
	static HBITMAP hOldBitmap;

	switch (msg)
	{
		//A WM_CREATE msg is sent when your application window is first created
	case WM_CREATE:
	{
		//TO get the size of hte client window first we need to create
		//a Rect and then ask Windows to fill in our RECT structure with
		//the client window size. when we assign to cxClient and CyClient accordingly
		RECT rect;
		GetClientRect(hwnd, &rect);

		cxClient = rect.right;
		cyClient = rect.bottom;

		//Seen random number generator
		srand((unsigned)time(NULL));

		//---------------Create a surface to render to(backbuffer)

		//Create a memory device context
		hdcBackBuffer = CreateCompatibleDC(NULL);

		//Get the DC for the front buffer
		HDC hdc = GetDC(hwnd);
		hBitmap = CreateCompatibleBitmap(hdc, cxClient, cyClient);

		//Select the bitmap into the memory device context
		hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);

		//Don't forget to release the DC
		ReleaseDC(hwnd, hdc);

		g_GameWorld = new GameWorld(cxClient, cyClient);

		ChangeMenuState(hwnd, ID_CALCULATEMETHOD_PRIORITIZED, MFS_CHECKED);
		ChangeMenuState(hwnd, ID_VIEW_FPS, MFS_CHECKED);

	}

	break;

	case WM_COMMAND:
	{
		g_GameWorld->HandleMenuItems(wParam, hwnd);
	}

	break;

	case WM_LBUTTONUP:
	{
		g_GameWorld->SetCrosshair(MAKEPOINTS(lParam));
	}

	break;

	case WM_KEYUP:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			SendMessage(hwnd, WM_DESTROY, NULL, NULL);
		}

		break;
		case 'R':
		{
			delete g_GameWorld;

			g_GameWorld = new GameWorld(cxClient, cyClient);
		}

		break;

		}//End Switch

		//Handle any others
		g_GameWorld->HandleKeyPresses(wParam);
	}

	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		BeginPaint(hwnd, &ps);

		//Fill our backbuffer with white
		BitBlt(hdcBackBuffer, 0, 0, cxClient, cyClient, NULL, NULL, NULL, WHITENESS);

		gdi->StartDrawing(hdcBackBuffer);

		g_GameWorld->Render();

		gdi->StopDrawing(hdcBackBuffer);

		//Now blit backbuffer to front
		BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY);

		EndPaint(hwnd, &ps);
	}

	break;

	case WM_DESTROY:
	{
		//Cleanu up our backbuffer objects
		SelectObject(hdcBackBuffer, hOldBitmap);

		DeleteDC(hdcBackBuffer);
		DeleteObject(hBitmap);

		//Kill the application, this sends a WM_QUIT message
		PostQuitMessage(0);
	}
	break;


	default:
		break;
	}		//End switch

	//This is where all the message not spacifically handled by our
	//winproc are sent to be processed
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


//-------------------------------- WinMain -------------------------------
//
//	The entry point of the windows program
//------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     szCmdLine,
	int       iCmdShow)
{
	//handle to our window
	HWND						hWnd;

	//our window class structure
	WNDCLASSEX     winclass;

	//First fill in the window class structure
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = NULL;
	winclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	winclass.lpszClassName = g_szWindowClassName;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);



}
