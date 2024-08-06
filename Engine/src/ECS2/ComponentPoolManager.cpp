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
		auto cpool = (ComponentPool<int>*)pool;
		
	}
}

bool ComponentPoolManager::hasComponentTID(int entID, std::type_index typeID)
{
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] == typeID)
		{
			return ((ComponentPool<int>*)poolsVec[i])->has(entID);
		}
	}
}

ComponentPoolManager::ComponentPoolManager()
{
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
