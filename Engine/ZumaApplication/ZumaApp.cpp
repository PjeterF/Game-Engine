#include "ZumaApp.hpp"

#include <chrono>
#include <thread>
#include "../src/Managers/Resource/ResourceManager.hpp"
#include "../src/Graphics/ParticleEmitter.hpp"
#include "../src/ECS/Entity/EntManager.hpp"
#include "../src/ECS/Systems/SystemsManager.hpp"

#include "Serialization/serialization.hpp"
#include "../src/Graphics/InstancedSpriteRenderer.hpp"

#include "../src/ECS2/Systems/MovementS.hpp"
#include "../src/ECS2/Systems/RenderingS.hpp"
#include "../src/ECS2/Systems/CollisionS.hpp"

ZumaApp::ZumaApp(float windowWidth, float windowHeight, std::string windowName) : EventObserver()
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

	ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/sprite");
	ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/quad");
	ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/line");
	ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/instancedQuad");

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

	view = new View(windowWidth * 0.25, windowHeight*0.03, windowWidth * 0.5, windowHeight * 0.97, viewportFramebuffer->getTextureID(), window, mainCamera, input);
	propertiesMenu = new EntityPropertiesMenu(windowWidth * 0.75, windowHeight * 0.5, windowWidth * 0.25, windowHeight);
	sceneMenu = new SceneMenu(windowWidth * 0.75, 0, windowWidth * 0.25, windowHeight * 0.5);
	assetLoader = new AssetLoader(0, 0, windowWidth * 0.25, windowHeight * 0.5);
	zumaMenu = new ZumaMenu(0, windowHeight * 0.5, windowWidth * 0.25, windowHeight * 0.5, &this->routes);
	bar = new TopBar(windowWidth * 0.25, 0, windowWidth * 0.5, windowHeight * 0.03);

	mainCamera->setFrustrumX(0, windowWidth);
	mainCamera->setFrustrumY(0, windowHeight);

	LayeredRenderingSystem::initialize(renderingAPI);
	CollisionSystem::initialize(-2000, -2000, 80, 80, 40);
	//CollisionSystem::initialize(-2000, -2000, 1, 1, 4000);
	ParticleSystem::getInstance();
	CounterKillerSystem::getInstance();
}


void ZumaApp::run()
{
	/*std::vector<TextureDivision> divisions;
	divisions.push_back(TextureDivision(0 * 122, 0, 112, 112));
	divisions.push_back(TextureDivision(1 * 112, 0, 112, 112));
	divisions.push_back(TextureDivision(2 * 112, 0, 112, 112));
	divisions.push_back(TextureDivision(3 * 112, 0, 112, 112));

	Spline* spline1 = new Spline
		(-500, 300, 100,
		ResourceManager::getInstance().getResource<Texture>("src/textures/control_point.png"),
		ResourceManager::getInstance().getResource<Texture>("src/textures/control_point2.png"),
		5
		);
	spline1->addSegment(500, 300);
	auto points = spline1->getControlPoints();
	points->at(1) = {300, 1000};
	spline1->sample();
	RouteManagementSystem* route1 = new RouteManagementSystem(spline1);
	RouteManagementSystem::marbleTemplates.push_back(MarbleTemplate(20, 1, "src/Textures/blue_marble.png", divisions, 30, { 0.2, 0.2, 1 }));
	RouteManagementSystem::marbleTemplates.push_back(MarbleTemplate(20, 2, "src/Textures/red_marble.png", divisions, 30, { 1, 0, 0 }));
	RouteManagementSystem::marbleTemplates.push_back(MarbleTemplate(20, 3, "src/Textures/yellow_marble.png", divisions, 30, { 1, 1, 0 }));

	Ent* shooter = EntManager::getInstance().createEntity();
	shooter->addComponent(new TransformC({ 0, 0 }, { 40, 40 }, 0));
	shooter->addComponent(new SpriteC(ResourceManager::getInstance().getResource<Texture>("src/Textures/frog.png")));
	shooter->addComponent(new ShooterC(5, 50));
	shooter->addComponent(new RenderingLayerC(0));

	ShooterManagementSystem::getInstance().addEntity(shooter);
	LayeredRenderingSystem::getInstance().addEntity(shooter);

	ParticeEmitter emitter(0, 3000, 10000);
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

	//deSerializeScene("Test", EntManager::getInstance(), SystemsManager::getInstance());
	//mainCamera->setPosition(400, 400);

	MovementS msys;
	CollisionS::initialize(0, 0, 30);
	RenderingS rsys(renderingAPI);

	for (int i = 0; i < 2000; i++)
	{
		Entity ent = EntityManager::getInstance().createEntity();
		ent.addComponent<Transform>(Transform(rand() % 2000, rand() % 2000, 10, 10, 0));
		ent.addComponent<Velocity>(Velocity(2*float(rand()%100)/100-1, 2*float(rand() % 100) / 100-1));
		ent.addComponent<Sprite>(ResourceManager::getInstance().getResource<Texture>("src/textures/red.jpg"));
		ent.addComponent<AABB>(AABB(10, 10));

		msys.addEntity(ent);
		rsys.addEntity(ent);
		CollisionS::getInstance().addEntity(ent);
	}

	Entity ent2 = EntityManager::getInstance().createEntity();
	ent2.addComponent<Transform>(Transform(100, 100, 100, 10, 0));
	ent2.addComponent<Velocity>(Velocity(0, 0));
	ent2.addComponent<Sprite>(ResourceManager::getInstance().getResource<Texture>("src/textures/control_point.png"));
	ent2.addComponent<AABB>(AABB(100, 10));

	msys.addEntity(ent2);
	rsys.addEntity(ent2);
	CollisionS::getInstance().addEntity(ent2);

	Entity ent = EntityManager::getInstance().createEntity();
	ent.addComponent<Transform>(Transform(-100, 100, 10, 10, 0));
	ent.addComponent<Velocity>(Velocity(1, 0));
	ent.addComponent<Sprite>(ResourceManager::getInstance().getResource<Texture>("src/textures/control_point2.png"));
	ent.addComponent<AABB>(AABB(10, 10));

	msys.addEntity(ent);
	rsys.addEntity(ent);
	CollisionS::getInstance().addEntity(ent);

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
		}
		else
		{
			SystemsManager::getInstance().update(dt, PAUSED);
		}

		// Drawing
		glClearColor(0.3, 0.3, 0.3, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		/*renderingAPI->drawSprite({ 400, 400 }, { 50, 50 }, 0, ResourceManager::getInstance().getResource<Texture>("src/textures/control_point.png")->getContents());
		renderingAPI->addQuadInstance({ 400, 400 }, { 50, 50 }, 0, {0.5, 0.5, 0.5, 1});
		renderingAPI->drawQuadInstances();
		renderingAPI->drawQuad({ 500, 400 }, { 50, 50 }, 0, { 0.5, 0.5, 0.5, 1 });*/

		rsys.update(0);

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

		/*view->draw();
		propertiesMenu->draw();
		sceneMenu->draw();
		assetLoader->draw();
		zumaMenu->draw();
		bar->draw();*/

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

void ZumaApp::handleEvent(Event& event)
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
		routes.push_back((RouteManagementSystem*)event.getPayload());
	}
	break;
	case Event::RouteDeletion:
	{
		routes.erase(std::find(routes.begin(), routes.end(), event.getPayload()));
	}
	break;
	}
}

void ZumaApp::initializeImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 300 es");
}