#pragma once

#include "ComponentPool.hpp"

#include <unordered_map>
#include <typeinfo>
#include <typeindex>

class ComponentPoolManager
{
public:
	static ComponentPoolManager& getInstance();
	template<typename T>
	bool addPool();
	template<typename T>
	T& getComponent(int ID);
	template<typename T>
	bool hasComponent(int ID);
	template<typename T>
	T& addComponent(int ID, T component);

	bool hasComponentTID(int entID, std::type_index typeID);
private:
	ComponentPoolManager();
	std::unordered_map<std::type_index, ComponentPoolBase*> pools;
};

template<typename T>
inline bool ComponentPoolManager::addPool()
{
	if (pools.find(std::type_index(typeid(T))) == pools.end())
	{
		pools[std::type_index(typeid(T))] = new ComponentPool<T>();
		return true;
	}
	return false;
}

template<typename T>
inline T& ComponentPoolManager::getComponent(int ID)
{
	ComponentPool<T>* pool = (ComponentPool<T>*)pools[std::type_index(typeid(T))];
	return pool->components[ID];
}

template<typename T>
inline bool ComponentPoolManager::hasComponent(int ID)
{
	ComponentPool<T>* pool = (ComponentPool<T>*)pools[std::type_index(typeid(T))];
	return pool->entityHasComponent[ID];
}

template<typename T>
inline T& ComponentPoolManager::addComponent(int ID, T component)
{
	ComponentPool<T>* pool = (ComponentPool<T>*)pools[std::type_index(typeid(T))];
	pool->entityHasComponent[ID] = true;
	pool->components[ID] = component;
	return pool->components[ID];
}

