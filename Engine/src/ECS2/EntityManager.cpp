#include "EntityManager.hpp"

EntityManager::EntityManager()
{
	for (int i = MAX_ENTITIES - 1; i >= 0; i--)
		availableID.push(i);

	alive = std::vector<bool>(MAX_ENTITIES, false);
}

EntityManager& EntityManager::getInstance()
{
	static EntityManager instance;
	return instance;
}

Entity EntityManager::createEntity()
{
	if (availableID.empty())
		return Entity(-1);

	int id = availableID.top();
	alive[id]=true;
	availableID.pop();

	return Entity(id);
}

Entity EntityManager::getEntity(int ID)
{
	if (ID < 0 || ID >= MAX_ENTITIES)
		throw "Entity ID out of range";

	if (alive[ID])
		return Entity(ID);
	else
		return Entity(-1);
}

bool EntityManager::entityExists(int ID)
{
	if (ID < 0 || ID >= MAX_ENTITIES)
		throw "Entity ID out of range";

	return alive[ID];
}

void EntityManager::deleteEntity(int ID)
{
	if (ID < 0 || ID >= MAX_ENTITIES)
		throw "Entity ID out of range";

	if (alive[ID])
	{
		availableID.push(ID);
		alive[ID] = false;
	}
}
