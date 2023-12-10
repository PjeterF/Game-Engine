#include "EntManager.hpp"
#include "../../Events/EventManager.hpp"

EntManager::EntManager()
{
}

EntManager& EntManager::getInstance()
{
	static EntManager manager;
	return manager;
}

void EntManager::update()
{
	for (auto& entity : toBeAdded)
	{
		entities.insert({ entity->getID(), entity });
	}
	toBeAdded.clear();

	for (auto& entity : entities)
	{
		if (entity.second->isDestroyed())
			toBeRemoved.push_back(entity.second);
	}

	for (auto& entity : toBeRemoved)
	{
		EventManager::getInstance().notify(Event(Event::EntityRemoval, entity), ObsBin::ECS);
		entities.erase(entity->getID());
		delete entity;
	}
	toBeRemoved.clear();
}

Ent* EntManager::createEntity(ETag tag)
{
	Ent* newEntity = new Ent(nextID++, tag);
	entities.insert({newEntity->getID(), newEntity});
	return newEntity;
}

Ent* EntManager::getEntity(int ID)
{
	auto it = entities.find(ID);
	if (it == entities.end())
		return nullptr;
	else
		return (*it).second;
}

std::unordered_map<int, Ent*>::iterator EntManager::begin()
{
	return entities.begin();
}

std::unordered_map<int, Ent*>::iterator EntManager::end()
{
	return entities.end();
}
