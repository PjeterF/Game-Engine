#include "SystemBase.hpp"
#include "SystemsManager.hpp"

int SystemBase::nextID = 0;

SystemBase::SystemBase(SystemSet set) : EventObserver(ECS)
{
	ID = nextID++;
	SystemsManager::getInstance().addSystem(this, set);
	name = "Sys(" + std::to_string(ID) + ")";
}

SystemBase::~SystemBase()
{
	SystemsManager::getInstance().removeSystem(this);
}

void SystemBase::earlyUpdate(float dt)
{
}

void SystemBase::lateUpdate(float dt)
{

}

bool SystemBase::addEntity(Ent* entity)
{
	if (entities.find(entity->getID()) != entities.end())
		return false;

	if (!validateComponents(entity))
		return false;

	entities.insert({ entity->getID(), entity });
	return true;
}

void SystemBase::removeEntity(int ID)
{
	entities.erase(ID);
}

void SystemBase::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::EntityRemoval:
	{
		Ent* entity = (Ent*)event.getPayload();
		removeEntity(entity->getID());
	}
	break;
	case Event::ComponentRemoval:
	{
		Ent* entity = (Ent*)event.getPayload();
		auto it = entities.find(entity->getID());
		if (it == entities.end())
			break;

		if (!validateComponents(entity))
			removeEntity(entity->getID());
	}
	break;
	default:
		break;
	}
}

int SystemBase::getID()
{
	return ID;
}

std::string SystemBase::getName()
{
	return name;
}

bool SystemBase::validateComponents(Ent* entity)
{
	for (auto component : requiredComponents)
	{
		if (!entity->hasComponent(component))
			return false;
	}
	return true;
}
