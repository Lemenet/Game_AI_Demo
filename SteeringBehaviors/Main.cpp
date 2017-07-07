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
