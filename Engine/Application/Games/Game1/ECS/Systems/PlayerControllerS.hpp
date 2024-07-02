#pragma once

#include "../../src/ECS2/Systems/SysBase.hpp"
#include "../../src/Input/InputManager.hpp"

class PlayerControllerS : public SysBase
{
public:
	PlayerControllerS(InputManager& input);
	void update(float dt) override;

	float velocityIncrease = 5.0f;
	float dashSpeed = 20;
private:
	InputManager& input;
};