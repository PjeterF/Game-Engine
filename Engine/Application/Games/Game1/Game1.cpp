#include "Game1.hpp"

#include "Scenes/Game1_scene1.hpp"

void Game1::initialize()
{
	Camera* cam1 = new Camera(0, wndWidth, 0, wndHeight);
	CollisionS* csys1 = new CollisionS(30);
	RenderingS* rsys1 = new RenderingS(renderingAPI);
	MovementS* msys1 = new MovementS();
	PlayerControllerS* playerControllerSys = new PlayerControllerS(GLFWInputManager::getInstance());
	Game1_scene1* scene1 = new Game1_scene1(*cam1, *csys1, *rsys1, *msys1, *playerControllerSys);

	SceneManager::getInstance().setScene(scene1);
}
