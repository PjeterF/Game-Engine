#include "GLFWInputManager.hpp"

float GLFWInputManager::wheel = 0;

GLFWInputManager::GLFWInputManager(GLFWwindow* window)
{
	this->window = window;
	glfwSetScrollCallback(window, scroll_callback);

	for (int i = 0; i < No_Keys; i++)
		keyDown[i] = false;
	for (int i = 0; i < No_MouseKeys; i++)
		mouseKeyDown[i] = false;
}

bool GLFWInputManager::isKeyPressed(int keycode)
{
	return !(glfwGetKey(window, keycode) - GLFW_PRESS);
}

bool GLFWInputManager::isMouseButtonPressed(int keycode)
{
	return !(glfwGetMouseButton(window, keycode) - GLFW_PRESS);
}

bool GLFWInputManager::wasKeyReleased(int keycode)
{
	int action = glfwGetMouseButton(window, keycode);

	if (keyDown[keycode])
	{
		if (action == GLFW_RELEASE)
		{
			keyDown[keycode] = false;
			return true;
		}
	}
	else
	{
		if (action == GLFW_PRESS)
			keyDown[keycode] = true;
	}

	return false;
}

bool GLFWInputManager::wasMouseButtonReleased(int keycode)
{
	int action = glfwGetMouseButton(window, keycode);

	if (mouseKeyDown[keycode])
	{
		if (action == GLFW_RELEASE)
		{
			mouseKeyDown[keycode] = false;
			return true;
		}
	}
	else
	{
		if (action == GLFW_PRESS)
			mouseKeyDown[keycode] = true;
	}

	return false;
}

bool GLFWInputManager::wasKeyPressed(int keycode)
{
	int action = glfwGetMouseButton(window, keycode);

	if (keyDown[keycode])
	{
		if (action == GLFW_RELEASE)
		{
			keyDown[keycode] = false;
		}
	}
	else
	{
		if (action == GLFW_PRESS)
		{
			keyDown[keycode] = true;
			return true;
		}

	}

	return false;
}

bool GLFWInputManager::wasMouseButtonPressed(int keycode)
{
	int action = glfwGetMouseButton(window, keycode);

	if (mouseKeyDown[keycode])
	{
		if (action == GLFW_RELEASE)
		{
			mouseKeyDown[keycode] = false;
		}
	}
	else
	{
		if (action == GLFW_PRESS)
		{
			mouseKeyDown[keycode] = true;
			return true;
		}

	}

	return false;
}

float GLFWInputManager::mouseWheel()
{
	float temp = wheel;
	wheel = 0;
	return temp;
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
