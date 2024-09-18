#include "Application.hpp"

#include <chrono>
#include <thread>
#include "../src/Managers/Resource/ResourceManager.hpp"
#include "../src/Graphics/ParticleEmitter.hpp"
#include "../src/Managers/Scene/SceneManager.hpp"

#include "../src/Graphics/InstancedSpriteRenderer.hpp"

#include "../src/ECS2/Systems/MovementS.hpp"
#include "../src/ECS2/Systems/RenderingS.hpp"
#include "../src/ECS2/Systems/CollisionS.hpp"
#include "../src/ECS2/Systems/UserControllerS.hpp"

#include "Tilemap/Tilemap.hpp"

#include "Games/Game1/Scenes/Game1_scene1.hpp"

Application::Application(float windowWidth, float windowHeight, std::string windowName) : EventObserver(ApplicationBin)
{
	EventManager::getInstance().addObserver(this, UI);

	srand(time(0));

	wndWidth = windowWidth;
	wndHeight = windowHeight;
	wndName=windowName;

	glfwInit();
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGL();

	glViewport(0, 0, windowWidth, windowHeight);

	GLFWInputManager::initialize(window);
	input = &GLFWInputManager::getInstance();
	initializeImGui();

	mainCamera = new Camera(0, windowWidth, 0, windowHeight);
	mainCamera->setPosition(0, 0);
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
}


void Application::run()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int fpsCap = 120;
	int iteration = 0;
	float dt = 0.0f;
	float accumulated_dt = 0;
	while (!glfwWindowShouldClose(window))
	{
		auto timeStart = std::chrono::high_resolution_clock::now();

/*======START OF LOGIC========*/

		if (paused)
		{
			SceneManager::getInstance().update(dt);
		}

/*======END OF LOGIC==========*/

/*======START OF DRAWING======*/

		glClearColor(0.3, 0.3, 0.3, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		SceneManager::getInstance().draw(renderingAPI);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

/*======END OF DRAWING=======*/

/*======START OF INPUT=======*/

		SceneManager::getInstance().input();

		glfwPollEvents();

/*======END OF INPUT=========*/

		auto timeEnd = std::chrono::high_resolution_clock::now();

		double frameDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();

		int nsDelay = (1000000000.0f / fpsCap) - frameDuration;
		if (nsDelay > 0) {
			std::this_thread::sleep_for(std::chrono::nanoseconds(nsDelay));
		}

		dt = frameDuration / 10000000.0f;

		//std::cout << dt << "\n";

		glfwSetWindowTitle(window, ((wndName + " " + std::to_string(frameDuration / 1000000.0f) + "ms").c_str()));
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
	case Event::TogglePause:
	{
		paused = !paused;
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