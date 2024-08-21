#include "TestingApplication.hpp"

#include "../../src/ECS2/SystemsManager.hpp"
#include "../../src/ECS2/Systems/RenderingS.hpp"
#include "../../src/ECS2/Systems/MovementS.hpp"
#include "../../src/ECS2/Systems/CollisionS.hpp"
#include "../../src/ECS2/Systems/CollisionRepulsionS.hpp"
#include "../../src/ECS2/Systems/ParticleS.hpp"

#include "Scenes/RenderingTestScene.hpp"

TestingApplicaton::TestingApplicaton(float windowWidth, float windowHeight, std::string windowName) : Application(windowWidth, windowHeight, windowName)
{
	Camera* cam1 = new Camera(0, wndWidth, 0, wndHeight);
	renderingAPI->setCamera(cam1);

	SystemsManager::getInstance().addSystem<RenderingS>(new RenderingS(renderingAPI));
	SystemsManager::getInstance().addSystem<MovementS>(new MovementS());
	SystemsManager::getInstance().addSystem<CollisionS>(new CollisionS(30));
	SystemsManager::getInstance().addSystem<CollisionRepulsionS>(new CollisionRepulsionS());
	SystemsManager::getInstance().addSystem<ParticleS>(new ParticleS());

	RenderingTestScene* renderingTestScene = new RenderingTestScene(*cam1);
	SceneManager::getInstance().addScene("renderingTestScene", renderingTestScene);

	SceneManager::getInstance().setCurrentScene("renderingTestScene");
}

void TestingApplicaton::initialize()
{
}
