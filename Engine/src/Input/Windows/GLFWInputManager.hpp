#pragma once

#include "../InputManager.hpp"
#include "GLFW/glfw3.h"

class GLFWInputManager : public InputManager
{
public:
	static void initialize(GLFWwindow* window)
	{
		instanceImp(window);
	}
	static GLFWInputManager& getInstance()
	{
		return instanceImp();
	}

	glm::ivec2 getWindowDimensions();

	virtual float mouseWheel() override;
	virtual void update() override;
	virtual glm::vec2 getCursorPos() override;
	virtual glm::vec2 getNormalizedCursorPos() override;

	virtual std::vector<TouchPoint> getActiveTouches() override;

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		wheel = yoffset;
	}
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
private:
	GLFWInputManager(GLFWwindow* window);
	static GLFWInputManager& instanceImp(GLFWwindow* window = nullptr)
	{
		static GLFWInputManager ins(window);
		return ins;
	}
	GLFWwindow* window;

};