#include "RenderingTestScene.hpp"

RenderingTestScene::RenderingTestScene(Camera& camera) : Scene(camera)
{
	std::vector<glm::vec4> fruitDivisions = utility::sampling::sampleEvenly(608, 96, 0, 0, 36, 6);

	int maxVel = 0;
	glm::vec2 xPos = { 0, 1920 };
	glm::vec2 yPos = { 0, 1080 };
	glm::vec2 sizeVar = { 5, 10 };

	for (int i = 0; i < 6000; i++)
	{
		Entity ent = EntityManager::getInstance().createEntity();

		glm::vec2 pos = { utility::random::randomFloat(xPos.x, xPos.y) , utility::random::randomFloat(yPos.x, yPos.y) };
		glm::vec2 vel = { utility::random::randomFloat(-maxVel, maxVel), utility::random::randomFloat(-maxVel, maxVel) };
		int size = utility::random::randomFloat(sizeVar.x, sizeVar.y);

		ent.addComponent<Transform>(Transform(pos.x, pos.y, size, size));
		ent.addComponent<Velocity>(Velocity(vel.x, vel.y, 0, 0, 0.95));
		ent.addComponent<RenderingLayer>(RenderingLayer());
		ent.addComponent<AABB>(AABB(size, size, 1));
		ent.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/Fruit+.png"), fruitDivisions[rand()%fruitDivisions.size()]));

		SystemsManager::getInstance().getSystem<RenderingS>()->addEntity(ent.getID());
		SystemsManager::getInstance().getSystem<MovementS>()->addEntity(ent.getID());
		SystemsManager::getInstance().getSystem<CollisionS>()->addEntity(ent.getID());
		SystemsManager::getInstance().getSystem<CollisionRepulsionS>()->addEntity(ent.getID());

		SystemsManager::getInstance().getSystem<CollisionRepulsionS>()->repulsionStrength = 0.5;
	}

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

	SystemsManager::getInstance().getSystem<CollisionRepulsionS>()->repulsionStrength = 5;
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
	
	SystemsManager::getInstance().getSystem<CollisionRepulsionS>()->update(dt);
	SystemsManager::getInstance().getSystem<MovementS>()->update(dt);

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
