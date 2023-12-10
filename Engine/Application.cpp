#include "Application.hpp"
#include "src/Loading/Loader.hpp"
#include <chrono>
#include <thread>
#include <string>

#include "src/Graphics/ParticleEmitter.hpp"
#include "src/ZumaLogic/ShortLivedParticleEmitter.hpp"
#include "src/ZumaLogic/TestObject.hpp"
#include "src/Managers/Resource/ResourceManager.hpp"

#include "src/ECS/Systems/MovementSystem.hpp"
#include "src/ECS/Systems/SpriteRenderingSystem.hpp"
#include "src/ECS/Systems/CollisionSystem.hpp"
#include "src/ECS/Systems/TestCollisionResponse.hpp"

void Application::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::TogglePause:
	{
		paused = !paused;
		gameOver = false;
	}
	break;
	case Event::ToggleCollisionGridVisibility:
	{
		showCollisions = !showCollisions;
	}
	break;
	case Event::GameOver:
	{
		gameOver = true;
	}
	break;
	case Event::CenterCamera:
	{
		camera->setOffset(0, 0);
	}
	break;
	default:
		break;
	}
}

Application::Application(float windowWidth, float windowHeight, std::string windowName)
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

	CollisionGrid::initialize(-2000, -2000, 20, 20, 200);

	ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/sprite");
	ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/quad");
	ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/line");
	ResourceManager::getInstance().createResourceFromFile<ShaderProgram>("src/shaders/instancedQuad");

	camera = new Camera(-windowWidth / 2, windowWidth/2, -windowHeight / 2, windowHeight/2);
	generalRenderer = new RenderingAPI(
		camera,
		ResourceManager::getInstance().getResource<ShaderProgram>("src/shaders/line")->getContents()->getId(),
		ResourceManager::getInstance().getResource<ShaderProgram>("src/shaders/sprite")->getContents()->getId(),
		ResourceManager::getInstance().getResource<ShaderProgram>("src/shaders/quad")->getContents()->getId()
	);
	generalRenderer->instancedQuadRenderer = new InstancedQuadRenderer(ResourceManager::getInstance().getResource<ShaderProgram>("src/shaders/instancedQuad")->getContents()->getId(), camera);

	Segment::segmentTemplates.push_back(SegmentTemplate
	(
		ResourceManager::getInstance().getResource<Texture>("src/textures/blue_marble.png"),
		4, 1,
		20,
		20,
		1
	));

	Segment::segmentTemplates.push_back(SegmentTemplate
	(
		ResourceManager::getInstance().getResource<Texture>("src/textures/red_marble.png"),
		4, 1,
		20,
		20,
		2
	));

	Segment::segmentTemplates.push_back(SegmentTemplate
	(
		ResourceManager::getInstance().getResource<Texture>("src/textures/yellow_marble.png"),
		4, 1,
		20,
		20,
		3
	));

	TextureLoader* textureLoader = new TextureLoader("Texture loader", &ResourceManager::getInstance(), 30);
	SegmentEditor* segmentEditor = new SegmentEditor("Segment Editor");

	mainMenu = new EditMenu(0, 0, windowWidth / 4, windowHeight);
	mainMenu->addSubcomponent(textureLoader);
	mainMenu->addSubcomponent(segmentEditor);
	viewportFramebuffer = new FrameBuffer(windowWidth, windowHeight);
	viewport = new Viewport(windowWidth / 4, 0, windowWidth*3/4, windowHeight, viewportFramebuffer->getTextureID(), window, camera);
	camera->setFrustrumX(0, viewport->xmax - viewport->xmin);

	camera->setFrustrumY(0, viewport->ymax - viewport->ymin);

	sceneMenu = new SceneMenu("SceneMenu", windowWidth * 3 / 4, 0, windowWidth, windowHeight/2);
	propertiesMenu = new PropertiesMenu("Properties", windowWidth * 3 / 4, windowHeight / 2, windowWidth, windowHeight, &TexturesManager::getInstance());
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

void  Application::run()
{
	Ent* ent1 = new Ent();
	ent1->addComponent(new TransformC({ 300, 0 }, { 10, 10 }, 0));
	ent1->addComponent(new BoxColliderC(0, 0, 10, 10, ent1));
	ent1->addComponent(new VelocityC({ -1, 0.05 }));
	ent1->addComponent(new SpriteC(ResourceManager::getInstance().getResource<Texture>("src/textures/frog.png")));

	Ent* ent2 = new Ent();
	ent2->addComponent(new TransformC({ -300, 0 }, { 10, 10 }, 0));
	ent2->addComponent(new BoxColliderC(0, 0, 10, 10, ent2));
	ent2->addComponent(new VelocityC({ 1, 0.05 }));
	ent2->addComponent(new SpriteC(ResourceManager::getInstance().getResource<Texture>("src/textures/marble1.png")));

	MovementSystem* msys = new MovementSystem();
	msys->addEntity(ent1);
	msys->addEntity(ent2);

	SpriteRenderingSystem* ssys = new SpriteRenderingSystem(generalRenderer);
	ssys->addEntity(ent1);
	ssys->addEntity(ent2);

	CollisionSystem* csys = new CollisionSystem(-100, -100, 10, 10, 20);
	csys->addEntity(ent1);
	csys->addEntity(ent2);

	TestCollisionResponse* crsys = new TestCollisionResponse(csys);
	crsys->addEntity(ent1);
	crsys->addEntity(ent2);

	ParticeEmitter emitter(-2000, -2000, 10000);
	emitter.defaultProperties.xPosVar=glm::vec2(-50, 50);
	emitter.defaultProperties.yPosVar=glm::vec2(-10, 30);
	emitter.defaultProperties.yVelVar=glm::vec2(1, 3);
	emitter.defaultProperties.startColour = glm::vec4(1, 0.9, 0.3, 1);
	emitter.defaultProperties.endColour = glm::vec4(0.8, 0.1, 0.1, 0.5);
	emitter.defaultProperties.startSize = 15;
	emitter.defaultProperties.endSize = 0;
	emitter.defaultProperties.velocityDecay = 0.9999;
	emitter.defaultProperties.particleLifetime = { 100, 300 };

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int fpsCap = 120;
	int fpsCap_collision = 120;
	int iteration = 0;

	EntityManager::getInstance().update();
	while (!glfwWindowShouldClose(window))
	{
		auto timeStart = std::chrono::high_resolution_clock::now();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		viewportFramebuffer->bind();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2, 0.2, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		generalRenderer->drawLine(glm::vec2(-50000, 0), glm::vec2(50000, 0), 3, glm::vec3(0.5, 0.5, 0.5));
		generalRenderer->drawLine(glm::vec2(0, -50000), glm::vec2(0, 50000), 5, glm::vec3(0.5, 0.5, 0.5));

		EntityManager::getInstance().draw(generalRenderer);
		emitter.draw(generalRenderer);

		ssys->update(0);
		csys->drawGrid(generalRenderer);
		csys->drawColliders(generalRenderer);

		if (showCollisions)
		{
			CollisionGrid::getInstance().drawGrid(generalRenderer);
			CollisionGrid::getInstance().drawColliders(generalRenderer);
		}

		viewportFramebuffer->unbind();
		glDisable(GL_DEPTH_TEST);

		viewport->render();
		mainMenu->render(generalRenderer);
		sceneMenu->render();
		propertiesMenu->render();

		if (!paused)
		{
			emitter.update();
			for(int i=0;i<10;i++)
				emitter.emit();
			emitter.applyForceInverseToSize(0.1, 0);

			csys->earlyUpdate(0);
			msys->update(0);
			csys->update(0);
			crsys->update(0);


			if (iteration % fpsCap/fpsCap_collision == 0)
			{
				CollisionGrid::getInstance().checkCollision();
				CollisionGrid::getInstance().handleCollisions();
			}

			if (iteration % fpsCap / fpsCap_collision == 0)
				CollisionGrid::getInstance().earlyUpdate();
			EntityManager::getInstance().update();
			if (iteration % fpsCap / fpsCap_collision == 0)
				CollisionGrid::getInstance().lateUpdate();
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