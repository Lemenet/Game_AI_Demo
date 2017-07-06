#include "ParamLoader.h"

ParamLoader * ParamLoader::Instance()
{
	static ParamLoader instance_;
	return &instance_;
}
