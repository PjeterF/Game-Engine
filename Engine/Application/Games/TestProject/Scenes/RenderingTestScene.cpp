#include "RenderingTestScene.hpp"

#include "../../Game1/ECS/Systems/FollowS.hpp"

RenderingTestScene::RenderingTestScene(Camera& camera) : Scene(camera)
{
	SystemsManager::getInstance().addSystem<FollowS>(new FollowS());
	SystemsManager::getInstance().addSystem<ElasticCollisionS>(new ElasticCollisionS());

	std::vector<glm::vec4> fruitDivisions = utility::sampling::sampleEvenly(608, 96, 0, 0, 36, 6);

	int maxVel = 5;
	glm::vec2 xPos = { 0, 1000 };
	glm::vec2 yPos = { 0, 1000 };
	glm::vec2 sizeVar = { 10, 50 };

	for (int i = 0; i < 5000; i++)
	{
		Entity ent = EntityManager::getInstance().createEntity();

		glm::vec2 pos = { utility::random::randomFloat(xPos.x, xPos.y) , utility::random::randomFloat(yPos.x, yPos.y) };
		glm::vec2 vel = { utility::random::randomFloat(-maxVel, maxVel), utility::random::randomFloat(-maxVel, maxVel) };
		int size = utility::random::randomFloat(sizeVar.x, sizeVar.y);

		ent.addComponent<Transform>(Transform(pos.x, pos.y, size, size));
		ent.addComponent<Velocity>(Velocity(vel.x, vel.y, 0, 0, 0.999));
		ent.addComponent<RenderingLayer>(RenderingLayer());
		ent.addComponent<AABB>(AABB(size, size, size));
		ent.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/Fruit+.png"), fruitDivisions[rand()%fruitDivisions.size()]));

		SystemsManager::getInstance().getSystem<RenderingS>()->addEntity(ent.getID());
		SystemsManager::getInstance().getSystem<MovementS>()->addEntity(ent.getID());
		SystemsManager::getInstance().getSystem<CollisionS>()->addEntity(ent.getID());
		SystemsManager::getInstance().getSystem<FollowS>()->addEntity(ent.getID());
		SystemsManager::getInstance().getSystem<CollisionRepulsionS>()->addEntity(ent.getID());
		SystemsManager::getInstance().getSystem<ElasticCollisionS>()->addEntity(ent.getID());
	}

	SystemsManager::getInstance().getSystem<CollisionRepulsionS>()->repulsionStrength = 0.000;
	SystemsManager::getInstance().getSystem<FollowS>()->acceleration = 0.000;

	SystemsManager::getInstance().getSystem<FollowS>()->setTarget(0);
	SystemsManager::getInstance().getSystem<FollowS>()->rePositioningDistance=10000000;
	SystemsManager::getInstance().getSystem<FollowS>()->teleportDistance=10000000;
	SystemsManager::getInstance().getSystem<MovementS>()->removeEntity(0);
	SystemsManager::getInstance().getSystem<CollisionRepulsionS>()->removeEntity(0);

	{
		int partilceCount = 1000000;

		Entity ent = EntityManager::getInstance().createEntity();

		ent.addComponent<RenderingLayer>(RenderingLayer());
		ent.addComponent<Transform>(Transform((xPos.y-xPos.x)/2, (yPos.y - yPos.x) / 2));
		auto& emitterC = ent.addComponent<Emitter>(Emitter(10000, 10000, partilceCount, partilceCount));

		emitterC.emitter.defaultProperties.xVelVar = { -10, 10 };
		emitterC.emitter.defaultProperties.yVelVar = { -10, 10 };
		emitterC.emitter.defaultProperties.xPosVar = xPos;
		emitterC.emitter.defaultProperties.yPosVar = yPos;
		emitterC.emitter.defaultProperties.particleLifetime = {10, 100};
		emitterC.emitter.defaultProperties.startColour = {1, 0, 0, 1};

		//SystemsManager::getInstance().getSystem<ParticleS>()->addEntity(ent.getID());
		//SystemsManager::getInstance().getSystem<RenderingS>()->addEntity(ent.getID());
		//
		//SystemsManager::getInstance().getSystem<ParticleS>()->update(0);
	}

	float s = 50;
	{
		Entity ent = EntityManager::getInstance().createEntity();

		ent.addComponent<Transform>(Transform(2090, 2060, s, s, 0));
		ent.addComponent<AABB>(AABB(s, s));
		ent.addComponent<RenderingLayer>(RenderingLayer());
		ent.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/Fruit+.png"), fruitDivisions[0]));

		SystemsManager::getInstance().getSystem<RenderingS>()->addEntity(ent.getID());
		SystemsManager::getInstance().getSystem<CollisionS>()->addEntity(ent.getID());
	}
	{
		Entity ent = EntityManager::getInstance().createEntity();

		ent.addComponent<Transform>(Transform(2150, 2150, s, s, 0));
		ent.addComponent<AABB>(AABB(s, s));
		ent.addComponent<RenderingLayer>(RenderingLayer());
		ent.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/Fruit+.png"), fruitDivisions[20]));

		SystemsManager::getInstance().getSystem<RenderingS>()->addEntity(ent.getID());
		SystemsManager::getInstance().getSystem<CollisionS>()->addEntity(ent.getID());
	}
}

void RenderingTestScene::initialize()
{
	auto& input = GLFWInputManager::getInstance();

	auto cursorPos = (input.getCursorPos() / camera.getZoom() + camera.getPosition());

	float scrollRegionWidth = 0.01f;
	float rate = 10.0f / camera.getZoom();
	auto normalizedCursorPos = input.getNormalizedCursorPos();
	auto cameraPosition = camera.getPosition();
	float wheel = input.mouseWheel();
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
}

void RenderingTestScene::update(float dt)
{
	timeStart = std::chrono::high_resolution_clock::now();

	EntityManager::getInstance().update();
	SystemsManager::getInstance().getSystem<CollisionS>()->lateUpdate(dt);
	SystemsManager::getInstance().getSystem<CollisionS>()->update(dt);
	
	SystemsManager::getInstance().getSystem<ElasticCollisionS>()->update(dt);
	SystemsManager::getInstance().getSystem<CollisionRepulsionS>()->update(dt);
	SystemsManager::getInstance().getSystem<MovementS>()->update(dt);
	SystemsManager::getInstance().getSystem<FollowS>()->update(dt);

	//SystemsManager::getInstance().getSystem<ParticleS>()->update(dt);
}

void RenderingTestScene::draw(RenderingAPI* renderingAPI)
{
	static std::vector<float> array;
	static int i = 0;

	SystemsManager::getInstance().getSystem<RenderingS>()->update(0);

	timeEnd = std::chrono::high_resolution_clock::now();

	auto difference = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd-timeStart).count();
	float ms = (float)difference / 1000000;
	array.push_back(ms);

	if (i == 500)
	{
		float sum = 0;
		for (int i = 0; i < array.size(); i++)
			sum += array[i];

		float avg = sum / array.size();

		std::cout << "AVG: " << avg << "\n";
	}

	i++;

	static bool showGrid = false;

	if (showGrid)
	{
		SystemsManager::getInstance().getSystem<CollisionS>()->drawGrid(*renderingAPI, camera, 1, { 1, 1, 1 ,1 }, true);
		SystemsManager::getInstance().getSystem<CollisionS>()->drawCellsWithColliders(*renderingAPI, camera, 1, { 1, 0, 0 ,1 }, true);
	}

	ImGui::Begin("Debug");

	ImGui::SliderFloat("Acceleration slider", &SystemsManager::getInstance().getSystem<FollowS>()->acceleration, -0.5, 0.5);
	ImGui::SliderFloat("Repulsion slider", &SystemsManager::getInstance().getSystem<CollisionRepulsionS>()->repulsionStrength, 0, 0.5);

	ImGui::InputFloat("Acceleration", &SystemsManager::getInstance().getSystem<FollowS>()->acceleration);
	ImGui::InputFloat("Repulsion", &SystemsManager::getInstance().getSystem<CollisionRepulsionS>()->repulsionStrength);
	ImGui::InputFloat("Cell size", &SystemsManager::getInstance().getSystem<CollisionS>()->cellSize);
	ImGui::Checkbox("Show grid", &showGrid);

	ImGui::End();
}

void RenderingTestScene::input()
{
	auto& input = GLFWInputManager::getInstance();

	auto cursorPos = (input.getCursorPos() / camera.getZoom() + camera.getPosition());

	float scrollRegionWidth = 0.01f;
	float rate = 10.0f / camera.getZoom();
	auto normalizedCursorPos = input.getNormalizedCursorPos();
	auto cameraPosition = camera.getPosition();
	float wheel = input.mouseWheel();
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

	input.update();
}
