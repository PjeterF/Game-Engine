#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../src/Graphics/GeneralRenderer.hpp"
#include "../src/ECS/Systems/CollisionSystem.hpp"
#include "../src/ECS/Systems/MovementSystem.hpp"
#include "../src/ECS/Systems/SpriteRenderingSystem.hpp"
#include "../src/ECS/Systems/TestCollisionResponse.hpp"
#include "../src/ECS/Systems/InputMovementSystem.hpp"
#include "../src/ECS/Systems/LayeredRenderingSystem.hpp"
#include "../src/ECS/Systems/ParticleSystem.hpp"
#include "../src/ECS/Systems/CounterKillerSystem.hpp"

#include "../src/OpenGL/FrameBuffer.hpp"

#include "../src/Input/Windows/GLFWInputManager.hpp"

#include <string>

class Application : public EventObserver
{
public:
	Application(float windowWidth, float windowHeight, std::string windowName);
	void run();

	virtual void handleEvent(Event& event) override;

private:
	void initializeImGui();

	GLFWwindow* window;
	float wndWidth;
	float wndHeight;

	FrameBuffer* viewportFramebuffer;

	RenderingAPI* renderingAPI=nullptr;
	Camera* mainCamera=nullptr;

	InputManager* input;

	bool paused = true;
};