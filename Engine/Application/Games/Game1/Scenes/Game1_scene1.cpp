#include "Game1_scene1.hpp"

#include "../../src/Managers/Resource/ResourceManager.hpp"
#include "../../src/Graphics/ParticleEmitter.hpp"
#include "../../src/ECS/Entity/EntManager.hpp"
#include "../../src/ECS/Systems/SystemsManager.hpp"
#include "../../src/Graphics/InstancedSpriteRenderer.hpp"
#include "../../src/Input/Windows/GLFWInputManager.hpp"

Game1_scene1::Game1_scene1(Camera& camera, CollisionS& csys, RenderingS& rsys, MovementS& msys, PlayerControllerS& controllerSys)
: Scene(camera), csys(csys), rsys(rsys), msys(msys), controllerSys(controllerSys)
{
	this->addSystem<FollowS>("Follow1", new FollowS(), true);
	auto followSys = this->getSystem<FollowS>("Follow1");
	followSys->acceleration = 0.1f;

	std::vector<glm::vec4> fruitDivisions;
	fruitDivisions = utility::sampling::sampleEvenly(608, 96, 0, 0, 36, 6);

	{
		Entity playerEnt = EntityManager::getInstance().createEntity();
		playerEnt.addComponent<Transform>(Transform(2000, 2000, 50, 50));
		playerEnt.addComponent<Velocity>(Velocity(0, 0));
		playerEnt.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/Fruit+.png"), fruitDivisions[0]));
		playerEnt.addComponent<AABB>(AABB(40, 40));
		playerEnt.setTag(PlayerCharacter);

		msys.addEntity(playerEnt);
		rsys.addEntity(playerEnt);
		csys.addEntity(playerEnt);
		controllerSys.addEntity(playerEnt);
		followSys->setTarget(playerEnt.getID());
	}

	for (int i = 0; i < 100; i++)
	{
		Entity ent = EntityManager::getInstance().createEntity();
		ent.addComponent<Transform>(Transform(rand() % 2000 - 200, rand() % 2000 - 200, 30, 30, 0));
		ent.addComponent<Velocity>(Velocity(2 * (1 * float(rand() % 100) / 100 - 1), 1 * (2 * float(rand() % 100) / 100 - 1), 0, -0.0));
		ent.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/Fruit+.png"), fruitDivisions[rand() % fruitDivisions.size()]));
		AABB& col = ent.addComponent<AABB>(AABB(15, 15));
		if (rand() % 100 < 0)
			col.enabled = false;

		msys.addEntity(ent);
		rsys.addEntity(ent);
		csys.addEntity(ent);
		followSys->addEntity(ent);
	}
}

Game1_scene1::~Game1_scene1()
{
	
}

void Game1_scene1::update(float dt)
{
	csys.update(dt);
	this->getSystem<FollowS>("Follow1")->update(dt);
	msys.update(dt);
	csys.updateResponse(dt);
	csys.lateUpdate(dt);
	EntityManager::getInstance().update();
}

void Game1_scene1::draw(RenderingAPI* renderingAPI)
{
	renderingAPI->setCamera(&camera);

	rsys.update(0);
}

void Game1_scene1::input()
{
	controllerSys.update(0);

	InputManager* input = &GLFWInputManager::getInstance();

	float scrollRegionWidth = 0.1f;
	float rate = 10.0f/camera.getZoom();
	auto normalizedCursorPos = input->getNormalizedCursorPos();

	if (normalizedCursorPos.x < scrollRegionWidth)
	{
		auto cameraPosition = camera.getPosition();
		camera.setPosition(cameraPosition.x - rate, cameraPosition.y);
	}
	if (normalizedCursorPos.x > 1.0f - scrollRegionWidth)
	{
		auto cameraPosition = camera.getPosition();
		camera.setPosition(cameraPosition.x + rate, cameraPosition.y);
	}
	if (normalizedCursorPos.y < scrollRegionWidth)
	{
		auto cameraPosition = camera.getPosition();
		camera.setPosition(cameraPosition.x, cameraPosition.y - rate);
	}
	if (normalizedCursorPos.y > 1.0f - scrollRegionWidth)
	{
		auto cameraPosition = camera.getPosition();
		camera.setPosition(cameraPosition.x, cameraPosition.y + rate);
	}

	
	float wheel = input->mouseWheel();
	float zoomRate = 0.02;
	if (wheel != 0)
	{
		camera.changeZoom(wheel * zoomRate);
	}

	/*if (input->keyClicked[ZE_KEY_F])
		EntityManager::getInstance().reset();*/

	input->update();
}
