#pragma once

#include "SysBase.hpp"

#include "../../Input/Windows/GLFWInputManager.hpp"

class UserControllerS : public SysBase
{
public:
	UserControllerS(InputManager* input);
	static void initialize(InputManager* input);
	static UserControllerS& getInstance();
	virtual void update(float dt) override;
private:
	static UserControllerS& instanceImp(InputManager* input);

	InputManager* input;
};