#pragma once

#include "../../Application.hpp"

#include "../../src/ECS2/Systems/MovementS.hpp"
#include "../../src/ECS2/Systems/CollisionS.hpp"
#include "../../src/ECS2/Systems/RenderingS.hpp"

class ZumaGame : public Application
{
public:
	ZumaGame(float windowWidth, float windowHeight, std::string windowName) : Application(windowWidth, windowHeight, windowName) {}
	virtual void initialize();
};