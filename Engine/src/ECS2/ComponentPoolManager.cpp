#include "ComponentPoolManager.hpp"

#include "Components/Transform.hpp"
#include "Components/Velocity.hpp"
#include "Components/Sprite.hpp"
#include "Components/AABB.hpp"
#include "Components/Animation.hpp"

bool ComponentPoolManager::hasComponentTID(int entID, std::type_index typeID)
{
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] == typeID)
		{
			auto pool = poolsVec[i];
			return pool->entityHasComponent[entID] = true;
		}
	}

	/*auto pool = pools[typeID];
	return pool->entityHasComponent[entID];*/
}

ComponentPoolManager::ComponentPoolManager()
{
	addPool<Transform>();
	addPool<Velocity>();
	addPool<Sprite>();
	addPool<AABB>();
	addPool<Animation>();
}

ComponentPoolManager& ComponentPoolManager::getInstance()
{
	static ComponentPoolManager singelton;
	return singelton;
}
