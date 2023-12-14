#include "ZumaApp.hpp"

#include <chrono>
#include <thread>
#include "../src/Managers/Resource/ResourceManager.hpp"
#include "../src/Graphics/ParticleEmitter.hpp"
#include "../src/ECS/Entity/EntManager.hpp"
#include "../src/ECS/Systems/SystemsManager.hpp"



ZumaApp::ZumaApp(float windowWidth, float windowHeight, std::string windowName)
{
	srand(time(0));

	wndWidth = windowHeight;
	wndHeight = windowHeight;

	glfwInit();
	window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGL();

	glViewport(0, 0, windowWidth, windowHeight);

	initializeImGui();
	GLFWInputManager::initialize(window);
	inputManager = &GLFWInputManager::getInstance();

	ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/sprite");
	ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/quad");
	ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/line");
	ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/instancedQuad");

	mainCamera = new Camera(0, windowWidth, 0, windowHeight);
	renderingAPI = new RenderingAPI(
		mainCamera,
		ResourceManager::getInstance().getResource<ShaderProgram>("src/shaders/line")->getContents()->getId(),
		ResourceManager::getInstance().getResource<ShaderProgram>("src/shaders/sprite")->getContents()->getId(),
		ResourceManager::getInstance().getResource<ShaderProgram>("src/shaders/quad")->getContents()->getId()
	);
	renderingAPI->instancedQuadRenderer = new InstancedQuadRenderer(ResourceManager::getInstance().getResource<ShaderProgram>("src/shaders/instancedQuad")->getContents()->getId(), mainCamera);

	viewportFramebuffer = new FrameBuffer(windowWidth, windowHeight);

	view = new View(windowWidth * 0.25, 0, windowWidth * 0.5, windowHeight, viewportFramebuffer->getTextureID(), window, mainCamera, inputManager);
	propertiesMenu = new EntityPropertiesMenu(windowWidth * 0.75, windowHeight * 0.5, windowWidth * 0.25, windowHeight);
	sceneMenu = new SceneMenu(windowWidth * 0.75, 0, windowWidth * 0.25, windowHeight * 0.5);
	assetLoader = new AssetLoader(0, 0, windowWidth * 0.25, windowHeight * 0.5);
	zumaMenu = new ZumaMenu(0, windowHeight * 0.5, windowWidth * 0.25, windowHeight * 0.5);

	mainCamera->setFrustrumX(0, view->width);
	mainCamera->setFrustrumY(0, view->height);
}


void ZumaApp::run()
{
	LayeredRenderingSystem::initialize(renderingAPI);

	Ent* ent1 = EntManager::getInstance().createEntity();
	ent1->addComponent(new TransformC({ 300, 0 }, { 10, 10 }, 0));
	ent1->addComponent(new BoxColliderC(0, 0, 5, 10, ent1));
	ent1->addComponent(new VelocityC({ -1, 0.05 }));
	ent1->addComponent(new RenderingLayerC(0));
	ent1->addComponent(new SpriteC(ResourceManager::getInstance().getResource<Texture>("src/textures/marble1.png")));

	LayeredRenderingSystem::getInstance().addEntity(ent1);

	auto transform = ent1->getComponent(Transform);

	nlohmann::json j;
	transform->to_json(j);
	std::cout << j.dump(4);

	Ent* ent2 = EntManager::getInstance().createEntity();
	ent2->addComponent(new TransformC({ -300, 0 }, { 10, 10 }, 0));
	//ent2->addComponent(new BoxColliderC(0, 0, 10, 10, ent2));
	ent2->addComponent(new VelocityC({ 1, 0.05 }));
	ent2->addComponent(new SpriteC(ResourceManager::getInstance().getResource<Texture>("src/textures/marble2.png")));
	ent2->addComponent(new RenderingLayerC(1));

	LayeredRenderingSystem::getInstance().addEntity(ent2);

	MovementSystem* msys = new MovementSystem();
	msys->addEntity(ent1);
	msys->addEntity(ent2);

	SpriteRenderingSystem* ssys = new SpriteRenderingSystem(renderingAPI);
	//ssys->addEntity(ent1);
	//ssys->addEntity(ent2);

	CollisionSystem* csys = new CollisionSystem(-2000, -2000, 80, 80, 50);
	csys->addEntity(ent1);
	csys->addEntity(ent2);

	TestCollisionResponse* crsys = new TestCollisionResponse(csys);
	crsys->addEntity(ent1);
	crsys->addEntity(ent2);

	InputMovementSystem* isys = new InputMovementSystem(inputManager);
	isys->addEntity(ent1);

	AnimatedSpriteSystem* aSpriteSys = new AnimatedSpriteSystem(renderingAPI);
	Ent* animatedEnt = EntManager::getInstance().createEntity();
	std::vector<TextureDivision> divisions;
	divisions.push_back(TextureDivision(0 * 122, 0, 112, 112));
	divisions.push_back(TextureDivision(1 * 112, 0, 112, 112));
	divisions.push_back(TextureDivision(2 * 112, 0, 112, 112));
	divisions.push_back(TextureDivision(3 * 112, 0, 112, 112));
	AnimatedSpriteC* aSprite = new AnimatedSpriteC
	(
		ResourceManager::getInstance().getResource<Texture>("src/Textures/blue_marble.png"),
		divisions,
		10
	);
	animatedEnt->addComponent(aSprite);
	animatedEnt->addComponent(new TransformC({ 200, 200 }, { 100, 100 }, 0));
	aSpriteSys->addEntity(animatedEnt);

	propertiesMenu->selectEntity(ent1);

	MarbleCollisionResolutionSystem* mcrs = new MarbleCollisionResolutionSystem(csys);

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
	RouteManagementSystem* route1 = new RouteManagementSystem(csys, aSpriteSys, mcrs, *spline1->getSampledPoints());
	RouteManagementSystem::marbleTemplates.push_back(MarbleTemplate(10, 1, "src/Textures/blue_marble.png", divisions, 30));
	RouteManagementSystem::marbleTemplates.push_back(MarbleTemplate(10, 2, "src/Textures/red_marble.png", divisions, 30));
	RouteManagementSystem::marbleTemplates.push_back(MarbleTemplate(10, 3, "src/Textures/yellow_marble.png", divisions, 30));

	Ent* shooter = EntManager::getInstance().createEntity();
	shooter->addComponent(new TransformC({ 0, 0 }, { 40, 40 }, 0));
	shooter->addComponent(new SpriteC(ResourceManager::getInstance().getResource<Texture>("src/Textures/frog.png")));
	shooter->addComponent(new ShooterC(5, 50));
	shooter->addComponent(new RenderingLayerC(0));

	ShooterManagementSystem* shooterSystem = new ShooterManagementSystem(csys, aSpriteSys, msys);

	shooterSystem->addEntity(shooter);
	ssys->addEntity(shooter);

	ParticeEmitter emitter(300, 300, 10000);
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

	int fpsCap = 120;
	int iteration = 0;

	while (!glfwWindowShouldClose(window))
	{
		auto timeStart = std::chrono::high_resolution_clock::now();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Logic Update
		if (!paused)
		{
			EntManager::getInstance().update();
			SystemsManager::getInstance().update(0, UNPAUSED);

			emitter.update();
			for (int i = 0; i < 1; i++)
				emitter.emit();
			emitter.applyForceInverseToSize(0.1, 0);
		}

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2, 0.2, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Drawing
		viewportFramebuffer->bind();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2, 0.2, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//csys->drawGrid(renderingAPI);
		//csys->drawColliders(renderingAPI);

		SystemsManager::getInstance().update(0, PAUSED);

		emitter.draw(renderingAPI);
		spline1->draw(renderingAPI);


		viewportFramebuffer->unbind();
		glDisable(GL_DEPTH_TEST);

		view->draw();
		propertiesMenu->draw();
		sceneMenu->draw();
		assetLoader->draw();
		zumaMenu->draw();

		if (inputManager->isKeyPressed(GLFW_KEY_P))
		{
			std::cout<<"True ";
		}

		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();

		auto timeEnd = std::chrono::high_resolution_clock::now();

		// Framerate delay 
		auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count();
		int msDelay = (1000 / fpsCap) - frameDuration;
		if (msDelay > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(msDelay));

		glfwSetWindowTitle(window, ("Frameduration: " + std::to_string(frameDuration) + " Delay: " + std::to_string(msDelay)).c_str());
		iteration++;
	}

	glfwTerminate();
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

void ZumaApp::updateSystems(float dt)
{
	for (auto system : systems)
	{
		system->update(dt);
	}
}

Ent* ZumaApp::getEntity(int id)
{
	auto it = entities.find(id);
	if (it == entities.end())
		return (*it).second;
	else
		return nullptr;
}

void ZumaApp::removeEntity(int id)
{
	entities.erase(id);
}
