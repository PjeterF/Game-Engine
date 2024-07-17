#pragma once

#include "UIWindow.hpp"

#include "../Scenes/GeneralZumaScene.hpp"
#include "../../src/ECS2/SystemsManager.hpp"

class ZumaMenu : public UIWindow
{
public:
	ZumaMenu(std::string name, int x, int y, int width, int height);
	virtual void render() override;
private:
	std::string routename="";
};