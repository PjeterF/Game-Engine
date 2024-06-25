#pragma once

#include "../../Graphics/GeneralRenderer.hpp"
#include "../../ECS2/Systems/SysBase.hpp"

class Scene
{
public:
	Scene(Camera& camera);
	~Scene();
	virtual void update(float dt) = 0;
	virtual void draw(RenderingAPI* renderingAPI) = 0;
	virtual void input() = 0;
	template<typename T>
	bool addSystem(std::string name, SysBase* system, bool deleteOnSceneEnd=false);
	template<typename T>
	T* getSystem(std::string name);

	Camera& camera;

	std::unordered_map<std::type_index, std::unordered_map<std::string, SysBase*>> systems;
};

class SceneManager
{
public:
	static SceneManager& getInstance();
	void update(float dt=0);
	void draw(RenderingAPI* renderingAPI);
	void input();
	void setScene(Scene* scene);
	Scene* getScene();
private:
	Scene* currentScene=nullptr;
};

template<typename T>
inline bool Scene::addSystem(std::string name, SysBase* system, bool deleteOnSceneEnd)
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
inline T* Scene::getSystem(std::string name)
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
