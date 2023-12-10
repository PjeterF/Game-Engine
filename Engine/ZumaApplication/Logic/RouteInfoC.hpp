#pragma once

#include "../../src/ECS/Components/ComponentBase.hpp"
#include "RouteManagementSystem.hpp"

class RouteInfoC : public ComponentBase
{
public:
	RouteInfoC(int tag = -1, int targetSample = 0);
	int tag;
	int targetSample;
	bool moving = true;
	bool inserting = false;
	RouteManagementSystem* route = nullptr;
};