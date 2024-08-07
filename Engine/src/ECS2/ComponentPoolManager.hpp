#pragma once

#include "ComponentPool.hpp"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <stdexcept>

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

	bool hasComponentTID(int entID, std::type_index typeID);

private:
	ComponentPoolManager();
	std::vector<std::type_index> indices;
	std::vector<ComponentPoolBase*> poolsVec;

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

	return true;
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

	throw std::runtime_error("Component pool not found");
	return nullptr;
}

template<typename T>
inline T& ComponentPoolManager::getComponent(int ID)
{
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] == std::type_index(typeid(T)))
		{
			return ((ComponentPool<T>*)poolsVec[i])->get(ID);
		}
	}

	throw std::runtime_error("Component pool not found");
}

template<typename T>
inline bool ComponentPoolManager::hasComponent(int ID)
{
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] == std::type_index(typeid(T)))
		{
			ComponentPool<T>* pool = (ComponentPool<T>*)poolsVec[i];
			
			return pool->has(ID);
		}
	}

	throw std::runtime_error("Component pool not found");
}

template<typename T>
inline T& ComponentPoolManager::addComponent(int ID, T component)
{
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] == std::type_index(typeid(T)))
		{
			ComponentPool<T>* pool = ((ComponentPool<T>*)poolsVec[i]);
			return pool->add(ID, component);
		}
	}

	throw std::runtime_error("Component pool not found");
}

template<typename T>
inline void ComponentPoolManager::removeComponent(int ID)
{
	for (int i = 0; i < indices.size(); i++)
	{
		if (indices[i] == std::type_index(typeid(T)))
		{
			ComponentPool<T>* pool = ((ComponentPool<T>*)poolsVec[i]);

			if(pool->remove(ID))
				EventManager::getInstance().notify(Event(Event::ComponentRemoval, &ID), ECS2);
			
			return;
		}
	}

	throw std::runtime_error("Component pool not found");
}
