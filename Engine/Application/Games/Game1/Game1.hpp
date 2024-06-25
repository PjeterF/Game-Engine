#pragma once

#include "../../Application.hpp"

class Game1 : public Application
{
public:
	Game1(float windowWidth, float windowHeight, std::string windowName) : Application(windowWidth, windowHeight, windowName) {}
	virtual void initialize();
};