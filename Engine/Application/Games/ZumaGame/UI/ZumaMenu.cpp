#include "ZumaMenu.hpp"
#include <filesystem>

int ZumaMenu::nextRouteID = 0;

ZumaMenu::ZumaMenu(std::string name, int x, int y, int width, int height) : UIWindow(name, x, y, width, height), EventObserver(UI)
{
	
}

void ZumaMenu::render()
{
	windowBegin();

	auto& routes = SystemsManager::getInstance().getSystemBin<RouteS>();

	if (ImGui::Button("Reset"))
	{
		for (auto& item : routes)
		{
			RouteS* route = (RouteS*)item.second;

			route->reset();
		}

		EntityManager::getInstance().update();
	}

	if(ImGui::BeginTabBar("Tabs"))
	{
		if (ImGui::BeginTabItem("Routes"))
		{
			if (ImGui::Button("Add route"))
			{
				std::string routeName = "Route" + std::to_string(nextRouteID++);
				routes[routeName] = new RouteS(routeName, {{-100, 200}}, 10, 2, 100, true);
			}
			ImGui::Separator();

			bool selected = false;
			for (auto& item : routes)
			{
				RouteS* route = (RouteS*)item.second;

				if (item.first == selectedRoute)
					selected = true;
				else
					selected = false;

				if (ImGui::Selectable(item.first.c_str(), selected))
				{
					auto it = routes.find(selectedRoute);
					if (it != routes.end())
						((RouteS*)(*it).second)->highlight = false;

					selectedRoute = item.first;
					route->highlight = true;
					ImGui::OpenPopup("Route Editor");
				}
			}

			if (ImGui::BeginPopupModal("Route Editor"))
			{
				RouteS* route = (RouteS*)routes[selectedRoute];

				ImGui::InputFloat("Spacing", &route->distanceBetween);
				ImGui::InputFloat("Speed", &route->speed);
				ImGui::InputInt("Pop Threshold", &route->popThreshold);

				static int nMarbles = 10;
				if (ImGui::InputInt("No Marbles", &nMarbles))
					route->setNumberOfMarbles(nMarbles);

				if (ImGui::Button("Delete")) {
					SystemsManager::getInstance().deleteSystem<RouteS>(selectedRoute);
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("Reset"))
					route->reset();

				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();

				ImGui::EndPopup();
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Shooters"))
		{
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("De/Serialization"))
		{
			if (ImGui::Button("Serialize"))
			{

			}
			
			auto path = std::filesystem::path("Application/Games/ZumaGame/Maps");
			ImGui::Text("Saved maps");
			for (auto& item : std::filesystem::directory_iterator(path))
			{
				if (ImGui::Selectable(item.path().string().c_str()))
				{

				}
			}

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	windowEnd();
}

void ZumaMenu::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::RouteSelection:
	{
		std::string* routeName = (std::string*)event.getPayload();

		auto routes = SystemsManager::getInstance().getSystemBin<RouteS>();

		auto it = routes.find(*routeName);

		if (it != routes.end()) {
			((RouteS*)(*it).second)->highlight = true;
			if (routes.find(selectedRoute) != routes.end())
				((RouteS*)routes[selectedRoute])->highlight = false;
		}
		selectedRoute = *routeName;
	}
	break;
	}
}
