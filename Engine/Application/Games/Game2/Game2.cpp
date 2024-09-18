#include "Game2.hpp"

#include "Scenes/Game2Scene1.hpp"
#include <ECS2/SystemsManager.hpp>
#include <ECS2/Systems/RenderingS.hpp>

Game2::Game2(float windowWidth, float windowHeight, std::string windowName) : Application(windowWidth, windowHeight, windowName)
{
	SystemsManager::getInstance().addSystem<RenderingS>(new RenderingS(renderingAPI));

	Game2Scene1* mainScene = new Game2Scene1(*mainCamera);
	SceneManager().getInstance().addScene("main", mainScene);
	SceneManager::getInstance().setCurrentScene("main");
}