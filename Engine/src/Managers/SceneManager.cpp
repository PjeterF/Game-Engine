#include "SceneManager.hpp"

SceneManager& SceneManager::getInstance()
{
    return instanceImp(nullptr);
}

void SceneManager::initialize(RenderingAPI* renderingAPI)
{
    instanceImp(renderingAPI);
}

void SceneManager::update(float dt)
{
    if (currentScene == nullptr)
        return;
    currentScene->update(dt);
}

void SceneManager::draw()
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

SceneManager& SceneManager::instanceImp(RenderingAPI* renderingAPI)
{
    static SceneManager impl(renderingAPI);
    return impl;
}

SceneManager::SceneManager(RenderingAPI* renderingAPI) : renderingAPI(renderingAPI)
{
}
