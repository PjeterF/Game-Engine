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
	void update(float dt);
	bool addSystem(SystemBase* system);
	void removeSystem(SystemBase* system);
	std::unordered_map<int, SystemBase*>::iterator begin();
	std::unordered_map<int, SystemBase*>::iterator end();
private:
	SystemsManager();
	std::unordered_map<int, SystemBase*> systems;
};