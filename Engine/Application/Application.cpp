#include "Application.hpp"

#include <chrono>
#include <thread>
#include "../src/Managers/Resource/ResourceManager.hpp"
#include "../src/Graphics/ParticleEmitter.hpp"
#include "../src/ECS/Entity/EntManager.hpp"
#include "../src/ECS/Systems/SystemsManager.hpp"

#include "../src/Graphics/InstancedSpriteRenderer.hpp"

#include "../src/ECS2/Systems/MovementS.hpp"
#include "../src/ECS2/Systems/RenderingS.hpp"
#include "../src/ECS2/Systems/CollisionS.hpp"
#include "../src/ECS2/Systems/UserControllerS.hpp"

#include "Tilemap/Tilemap.hpp"

Application::Application(float windowWidth, float windowHeight, std::string windowName) : EventObserver()
{
	EventManager::getInstance().addObserver(this, UI);

	srand(time(0));

	wndWidth = windowHeight;
	wndHeight = windowHeight;

	glfwInit();
	window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGL();

	glViewport(0, 0, windowWidth, windowHeight);

	GLFWInputManager::initialize(window);
	input = &GLFWInputManager::getInstance();
	initializeImGui();

	mainCamera = new Camera(0, windowWidth, 0, windowHeight);
	renderingAPI = new RenderingAPI(
		mainCamera,
		ResourceManager::getInstance().getResource<ShaderProgram>("src/shaders/line")->getContents()->getId(),
		ResourceManager::getInstance().getResource<ShaderProgram>("src/shaders/sprite")->getContents()->getId(),
		ResourceManager::getInstance().getResource<ShaderProgram>("src/shaders/quad")->getContents()->getId(),
		ResourceManager::getInstance().getResource<ShaderProgram>("src/shaders/instancedQuad")->getContents()->getId(),
		ResourceManager::getInstance().getResource<ShaderProgram>("src/shaders/instancedSprite")->getContents()->getId()
	);

	viewportFramebuffer = new FrameBuffer(windowWidth, windowHeight);
	viewportFramebuffer->unbind();

	CollisionS::initialize(30);
	RenderingS::initialize(renderingAPI);
}


void Application::run()
{
	ParticeEmitter emitter(0, 3000, 10000);
	emitter.defaultProperties.xPosVar = glm::vec2(-50, 50);
	emitter.defaultProperties.yPosVar = glm::vec2(-10, 30);
	emitter.defaultProperties.yVelVar = glm::vec2(1, 3);
	emitter.defaultProperties.startColour = glm::vec4(1, 0.9, 0.3, 1);
	emitter.defaultProperties.endColour = glm::vec4(0.8, 0.1, 0.1, 0.5);
	emitter.defaultProperties.startSize = 15;
	emitter.defaultProperties.endSize = 0;
	emitter.defaultProperties.velocityDecay = 0.9999;
	emitter.defaultProperties.particleLifetime = { 100, 300 };

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int fpsCap = 90;
	int iteration = 0;

	float dt = 1;

	MovementS msys;
	UserControllerS inputsys(input);

	std::vector<glm::vec4> fruitDivisions;
	fruitDivisions = utility::sampling::sampleEvenly(608, 96, 0, 0, 36, 6);
	std::vector<glm::vec4> tileDivisions;
	tileDivisions = utility::sampling::sampleEvenly(32*8, 32*8, 0, 0, 8, 8);

	TileArchetype::map[TileType::Grass] = TileArchetype("src/Textures/Tilesets/TX Tileset Grass.png", tileDivisions[0 + 3 * 8], true);
	TileArchetype::map[TileType::Path1] = TileArchetype("src/Textures/Tilesets/TX Tileset Grass.png", tileDivisions[0 + 4 * 8], true);

	std::vector<Tile> tiles;
	for (int i = 0; i <TILE_ARRAY_SIZE; i++)
	{
		if (i % 2)
			tiles.push_back(Tile(TileArchetype::map[TileType::Grass]));
		else
			tiles.push_back(Tile(TileArchetype::map[TileType::Path1]));
	}

	Tilemap tilemap = Tilemap(0, 0, tiles);
	auto ser = tilemap.serialize();
	tilemap.deserialize(ser);

	//{
	//	Entity ent = EntityManager::getInstance().createEntity();
	//	ent.addComponent<Transform>(Transform(0, 0, 100, 100));
	//	ent.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/Tilesets/TX Tileset Grass.png"), tileDivisions[0 + 4 * 8]));
	//	//ent.addComponent<Animation>(Animation(0, 10, { { 0, 64, 96, 192 }, { 0, 64, 192, 192 } }));
	//	RenderingS::getInstance().addEntity(ent, 1);
	//}
	int id;
	for (int i = 0; i < 3000; i++)
	{
		Entity ent = EntityManager::getInstance().createEntity();
		if (i == 0)
			id = ent.getID();
		ent.addComponent<Transform>(Transform(rand() % 2000-200, rand() % 2000-200, 30, 30, 0));
		ent.addComponent<Velocity>(Velocity(2*(1*float(rand()%100)/100-1), 1*(2*float(rand() % 100) / 100-1), 0, -0.0));
		ent.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/Fruit+.png"), fruitDivisions[rand()%fruitDivisions.size()]));
		AABB& col = ent.addComponent<AABB>(AABB(15, 15));
		if (rand() % 100 < 0)
			col.enabled = false;

		msys.addEntity(ent);
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
	
	mainCamera->setPosition(0, 0);

	enum UIState {Normal, TileMapEditor };
	UIState UIState = Normal;

	while (!glfwWindowShouldClose(window))
	{
		auto timeStart = std::chrono::high_resolution_clock::now();

/*======START OF LOGIC========*/

		if (paused)
		{
			emitter.update();
			for (int i = 0; i < 10; i++)
				emitter.emit();
			emitter.applyForceInverseToSize(0.1, 0);

			/*Entity ent(id);
			Transform& transform = ent.getComponent<Transform>();

			mainCamera->setPosition(transform.x, transform.y);*/

			CollisionS::getInstance().update(0);
			inputsys.update(0);
			msys.update(0);
			CollisionS::getInstance().updateResponse(0);
			CollisionS::getInstance().lateUpdate(0);
			EntityManager::getInstance().update();
		}
		else
		{
			SystemsManager::getInstance().update(dt, PAUSED);
		}

/*======END OF LOGIC==========*/

/*======START OF DRAWING======*/

		glClearColor(0.3, 0.3, 0.3, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		tilemap.draw(renderingAPI);

		RenderingS::getInstance().update(0);

		emitter.draw(renderingAPI);

		ImGui::Begin("Tile map edit");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

/*======END OF DRAWING=======*/

/*======START OF INPUT=======*/

		auto camOffset = mainCamera->getPosition();
		int rate = 10 / mainCamera->getZoom();
		if (input->keyDown[ZE_KEY_UP])
			mainCamera->setPosition(camOffset.x, camOffset.y + rate);
		if (input->keyDown[ZE_KEY_DOWN])
			mainCamera->setPosition(camOffset.x, camOffset.y - rate);
		if (input->keyDown[ZE_KEY_LEFT])
			mainCamera->setPosition(camOffset.x - rate, camOffset.y);
		if (input->keyDown[ZE_KEY_RIGHT])
			mainCamera->setPosition(camOffset.x + rate, camOffset.y);
		auto nCursorPos = input->getNormalizedCursorPos();
		float scrollRegionWidth = 0.1f;
		if (nCursorPos.y > 1.0f - scrollRegionWidth)
			mainCamera->setPosition(camOffset.x, camOffset.y + rate);
		if (nCursorPos.y < scrollRegionWidth)
			mainCamera->setPosition(camOffset.x, camOffset.y - rate);
		if (nCursorPos.x<scrollRegionWidth)
			mainCamera->setPosition(camOffset.x - rate, camOffset.y);
		if (nCursorPos.x > 1.0f - scrollRegionWidth)
			mainCamera->setPosition(camOffset.x + rate, camOffset.y);
		if (input->mouseKeyClicked[ZE_MOUSE_BUTTON_1])
		{
			auto normalized = mainCamera->viewPortPointToWorldCoord(input->getNormalizedCursorPos());
			std::cout << normalized.x << " " << normalized.y << "\n";

			Entity ent = EntityManager::getInstance().createEntity();
			ent.addComponent<Transform>(Transform(normalized.x, normalized.y, 30, 30, 0));
			ent.addComponent<Velocity>(Velocity(rand()%2-0.5, rand() % 2 - 0.5, 0, 0));
			ent.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/Fruit+.png"), fruitDivisions[rand() % fruitDivisions.size()]));
			AABB& col = ent.addComponent<AABB>(AABB(15, 15));
			msys.addEntity(ent);
			RenderingS::getInstance().addEntity(ent);
			CollisionS::getInstance().addEntity(ent);
			inputsys.addEntity(ent);

			//auto vec = CollisionS::getInstance().pointPick(((input->getCursorPos()+mainCamera->getPosition())/mainCamera->getZoom()));
			/*auto vec = CollisionS::getInstance().pointPick(mainCamera->viewPortPointToWorldCoord(input->getNormalizedCursorPos()));
			for (auto ID : vec)
			{
				EntityManager::getInstance().deleteEntity(ID);
			}*/
		}
		float wheel = input->mouseWheel();
		if (input->keyClicked[ZE_KEY_Q])
			wheel = 1;
		if (input->keyClicked[ZE_KEY_E])
			wheel = -1;
		if (wheel)
		{
			mainCamera->changeZoom(0.1*wheel);
		}

		input->update();

/*======END OF INPUT=========*/


		auto timeEnd = std::chrono::high_resolution_clock::now();

		// Framerate delay 
		auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count();
		dt = (float)(frameDuration/1000);

		int msDelay = (1000 / fpsCap) - frameDuration;
		if (msDelay > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(msDelay));

		glfwSetWindowTitle(window, ("Frameduration: " + std::to_string(frameDuration) + " dt: " + std::to_string(dt)).c_str());
		iteration++;
	}

	glfwTerminate();
}

void Application::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::Pause:
	{
		paused = true;
	}
	break;
	case Event::Unpause:
	{
		paused = false;
	}
	break;
	case Event::RouteCreation:
	{
		
	}
	break;
	case Event::RouteDeletion:
	{
		
	}
	break;
	}
}

void Application::initializeImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 300 es");
}