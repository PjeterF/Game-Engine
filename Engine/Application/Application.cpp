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

	//ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/sprite");
	//ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/quad");
	//ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/line");
	//ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/instancedQuad");

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

	mainCamera->setFrustrumX(0, windowWidth);
	mainCamera->setFrustrumY(0, windowHeight);

	CollisionS::initialize(30);
	RenderingS::initialize(renderingAPI);
}


void Application::run()
{
	/*ParticeEmitter emitter(0, 3000, 10000);
	emitter.defaultProperties.xPosVar = glm::vec2(-50, 50);
	emitter.defaultProperties.yPosVar = glm::vec2(-10, 30);
	emitter.defaultProperties.yVelVar = glm::vec2(1, 3);
	emitter.defaultProperties.startColour = glm::vec4(1, 0.9, 0.3, 1);
	emitter.defaultProperties.endColour = glm::vec4(0.8, 0.1, 0.1, 0.5);
	emitter.defaultProperties.startSize = 15;
	emitter.defaultProperties.endSize = 0;
	emitter.defaultProperties.velocityDecay = 0.9999;
	emitter.defaultProperties.particleLifetime = { 100, 300 };*/

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int fpsCap = 90;
	int iteration = 0;

	float dt = 1;

	MovementS msys;

	std::vector<glm::vec4> fruitDivisions;
	fruitDivisions = utility::sampling::sampleEvenly(608, 96, 0, 0, 36, 6);
	std::vector<glm::vec4> tileDivisions;
	tileDivisions = utility::sampling::sampleEvenly(256, 256, 0, 0, 8, 8);


	{
		Entity ent = EntityManager::getInstance().createEntity();
		ent.addComponent<Transform>(Transform(0, 0, 100, 100));
		ent.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("src/Textures/Tilesets/TX Tileset Grass.png"), tileDivisions[0]));
		//ent.addComponent<Animation>(Animation(0, 10, { { 0, 64, 96, 192 }, { 0, 64, 192, 192 } }));
		RenderingS::getInstance().addEntity(ent, 1);
	}
	for (int i = 0; i < 4000; i++)
	{
		Entity ent = EntityManager::getInstance().createEntity();
		ent.addComponent<Transform>(Transform(rand() % 2000-200, rand() % 2000 -200, 30, 30, 0));
		ent.addComponent<Velocity>(Velocity(2*(1*float(rand()%100)/100-1), 1*(2*float(rand() % 100) / 100-1), 0, -0.01));
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
	

	while (!glfwWindowShouldClose(window))
	{
		auto timeStart = std::chrono::high_resolution_clock::now();

		// Logic Update
		if (paused)
		{
			EntManager::getInstance().update();
			SystemsManager::getInstance().update(dt, UNPAUSED);

			//emitter.update();
			//for (int i = 0; i < 10; i++)
			//	emitter.emit();
			//emitter.applyForceInverseToSize(0.1, 0);

			CollisionS::getInstance().update(0);
			msys.update(0);
			CollisionS::getInstance().updateResponse(0);
			CollisionS::getInstance().lateUpdate(0);
			EntityManager::getInstance().update();
		}
		else
		{
			SystemsManager::getInstance().update(dt, PAUSED);
		}

		// Drawing
		glClearColor(0.3, 0.3, 0.3, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		RenderingS::getInstance().update(0);

		auto camOffset = mainCamera->getPosition();
		static int rate = 10;
		if (input->keyDown[ZE_KEY_W])
			mainCamera->setPosition(camOffset.x, camOffset.y + rate);
		if (input->keyDown[ZE_KEY_S])
			mainCamera->setPosition(camOffset.x, camOffset.y - rate);
		if (input->keyDown[ZE_KEY_A])
			mainCamera->setPosition(camOffset.x - rate, camOffset.y);
		if (input->keyDown[ZE_KEY_D])
			mainCamera->setPosition(camOffset.x + rate, camOffset.y);
		if (input->mouseKeyClicked[ZE_MOUSE_BUTTON_1])
		{
			auto vec = CollisionS::getInstance().pointPick(((input->getCursorPos()+mainCamera->getPosition())/mainCamera->getZoom()));
			for (auto ID : vec)
			{
				EntityManager::getInstance().deleteEntity(ID);
			}
		}
		float wheel = input->mouseWheel();
		if (input->keyDown[ZE_KEY_Q])
			wheel = 0.2;
		if (input->keyDown[ZE_KEY_E])
			wheel = -0.2;
		if (wheel)
		{
			mainCamera->changeZoom(0.05*wheel);
		}

		// UI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		input->update();

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