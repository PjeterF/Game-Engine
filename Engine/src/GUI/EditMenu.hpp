#pragma once

#include "GUI_Window.hpp"
#include "Subcomponents/TextureLoader.hpp"
#include "Subcomponents/RouteEdit.hpp"
#include "Viewport.hpp"

#include "../Managers/TexturesManager.hpp"
#include "Subcomponents/Subcomponent.hpp"

#include "../Events/EventPackage.hpp"

class EditMenu : public GUI_Window
{
public:
	EditMenu(int xmin, int ymin, int xmax, int ymax);
	void render(RenderingAPI* renderer);
	void addSubcomponent(Subcomponent* subcomponent);
private:
	std::vector<Subcomponent*> subcomponents;
};

