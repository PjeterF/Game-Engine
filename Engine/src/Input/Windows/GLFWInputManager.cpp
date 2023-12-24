#include "GLFWInputManager.hpp"

float GLFWInputManager::wheel = 0;

GLFWInputManager::GLFWInputManager(GLFWwindow* window)
{
	this->window = window;
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetWindowUserPointer(window, this);

	for (int i = 0; i < No_Keys; i++)
		keyDown[i] = false;
	for (int i = 0; i < No_MouseKeys; i++)
		mouseKeyDown[i] = false;
	for (int i = 0; i < No_Keys; i++)
		keyReleased[i] = false;
	for (int i = 0; i < No_MouseKeys; i++)
		mouseKeyReleased[i] = false;
	for (int i = 0; i < No_Keys; i++)
		keyClicked[i] = false;
	for (int i = 0; i < No_MouseKeys; i++)
		mouseKeyClicked[i] = false;
}

void GLFWInputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GLFWInputManager* manager = (GLFWInputManager*)glfwGetWindowUserPointer(window);

	if (action == GLFW_RELEASE)
	{
		manager->keyReleased[key] = true;
		manager->keyDown[key] = false;
	}
	if (action == GLFW_PRESS)
	{
		if (!manager->keyDown[key])
			manager->keyClicked[key] = true;
		manager->keyDown[key] = true;
	}
}

void GLFWInputManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	GLFWInputManager* manager = (GLFWInputManager*)glfwGetWindowUserPointer(window);

	if (action == GLFW_RELEASE)
	{
		manager->mouseKeyReleased[button] = true;
		manager->mouseKeyDown[button] = false;
	}
	if (action == GLFW_PRESS)
	{
		if(!manager->mouseKeyDown[button])
			manager->mouseKeyClicked[button] = true;
		manager->mouseKeyDown[button] = true;
	}
}

float GLFWInputManager::mouseWheel()
{
	float temp = wheel;
	wheel = 0;
	return temp;
}

void GLFWInputManager::update()
{
	for (int i = 0; i < No_Keys; i++)
		keyClicked[i] = false;
	for (int i = 0; i < No_MouseKeys; i++)
		mouseKeyClicked[i] = false;
	for (int i = 0; i < No_Keys; i++)
		keyReleased[i] = false;
	for (int i = 0; i < No_MouseKeys; i++)
		mouseKeyReleased[i] = false;

	glfwPollEvents();
}

glm::vec2 GLFWInputManager::getCursorPos()
{
	glm::ivec2 wndSize = { 0, 0 };

	glfwGetWindowSize(window, &wndSize.x, &wndSize.y);

	glm::dvec2 pos = { 0, 0 };
	glfwGetCursorPos(window, &pos.x, &pos.y);
	pos.y = wndSize.y - pos.y;
	return pos;
}
