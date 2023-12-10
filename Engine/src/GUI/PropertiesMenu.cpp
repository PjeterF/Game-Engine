#include "PropertiesMenu.hpp"

#include "../ZumaLogic/Shooter.hpp"
#include "../ZumaLogic/Segment.hpp"
#include "../ZumaLogic/Snake.hpp"

PropertiesMenu::PropertiesMenu(std::string name, int xmin, int ymin, int xmax, int ymax, TexturesManager* texManager) : GUI_Window(xmin, ymin, xmax, ymax)
{
	this->texManager = texManager;
	this->name = name;
}

void PropertiesMenu::render()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(xmax - xmin, ymax - ymin));
	ImGui::SetNextWindowPos(ImVec2(xmin, ymin));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	ImGui::PushItemWidth(200);
	ImGui::Text(name.c_str());
	ImGui::PopItemWidth();

	ImGui::Separator();

	if (currentEnt != nullptr)
	{
		try
		{
			switch (currentEnt->type)
			{
			case Sprite::shooter:
			{
				Shooter* entity = (Shooter*)currentEnt;
				ImGui::Text("Object type: shooter");

				shooterProperties(entity);
			}
			break;
			case Sprite::route:
			{
				Route* entity = (Route*)currentEnt;

				ImGui::Text("Object type: route");

				ImGui::SameLine();
				if (ImGui::Button("Remove segment"))
				{
					entity->spline->removeLastSegment();
				}

				routeProperties(entity);
			}
			break;
			case Sprite::sprite:
			{
				Sprite* entity = (Sprite*)currentEnt;
				ImGui::Text("Object type: sprite");

				spriteProperties(entity);
			}
			break;
			case Sprite::segment:
			{
				Segment* entity = (Segment*)currentEnt;
				ImGui::Text("Object type: segment");

				segmentProperties(entity);
			}
			break;
			case Sprite::snake:
			{
				Snake* entity = (Snake*)currentEnt;
				ImGui::Text("Object type: snake");

				snakeProperties(entity);
			}
			break;
			}
		}
		catch (...)
		{
			
		}
		
	}

	ImGui::End();
	ImGui::PopStyleVar(1);
}

void PropertiesMenu::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::SelectObject:
	{
		currentEnt = (Entity*)event.getPayload();
	}
	break;
	default:
		break;
	}
}

void PropertiesMenu::entityProperties(Entity* ent)
{
	if (ImGui::Button("Delete"))
	{
		currentEnt = nullptr;
		ent->destroy();
		EntityManager::getInstance().update();
		EntityManager::getInstance().update();
		throw "DELETED";
		return;
	}

	int layer = ent->layer;
	if (ImGui::InputInt("Layer", &layer))
	{
		if (layer >= 0)
		{
			ent->setLayer(layer);
		}
	}
}

void PropertiesMenu::spriteProperties(Sprite* ent)
{
	try {
		entityProperties(ent);
	}
	catch(...){
		throw "DELETE";
		return;
	}
		

	ImGui::Separator();

	float pos[2] = { ent->position.x, ent->position.y };
	if (ImGui::InputFloat2("Position", pos))
	{
		ent->position = glm::vec2(pos[0], pos[1]);
	}

	float siz[2] = { ent->size.x, ent->size.y };
	if (ImGui::InputFloat2("Size", siz))
	{
		ent->size = glm::vec2(siz[0], siz[1]);
	}

	ImGui::InputFloat("Angle", &ent->angle);

	ImGui::InputFloat("Velocity", &ent->velocity);

	float dir[2] = { ent->direction.x, ent->direction.y };
	if (ImGui::InputFloat2("Direction", dir))
	{
		ent->direction = glm::vec2(dir[0], dir[1]);
	}

	ImGui::Checkbox("Collision", &ent->collision);

	if (ImGui::BeginCombo("Texture", nullptr, ImGuiComboFlags_NoPreview))
	{
		auto it = ResourceManager::getInstance().begin<Texture>();
		while (it != ResourceManager::getInstance().end<Texture>())
		{
			std::string name = " * " + (*it).first;
			if (ImGui::Selectable(name.c_str()))
			{
				ent->texture = (Resource<Texture>*)(*it).second;
			}
			ImGui::SameLine();

			Resource<Texture>* res = (Resource<Texture>*)(*it).second;
			if(res->getContents() == nullptr)
				ImGui::Image((ImTextureID)0, ImVec2(10, 10));
			else
				ImGui::Image((ImTextureID)res->getContents()->getId(), ImVec2(10, 10));
			
			ImGui::SameLine();
			ImGui::Text(" ");
			it++;
		}
		ImGui::EndCombo();
	}
}

void PropertiesMenu::snakeProperties(Snake* ent)
{
	int i = 0;
	for (auto segment : ent->segments)
	{
		std::string name = "Segment " + std::to_string(i);
		if (ImGui::TreeNode(name.c_str()))
		{
			segmentProperties(segment);
			ImGui::TreePop();
		}
		i++;
	}
}

void PropertiesMenu::routeProperties(Route* ent)
{
	ImGui::Text("Left click while paused to edit the path");
	ImGui::Separator();

	try {
		entityProperties(ent);
	}
	catch (...) {
		throw "DELETE";
		return;
	}

	ImGui::Separator();

	
	static int initialNumberOfSegments = 10;
	if(ImGui::InputInt("Initial segment number", &initialNumberOfSegments) && initialNumberOfSegments < 0)
	{
		initialNumberOfSegments = 0;
	}

	static float segmentSize = 20;
	if(ImGui::InputFloat("Segment size", &segmentSize, 0.5, 1) && segmentSize < 0)
	{
		segmentSize = 0;
	}

	static int maxSameSegments = 2;
	if(ImGui::InputInt("Max concurrent segments of the same type", &maxSameSegments) && maxSameSegments < 1)
	{
		maxSameSegments = 1;
	}

	static float snakeVelocity = 2;
	if(ImGui::InputFloat("Snake velocity", &snakeVelocity, 0.1, 0.5) && snakeVelocity < 0)
	{
		snakeVelocity = 0;
	}

	if (ImGui::Button("Set initial snake"))
	{
		ent->setInitialSnake(initialNumberOfSegments, maxSameSegments, snakeVelocity, segmentSize);
	}

	ImGui::Checkbox("Hide", &ent->spline->hidden);
}

//void PropertiesMenu::routeProperties(Route* ent)
//{
//	auto it = ent->snakes.begin();
//
//	if(ImGui::InputInt("Layer", &ent->layer))
//	{
//		if (ent->layer < 0)
//		{
//			ent->layer = 0;
//		}
//		if (ent->layer >= EntityManager::getInstance().layers.size())
//		{
//			ent->layer--;
//		}
//		EntityManager::getInstance().moveEntityToLayer((*it)->getID(), ent->layer);
//		EntityManager::getInstance().moveEntityToLayer((*it)->spline->getID(), ent->layer);
//		(*it)->updateSegmentsLayer();
//	}
//
//	ImGui::Separator();
//
//	static int initialNumberOfSegments = 10;
//	if(ImGui::InputInt("Initial segment number", &initialNumberOfSegments) && initialNumberOfSegments < 0)
//	{
//		initialNumberOfSegments = 0;
//	}
//
//	static float segmentSize = 20;
//	if(ImGui::InputFloat("Segment size", &segmentSize, 0.5, 1) && segmentSize < 0)
//	{
//		segmentSize = 0;
//	}
//
//	static int maxSameSegments = 2;
//	if(ImGui::InputInt("Max concurrent segments of the same type", &maxSameSegments) && maxSameSegments < 1)
//	{
//		maxSameSegments = 1;
//	}
//
//	static float snakeVelocity = 2;
//	if(ImGui::InputFloat("Snake velocity", &snakeVelocity, 0.1, 0.5) && snakeVelocity < 0)
//	{
//		snakeVelocity = 0;
//	}
//
//	if (ImGui::Button("Commit changes"))
//	{
//		for (auto snake : ent->snakes)
//		{
//			delete snake;
//		}
//		ent->snakes.clear();
//		ent->snakes.push_back(new Snake(initialNumberOfSegments, segmentSize, maxSameSegments, snakeVelocity, ent->spline));
//	}
//
//	ImGui::Separator();
//
//	ImGui::Text("Control points textures");
//	ImGui::Spacing();
//
//	if (ImGui::BeginCombo("Defining control points", nullptr, ImGuiComboFlags_NoPreview))
//	{
//		auto it = texManager->begin();
//		while (it != texManager->end())
//		{
//			std::string name = " * " + (*it).first;
//			if (ImGui::Selectable(name.c_str()))
//			{
//				ent->spline->tex1 = (*it).second.texture;
//			}
//			ImGui::SameLine();
//			ImGui::Image((ImTextureID)(*it).second.texture->getId(), ImVec2(10, 10));
//			ImGui::SameLine();
//			ImGui::Text(" ");
//			it++;
//		}
//		ImGui::EndCombo();
//	}
//	ImGui::Spacing();
//	if (ImGui::BeginCombo("Intermediate control points", nullptr, ImGuiComboFlags_NoPreview))
//	{
//		auto it = texManager->begin();
//		while (it != texManager->end())
//		{
//			std::string name = " * " + (*it).first;
//			if (ImGui::Selectable(name.c_str()))
//			{
//				ent->spline->tex2 = (*it).second.texture;
//			}
//			ImGui::SameLine();
//			ImGui::Image((ImTextureID)(*it).second.texture->getId(), ImVec2(10, 10));
//			ImGui::SameLine();
//			ImGui::Text(" ");
//			it++;
//		}
//		ImGui::EndCombo();
//	}
//
//	ImGui::Checkbox("Hidden", &ent->spline->hidden);
//
//	ImGui::Separator();
//
//	ImGui::Text("Snakes:");
//	int i = 0;
//	for (auto snake : ent->snakes)
//	{
//		std::string name = "Snake " + std::to_string(i);
//		if (ImGui::TreeNode(name.c_str()))
//		{
//			snakeProperties(snake);
//			ImGui::TreePop();
//		}
//			i++;
//	}
//}

void PropertiesMenu::segmentProperties(Segment* ent)
{
	try {
		spriteProperties((Sprite*)ent);
	}
	catch (...) {
		return;
	}

	if(ent->snake!=nullptr)
		ImGui::Text(("Snake: " + std::to_string(ent->snake->getID())).c_str());
	else
		ImGui::Text("Snake: null");

	int n = 0;
	auto it = ent->snake->segments.begin();
	while (*it != ent)
	{
		it++;
		n++;
	}

	ImGui::Text(("Order no: " + std::to_string(n)).c_str());

	ImGui::Text("Target Sample Index: %d", ent->targetSampleIndex);
	ImGui::Text("Tag: %d", ent->tag);
	ImGui::Checkbox("Visible", &ent->visible);
}

void PropertiesMenu::shooterProperties(Shooter* ent)
{
	try {
		spriteProperties((Sprite*)ent);
	}
	catch (...) {
		return;
	}

	ImGui::Separator();

	ImGui::InputFloat("Shot velocity", &ent->shotVelocity);

	ImGui::InputFloat("Shot size", &ent->shotSize);

	ImGui::Separator();

	ImGui::Text("Projectiles:");

	int n = 0;
	for (auto projectile : ent->projectiles)
	{
		if (ImGui::TreeNode(std::to_string(n).c_str()))
		{
			segmentProperties(projectile);
			ImGui::TreePop();
		}
		n++;
	}
}
