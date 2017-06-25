#pragma once

#pragma comment(lib, "winmm.lib")

#include <windows.h>

#define Clock CrudeTimer::Instance()

class CrudeTimer
{
private:

	double m_dStartTime;


	CrudeTimer()
	{
		m_dStartTime = timeGetTime() * 0.001;
	}

	CrudeTimer(const CrudeTimer&);
	CrudeTimer& operator = (const CrudeTimer&);

public:
	
	static CrudeTimer* Instance();

	double GetCurrentTime()
	{
		return timeGetTime()*0.001 - m_dStartTime;
	}
};

