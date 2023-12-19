#pragma once

#include "InterfaceWindow.hpp"

class TopBar : public InterfaceWindow
{
public:
	TopBar(int x, int y, int width, int height);
	virtual void draw() override;
private:
	std::string button1Name = "Start";
};