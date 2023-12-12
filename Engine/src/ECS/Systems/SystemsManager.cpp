#include "SystemsManager.hpp"

SystemsManager::SystemsManager()
{

}

void SystemsManager::update(float dt, SystemSet set)
{
	for (auto& system : systemSets[set])
		system.second->earlyUpdate(dt);

	for (auto& system : systemSets[set])
		system.second->update(dt);

	for (auto& system : systemSets[set])
		system.second->lateUpdate(dt);
}

bool SystemsManager::addSystem(SystemBase* system, SystemSet set)
{
	auto it = systemSets[set].find(system->getID());
	if (it != systemSets[set].end())
		return false;

	systemSets[set].insert({ system->getID(), system });
	return true;
}

void SystemsManager::removeSystem(SystemBase* system)
{
	for (auto& set : systemSets)
	{
		set.second.erase(system->getID());
	}
}