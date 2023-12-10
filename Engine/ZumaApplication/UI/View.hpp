#pragma once

#include "InterfaceWindow.hpp"
#include "../../src/Graphics/Camera.hpp"
#include "../../src/Input/InputManager.hpp"

class View : public InterfaceWindow
{
public:
	View(int x, int y, int width, int height, unsigned int textureID, GLFWwindow* window, Camera* camera, InputManager* input);
	virtual void draw() override;
private:
	glm::vec2 convertPos(float x, float y);

	ImGuiIO& io = ImGui::GetIO();

	bool mouseMoved();
	glm::vec2 prevMousePos = glm::vec2(0, 0);

	unsigned int textureID;
	GLFWwindow* window;
	Camera* camera;
	InputManager* input;

	bool panning = false;
	glm::vec2 panAnchor = glm::vec2(0, 0);

	bool paused = false;
};