#include "SceneMenu.hpp"

#include "../../src/ECS/Entity/EntManager.hpp"

SceneMenu::SceneMenu(int x, int y, int width, int height) : InterfaceWindow(x, y, width, height), EventObserver(UI)
{
	name = "Scene manager";
}

void SceneMenu::draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	if (ImGui::Button("Create entity"))
	{
		EntManager::getInstance().createEntity();
	}
	ImGui::Separator();

	auto it = EntManager::getInstance().begin();
	while (it != EntManager::getInstance().end())
	{
		if (ImGui::Selectable(("Entity(" + std::to_string((*it).first) + ")").c_str()))
		{
			EventManager::getInstance().notify(Event(Event::EntitySelection, (*it).second), ObsBin::UI);
		}

		it++;
	}

	ImGui::End();
	ImGui::PopStyleVar(1);
}

void SceneMenu::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::EntitySelection:
	{

	}
	break;
	}
}
