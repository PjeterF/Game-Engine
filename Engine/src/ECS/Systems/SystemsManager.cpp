#include "SystemsManager.hpp"

SystemsManager::SystemsManager()
{

}

void SystemsManager::update(float dt)
{
	for (auto& system : systems)
		system.second->earlyUpdate(dt);

	for (auto& system : systems)
		system.second->update(dt);

	for (auto& system : systems)
		system.second->lateUpdate(dt);
}

bool SystemsManager::addSystem(SystemBase* system)
{
	auto it = systems.find(system->getID());
	if (it != systems.end())
		return false;

	systems.insert({ system->getID(), system });
	return true;
}

void SystemsManager::removeSystem(SystemBase* system)
{
	systems.erase(system->getID());
}

std::unordered_map<int, SystemBase*>::iterator SystemsManager::begin()
{
	return systems.begin();
}

std::unordered_map<int, SystemBase*>::iterator SystemsManager::end()
{
	return systems.end();
}
