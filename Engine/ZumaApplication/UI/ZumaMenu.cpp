#include "ZumaMenu.hpp"

ZumaMenu::ZumaMenu(int x, int y, int width, int height, std::vector<RouteManagementSystem*>* routes) : InterfaceWindow(x, y, width, height), EventObserver(UI)
{
	this->routes = routes;
    name = "ZumaMenu";
}

void ZumaMenu::draw()
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(width, height));
    ImGui::SetNextWindowPos(ImVec2(x, y));
    ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if (ImGui::BeginTabBar("Zuma"))
    {
        if (ImGui::BeginTabItem("Marble types"))
        {
			ImVec2 wndSize = ImGui::GetWindowSize();

			if (ImGui::Button("Create segment"))
			{
				TextureDivision division(0, 0, 1, 1);

				MarbleTemplate newTemplate = MarbleTemplate(10, -1, "src/Textures/marble1.png", { division }, 30, { 1, 1, 1 });

				RouteManagementSystem::marbleTemplates.push_back(newTemplate);
				EventManager::getInstance().notify(Event(Event::ReinitializeRoutes, nullptr));
			}

			ImGui::Text("Current segments");

			static int templateIndex = 0;
			int i = 0;
			for (auto it = RouteManagementSystem::marbleTemplates.begin(); it != RouteManagementSystem::marbleTemplates.end(); it++)
			{
				Resource<Texture>* tex = (Resource<Texture>*)ResourceManager::getInstance().getResource<Texture>((*it).textureFilepath);

				if (tex->getContents() != nullptr)
				{
					if (ImGui::ImageButton((ImTextureID)tex->getContents()->getId(), ImVec2(50, 50)))
					{
						templateIndex = i;
						ImGui::OpenPopup("Editor");
					}
				}
				else
				{
					if (ImGui::ImageButton((ImTextureID)0, ImVec2(50, 50)))
					{
						templateIndex = i;
						ImGui::OpenPopup("Editor");
					}
				}

				i++;
			}

			if (ImGui::BeginPopupModal("Editor", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				static int tag = -1;
				ImGui::InputInt("Tag", &tag);
				static int size = 30;
				ImGui::InputInt("Size", &size);
				static int frameDuration = 10;
				ImGui::InputInt("Frame duration", &frameDuration);
				static int dimensions[2] = { 1, 1 };
				ImGui::InputInt2("Sprite sheet dimensions", dimensions);
				static float explosionColor[3] = { 1, 1, 1 };
				ImGui::InputFloat3("Explosion color", explosionColor);

				static Resource<Texture>* texture = ResourceManager::getInstance().getResource<Texture>("default");
				if (texture->getContents() != nullptr)
					ImGui::Image((ImTextureID)texture->getContents()->getId(), ImVec2(30, 30));
				else
					ImGui::Image((ImTextureID)0, ImVec2(30, 30));
				ImGui::SameLine();

				if (ImGui::BeginCombo("Texture", nullptr, ImGuiComboFlags_NoPreview))
				{
					auto it = ResourceManager::getInstance().begin<Texture>();
					while (it != ResourceManager::getInstance().end<Texture>())
					{
						std::string name = " * " + (*it).first;
						if (ImGui::Selectable(name.c_str()))
						{
							texture = (Resource<Texture>*)(*it).second;
						}
						ImGui::SameLine();

						Resource<Texture>* res = (Resource<Texture>*)(*it).second;
						if (res->getContents() == nullptr)
							ImGui::Image((ImTextureID)0, ImVec2(10, 10));
						else
							ImGui::Image((ImTextureID)res->getContents()->getId(), ImVec2(10, 10));

						ImGui::SameLine();
						ImGui::Text(" ");
						it++;
					}
					ImGui::EndCombo();
				}

				if (ImGui::Button("Commit changes"))
				{
					RouteManagementSystem::marbleTemplates[templateIndex] = MarbleTemplate(size, tag, texture->getContents()->getFilepath(), createDivisions(dimensions[0], dimensions[1], texture->getContents()), frameDuration, { explosionColor[0], explosionColor[1] , explosionColor[2] });
					EventManager::getInstance().notify(Event(Event::ReinitializeRoutes, nullptr));
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();

				ImGui::Dummy(ImVec2(50, 0));

				ImGui::SameLine();
				if (ImGui::Button("Delete"))
				{
					int n = 0;
					for (auto it = RouteManagementSystem::marbleTemplates.begin(); it != RouteManagementSystem::marbleTemplates.end(); it++)
					{
						if (n == templateIndex)
						{
							RouteManagementSystem::marbleTemplates.erase(it);
							EventManager::getInstance().notify(Event(Event::ReinitializeRoutes, nullptr));
							break;
						}
						n++;
					}
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();

				ImGui::Dummy(ImVec2(50, 0));

				ImGui::SameLine();
				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Route management"))
		{
			if (ImGui::BeginCombo("Routes", nullptr, ImGuiComboFlags_NoPreview))
			{
				if (ImGui::Button("Add route"))
				{
					Spline* newSpline = new Spline
					(
						0,
						0,
						100,
						ResourceManager::getInstance().getResource<Texture>("src/textures/control_point.png"),
						ResourceManager::getInstance().getResource<Texture>("src/textures/control_point2.png"),
						5
					);

					newSpline->addSegment(1000, 1000);

					auto newRoute = new RouteManagementSystem(newSpline);
					
				}

				auto it = routes->begin();
				while (it != routes->end())
				{
					if (ImGui::Selectable((*it)->getName().c_str()))
					{
						selectedRoute = *it;
					}
					it++;
				}

				ImGui::EndCombo();
			}
			ImGui::Separator();

			if (selectedRoute != nullptr)
				routeProperties(selectedRoute);

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
	ImGui::PopStyleVar(1);
}

void ZumaMenu::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::RouteSelection:
	{
		RouteManagementSystem* route = (RouteManagementSystem*)event.getPayload();
		selectedRoute = route;
	}
	break;
	case Event::RouteDeletion:
	{
		if (selectedRoute == event.getPayload())
			selectedRoute = nullptr;
	}
	break;
	case Event::PlacePoint:
	{
		if (selectedRoute != nullptr)
		{
			glm::vec2 pos = *(glm::vec2*)event.getPayload();
			selectedRoute->addRoutePoint(pos);
		}
	}
	break;
	case Event::RemoveLastPoint:
	{
		if (selectedRoute != nullptr)
		{
			selectedRoute->removeLastRoutePoint();
		}
	}
	break;
	}
}

std::vector<TextureDivision> ZumaMenu::createDivisions(int dimX, int dimY, Texture* texture)
{
	std::vector<TextureDivision> templates;

	for (int x = 0; x < dimX; x++)
	{
		for (int y = 0; y < dimY; y++)
		{
			templates.push_back(TextureDivision(x * texture->getWidth() / dimX, texture->getHeight() / dimY, texture->getWidth() / dimX, texture->getHeight() / dimY));
		}
	}
	return templates;
}

void ZumaMenu::routeProperties(RouteManagementSystem* route)
{
	static int layer = 0;
	if (ImGui::InputInt("layer", &layer))
	{
		route->setLayer(layer);
	}

	ImGui::InputFloat("speed", &route->marbleSpeed);
	static int NoMarbles = 100;
	if(ImGui::InputInt("remaining marbles", &NoMarbles))
	{
		route->setInitialMarbleCount(NoMarbles);
	}
}
