#include "Game1_scene1.hpp"

#include "../../src/Managers/Resource/ResourceManager.hpp"
#include "../../src/Graphics/ParticleEmitter.hpp"
#include "../../src/ECS/Entity/EntManager.hpp"
#include "../../src/ECS/Systems/SystemsManager.hpp"
#include "../../src/Graphics/InstancedSpriteRenderer.hpp"
#include "../../src/Input/Windows/GLFWInputManager.hpp"
#include "../ECS/Systems/CollisionResolutionS.hpp"
#include "../ECS/Systems/HealthBarDrawingS.hpp"
#include "../ECS/Systems/ORbitParentS.hpp"

Game1_scene1::Game1_scene1(Camera& camera) : Scene(camera)
{
	
}

void Game1_scene1::initialize()
{
	addSystem<CollisionRepulsionS>("Repulsion", new CollisionRepulsionS(), true);
	getSystem<CollisionRepulsionS>("Repulsion")->repulsionStrength = 0.5;
	addSystem<AnimationS>("Anim", new AnimationS(), true);

	addSystem<CollisionResolutionS>("ColRes", new CollisionResolutionS(), true);

	addSystem<OrbitParentS>("Orbit", new OrbitParentS(), true);

	this->addSystem<FollowS>("Follow", new FollowS(), true);
	auto followSys = this->getSystem<FollowS>("Follow");
	followSys->acceleration = 0.25f;

	std::vector<glm::vec4> fruitDivisions;
	fruitDivisions = utility::sampling::sampleEvenly(608, 96, 0, 0, 36, 6);

	{
		playerEntID = spawnEntity("Application/Games/Game1/EntityArchetypes/GoblinWarriorArchetype.json", { 0, 0 });
		Entity playerEnt(playerEntID);
		playerEnt.getComponent<Velocity>().decay = 0.1;

		getSystem<PlayerControllerS>("Controller")->addEntity(playerEntID);
		getSystem<FollowS>("Follow")->setTarget(playerEntID);
		getSystem<MovementS>("Movement")->addEntity(playerEntID);
		getSystem<RenderingS>("Rendering")->addEntity(playerEntID);
		getSystem<AnimationS>("Anim")->addEntity(playerEntID);
		getSystem<CollisionS>("Collision")->addEntity(playerEntID);
		getSystem<CollisionRepulsionS>("Repulsion")->addEntity(playerEntID);
		getSystem<CollisionResolutionS>("ColRes")->addEntity(playerEntID);
		getSystem<HealthBarDrawingS>("HealthBarDrawing")->addEntity(playerEntID);
	}

	std::vector<glm::vec4> groundTileDivisions = utility::sampling::sampleEvenly(256, 256, 0, 0, 8, 8);
	groundTileDivisions.pop_back();
	groundTileDivisions.pop_back();
	int dimX=100, dimY=100;
	float tileSize = 64;
	std::vector<Tile> tiles;

	for (int i = 0; i < dimX * dimY; i++)
		tiles.push_back(Tile("Application/Games/Game1/Textures/Tilesets/TX Tileset Grass.png", groundTileDivisions[rand()% groundTileDivisions.size()/4], true));
	
	tilemap = new Tilemap(-tileSize*dimX, -tileSize * dimY, dimX, dimY, tileSize, tiles);

	/*std::vector<glm::vec4> tiles2 = utility::sampling::sampleEvenly(16*3, 16*3, 16*3, 16*2, 3, 3);
	for (int i = 0; i < dimX * dimY; i++)
		tiles.push_back(Tile("Application/Games/Game1/Textures/Tilesets/FantasyForest/Tiles/Tileset.png", tiles2[rand() % tiles2.size()], true));

	tilemap = new Tilemap(-tileSize * dimX, -tileSize * dimY, dimX, dimY, tileSize, tiles);*/

	for (int i = 0; i < 1000; i++)
	{
		Entity ent = EntityManager::getInstance().createEntity();
		ent.addComponent<Transform>(Transform(rand() % 2000 - 200, rand() % 2000 - 200, 30, 30));
		ent.addComponent<Velocity>(Velocity(2 * (1 * float(rand() % 100) / 100 - 1), 1 * (2 * float(rand() % 100) / 100 - 1), 0, -0.0, 0.9));
		ent.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/Fruit+.png"), fruitDivisions[rand() % fruitDivisions.size()]));
		AABB& col = ent.addComponent<AABB>(AABB(15, 15, 1));
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
	auto& playerTrans = Entity(playerEntID).getComponent<Transform>();
	camera.setPosition(playerTrans.x-(camera.getFrustrumX().y - camera.getFrustrumX().x)/2, playerTrans.y - (camera.getFrustrumY().y - camera.getFrustrumY().x) / 2);

	getSystem<CollisionS>("Collision")->update(dt);

	getSystem<FollowS>("Follow")->update(dt);
	getSystem<MovementS>("Movement")->update(dt);
	getSystem<AnimationS>("Anim")->update(dt);

	getSystem<CollisionRepulsionS>("Repulsion")->update(dt);
	getSystem<CollisionResolutionS>("ColRes")->update(dt);

	getSystem<CollisionS>("Collision")->lateUpdate(dt);
	EntityManager::getInstance().update();

	static int i = 0;
	i++;
	if (i % 30==0)
	{
		float distance=2000;

		Entity playerEnt(playerEntID);
		auto& trans = playerEnt.getComponent<Transform>();

		int x = rand() % 1000 - 500;
		int y = rand() % 1000 - 500;
		if (x == 0)
			x = 1;
		glm::vec2 dir = distance * glm::normalize(glm::vec2(x, y));

		int newEntID = spawnEntity("Application/Games/Game1/EntityArchetypes/Ghoul.json", { trans.x + dir.x, trans.y + dir.y });

		Entity newEnt(newEntID);
		newEnt.getComponent<Velocity>().decay = 0.9;

		getSystem<MovementS>("Movement")->addEntity(newEntID);
		getSystem<RenderingS>("Rendering")->addEntity(newEntID);
		getSystem<AnimationS>("Anim")->addEntity(newEntID);
		getSystem<CollisionS>("Collision")->addEntity(newEntID);
		getSystem<CollisionRepulsionS>("Repulsion")->addEntity(newEntID);
		getSystem<CollisionResolutionS>("ColRes")->addEntity(newEntID);
		getSystem<FollowS>("Follow")->addEntity(newEntID);
		getSystem<HealthBarDrawingS>("HealthBarDrawing")->addEntity(newEntID);
	}
}

void Game1_scene1::draw(RenderingAPI* renderingAPI)
{
	renderingAPI->setCamera(&camera);

	tilemap->draw(renderingAPI);

	getSystem<RenderingS>("Rendering")->ySortLayer(0);
	getSystem<RenderingS>("Rendering")->update(0);

	getSystem<HealthBarDrawingS>("HealthBarDrawing")->update(0);

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

	auto cameraPosition = camera.getPosition();
	float wheel = input->mouseWheel();
	float zoomRate = 0.02;
	if (normalizedCursorPos.x < scrollRegionWidth)
		camera.setPosition(cameraPosition.x - rate, cameraPosition.y);
	if (normalizedCursorPos.x > 1.0f - scrollRegionWidth)
		camera.setPosition(cameraPosition.x + rate, cameraPosition.y);
	if (normalizedCursorPos.y < scrollRegionWidth)
		camera.setPosition(cameraPosition.x, cameraPosition.y - rate);
	if (normalizedCursorPos.y > 1.0f - scrollRegionWidth)
		camera.setPosition(cameraPosition.x, cameraPosition.y + rate);
	if (wheel != 0)
		camera.changeZoom(wheel * zoomRate);

	/*if (input->keyClicked[ZE_KEY_F])
		EntityManager::getInstance().reset();*/

	if (input->mouseKeyClicked[ZE_MOUSE_BUTTON_2])
	{
		auto pos = (input->getCursorPos() / camera.getZoom() + camera.getPosition());
		int newEntID = spawnEntity("Application/Games/Game1/EntityArchetypes/Ghoul.json", pos);

		Entity newEnt(newEntID);
		newEnt.getComponent<Velocity>().decay = 0.9;

		getSystem<MovementS>("Movement")->addEntity(newEntID);
		getSystem<RenderingS>("Rendering")->addEntity(newEntID);
		getSystem<CollisionS>("Collision")->addEntity(newEntID);
		getSystem<CollisionRepulsionS>("Repulsion")->addEntity(newEntID);
		getSystem<CollisionResolutionS>("ColRes")->addEntity(newEntID);
		getSystem<FollowS>("Follow")->addEntity(newEntID);
		getSystem<HealthBarDrawingS>("HealthBarDrawing")->addEntity(newEntID);
		getSystem<AnimationS>("Anim")->addEntity(newEntID);
	}

	if (input->mouseKeyClicked[ZE_MOUSE_BUTTON_1])
	{
		Entity playerEnt(playerEntID);
		auto& pTrans = playerEnt.getComponent<Transform>();

		auto pos = (input->getCursorPos() / camera.getZoom() + camera.getPosition());
		int newEntID = spawnEntity("Application/Games/Game1/EntityArchetypes/Projectile1.json", { pTrans.x, pTrans.y});

		Entity projectileEnt(newEntID);
		projectileEnt.getComponent<Transform>().rotateToDir = true;

		glm::vec2 diff = { pos.x - pTrans.x, pos.y - pTrans.y };
		if (glm::length(diff) < 0.0001)
			diff.x = 1;

		float speed = 5;
		diff = speed * glm::normalize(diff);

		auto& projectileVel = projectileEnt.getComponent<Velocity>();
		projectileVel.x = speed * diff.x;
		projectileVel.y = speed * diff.y;

		getSystem<MovementS>("Movement")->addEntity(newEntID);
		getSystem<RenderingS>("Rendering")->addEntity(newEntID);
		getSystem<AnimationS>("Anim")->addEntity(newEntID);
		getSystem<CollisionS>("Collision")->addEntity(newEntID);
		getSystem<CollisionResolutionS>("ColRes")->addEntity(newEntID);
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
	newEnt.addComponent<Transform>(Transform(pos.x, pos.y, j["size"], j["size"]));
	newEnt.addComponent<Velocity>(Velocity());
	newEnt.addComponent<AABB>(AABB(j["colliderSize"], j["colliderSize"], j["mass"]));
	newEnt.addComponent<CharacterStats>(CharacterStats(j["maxHealth"], j["maxHealth"], j["collisionDamage"], j["attackDamage"], j["speed"]));
	EntityManager::getInstance().setTag(newEnt.getID(), j["tag"]);
	
	std::map<Animation::State, std::vector<glm::vec4>> framesMap;
	for (auto& item : j["animationFrames"].items())
	{
		Animation::State state = (Animation::State)std::stoi(item.key());
		std::vector<glm::vec4> divisions;
		auto& array = item.value();
		for (int i = 0; i < array.size(); i++)
		{
			divisions.push_back({ array.at(i).at(0), array.at(i).at(1), array.at(i).at(2), array.at(i).at(3) });
		}
		framesMap[state] = divisions;
	}
	
	newEnt.addComponent<Animation>(Animation(
		framesMap,
		Animation::run,
		j["frameDuration"]
	));
	auto it = framesMap.begin();
	auto div = (*it).second[0];
	newEnt.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>(j["texture"]), div));

	return newEnt.getID();
}
