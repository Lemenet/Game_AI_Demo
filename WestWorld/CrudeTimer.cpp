#include "CrudeTimer.h"



CrudeTimer* CrudeTimer::Instance()
{
	static CrudeTimer instance_;
	return &instance_;
}
