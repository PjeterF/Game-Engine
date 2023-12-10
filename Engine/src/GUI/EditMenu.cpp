#include "EditMenu.hpp"
#include "../Loading/Loader.hpp"
#include "SpriteSheetEditor.hpp"

EditMenu::EditMenu(int xmin, int ymin, int xmax, int ymax) : GUI_Window(xmin, ymin, xmax, ymax)
{

}

void EditMenu::addSubcomponent(Subcomponent* subcomponent)
{
	subcomponents.push_back(subcomponent);
}

void EditMenu::render(RenderingAPI* renderer)
{
	ImGui::SetNextWindowSize(ImVec2(xmax - xmin, ymax - ymin));
	ImGui::SetNextWindowPos(ImVec2(xmin, ymin));

	ImGui::Begin("EditMenu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	ImVec2 windowSize = ImGui::GetWindowSize();
	if(ImGui::Button("Start", ImVec2(windowSize.x, 30)))
	{
		EventManager::getInstance().notify(Event(Event::TogglePause, nullptr));
	}

	if (ImGui::Button("Load scene", ImVec2(windowSize.x, 30)))
	{
		ImGui::OpenPopup("Scene loader");
	}
	
	if (ImGui::Button("Save scene", ImVec2(windowSize.x, 30)))
	{
		ImGui::OpenPopup("Scene saver");
	}

	/*if (ImGui::Button("Open Sprite Sheet Editor", ImVec2(windowSize.x, 30)))
	{
		ImGui::OpenPopup("Sprite Sheet Editor");
	}*/

	if (ImGui::BeginPopupModal("Scene loader", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static char buf[1000];
		ImGui::InputText("Scene filepath", buf, 1000);

		if (ImGui::Button("Load"))
		{
			try
			{
				Loader::deserializeScene(&EntityManager::getInstance(), buf);
			}
			catch(...)
			{
				if (ImGui::BeginPopupModal("Error loading", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Error loading");
					if (ImGui::Button("Close"))
					{
						ImGui::CloseCurrentPopup();
					}
				}
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		
		ImGui::Dummy(ImVec2(215, 0));
		
		ImGui::SameLine();
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Scene saver", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static char buf[1000];
		ImGui::InputText("Scene filepath", buf, 1000);

		if (ImGui::Button("Save"))
		{
			try
			{
				Loader::serializeScene(&EntityManager::getInstance(), buf);
			}
			catch (...)
			{
				if (ImGui::BeginPopupModal("Error saving", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Error saving");
					if (ImGui::Button("Close"))
					{
						ImGui::CloseCurrentPopup();
					}
				}
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();

		ImGui::Dummy(ImVec2(215, 0));

		ImGui::SameLine();
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Sprite Sheet Editor"))
	{
		static SpriteSheetEditor* spriteSheetEditor = new SpriteSheetEditor(500, 500);
		spriteSheetEditor->render();

		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	for (auto component : subcomponents)
	{
		if (ImGui::TreeNode(component->getName().c_str()))
		{
			component->render();
			ImGui::TreePop();
		}
		ImGui::NewLine();
	}

	if(ImGui::Button("Toggle collision grid"))
	{
		EventManager::getInstance().notify(Event(Event::ToggleCollisionGridVisibility, nullptr));
	}

	if (ImGui::Button("Center camera"))
	{
		EventManager::getInstance().notify(Event(Event::CenterCamera, nullptr));
	}

	ImGui::End();
}