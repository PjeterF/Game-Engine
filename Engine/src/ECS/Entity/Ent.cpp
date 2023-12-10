#include "Ent.hpp"
#include "../../Events/EventManager.hpp"
#include "../Components/TransformC.hpp"
#include "../Components/VelocityC.hpp"
#include "../Components/SpriteC.hpp"
#include "../Components/AnimatedSpriteC.hpp"
#include "../Components/BoxColliderC.hpp"

Ent::~Ent()
{
	for (auto it = components.begin() ; it != components.end(); it++)
	{
		delete (*it).second;
	}
}

void Ent::destroy()
{
	deleted = true;
}

void Ent::to_json(nlohmann::json& j) const
{
	j["ID"] = ID;
	j["components"] = nlohmann::json::array();
	for (auto& component : components)
	{
		nlohmann::json j_comp;
		component.second->to_json(j_comp);
		j["components"].push_back(j_comp);
	}
}

void Ent::from_json(nlohmann::json& j)
{
	ID = j["ID"];
	for (auto& component : j["components"])
	{
		int type = component["type"];
		switch (type)
		{
		case Transform:
		{
			TransformC* newC = new TransformC();
			(*newC).from_json(component);
			this->addComponent(newC);
		}
		break;
		case Velocity:
		{
			VelocityC* newC = new VelocityC();
			(*newC).from_json(component);
			this->addComponent(newC);
		}
		break;
		case BoxCollider:
		{
			BoxColliderC* newC = new BoxColliderC();
			(*newC).from_json(component);
			this->addComponent(newC);
		}
		break;
		case Sprite:
		{
			SpriteC* newC = new SpriteC();
			(*newC).from_json(component);
			this->addComponent(newC);
		}
		break;
		case AnimatedSprite:
		{
			AnimatedSpriteC* newC = new AnimatedSpriteC();
			(*newC).from_json(component);
			this->addComponent(newC);
		}
		break;
		}
	}
}

bool Ent::addComponent(ComponentBase* component)
{
	auto it = components.find(component->getType());
	if (it != components.end())
		return false;

	component->entityID = this->ID;
	components.insert({ component->getType() , component});

	return true;
}

void Ent::removeComponent(CType type)
{
	auto it = components.find(type);
	if (it == components.end())
		return;

	(*it).second->entityID = -1;
	components.erase(type);
	EventManager::getInstance().notify(Event(Event::ComponentRemoval, this), ObsBin::ECS);
}

bool Ent::hasComponent(CType type)
{
	auto it = components.find(type);
	if (it == components.end())
		return false;
	else
		return true;
}

ComponentBase* Ent::getComponent(CType type)
{
	auto it = components.find(type);
	if (it == components.end())
		return nullptr;
	else
		return (*it).second;
}

void Ent::deleteAllComponents()
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		delete (*it).second;
	}
	components.clear();
	EventManager::getInstance().notify(Event(Event::ComponentRemoval, this), ObsBin::ECS);
}
