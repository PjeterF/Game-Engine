#pragma once

#include <string>

struct MarbleComponent
{
	MarbleComponent(int type = -1, std::string routeName="", int targetPointIdx =-1, bool separated = false)
		: type(type), routeName(routeName), targetPointIdx(targetPointIdx), separated(separated) {}
	int type;
	std::string routeName;
	int targetPointIdx;
	bool separated;
};