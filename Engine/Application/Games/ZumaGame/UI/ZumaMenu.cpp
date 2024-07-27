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
			if (ImGui::Button("Create Shooter"))
			{
				Entity shooter = EntityManager::getInstance().createEntity();
				shooter.addComponent<MarbleShooter>(MarbleShooter(1, 10));
				shooter.addComponent<Transform>(Transform(0, 0, 100, 100));
				shooter.addComponent<Velocity>(Velocity());
				shooter.addComponent<Sprite>(Sprite());

				SystemsManager::getInstance().getSystem<RenderingS>()->addEntity(shooter.getID());
				SystemsManager::getInstance().getSystem<ShooterS>()->addEntity(shooter.getID());

				int payload = shooter.getID();
				EventManager::getInstance().notify(Event(Event::EntitySelection, &payload), UI);
			}

			for (auto& ID : SystemsManager::getInstance().getSystem<ShooterS>()->getEntitySet())
			{
				if (ImGui::Selectable(std::to_string(ID).c_str()))
				{
					int payload = ID;
					EventManager::getInstance().notify(Event(Event::EntitySelection, &payload), UI);
				}	
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("De/Serialization"))
		{
			std::string p = "Application/Games/ZumaGame/Maps";
			GeneralZumaScene* scene = (GeneralZumaScene*)SceneManager::getInstance().getCurrentScene();

			static char serializationName[100]="";
			ImGui::InputText("File Name", serializationName, 100);
			if (ImGui::Button("Serialize"))
			{
				if (std::strcmp(serializationName, "")!=0) {
					scene->serialize(p + "/" + serializationName);
					std::strcpy(serializationName, "");
				}
			}
			
			auto path = std::filesystem::path(p);
			ImGui::Separator();
			ImGui::Text("Saved maps");
			ImGui::Separator();
			for (auto& item : std::filesystem::directory_iterator(path))
			{
				std::string itemName = item.path().string();
				itemName.erase(0, p.size()+1);
				if (ImGui::Selectable(itemName.c_str()))
					scene->deSerialize(item.path().string().c_str());
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
