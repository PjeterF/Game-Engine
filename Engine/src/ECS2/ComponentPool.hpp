#pragma once

#include <vector>
#include "../Events/Eventmanager.hpp"

#define MAX_ENTITIES 100000

class ComponentPoolBase
{
public:
	virtual ~ComponentPoolBase() = default;
	std::vector<bool> entityHasComponent;
};

template<typename T>
class ComponentPool : public ComponentPoolBase, public EventObserver
{
public:
	ComponentPool();
	virtual void handleEvent(Event& event) override;

	virtual T& get(int ID);
	T& add(int ID, T component);
	bool remove(int ID);
	bool has(int ID);
	void resetComponent(int ID);

	std::vector<int> sparse;
private:
	std::vector<T> components;
	friend class ComponentPoolManager;
};

template<typename T>
ComponentPool<T>::ComponentPool() : EventObserver(ComponentPools)
{
	sparse = std::vector<int>(MAX_ENTITIES, -1);
	sparse.shrink_to_fit();
}

template<typename T>
inline void ComponentPool<T>::handleEvent(Event& ev)
{
	switch (ev.getType())
	{
	case Event::EntityRemoval:
	{
		int* ID = (int*)ev.getPayload();
		remove(*ID);
	}
	break;
	}
}

template<typename T>
inline T& ComponentPool<T>::get(int ID)
{
	return components[sparse[ID]];
}

template<typename T>
inline T& ComponentPool<T>::add(int ID, T component)
{
	sparse[ID] = components.size();
	components.push_back(component);

	return components.back();
}

template<typename T>
inline bool ComponentPool<T>::remove(int ID)
{
	if (!has(ID))
		return false;

	int lastEntID=-1;
	for (int i=0;i<sparse.size();i++)
	{
		if (sparse[i] == components.size() - 1)
		{
			lastEntID = i;
			break;
		}
	}

	components[sparse[ID]] = components.back();
	sparse[lastEntID] = sparse[ID];
	components.pop_back();
	sparse[ID] = -1;

	return true;
}

template<typename T>
inline bool ComponentPool<T>::has(int ID)
{
	if (sparse[ID] != -1)
		return true;
	else
		return false;
}

template<typename T>
inline void ComponentPool<T>::resetComponent(int ID)
{
	components[sparse[ID]] = T();
}
