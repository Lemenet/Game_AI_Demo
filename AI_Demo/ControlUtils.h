#pragma once


#include <windows.h>
#include <conio.h>
#include <iostream>

inline void SetTextColor(WORD color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, color);
}