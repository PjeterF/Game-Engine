#pragma once

#include "keyCodes.h"
#include <glm/vec2.hpp>

class InputManager
{
public:
	virtual bool isKeyPressed(int keycode) = 0;
	virtual bool isMouseButtonPressed(int keycode) = 0;

	virtual bool wasKeyReleased(int keycode) = 0;
	virtual bool wasMouseButtonReleased(int keycode) = 0;

	virtual bool wasKeyPressed(int keycode) = 0;
	virtual bool wasMouseButtonPressed(int keycode) = 0;

	virtual float mouseWheel() = 0;

	virtual glm::vec2 getCursorPos() = 0;
};