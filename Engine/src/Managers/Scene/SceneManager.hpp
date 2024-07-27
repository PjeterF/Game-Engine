#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../../Graphics/GeneralRenderer.hpp"
#include "../../ECS2/Systems/SysBase.hpp"

#include <optional>

class Scene
{
public:
	Scene(Camera& camera);
	~Scene();
	virtual void initialize() = 0;
	virtual void update(float dt) = 0;
	virtual void draw(RenderingAPI* renderingAPI) = 0;
	virtual void input() = 0;
	template<typename T>
	bool addSystem(std::string name, SysBase* system, bool deleteOnSceneEnd=false);
	template<typename T>
	T* getSystem(std::string name);
	template<typename T>
	std::unordered_map<std::string, SysBase*>* getSystemBin();
	void setCamera(Camera& camera);

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

	bool addScene(std::string name, Scene* scene);
	bool deleteScene(std::string name);
	bool setCurrentScene(std::string name);
	Scene* getCurrentScene();
private:
	std::optional<std::string> currentSceneName;
	std::unordered_map<std::string, Scene*> scenes;
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

template<typename T>
inline std::unordered_map<std::string, SysBase*>* Scene::getSystemBin()
{
	if (systems.find(std::type_index(typeid(T))) != systems.end())
		return systems[std::type_index(typeid(T))];
	else
		return nullptr;
	
}
