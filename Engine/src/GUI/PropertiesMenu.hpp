#pragma once

#include <string>

#include "../Events/EventPackage.hpp"
#include "../ZumaLogic/Sprite.hpp"
#include "../ZumaLogic/CubicBezierSpline.hpp"
#include "../ZumaLogic/Snake.hpp"
#include "../ZumaLogic/Shooter.hpp"
#include "../ZumaLogic/Segment.hpp"
#include "../ZumaLogic/Route.hpp"
#include "GUI_Window.hpp"
#include "../Managers/TexturesManager.hpp"
#include "../Managers/EntityManager.hpp"
#include "../Managers/Resource/ResourceManager.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class PropertiesMenu : public EventObserver, public GUI_Window 
{
public:
	PropertiesMenu(std::string name, int xmin, int ymin, int xmax, int ymax, TexturesManager* texManager);
	void render();
	virtual void handleEvent(Event& event);
private:
	void entityProperties(Entity* ent);
	void spriteProperties(class Sprite* ent);
	void snakeProperties(Snake* ent);
	void routeProperties(Route* ent);
	void segmentProperties(Segment* ent);
	void shooterProperties(Shooter* ent);

	TexturesManager* texManager;
	Entity* currentEnt = nullptr;
	std::string name;
};