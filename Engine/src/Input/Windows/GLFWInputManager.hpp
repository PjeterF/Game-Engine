#pragma once

#include "../InputManager.hpp"
#include "GLFW/glfw3.h"



class GLFWInputManager : public InputManager
{
public:
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		wheel = yoffset;
	}
	static void initialize(GLFWwindow* window)
	{
		instanceImp(window);
	}
	static GLFWInputManager& getInstance()
	{
		return instanceImp();
	}
	virtual bool isKeyPressed(int keycode) override;
	virtual bool isMouseButtonPressed(int keycode) override;

	virtual bool wasKeyReleased(int keycode) override;
	virtual bool wasMouseButtonReleased(int keycode) override;

	virtual bool wasKeyPressed(int keycode) override;
	virtual bool wasMouseButtonPressed(int keycode) override;

	virtual float mouseWheel() override;

	virtual glm::vec2 getCursorPos() override;
private:
	GLFWInputManager(GLFWwindow* window);
	static GLFWInputManager& instanceImp(GLFWwindow* window = nullptr)
	{
		static GLFWInputManager ins(window);
		return ins;
	}
	GLFWwindow* window;

	static const int No_Keys = 512;
	static const int No_MouseKeys = 10;

	bool keyDown[No_Keys];
	bool mouseKeyDown[No_MouseKeys];
	static float wheel;
};