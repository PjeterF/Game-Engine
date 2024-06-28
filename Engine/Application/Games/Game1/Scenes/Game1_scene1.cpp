#include "Game1_scene1.hpp"

#include "../../src/Managers/Resource/ResourceManager.hpp"
#include "../../src/Graphics/ParticleEmitter.hpp"
#include "../../src/ECS/Entity/EntManager.hpp"
#include "../../src/ECS/Systems/SystemsManager.hpp"
#include "../../src/Graphics/InstancedSpriteRenderer.hpp"
#include "../../src/Input/Windows/GLFWInputManager.hpp"
#include "../ECS/Systems/CollisionResolutionS.hpp"

Game1_scene1::Game1_scene1(Camera& camera) : Scene(camera)
{
	
}

void Game1_scene1::initialize()
{
	addSystem<CollisionRepulsionS>("Repulsion", new CollisionRepulsionS(), true);
	getSystem<CollisionRepulsionS>("Repulsion")->repulsionStrength = 0.5;

	addSystem<CollisionResolutionS>("ColRes", new CollisionResolutionS(), true);

	this->addSystem<FollowS>("Follow", new FollowS(), true);
	auto followSys = this->getSystem<FollowS>("Follow");
	followSys->acceleration = 0.1f;

	std::vector<glm::vec4> fruitDivisions;
	fruitDivisions = utility::sampling::sampleEvenly(608, 96, 0, 0, 36, 6);

	/*{
		Entity playerEnt = EntityManager::getInstance().createEntity();
		playerEnt.addComponent<Transform>(Transform(2000, 2000, 50, 50));
		playerEnt.addComponent<Velocity>(Velocity(0, 0));
		playerEnt.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/Fruit+.png"), fruitDivisions[0]));
		playerEnt.addComponent<AABB>(AABB(40, 40));
		playerEnt.setTag(PlayerCharacter);
		playerEntID = playerEnt.getID();

		getSystem<MovementS>("Movement")->addEntity(playerEnt);
		getSystem<RenderingS>("Rendering")->addEntity(playerEnt);
		getSystem<CollisionS>("Collision")->addEntity(playerEnt);
		getSystem<PlayerControllerS>("Controller")->addEntity(playerEnt);
		getSystem<FollowS>("Follow")->setTarget(playerEnt.getID());
		getSystem<CollisionRepulsionS>("Repulsion")->addEntity(playerEnt);
	}*/

	{
		playerEntID = spawnEntity("Application/Games/Game1/EntityArchetypes/GoblinWarriorArchetype.json", { 0, 0 });
		EntityManager::getInstance().setTag(playerEntID, PlayerCharacter);

		getSystem<PlayerControllerS>("Controller")->addEntity(playerEntID);
		getSystem<FollowS>("Follow")->setTarget(playerEntID);
		getSystem<MovementS>("Movement")->addEntity(playerEntID);
		getSystem<RenderingS>("Rendering")->addEntity(playerEntID);
		getSystem<CollisionS>("Collision")->addEntity(playerEntID);
		getSystem<CollisionRepulsionS>("Repulsion")->addEntity(playerEntID);
		getSystem<CollisionResolutionS>("ColRes")->addEntity(playerEntID);
	}

	for (int i = 0; i < 0; i++)
	{
		Entity ent = EntityManager::getInstance().createEntity();
		ent.addComponent<Transform>(Transform(rand() % 2000 - 200, rand() % 2000 - 200, 30, 30, 0));
		ent.addComponent<Velocity>(Velocity(2 * (1 * float(rand() % 100) / 100 - 1), 1 * (2 * float(rand() % 100) / 100 - 1), 0, -0.0));
		ent.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/Fruit+.png"), fruitDivisions[rand() % fruitDivisions.size()]));
		AABB& col = ent.addComponent<AABB>(AABB(15, 15));
		if (rand() % 100 < 0)
			col.enabled = false;

		getSystem<MovementS>("Movement")->addEntity(ent);
		getSystem<RenderingS>("Rendering")->addEntity(ent);
		getSystem<CollisionS>("Collision")->addEntity(ent);
		//getSystem<FollowS>("Follow")->addEntity(ent);
		getSystem<CollisionRepulsionS>("Repulsion")->addEntity(ent);
	}
}

void Game1_scene1::update(float dt)
{
	getSystem<CollisionS>("Collision")->update(dt);
	getSystem<FollowS>("Follow")->update(dt);
	getSystem<CollisionResolutionS>("ColRes")->update(dt);
	getSystem<MovementS>("Movement")->update(dt);
	getSystem<CollisionRepulsionS>("Repulsion")->update(dt);
	getSystem<CollisionS>("Collision")->lateUpdate(dt);
	EntityManager::getInstance().update();
}

void Game1_scene1::draw(RenderingAPI* renderingAPI)
{
	renderingAPI->setCamera(&camera);

	getSystem<RenderingS>("Rendering")->ySortLayer(0);
	getSystem<RenderingS>("Rendering")->update(0);

	getSystem<CollisionResolutionS>("ColRes")->drawHealthBars(renderingAPI);

	renderingAPI->addQuadInstance({ 100, 100 }, { 10, 1 }, 0, { 0, 0, 0.5, 1 });
	renderingAPI->drawQuadInstances();

	ImGui::Begin("Test");
	ImGui::SliderFloat("RepulsionStrength", &getSystem<CollisionRepulsionS>("Repulsion")->repulsionStrength, 0, 1);
	ImGui::SliderFloat("FollowAcceleration", &getSystem<FollowS>("Follow")->acceleration, 0, 1);
	ImGui::Text("Text");
	ImGui::End();
}

void Game1_scene1::input()
{
	getSystem<PlayerControllerS>("Controller")->update(0);

	InputManager* input = &GLFWInputManager::getInstance();

	float scrollRegionWidth = 0.01f;
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

	if (input->mouseKeyClicked[ZE_MOUSE_BUTTON_1])
	{
		auto pos = (input->getCursorPos() / camera.getZoom() + camera.getPosition());
		int newEntID = spawnEntity("Application/Games/Game1/EntityArchetypes/Ghoul.json", pos);
		EntityManager::getInstance().setTag(newEntID, Enemy);

		getSystem<MovementS>("Movement")->addEntity(newEntID);
		getSystem<RenderingS>("Rendering")->addEntity(newEntID);
		getSystem<CollisionS>("Collision")->addEntity(newEntID);
		getSystem<CollisionRepulsionS>("Repulsion")->addEntity(newEntID);
		getSystem<CollisionResolutionS>("ColRes")->addEntity(newEntID);
		getSystem<FollowS>("Follow")->addEntity(newEntID);
	}

	input->update();
}

int Game1_scene1::spawnEntity(std::string archetypeFilePath, glm::vec2 pos)
{
	std::ifstream file(archetypeFilePath);
	if (!file.is_open())
	{
		file.close();
		return -1;
	}

	nlohmann::json j = nlohmann::json::parse(file);

	Entity newEnt = EntityManager::getInstance().createEntity();
	newEnt.addComponent<Transform>(Transform(pos.x, pos.y, j["size"], j["size"], 0));
	newEnt.addComponent<Velocity>(Velocity());
	newEnt.addComponent<AABB>(AABB(j["colliderSize"], j["colliderSize"]));
	newEnt.addComponent<CharacterStats>(CharacterStats(j["maxHealth"], j["maxHealth"], j["collisionDamage"], j["attackDamage"], j["speed"]));
	
	std::map<Animation::State, std::vector<glm::vec4>> framesMap;
	for (auto& item : j["animationFrames"].items())
	{
		Animation::State state = (Animation::State)std::stoi(item.key());
		std::vector<glm::vec4> divisions;
		auto name = item.value().type_name();
		int size = item.value().size();
		auto& array = item.value();
		for (int i = 0; i < array.size(); i++)
		{
			divisions.push_back({ array.at(i).at(0), array.at(i).at(1), array.at(i).at(2), array.at(i).at(3) });
		}
		framesMap[state] = divisions;
	}
	
	newEnt.addComponent<Animation>(Animation(
		framesMap,
		Animation::idle,
		j["frameDuration"]
	));
	newEnt.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>(j["texture"]), framesMap[Animation::idle][0]));

	return newEnt.getID();
}
