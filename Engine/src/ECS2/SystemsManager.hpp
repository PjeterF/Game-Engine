#pragma once

#include "Systems/SysBase.hpp"

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <string>

class SystemsManager
{
public:
	static SystemsManager& getInstance();
	template<typename T>
	bool addSystem(SysBase* system, std::string name="DEFAULT");
	template<typename T>
	T* getSystem(std::string name="DEFAULT");
	template<typename T>
	std::unordered_map<std::string, SysBase*>& getSystemBin();
	template<typename T>
	bool systemBinExists();
	template<typename T>
	bool deleteSystem(std::string name = "DEFAULT");

	void update();
private:
	SystemsManager();
	std::unordered_map<std::type_index, std::unordered_map<std::string, SysBase*>> systems;
	std::unordered_map < std::type_index, std::vector<std::string>> systemsToDelete;
};

template<typename T>
inline bool SystemsManager::addSystem(SysBase* system, std::string name)
{
	auto it = systems.find(std::type_index(typeid(T)));
	if (it == systems.end())
	{
		systems[std::type_index(typeid(T))][name] = system;
	}
	else
	{
		auto it2 = (*it).second.find(name);
		if (it2 == (*it).second.end())
		{
			(*it).second[name] = system;
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
inline std::unordered_map<std::string, SysBase*>& SystemsManager::getSystemBin()
{
	if (systems.find(std::type_index(typeid(T))) == systems.end())
		systems[std::type_index(typeid(T))] = std::unordered_map<std::string, SysBase*>();

	return systems[std::type_index(typeid(T))];
}

template<typename T>
inline bool SystemsManager::systemBinExists()
{
	if(systems.find(std::type_index(typeid(T))) != systems.end())
		return true;
	else
		return false;
}

template<typename T>
inline bool SystemsManager::deleteSystem(std::string name)
{
	auto typeIndex = std::type_index(typeid(T));
	if (systems.find(typeIndex) == systems.end())
		return false;

	auto& nameSystemMap = systems[typeIndex];
	if (nameSystemMap.find(name) == nameSystemMap.end())
		return false;

	systemsToDelete[typeIndex].push_back(name);

	return true;
}
