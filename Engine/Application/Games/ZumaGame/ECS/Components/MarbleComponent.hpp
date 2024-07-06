#pragma once

#include <string>

struct MarbleComponent
{
	MarbleComponent(int type = -1, std::string routeName="", int targetPointIdx =-1)
		: type(type), routeName(routeName), targetPointIdx(targetPointIdx) {}
	int type;
	std::string routeName;
	int targetPointIdx;
	bool shouldMove = false;
};