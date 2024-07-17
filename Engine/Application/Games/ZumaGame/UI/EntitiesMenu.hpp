#pragma once

#include "UIWindow.hpp"

class EntitiesMenu : public UIWindow
{
public:
	EntitiesMenu(std::string name, int x, int y, int width, int height);
	virtual void render() override;
};