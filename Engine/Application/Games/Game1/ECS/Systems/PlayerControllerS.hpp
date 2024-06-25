#pragma once

#include "../../src/ECS2/Systems/SysBase.hpp"
#include "../../src/Input/InputManager.hpp"

class PlayerControllerS : public SysBase
{
public:
	PlayerControllerS(InputManager& input);
	void update(float dt) override;

	float maxVelocity = 50.0f;
	float velocityIncrease = 1.0f;
private:
	InputManager& input;
};