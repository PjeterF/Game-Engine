#pragma once

#include "InterfaceWindow.hpp"
#include "../Logic/RouteManagementSystem.hpp"
#include "../../src/Events/EventManager.hpp"

class ZumaMenu : public InterfaceWindow, public EventObserver
{
public:
	ZumaMenu(int x, int y, int width, int height);
	virtual void draw() override;
	virtual void handleEvent(Event& event) override;
private:
	std::vector<TextureDivision> createDivisions(int x, int y, Texture* texture);
	void routeProperties(RouteManagementSystem* route);

	std::vector<RouteManagementSystem*> routes;
	RouteManagementSystem* selectedRoute = nullptr;
};