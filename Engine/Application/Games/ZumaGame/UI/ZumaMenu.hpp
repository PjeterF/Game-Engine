#pragma once

#include "UIWindow.hpp"

#include "../Scenes/GeneralZumaScene.hpp"

class ZumaMenu : public UIWindow
{
public:
	ZumaMenu(std::string name, int x, int y, int width, int height, std::unordered_map<std::string, SysBase*>& routes);
	virtual void render() override;
private:
	std::unordered_map<std::string, SysBase*>& routes;
	std::string routename="";
};