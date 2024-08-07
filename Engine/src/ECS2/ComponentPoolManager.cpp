#include "ComponentPoolManager.hpp"

#include "Components/Transform.hpp"
#include "Components/Velocity.hpp"
#include "Components/Sprite.hpp"
#include "Components/AABB.hpp"
#include "Components/Animation.hpp"
#include "Components/RenderingLayer.hpp"
#include "Components/Emitter.hpp"
#include "Components/Counter.hpp"

bool ComponentPoolManager::hasComponentTID(int entID, std::type_index typeID)
{
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] == typeID)
		{
			return ((ComponentPool<int>*)poolsVec[i])->has(entID);
		}
	}

	return false;
}

ComponentPoolManager::ComponentPoolManager()
{
	if(ComponentSerialization::addPairing(std::type_index(typeid(Transform)), "Transform"))
		addPool<Transform>();
	if(ComponentSerialization::addPairing(std::type_index(typeid(Velocity)), "Velocity"))
		addPool<Velocity>();
	if (ComponentSerialization::addPairing(std::type_index(typeid(Sprite)), "Sprite"))
		addPool<Sprite>();
	if (ComponentSerialization::addPairing(std::type_index(typeid(AABB)), "AABB"))
		addPool<AABB>();
	if (ComponentSerialization::addPairing(std::type_index(typeid(Animation)), "Animation"))
		addPool<Animation>();
	if (ComponentSerialization::addPairing(std::type_index(typeid(RenderingLayer)), "RenderingLayer"))
		addPool<RenderingLayer>();
	if (ComponentSerialization::addPairing(std::type_index(typeid(Emitter)), "Emitter"))
		addPool<Emitter>();
	if (ComponentSerialization::addPairing(std::type_index(typeid(Counter)), "Counter"))
		addPool<Counter>();
}

ComponentPoolManager& ComponentPoolManager::getInstance()
{
	static ComponentPoolManager singelton;
	return singelton;
}
