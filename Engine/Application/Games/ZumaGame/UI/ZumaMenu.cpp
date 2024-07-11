#include "ZumaMenu.hpp"

ZumaMenu::ZumaMenu(std::string name, int x, int y, int width, int height, std::unordered_map<std::string, SysBase*>& routes) : UIWindow(name, x, y, width, height), routes(routes)
{
}

void ZumaMenu::render()
{
	windowConfig();

	ImGui::Separator();
	ImGui::Text("Routes");
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
	{
		for (auto& item : routes)
		{
			RouteS* route = (RouteS*)item.second;

			route->reset();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Add"))
	{
		routes["Route2"] = new RouteS("Route2", { {-100, 200} }, 10, 2, 100, true);
	}
	ImGui::Separator();

	for (auto& item : routes)
	{
		RouteS* route = (RouteS*)item.second;

		if (ImGui::Selectable(item.first.c_str()))
		{
			routename = item.first;
			ImGui::OpenPopup("Route Editor");
		}
	}

	if (ImGui::BeginPopupModal("Route Editor"))
	{
		RouteS* route = (RouteS*)routes[routename];

		ImGui::InputFloat("Spacing", &route->distanceBetween);
		ImGui::InputFloat("Speed", &route->speed);
		ImGui::InputInt("Pop Threshold", &route->popThreshold);

		static int nMarbles = 10;
		if (ImGui::InputInt("No Marbles", &nMarbles))
			route->setNumberOfMarbles(nMarbles);

		if (ImGui::Button("Reset"))
			route->reset();

		if (ImGui::Button("Close"))
		{
			routename = "";
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	windowEnd();
}
