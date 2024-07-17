#include "PropertiesMenu.hpp"
#include "../../src/ECS2/EntityManager.hpp"
#include "../../src/ECS2/ComponentPoolManager.hpp"

#include "../../src/ECS2/Components/Transform.hpp"
#include "../../src/ECS2/Components/Velocity.hpp"
#include "../../src/ECS2/Components/AABB.hpp"
#include "../../src/ECS2/Components/Sprite.hpp"
#include "../../src/ECS2/Components/Animation.hpp"
	
PropertiesMenu::PropertiesMenu(std::string name, int x, int y, int width, int height) : UIWindow(name, x, y, width, height), EventObserver(UI)
{
}

void PropertiesMenu::render()
{
	windowBegin();

	hovered = ImGui::IsWindowHovered();

	ImGui::Separator();
	if (selectedEntID == -1)
		ImGui::Text("Selected entity: NONE");
	else
		ImGui::Text((std::string("Selected entity: ") + std::to_string(selectedEntID)).c_str());
	ImGui::Separator();

	if (selectedEntID == -1)
	{
		windowEnd();
		return;
	}

	if (ImGui::BeginCombo("##Add component", "Add component"))
	{
		if (ImGui::Selectable("Transform"))
		{

		}

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("##Remove component", "Remove component"))
	{
		if (ImGui::Selectable("Transform"))
		{

		}

		ImGui::EndCombo();
	}

	if (ImGui::Button("Delete"))
	{
		EntityManager::getInstance().deleteEntity(selectedEntID);
		selectedEntID = -1;

		windowEnd();
		return;
	}

	ImGui::Separator();

	if (!EntityManager::getInstance().entityExists(selectedEntID))
	{
		windowEnd();
		return;
	}

	Entity ent(selectedEntID);

	ImGui::BeginChild("##List");

	hovered = ImGui::IsWindowHovered();

	if (ent.hasComponent<Transform>())
	{
		if (ImGui::TreeNode("Transform"))
		{
			auto& comp = ent.getComponent<Transform>();
			auto nodeWidth = ImGui::CalcItemWidth();

			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::InputFloat("##x", &comp.x);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::InputFloat("##y", &comp.y);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::Text("x, y");

			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::InputFloat("##width", &comp.width);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::InputFloat("##height", &comp.height);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::Text("width, height");

			ImGui::SetNextItemWidth(nodeWidth / 3 * 2 + 8);
			ImGui::InputFloat("##rot", &comp.rot);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::Text("rotation");

			ImGui::Checkbox("rotate to direction", &comp.rotateToDir);

			ImGui::TreePop();
		}
	}
	if (ent.hasComponent<Velocity>())
	{
		if (ImGui::TreeNode("Veloicty"))
		{
			auto& comp = ent.getComponent<Velocity>();
			auto nodeWidth = ImGui::CalcItemWidth();

			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::InputFloat("##x", &comp.x);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::InputFloat("##y", &comp.y);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::Text("vx, vy");

			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::InputFloat("##ax", &comp.ax);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::InputFloat("##ay", &comp.ay);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::Text("ax, ay");

			ImGui::SetNextItemWidth(nodeWidth / 3 * 2 + 8);
			ImGui::InputFloat("##decay", &comp.decay);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::Text("velocity decay");

			ImGui::TreePop();
		}
	}
	if (ent.hasComponent<AABB>())
	{
		if (ImGui::TreeNode("Physics body"))
		{
			auto& comp = ent.getComponent<AABB>();
			auto nodeWidth = ImGui::CalcItemWidth();

			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::InputFloat("##width", &comp.width);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::InputFloat("##height", &comp.height);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::Text("width, height");

			ImGui::SetNextItemWidth(nodeWidth / 3 * 2 + 8);
			ImGui::InputFloat("##mass", &comp.mass);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 3);
			ImGui::Text("mass");

			ImGui::Checkbox("collisions enabled", &comp.enabled);

			ImGui::TreePop();
		}
	}
	if (ent.hasComponent<Sprite>())
	{
		if (ImGui::TreeNode("Sprite"))
		{
			auto& comp = ent.getComponent<Sprite>();
			auto nodeWidth = ImGui::CalcItemWidth();

			ImGui::SetNextItemWidth(nodeWidth / 5);
			ImGui::InputFloat("##xDivision", &comp.textureSample.x);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 5);
			ImGui::InputFloat("##yDivision", &comp.textureSample.y);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 5);
			ImGui::InputFloat("##widthDivision", &comp.textureSample.z);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 5);
			ImGui::InputFloat("##heightDivision", &comp.textureSample.w);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(nodeWidth / 5);
			ImGui::Text("division");

			if (ImGui::BeginCombo("texture", nullptr, ImGuiComboFlags_NoPreview))
			{
				auto it = ResourceManager::getInstance().begin<Texture>();
				auto it_end = ResourceManager::getInstance().end<Texture>();
				while (it != it_end)
				{
					if ((*it).first == "" || (*it).first == "default") {
						it++;
						continue;
					}

					Resource<Texture>* texture = (Resource<Texture>*)(*it).second;
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::Image((ImTextureID)texture->getContents()->getId(), ImVec2(15, 15), ImVec2(0, 0), ImVec2(-1, -1));
					ImGui::SameLine();
					if (ImGui::Selectable((*it).first.c_str()))
					{
						comp.setTexture(texture);
					}
					it++;
				}
				ImGui::EndCombo();
			}
			ImGui::Checkbox("collisions enabled", &comp.flipHorizontally);

			ImGui::TreePop();
		}
		if (ent.hasComponent<Animation>())
		{
			if (ImGui::TreeNode("Animation"))
			{
				auto& comp = ent.getComponent<Animation>();
				auto nodeWidth = ImGui::CalcItemWidth();

				ImGui::SetNextItemWidth(nodeWidth / 2);
				if (ImGui::InputInt("##currentFrame", &comp.currentFrame))
				{
					if (comp.currentFrame >= comp.framesMap[comp.state].size()) {
						comp.currentFrame = comp.framesMap[comp.state].size() - 1;
					}
					else if (comp.currentFrame <= 0) {
						comp.currentFrame = 0;
					}
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(nodeWidth / 2);
				ImGui::Text("current frame");

				ImGui::SetNextItemWidth(nodeWidth / 2);
				if (ImGui::InputInt("##frameDuration", &comp.frameDuration))
				{
					if (comp.frameDuration < 1)
						comp.frameDuration = 1;
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(nodeWidth / 2);
				ImGui::Text("frame duration");

				ImGui::Text("Animation states' frames");
				int i = 0;
				for (auto& state : comp.framesMap)
				{
					if(ImGui::TreeNode(std::to_string(state.first).c_str()))
					{
						ImGui::Text("x, y, width, height");
						for (auto& frame : state.second)
						{
							ImGui::PushID(i);
							
							ImGui::SetNextItemWidth(nodeWidth / 4);
							ImGui::InputFloat("##xDivisionFrame", &frame.x);
							ImGui::SameLine();
							ImGui::SetNextItemWidth(nodeWidth / 4);
							ImGui::InputFloat("##yDivisionFrame", &frame.y);
							ImGui::SameLine();
							ImGui::SetNextItemWidth(nodeWidth / 4);
							ImGui::InputFloat("##widthDivisionFrame", &frame.z);
							ImGui::SameLine();
							ImGui::SetNextItemWidth(nodeWidth / 4);
							ImGui::InputFloat("##heightDivisionFrame", &frame.w);

							ImGui::PopID();

							i++;
						}

						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}
		}
	}

	ImGui::EndChild();

	windowEnd();
}

void PropertiesMenu::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::EntitySelection:
	{
		int* ID = (int*)(event.getPayload());
		selectedEntID = *ID;
	}
	break;
	}
}
