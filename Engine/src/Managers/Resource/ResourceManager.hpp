#pragma once

#include <unordered_map>
#include <string>
#include <typeindex>

#include "Resource.hpp"
#include "../../OpenGL/ShaderProgram.hpp"
#include "../../OpenGL/Texture.hpp"

class ResourceManager
{
public:
	ResourceManager()
	{
		createResource<Texture>("default", nullptr);
		createResource<ShaderProgram>("default", nullptr);
	}

	static ResourceManager& getInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	template <typename T>
	Resource<T>* getResource(std::string name);

	template <typename T>
	bool createResource(std::string name, T* resource);

	template <typename T>
	bool createResourceFromFile(std::string filepath) { throw "Not implemented"; };

	template <typename T>
	bool deleteResource(std::string name);


	template <typename T>
	std::unordered_map<std::string, ResourceBase*>::iterator begin();
	template <typename T>
	std::unordered_map<std::string, ResourceBase*>::iterator end();


private:
	using Payload = std::unordered_map<std::string, ResourceBase*>;
	std::unordered_map<std::type_index, Payload> resources;
};

template <>
inline bool ResourceManager::createResourceFromFile<Texture>(std::string filepath)
{
	auto it = resources[std::type_index(typeid(Texture))].find(filepath);
	if (it != resources[std::type_index(typeid(Texture))].end())
		return false;

	resources[std::type_index(typeid(Texture))].insert({ filepath, (ResourceBase*)(new Resource<Texture>(new Texture(filepath))) });
	return true;
}

template <>
inline bool ResourceManager::createResourceFromFile<ShaderProgram>(std::string filepath)
{
	auto it = resources[std::type_index(typeid(ShaderProgram))].find(filepath);
	if (it != resources[std::type_index(typeid(ShaderProgram))].end())
		return false;

	resources[std::type_index(typeid(ShaderProgram))].insert({ filepath, (ResourceBase*)(new Resource<ShaderProgram>(new ShaderProgram(filepath + ".vert", filepath + ".frag"))) });
	return true;
}


template<typename T>
inline Resource<T>* ResourceManager::getResource(std::string name)
{
	auto it = resources[std::type_index(typeid(T))].find(name);
	if (it == resources[std::type_index(typeid(T))].end())
	{
		if(createResourceFromFile<T>(name))
		{
			return (Resource<T>*)resources[std::type_index(typeid(T))][name];
		}
		else
		{
			return (Resource<T>*)resources[std::type_index(typeid(T))]["default"];
		}
	}
	
	return (Resource<T>*)(*it).second;
}

template<typename T>
inline bool ResourceManager::createResource(std::string name, T* resource)
{
	auto it = resources[std::type_index(typeid(T))].find(name);
	if (it != resources[std::type_index(typeid(T))].end())
		return false;

	resources[std::type_index(typeid(T))].insert({ name, (ResourceBase*)(new Resource<T>(resource)) });
	return true;
}

template<typename T>
inline bool ResourceManager::deleteResource(std::string name)
{
	auto it2 = resources[std::type_index(typeid(T))].find(name);
	if (it2 == resources[std::type_index(typeid(T))].end())
		return false;

	Resource<T>* resource = (Resource<T>*)(*it2).second;
	resource->unload();
	return true;
}

template<typename T>
inline std::unordered_map<std::string, ResourceBase*>::iterator ResourceManager::begin()
{
	return resources[std::type_index(typeid(T))].begin();
}

template<typename T>
inline std::unordered_map<std::string, ResourceBase*>::iterator ResourceManager::end()
{
	return resources[std::type_index(typeid(T))].end();
}
