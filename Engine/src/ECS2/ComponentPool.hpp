#pragma once

#include <vector>

#define MAX_ENTITIES 100000

class ComponentPoolBase
{
public:
	virtual ~ComponentPoolBase() = default;
	std::vector<bool> entityHasComponent;
};

template<typename T>
class ComponentPool : public ComponentPoolBase
{
public:
	ComponentPool();
	virtual T& get(int ID);
	void resetComponent(int ID);
private:
	std::vector<T> components;
	friend class ComponentPoolManager;
};

template<typename T>
ComponentPool<T>::ComponentPool()
{
	components = std::vector<T>(MAX_ENTITIES, T());
	components.shrink_to_fit();
	entityHasComponent = std::vector<bool>(MAX_ENTITIES, false);
	entityHasComponent.shrink_to_fit();
}

template<typename T>
inline T& ComponentPool<T>::get(int ID)
{
	return components[ID];
}

template<typename T>
inline void ComponentPool<T>::resetComponent(int ID)
{
	components[ID] = T();
}