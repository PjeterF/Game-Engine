#pragma once

#include "keyCodes.h"
#include <glm/vec2.hpp>

class InputManager
{
protected:
	static const int No_Keys = 512;
	static const int No_MouseKeys = 10;
public:
	virtual float mouseWheel() = 0;
	virtual void update() = 0;
	virtual glm::vec2 getCursorPos() = 0;

	bool keyDown[No_Keys];
	bool keyClicked[No_Keys];
	bool keyReleased[No_Keys];

	bool mouseKeyDown[No_MouseKeys];
	bool mouseKeyClicked[No_MouseKeys];
	bool mouseKeyReleased[No_MouseKeys];
	static float wheel;
};