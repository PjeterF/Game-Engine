#include "EntitiesMenu.hpp"
#include "../../src/ECS2/EntityManager.hpp"
#include "../../src/Events/EventManager.hpp"

EntitiesMenu::EntitiesMenu(std::string name, int x, int y, int width, int height) : UIWindow(name, x, y, width, height)
{
}

void EntitiesMenu::render()
{
	windowBegin();

	if (ImGui::Button("Create entity"))
	{
		EntityManager::getInstance().createEntity();
	}

	for (auto ID : EntityManager::getInstance().getExistingEntities())
	{
		if (ImGui::Selectable(std::to_string(ID).c_str()))
		{
			EventManager::getInstance().notify(Event(Event::EntitySelection, &ID), UI);
		}
	}
	windowEnd();
}
