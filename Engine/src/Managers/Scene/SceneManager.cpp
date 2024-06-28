#include "SceneManager.hpp"

SceneManager& SceneManager::getInstance()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::update(float dt)
{
    if (currentScene == nullptr)
        return;
    currentScene->update(dt);
}

void SceneManager::draw(RenderingAPI* renderingAPI)
{
    if (currentScene == nullptr)
        return;
    currentScene->draw(renderingAPI);
}

void SceneManager::input()
{
    if (currentScene == nullptr)
        return;
    currentScene->input();
}

void SceneManager::setScene(Scene* scene)
{
    this->currentScene = scene;
}

Scene* SceneManager::getScene()
{
    return currentScene;
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
