#pragma once

#include <bitset>
#include "ComponentPoolManager.hpp"

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
private:
	int ID;
	bool valid;
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
