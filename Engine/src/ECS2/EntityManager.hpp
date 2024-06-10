#pragma once

#include "ComponentPoolManager.hpp"

#include <stack>
#include <set>

enum EntityTag
{
	DefaultTag = 1<<0,
	Barrier = 1 << 1
};

class Entity
{
public:
	Entity(int ID);
	int getID();
	bool isValid();
	template<typename T>
	bool hasComponent();
	template<typename T>
	T& addComponent(T component);
	template<typename T>
	T& getComponent();
	EntityTag getTag();
	void setTag(EntityTag tag);
private:
	int ID;
	bool valid;
};

class EntityManager
{
public:
	static EntityManager& getInstance();
	Entity createEntity(EntityTag tag = EntityTag::DefaultTag);
	Entity getEntity(int ID);
	bool entityExists(int ID);
	void deleteEntity(int ID);
	EntityTag getTag(int ID);
	void setTag(int ID, EntityTag tag);
private:
	EntityManager();
	std::stack<int> availableID;
	std::vector<bool> alive;
	std::vector<EntityTag> tags;
};

template<typename T>
inline bool Entity::hasComponent()
{
	return ComponentPoolManager::getInstance().hasComponent<T>(this->ID);
}

template<typename T>
inline T& Entity::addComponent(T component)
{
	return ComponentPoolManager::getInstance().addComponent<T>(this->ID, component);
}

template<typename T>
inline T& Entity::getComponent()
{
	return ComponentPoolManager::getInstance().getComponent<T>(this->ID);
}