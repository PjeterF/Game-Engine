#pragma once

#include "ComponentPool.hpp"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <typeinfo>
#include <typeindex>

#include "../Events/EventManager.hpp"

class ComponentPoolManager
{
public:
	static ComponentPoolManager& getInstance();
	template<typename T>
	bool addPool();
	template<typename T>
	ComponentPool<T>* getPool();
	template<typename T>
	T& getComponent(int ID);
	template<typename T>
	bool hasComponent(int ID);
	template<typename T>
	T& addComponent(int ID, T component);
	template<typename T>
	void removeComponent(int ID);
	void disableComponents(int ID);

	bool hasComponentTID(int entID, std::type_index typeID);
private:
	ComponentPoolManager();
	std::unordered_map<std::type_index, ComponentPoolBase*> pools;
	std::vector<std::type_index> indices;
	std::vector<ComponentPoolBase*> poolsVec;
	std::vector<std::unordered_set<std::type_index>> hasComponents;

	friend class EntityManager;
	friend class Entity;
};

template<typename T>
inline bool ComponentPoolManager::addPool()
{
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] == std::type_index(typeid(T)))
			return false;
	}
	indices.push_back(std::type_index(typeid(T)));
	poolsVec.push_back(new ComponentPool<T>());
}

template<typename T>
inline ComponentPool<T>* ComponentPoolManager::getPool()
{
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] == std::type_index(typeid(T)))
		{
			return ((ComponentPool<T>*)poolsVec[i]);
		}
	}
}

template<typename T>
inline T& ComponentPoolManager::getComponent(int ID)
{
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] == std::type_index(typeid(T)))
		{
			return ((ComponentPool<T>*)poolsVec[i])->components[ID];
		}
	}
}

template<typename T>
inline bool ComponentPoolManager::hasComponent(int ID)
{
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] == std::type_index(typeid(T)))
		{
			return ((ComponentPool<T>*)poolsVec[i])->entityHasComponent[ID];
		}
	}
}

template<typename T>
inline T& ComponentPoolManager::addComponent(int ID, T component)
{
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] == std::type_index(typeid(T)))
		{
			ComponentPool<T>* pool = ((ComponentPool<T>*)poolsVec[i]);
			pool->entityHasComponent[ID] = true;
			pool->components[ID] = component;
			return pool->components[ID];
		}
	}
}

template<typename T>
inline void ComponentPoolManager::removeComponent(int ID)
{
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] == std::type_index(typeid(T)))
		{
			ComponentPool<T>* pool = ((ComponentPool<T>*)poolsVec[i]);
			pool->entityHasComponent[ID] = false;
			EventManager::getInstance().notify(Event(Event::ComponentRemoval, &ID), ECS2);
		}
	}
}

