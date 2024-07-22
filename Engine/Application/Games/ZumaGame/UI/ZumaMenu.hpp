#pragma once

#include "UIWindow.hpp"

#include "../Scenes/GeneralZumaScene.hpp"
#include "../../src/ECS2/SystemsManager.hpp"
#include "../../src/Events/EventManager.hpp"

class ZumaMenu : public UIWindow, public EventObserver
{
public:
	ZumaMenu(std::string name, int x, int y, int width, int height);
	virtual void render() override;
	virtual void handleEvent(Event& event) override;
private:
	std::string selectedRoute="";
	static int nextRouteID;
};