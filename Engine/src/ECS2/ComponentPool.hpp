#pragma once

#include <vector>
#include <nlohmann/json.hpp>
#include "../Events/Eventmanager.hpp"

#include "Components/ComponentSerializationTraits.hpp"

#define MAX_ENTITIES 100000

class ComponentPoolBase
{
public:
	virtual ~ComponentPoolBase() = default;
	bool has(int ID);

	virtual nlohmann::json serialize(int ID);
	virtual void deSerialize(int ID, nlohmann::json j);
protected:
	std::vector<int> sparse;
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
	void resetComponent(int ID);

	virtual nlohmann::json serialize(int ID) override;
	virtual void deSerialize(int ID, nlohmann::json j) override;
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

	int componentIdx = sparse[ID];
	int lastComponentIdx = components.size() - 1;

	if (componentIdx != lastComponentIdx)
	{
		components[componentIdx] = components.back();

		int lastID = -1;
		for (int i = 0; i < sparse.size(); ++i)
		{
			if (sparse[i] == lastComponentIdx)
			{
				lastID = i;
				break;
			}
		}

		if (lastID != -1)
			sparse[lastID] = componentIdx;
	}

	components.pop_back();
	sparse[ID] = -1;

	return true;
}

template<typename T>
inline void ComponentPool<T>::resetComponent(int ID)
{
	components[sparse[ID]] = T();
}

template<typename T>
inline nlohmann::json ComponentPool<T>::serialize(int ID)
{
	assert(has(ID) == true);

	return ComponentSerializationTraits<T>::serialize(get(ID));
}

template<typename T>
inline void ComponentPool<T>::deSerialize(int ID, nlohmann::json j)
{
	assert(has(ID) == true);

	ComponentSerializationTraits<T>::deSerialize(get(ID), j);
}
