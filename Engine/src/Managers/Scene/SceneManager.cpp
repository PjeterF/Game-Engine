#include "SceneManager.hpp"

SceneManager& SceneManager::getInstance()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::update(float dt)
{
    if (!currentSceneName.has_value())
        return;
    scenes[currentSceneName.value()]->update(dt);
}

void SceneManager::draw(RenderingAPI* renderingAPI)
{
    if (!currentSceneName.has_value())
        return;
    scenes[currentSceneName.value()]->draw(renderingAPI);
}

void SceneManager::input()
{
    if (!currentSceneName.has_value())
        return;
    scenes[currentSceneName.value()]->input();
}

bool SceneManager::addScene(std::string name, Scene* scene)
{
    if (scenes.find(name) != scenes.end())
        return false;
    scenes[name] = scene;
    return true;
}

bool SceneManager::deleteScene(std::string name)
{
    if (scenes.find(name) == scenes.end())
        return false;

    if (name == currentSceneName)
        currentSceneName.reset();

    delete scenes[name];
    scenes.erase(name);

    return true;
}

bool SceneManager::setCurrentScene(std::string name)
{
    if (scenes.find(name) == scenes.end())
        return false;
    currentSceneName = name;
    return true;
}

Scene* SceneManager::getCurrentScene()
{
    if (!currentSceneName.has_value())
        return nullptr;

    return scenes[currentSceneName.value()];
}

Scene::Scene(Camera& camera) : camera(camera)
{
}

Scene::~Scene()
{
    for (auto& sysBin : systems)
    {
        for (auto& system : sysBin.second)
        {
            if (system.second->deleteOnSceneEnd)
                delete system.second;
        }
    }
}

void Scene::setCamera(Camera& camera)
{
    this->camera = camera;
}
