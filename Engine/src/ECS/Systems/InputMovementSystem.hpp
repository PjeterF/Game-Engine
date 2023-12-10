#pragma once

#include "SystemBase.hpp"
#include "../../Input/InputManager.hpp"
#include "../Components/TransformC.hpp"
#include "../Components/VelocityC.hpp"

class InputMovementSystem : public SystemBase
{
public:
	InputMovementSystem(InputManager* inputManager);
	virtual void update(float dt) override;
private:
	InputManager* input;
};