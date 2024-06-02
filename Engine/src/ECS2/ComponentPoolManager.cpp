#include "ComponentPoolManager.hpp"

#include "Components/Transform.hpp"
#include "Components/Velocity.hpp"
#include "Components/Sprite.hpp"

bool ComponentPoolManager::hasComponentTID(int entID, std::type_index typeID)
{
	auto pool = pools[typeID];
	return pool->entityHasComponent[entID];
}

ComponentPoolManager::ComponentPoolManager()
{
	addPool<Transform>();
	addPool<Velocity>();
	addPool<Sprite>();
}

ComponentPoolManager& ComponentPoolManager::getInstance()
{
	static ComponentPoolManager singelton;
	return singelton;
}
