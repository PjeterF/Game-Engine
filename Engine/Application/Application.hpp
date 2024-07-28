#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../src/Graphics/GeneralRenderer.hpp"

#include "../src/OpenGL/FrameBuffer.hpp"

#include "../src/Input/Windows/GLFWInputManager.hpp"
#include "../src/Events/EventManager.hpp"

#include <string>

class Application : public EventObserver
{
public:
	Application(float windowWidth, float windowHeight, std::string windowName);
	void run();

	virtual void handleEvent(Event& event) override;

protected:
	void initializeImGui();

	GLFWwindow* window;
	float wndWidth;
	float wndHeight;
	std::string wndName;

	FrameBuffer* viewportFramebuffer;

	RenderingAPI* renderingAPI=nullptr;
	Camera* mainCamera=nullptr;

	InputManager* input;

	bool paused = true;
};