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

void SceneManager::draw()
{
    if (currentScene == nullptr)
        return;
    currentScene->draw();
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
