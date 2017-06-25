#pragma once
#include <windows.h>
#include <conio.h>
#include "ControlUtils.h"


inline void SetTextColor(unsigned short color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, color);
}