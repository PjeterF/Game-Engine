#include "Game1.hpp"

#include "Scenes/Game1_scene1.hpp"
#include "ECS/Components/CharacterStats.hpp"
#include "ECS/Systems/HealthBarDrawingS.hpp"

void Game1::initialize()
{
	ComponentPoolManager::getInstance().addPool<CharacterStats>();

	Camera* cam1 = new Camera(0, wndWidth, 0, wndHeight);
	renderingAPI->setCamera(cam1);
	CollisionS* csys1 = new CollisionS(30);
	RenderingS* rsys1 = new RenderingS(renderingAPI);
	HealthBarDrawingS* hbdsys = new HealthBarDrawingS(renderingAPI);
	MovementS* msys1 = new MovementS();
	PlayerControllerS* playerControllerSys = new PlayerControllerS(GLFWInputManager::getInstance());

	Game1_scene1* scene1 = new Game1_scene1(*cam1);

	scene1->addSystem<CollisionS>("Collision", csys1);
	scene1->addSystem<RenderingS>("Rendering", rsys1);
	scene1->addSystem<MovementS>("Movement", msys1);
	scene1->addSystem<PlayerControllerS>("Controller", playerControllerSys);
	scene1->addSystem<HealthBarDrawingS>("HealthBarDrawing", hbdsys);

	scene1->initialize();

	SceneManager::getInstance().addScene("HordeDefense", scene1);
	SceneManager::getInstance().setCurrentScene("HordeDefense");
}
