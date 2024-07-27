#include "SystemsManager.hpp"

SystemsManager& SystemsManager::getInstance()
{
	static SystemsManager instance;
	return instance;
}

void SystemsManager::update()
{
	for (auto& bin : systemsToDelete)
	{
		for (auto& name : bin.second)
		{
			delete systems[bin.first][name];
			systems[bin.first].erase(name);
		}
	}
	systemsToDelete.clear();
}

SystemsManager::SystemsManager()
{

}
