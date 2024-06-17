#include "Game1_scene1.hpp"

#include "../../src/Managers/Resource/ResourceManager.hpp"
#include "../../src/Graphics/ParticleEmitter.hpp"
#include "../../src/ECS/Entity/EntManager.hpp"
#include "../../src/ECS/Systems/SystemsManager.hpp"
#include "../../src/Graphics/InstancedSpriteRenderer.hpp"
#include "../../src/Input/Windows/GLFWInputManager.hpp"

Game1_scene1::Game1_scene1()
{
	msys = new MovementS();
	inputsys = new UserControllerS(&GLFWInputManager::getInstance());

	std::vector<glm::vec4> fruitDivisions;
	fruitDivisions = utility::sampling::sampleEvenly(608, 96, 0, 0, 36, 6);
	for (int i = 0; i < rand()%100; i++)
	{
		Entity ent = EntityManager::getInstance().createEntity();
		ent.addComponent<Transform>(Transform(rand() % 2000 - 200, rand() % 2000 - 200, 30, 30, 0));
		ent.addComponent<Velocity>(Velocity(2 * (1 * float(rand() % 100) / 100 - 1), 1 * (2 * float(rand() % 100) / 100 - 1), 0, -0.0));
		ent.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/Fruit+.png"), fruitDivisions[rand() % fruitDivisions.size()]));
		AABB& col = ent.addComponent<AABB>(AABB(15, 15));
		if (rand() % 100 < 0)
			col.enabled = false;

		msys->addEntity(ent);
		RenderingS::getInstance().addEntity(ent);
		CollisionS::getInstance().addEntity(ent);
	}

	int boxSize = 500;
	int boxWidth = 5;
	{
		Entity box = EntityManager::getInstance().createEntity();
		box.addComponent<Transform>(Transform(0, boxSize, boxSize, boxWidth));
		box.addComponent<AABB>(AABB(boxSize, boxWidth));
		box.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/red.jpg")));
		box.setTag(Barrier);

		CollisionS::getInstance().addEntity(box);
		RenderingS::getInstance().addEntity(box);
	}
	{
		Entity box = EntityManager::getInstance().createEntity();
		box.addComponent<Transform>(Transform(0, -boxSize, boxSize, boxWidth));
		box.addComponent<AABB>(AABB(boxSize, boxWidth));
		box.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/red.jpg")));
		box.setTag(Barrier);

		CollisionS::getInstance().addEntity(box);
		RenderingS::getInstance().addEntity(box);
	}
	{
		Entity box = EntityManager::getInstance().createEntity();
		box.addComponent<Transform>(Transform(boxSize, 0, boxWidth, boxSize));
		box.addComponent<AABB>(AABB(boxWidth, boxSize));
		box.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/red.jpg")));
		box.setTag(Barrier);

		CollisionS::getInstance().addEntity(box);
		RenderingS::getInstance().addEntity(box);
	}
	{
		Entity box = EntityManager::getInstance().createEntity();
		box.addComponent<Transform>(Transform(-boxSize, 0, boxWidth, boxSize));
		box.addComponent<AABB>(AABB(boxWidth, boxSize));
		box.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/red.jpg")));
		box.setTag(Barrier);

		CollisionS::getInstance().addEntity(box);
		RenderingS::getInstance().addEntity(box);
	}
}

Game1_scene1::~Game1_scene1()
{
	delete msys;
	delete inputsys;
}

void Game1_scene1::update(float dt)
{
	CollisionS::getInstance().update(dt);
	inputsys->update(dt);
	msys->update(dt);
	CollisionS::getInstance().updateResponse(dt);
	CollisionS::getInstance().lateUpdate(dt);
	EntityManager::getInstance().update();
}

void Game1_scene1::draw(RenderingAPI* renderingAPI)
{
	RenderingS::getInstance().update(0);
}

void Game1_scene1::input()
{
	InputManager* input = &GLFWInputManager::getInstance();
	//auto camOffset = mainCamera->getPosition();
	//int rate = 10 / mainCamera->getZoom();
	//if (input->keyDown[ZE_KEY_UP])
	//	mainCamera->setPosition(camOffset.x, camOffset.y + rate);
	//if (input->keyDown[ZE_KEY_DOWN])
	//	mainCamera->setPosition(camOffset.x, camOffset.y - rate);
	//if (input->keyDown[ZE_KEY_LEFT])
	//	mainCamera->setPosition(camOffset.x - rate, camOffset.y);
	//if (input->keyDown[ZE_KEY_RIGHT])
	//	mainCamera->setPosition(camOffset.x + rate, camOffset.y);
	//auto nCursorPos = input->getNormalizedCursorPos();
	//float scrollRegionWidth = 0.1f;
	//if (nCursorPos.y > 1.0f - scrollRegionWidth)
	//	mainCamera->setPosition(camOffset.x, camOffset.y + rate);
	//if (nCursorPos.y < scrollRegionWidth)
	//	mainCamera->setPosition(camOffset.x, camOffset.y - rate);
	//if (nCursorPos.x < scrollRegionWidth)
	//	mainCamera->setPosition(camOffset.x - rate, camOffset.y);
	//if (nCursorPos.x > 1.0f - scrollRegionWidth)
	//	mainCamera->setPosition(camOffset.x + rate, camOffset.y);
	//if (input->mouseKeyClicked[ZE_MOUSE_BUTTON_1])
	//{
	//	auto normalized = mainCamera->viewPortPointToWorldCoord(input->getNormalizedCursorPos());
	//	std::cout << normalized.x << " " << normalized.y << "\n";

	//	Entity ent = EntityManager::getInstance().createEntity();
	//	ent.addComponent<Transform>(Transform(normalized.x, normalized.y, 30, 30, 0));
	//	ent.addComponent<Velocity>(Velocity(rand() % 2 - 0.5, rand() % 2 - 0.5, 0, 0));
	//	ent.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/Fruit+.png"), fruitDivisions[rand() % fruitDivisions.size()]));
	//	AABB& col = ent.addComponent<AABB>(AABB(15, 15));
	//	msys->addEntity(ent);
	//	RenderingS::getInstance().addEntity(ent);
	//	CollisionS::getInstance().addEntity(ent);
	//	inputsys->addEntity(ent);

	//	//auto vec = CollisionS::getInstance().pointPick(((input->getCursorPos()+mainCamera->getPosition())/mainCamera->getZoom()));
	//	/*auto vec = CollisionS::getInstance().pointPick(mainCamera->viewPortPointToWorldCoord(input->getNormalizedCursorPos()));
	//	for (auto ID : vec)
	//	{
	//		EntityManager::getInstance().deleteEntity(ID);
	//	}*/
	//}
	//float wheel = input->mouseWheel();
	//if (input->keyClicked[ZE_KEY_Q])
	//	wheel = 1;
	//if (input->keyClicked[ZE_KEY_E])
	//	wheel = -1;
	//if (wheel)
	//{
	//	mainCamera->changeZoom(0.1 * wheel);
	//}

	input->update();
}
