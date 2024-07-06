#include "ZumaGame.hpp"

#include "Scenes/GeneralZumaScene.hpp"
#include"ECS/Components/MarbleComponent.hpp"
#include"ECS/Components/MarbleShooter.hpp"

void ZumaGame::initialize()
{
	ComponentPoolManager::getInstance().addPool<MarbleComponent>();
	ComponentPoolManager::getInstance().addPool<MarbleShooter>();

	Camera* cam1 = new Camera(0, wndWidth, 0, wndHeight);
	renderingAPI->setCamera(cam1);

	CollisionS* csys1 = new CollisionS(30);
	RenderingS* rsys1 = new RenderingS(renderingAPI);
	MovementS* msys1 = new MovementS();

	GeneralZumaScene* generalScene = new GeneralZumaScene(*cam1);

	generalScene->addSystem<RenderingS>("Rend", rsys1);

	generalScene->initialize();
	SceneManager::getInstance().setScene(generalScene);
}
