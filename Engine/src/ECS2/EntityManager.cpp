#include "EntityManager.hpp"

#include "../Events/EventManager.hpp"

Entity::Entity(int ID) : ID(ID)
{
	if (ID == -1)
		valid = false;
	else
		valid = true;
}

int Entity::getID()
{
	return ID;
}

bool Entity::isValid()
{
	return valid;
}

EntityTag Entity::getTag()
{
	return EntityManager::getInstance().getTag(ID);
}

void Entity::setTag(EntityTag tag)
{
	EntityManager::getInstance().setTag(ID, tag);
}

void Entity::delete_()
{
	EntityManager::getInstance().deleteEntity(this->ID);
}

nlohmann::json Entity::serialize()
{
	nlohmann::json jOut=nlohmann::json::array();
	std::vector<ComponentPoolBase*>& pools = ComponentPoolManager::getInstance().poolsVec;

	for (auto& pool : pools)
	{
		if (!pool->has(ID))
			continue;

		nlohmann::json j = pool->serialize(ID);
		if (!j.is_null())
		{
			jOut.push_back(j);
			std::cout << j.dump(4) << std::endl;
		}
	}

	return jOut;
}

EntityManager::EntityManager()
{
	for (int i = MAX_ENTITIES - 1; i >= 0; i--)
		availableID.push(i);

	alive = std::vector<bool>(MAX_ENTITIES, false);
	tags = std::vector<EntityTag>(MAX_ENTITIES, EntityTag::DefaultTag);
}

EntityManager& EntityManager::getInstance()
{
	static EntityManager instance;
	return instance;
}

Entity EntityManager::createEntity(EntityTag tag)
{
	if (availableID.empty())
		return Entity(-1);

	int id = availableID.top();
	alive[id]=true;
	availableID.pop();
	tags[id] = tag;

	existingEntities.insert(id);

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

	entitiesToDelete.push_back(ID);
}

EntityTag EntityManager::getTag(int ID)
{
	if (ID < 0 || ID >= MAX_ENTITIES)
		throw "Entity ID out of range";

	return this->tags[ID];
}

void EntityManager::setTag(int ID, EntityTag tag)
{
	if (ID < 0 || ID >= MAX_ENTITIES)
		throw "Entity ID out of range";

	this->tags[ID] = tag;
}

void EntityManager::update()
{
	for (auto& ID : entitiesToDelete)
	{
		EventManager::getInstance().notify(Event(Event::EntityRemoval, &ID), ECS2);
		if (alive[ID])
		{
			availableID.push(ID);
			alive[ID] = false;
			tags[ID] = DefaultTag;
			existingEntities.erase(ID);
		}
	}

	for (auto& ID : entitiesToDelete)
	{
		EventManager::getInstance().notify(Event(Event::EntityRemoval, &ID), ComponentPools);
	}
	entitiesToDelete.clear();
}

void EntityManager::reset()
{
	for (auto ID : existingEntities)
		deleteEntity(ID);
	update();

	availableID = std::stack<int>();
	for (int i = MAX_ENTITIES - 1; i >= 0; i--)
		availableID.push(i);
}

const std::unordered_set<int>& EntityManager::getExistingEntities()
{
	return existingEntities;
}
