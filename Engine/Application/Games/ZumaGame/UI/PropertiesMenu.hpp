#pragma once

#include "UIWindow.hpp"
#include "../../src/Events/EventManager.hpp"

class PropertiesMenu : public UIWindow, public EventObserver
{
public:
	PropertiesMenu(std::string name, int x, int y, int width, int height);
	virtual void render() override;
	virtual void handleEvent(Event& event) override;

	int selectedEntID=-1;
};