#pragma once

#include "Systems/SysBase.hpp"

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <string>

class SystemsManager
{
public:
	template<typename T>
	bool addSystem(SysBase* system, std::string name="DEFAULT");
	template<typename T>
	T* getSystem(std::string name="DEFAULT");
	template<typename T>
	std::unordered_map<std::string, SysBase*>* getSystemBin();
private:
	std::unordered_map<std::type_index, std::unordered_map<std::string, SysBase*>> systems;
};

template<typename T>
inline bool SystemsManager::addSystem(SysBase* system, std::string name)
{
	auto it = systems.find(std::type_index(typeid(T)));
	if (it == systems.end())
	{
		systems[std::type_index(typeid(T))][name] = system;
		system->deleteOnSceneEnd = deleteOnSceneEnd;
	}
	else
	{
		auto it2 = (*it).second.find(name);
		if (it2 == (*it).second.end())
		{
			(*it).second[name] = system;
			system->deleteOnSceneEnd = deleteOnSceneEnd;
		}
		else
		{
			return false;
		}
	}

	return true;
}

template<typename T>
inline T* SystemsManager::getSystem(std::string name)
{
	auto it = systems.find(std::type_index(typeid(T)));
	if (it == systems.end())
	{
		return nullptr;
	}
	else
	{
		auto it2 = (*it).second.find(name);
		if (it2 == (*it).second.end())
		{
			return nullptr;
		}
		else
		{
			return (T*)(*it2).second;
		}
	}

	return nullptr;
}

template<typename T>
inline std::unordered_map<std::string, SysBase*>* SystemsManager::getSystemBin()
{
	if (systems.find(std::type_index(typeid(T))) != systems.end())
		return systems[std::type_index(typeid(T))];
	else
		return nullptr;

}
