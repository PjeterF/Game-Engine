#pragma once

#include "SystemBase.hpp"

class EntManager;

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
	void removeSystem(int sysID);
	void deleteSystem(int sysID);

	void deleteAllNonPermSystems();

	std::unordered_map<int, std::unordered_map<int, SystemBase*>> systemSets;
private:
	SystemsManager();

	friend class EntityPropertiesMenu;
	friend bool serializeScene(std::string filepath, EntManager& entityManager, SystemsManager& systemsManager);
};