#pragma once

#include "../../Application.hpp"

class ZumaGame : public Application
{
public:
	ZumaGame(float windowWidth, float windowHeight, std::string windowName) : Application(windowWidth, windowHeight, windowName) {}
	virtual void initialze();
};