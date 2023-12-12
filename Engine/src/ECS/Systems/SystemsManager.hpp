#pragma once

#include "SystemBase.hpp"


class SystemsManager
{
public:
	static SystemsManager& getInstance()
	{
		static SystemsManager instance;
		return instance;
	}
	void update(float dt, SystemSet set);
	bool addSystem(SystemBase* system, SystemSet set = UNPAUSED);
	void removeSystem(SystemBase* system);
private:
	SystemsManager();
	std::unordered_map<int, SystemBase*> systems;
	std::unordered_map<int, std::unordered_map<int, SystemBase*>> systemSets;

	friend class EntityPropertiesMenu;
};