#include "EntityPropertiesMenu.hpp"
#include "../../src/ECS/Systems/SystemsManager.hpp"
#include "../../src/Events/EventManager.hpp"

EntityPropertiesMenu::EntityPropertiesMenu(int x, int y, int width, int height) : InterfaceWindow(x, y, width, height), EventObserver(UI)
{
	name = "Properties";
	EventManager::getInstance().addObserver(this, ECS);
}

void EntityPropertiesMenu::draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	if (selectedEntity == nullptr)
	{
		ImGui::End();
		ImGui::PopStyleVar(1);
		return;
	}
	if (ImGui::Button("DELETE"))
	{
		selectedEntity->destroy();
		selectedEntity = nullptr;
		
		ImGui::End();
		ImGui::PopStyleVar(1);
		return;
	}
		
	ImGui::SameLine();
	ImGui::Text(("Entity ID: " + std::to_string(selectedEntity->getID())).c_str());
	ImGui::Separator();
	componentAdditionMenu();
	ImGui::Spacing();
	addToSystemMenu();
	ImGui::Separator();

	for (auto& component : selectedEntity->components)
	{
		switch (component.second->getType())
		{
		case Transform:
			transformProp((TransformC*)component.second);
		break;
		case Velocity:
			velocityProp((VelocityC*)component.second);
		break;
		case Sprite:
			spriteProp((SpriteC*)component.second);
		break;
		case BoxCollider:
			colliderProp((BoxColliderC*)component.second);
		break;
		case AnimatedSprite:
			animatedSpriteProp((AnimatedSpriteC*)component.second);
			break;
		}
	}

	ImGui::End();
	ImGui::PopStyleVar(1);
}

void EntityPropertiesMenu::selectEntity(Ent* entity)
{
	selectedEntity = entity;
}

void EntityPropertiesMenu::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::EntitySelection:
	{
		selectedEntity = (Ent*)event.getPayload();
	}
	break;
	case Event::EntityRemoval:
	{
		if (selectedEntity != nullptr)
		{
			if (((Ent*)event.getPayload())->getID() == selectedEntity->getID())
				selectedEntity = nullptr;
		}
	}
	break;
	}
}

void EntityPropertiesMenu::componentAdditionMenu()
{
	static int errorCounter = 0;

	if (ImGui::BeginCombo("Add component", nullptr, ImGuiComboFlags_NoPreview))
	{
		if (ImGui::Selectable("Transform"))
		{
			auto newComponent = new TransformC({ 0, 0 }, { 1, 1 }, 0);
			if (!selectedEntity->addComponent(newComponent))
			{
				errorCounter = 200;
				delete newComponent;
			}
		}
		if (ImGui::Selectable("Sprite"))
		{
			auto newComponent = new SpriteC(ResourceManager::getInstance().getResource<Texture>("default"));
			if (!selectedEntity->addComponent(newComponent))
			{
				errorCounter = 200;
				delete newComponent;
			}
		}
		if (ImGui::Selectable("AnimatedSprite"))
		{
			auto newComponent = new AnimatedSpriteC
			(
				ResourceManager::getInstance().getResource<Texture>("default"),
				std::vector<TextureDivision>(),
				10
			);
			if (!selectedEntity->addComponent(newComponent))
			{
				errorCounter = 200;
				delete newComponent;
			}
		}
		if (ImGui::Selectable("Velocity"))
		{
			auto newComponent = new VelocityC({ 0 ,0 });
			if (!selectedEntity->addComponent(newComponent))
			{
				errorCounter = 200;
				delete newComponent;
			}

		}
		if (ImGui::Selectable("Collider"))
		{
			BoxColliderC* newComponent;

			auto transform = (TransformC*)selectedEntity->getComponent(Transform);
			if (transform != nullptr)
				newComponent = new BoxColliderC(transform->position.x, transform->position.x, transform->size.x, transform->size.y, selectedEntity);
			else
				newComponent = new BoxColliderC(0, 0, 1, 1, selectedEntity);

			if (!selectedEntity->addComponent(newComponent))
			{
				errorCounter = 200;
				delete newComponent;
			}
		}

		ImGui::EndCombo();
	}

	if (errorCounter)
	{
		errorCounter--;
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Entity already has this component");
	}
}

void EntityPropertiesMenu::addToSystemMenu()
{
	static int errorCounter = 0;

	if (ImGui::BeginCombo("Add to system", nullptr, ImGuiComboFlags_NoPreview))
	{
		auto it = SystemsManager::getInstance().begin();

		while (it != SystemsManager::getInstance().end())
		{
			if (ImGui::Selectable((*it).second->getName().c_str()))
			{
				if (!(*it).second->addEntity(selectedEntity))
					errorCounter = 200;
			}

			it++;
		}

		ImGui::EndCombo();
	}

	if (errorCounter)
	{
		errorCounter--;
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Entity cound not be added");
	}
}

void EntityPropertiesMenu::transformProp(TransformC* component)
{
	if (ImGui::TreeNode("Transform"))
	{
		static float position[2];
		position[0] = component->position.x;
		position[1] = component->position.y;
		if (ImGui::InputFloat2("position", position))
			component->position = { position[0], position[1] };

		static float size[2];
		size[0] = component->size.x;
		size[1] = component->size.y;
		if (ImGui::InputFloat2("size", size))
			component->size = { size[0], size[1] };

		ImGui::InputFloat("rotation", &component->rotation);

		if (ImGui::Button("Remove"))
			selectedEntity->removeComponent(Transform);

		ImGui::Separator();
		ImGui::TreePop();
	}
}

void EntityPropertiesMenu::spriteProp(SpriteC* component)
{
	if (ImGui::TreeNode("Sprite"))
	{
		if(component->getTexture()!=nullptr)
			ImGui::Image((ImTextureID)component->getTexture()->getId(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
		else
			ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginCombo("Texture", nullptr, ImGuiComboFlags_NoPreview))
		{
			auto it = ResourceManager::getInstance().begin<Texture>();
			while (it != ResourceManager::getInstance().end<Texture>())
			{
				std::string name = " * " + (*it).first;
				Resource<Texture>* res = (Resource<Texture>*)(*it).second;
				if (ImGui::Selectable(name.c_str()))
				{
					component->setTexture(res);
				}
				ImGui::SameLine();

				if (res->getContents() == nullptr)
					ImGui::Image((ImTextureID)0, ImVec2(10, 10));
				else
					ImGui::Image((ImTextureID)res->getContents()->getId(), ImVec2(10, 10), ImVec2(0, 1), ImVec2(1, 0));

				ImGui::SameLine();
				ImGui::Text(" ");
				it++;
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Remove"))
			selectedEntity->removeComponent(Sprite);

		ImGui::Separator();
		ImGui::TreePop();
	}
}

void EntityPropertiesMenu::velocityProp(VelocityC* component)
{
	if (ImGui::TreeNode("Velocity"))
	{
		static float velocity[2];
		velocity[0] = component->velocity.x;
		velocity[1] = component->velocity.y;
		if (ImGui::InputFloat2("velocity", velocity))
			component->velocity = { velocity[0], velocity[1] };

		if (ImGui::Button("Remove"))
			selectedEntity->removeComponent(Velocity);

		ImGui::Separator();
		ImGui::TreePop();
	}
}

void EntityPropertiesMenu::colliderProp(BoxColliderC* component)
{
	if (ImGui::TreeNode("BoxCollider"))
	{
		static float position[2];
		position[0] = component->colX;
		position[1] = component->colY;
		if (ImGui::InputFloat2("position", position))
		{
			component->colX = position[0];
			component->colY = position[1];
		}

		static float size[2];
		size[0] = component->colSizeX;
		size[1] = component->colSizeY;
		if (ImGui::InputFloat2("size", size))
		{
			component->colSizeX = size[0];
			component->colSizeY = size[1];
		}

		ImGui::Text(("Collider ID: " + std::to_string(component->ID)).c_str());

		if (ImGui::Button("Remove"))
			selectedEntity->removeComponent(BoxCollider);

		ImGui::Separator();
		ImGui::TreePop();
	}
}

void EntityPropertiesMenu::animatedSpriteProp(AnimatedSpriteC* component)
{
	if (ImGui::TreeNode("AnimatedSprite"))
	{
		if (component->getTexture() != nullptr)
			ImGui::Image((ImTextureID)component->getTexture()->getId(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
		else
			ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginCombo("Texture", nullptr, ImGuiComboFlags_NoPreview))
		{
			auto it = ResourceManager::getInstance().begin<Texture>();
			while (it != ResourceManager::getInstance().end<Texture>())
			{
				std::string name = " * " + (*it).first;
				Resource<Texture>* res = (Resource<Texture>*)(*it).second;
				if (ImGui::Selectable(name.c_str()))
					component->setTexture(res);
				ImGui::SameLine();

				if (res->getContents() == nullptr)
					ImGui::Image((ImTextureID)0, ImVec2(10, 10));
				else
					ImGui::Image((ImTextureID)res->getContents()->getId(), ImVec2(10, 10), ImVec2(0, 1), ImVec2(1, 0));

				ImGui::SameLine();
				ImGui::Text(" ");
				it++;
			}
			ImGui::EndCombo();
		}

		if (ImGui::InputInt("Current frame", &component->currentFrame))
		{
			if (component->currentFrame >= component->divisions.size())
				component->currentFrame = component->divisions.size() - 1;
		}

		ImGui::InputInt("Frame duration", &component->frameDuration);

		if (ImGui::Button("Remove"))
			selectedEntity->removeComponent(Sprite);

		int n = 0;
		for (auto& division : component->divisions)
		{
			if (ImGui::TreeNode(("div" + std::to_string(n)).c_str()))
			{
				float* pos[2] = { &division.x, &division.y };
				float* size[2] = { &division.width, &division.height };

				ImGui::InputFloat2("pos", *pos);
				ImGui::InputFloat2("size", *size);

				ImGui::TreePop();
			}
			n++;
		}

		ImGui::Separator();
		ImGui::TreePop();
	}
}
