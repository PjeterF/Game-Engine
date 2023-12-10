#pragma once

#include "InterfaceWindow.hpp"
#include "../../src/Events/EventManager.hpp"

class SceneMenu : public InterfaceWindow, public EventObserver
{
public:
	SceneMenu(int x, int y, int width, int height);
	virtual void draw();
	virtual void handleEvent(Event& event) override;
private:
};