#include "ComponentPoolManager.hpp"

#include "Components/Transform.hpp"
#include "Components/Velocity.hpp"
#include "Components/Sprite.hpp"
#include "Components/AABB.hpp"
#include "Components/Animation.hpp"
#include "Components/RenderingLayer.hpp"
#include "Components/Emitter.hpp"
#include "Components/Counter.hpp"

void ComponentPoolManager::disableComponents(int ID)
{
	for (auto& pool : poolsVec)
	{
		pool->entityHasComponent[ID] = false;
	}
}

bool ComponentPoolManager::hasComponentTID(int entID, std::type_index typeID)
{
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] == typeID)
		{
			auto pool = poolsVec[i];
			return pool->entityHasComponent[entID];
		}
	}
}

ComponentPoolManager::ComponentPoolManager()
{
	hasComponents = std::vector<std::unordered_set<std::type_index>>(MAX_ENTITIES, std::unordered_set<std::type_index>());

	addPool<Transform>();
	addPool<Velocity>();
	addPool<Sprite>();
	addPool<AABB>();
	addPool<Animation>();
	addPool<RenderingLayer>();
	addPool<Emitter>();
	addPool<Counter>();
}

ComponentPoolManager& ComponentPoolManager::getInstance()
{
	static ComponentPoolManager singelton;
	return singelton;
}
