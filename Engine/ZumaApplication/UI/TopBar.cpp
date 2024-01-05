#include "TopBar.hpp"
#include "../../src/Events/EventManager.hpp"
#include "../Serialization/serialization.hpp"

TopBar::TopBar(int x, int y, int width, int height) : InterfaceWindow(x, y, width, height)
{
	name = "Main bar";
}

void TopBar::draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	if (ImGui::Button(button1Name.c_str(), ImVec2(width, height)))
	{
		if (button1Name == "Start")
		{
			EventManager::getInstance().notifyAllBins(Event(Event::Unpause, nullptr));
			button1Name = "Pause";
		}
		else if (button1Name == "Pause")
		{
			EventManager::getInstance().notifyAllBins(Event(Event::Pause, nullptr));
			button1Name = "Start";
		}
	}

	ImGui::End();
	ImGui::PopStyleVar(2);
}
