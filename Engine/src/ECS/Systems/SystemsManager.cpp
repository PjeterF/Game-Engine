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

void SystemsManager::removeSystem(int sysID)
{
	for (auto& set : systemSets)
	{
		set.second.erase(sysID);
	}
}

void SystemsManager::deleteSystem(int sysID)
{
	for (auto& set : systemSets)
	{
		for (auto& system : set.second)
		{
			if (system.second->getID() == sysID)
			{
				delete system.second;
				set.second.erase(sysID);
				break;
			}
		}
	}
}

void SystemsManager::deleteAllNonPermSystems()
{
	std::vector<int> deleteIDs;
	for (auto& set : this->systemSets)
	{
		for (auto& sys : set.second)
		{
			if (!sys.second->isPermanent())
			{
				deleteIDs.push_back(sys.first);
			}
		}
	}
	for (auto ID : deleteIDs)
		this->deleteSystem(ID);
}
