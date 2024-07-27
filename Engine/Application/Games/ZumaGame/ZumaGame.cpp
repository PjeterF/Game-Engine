#include "ZumaGame.hpp"

#include "Scenes/GeneralZumaScene.hpp"
#include"ECS/Components/MarbleComponent.hpp"
#include"ECS/Components/MarbleShooter.hpp"

#include "../../src/ECS2/SystemsManager.hpp"
void ZumaGame::initialize()
{
	ComponentPoolManager::getInstance().addPool<MarbleComponent>();
	ComponentPoolManager::getInstance().addPool<MarbleShooter>();

	Camera* cam1 = new Camera(0, wndWidth, 0, wndHeight);
	renderingAPI->setCamera(cam1);

	SystemsManager::getInstance().addSystem<RenderingS>(new RenderingS(renderingAPI));

	GeneralZumaScene* generalScene = new GeneralZumaScene(*cam1);

	generalScene->initialize();

	SceneManager::getInstance().addScene("GeneralScene", generalScene);
	SceneManager::getInstance().setCurrentScene("GeneralScene");
}
